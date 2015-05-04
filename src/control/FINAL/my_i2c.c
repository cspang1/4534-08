#include "maindefs.h"
#ifndef __XC8
#include <i2c.h>
#else
#include <plib/i2c.h>
#endif
#include "my_i2c.h"
//#include "parser.h"
//#include "messages.h"


static i2c_comm *ic_ptr;
//unsigned char msgbuffer[MSGLEN + 1];
//parser_t parser;


// Configure for I2C Master mode -- the variable "slave_addr" should be stored in
// i2c_comm (as pointed to by ic_ptr) for later use.
// setup the PIC to operate as a Master
void i2c_configure_master()
{
    TRISBbits.TRISB4 = 1; //RB4 = SCL1
    TRISBbits.TRISB5 = 1; //RB5 = SDA1
    
    //TRISDbits.TRISD0 = 1;//RD0 = SCL2
    //TRISDbits.TRISD1 = 1;//RD1 = SDA2

    //Baud clock = Fosc / 4(x+1)
    //100 kHz = 40 MHz / 4(x+1)
    //x= 99 = 0x63
    SSPADD = 0x63;

    SSPSTAT = 0x0;
    SSPSTAT |= SLEW_OFF;

    SSPCON1 = 0x0;
    SSPCON1bits.SSPEN = 1; //Enables the serial port and configures the SDA1 and the SCL1 pins as the serial port pins
    SSPCON1bits.SSPM = 0x08; // enable master mode with Clock  = FOSC/(4*(SSPXADD+1)


    // enable clock-stretching
    SSPCON2 = 0x0;


    PORTBbits.SCL1 = 1;
    PORTBbits.SDA1 = 1;
    //PORTDbits.SCL2 = 1;
    //PORTDbits.SDA2 = 1;
    // end of i2c configure
}




void start_i2c_slave_reply(unsigned char length, unsigned char *msg) {

    for (ic_ptr->outbuflen = 0; ic_ptr->outbuflen < length; ic_ptr->outbuflen++) {
        ic_ptr->outbuffer[ic_ptr->outbuflen] = msg[ic_ptr->outbuflen];
    }
    ic_ptr->outbuflen = length;
    ic_ptr->outbufind = 1; // point to the second byte to be sent

    // put the first byte into the I2C peripheral
    SSPBUF = ic_ptr->outbuffer[0];
    // we must be ready to go at this point, because we'll be releasing the I2C
    // peripheral which will soon trigger an interrupt
    SSPCON1bits.CKP = 1;

}

// an internal subroutine used in the slave version of the i2c_int_handler

void handle_start(unsigned char data_read) {
    ic_ptr->event_count = 1;
    ic_ptr->buflen = 0;
    // check to see if we also got the address
    if (data_read) {
        if (SSPSTATbits.D_A == 1) {
            // this is bad because we got data and
            // we wanted an address
            ic_ptr->status = I2C_IDLE;
            ic_ptr->error_count++;
            ic_ptr->error_code = I2C_ERR_NOADDR;
        } else {
            if (SSPSTATbits.R_W == 1) {
                ic_ptr->status = I2C_SLAVE_SEND;
            } else {
                ic_ptr->status = I2C_RCV_DATA;
            }
        }
    } else {
        ic_ptr->status = I2C_STARTED;
    }
}


// Sending in I2C Master mode [slave write]
// 		returns -1 if the i2c bus is busy
// 		return 0 otherwise
// Will start the sending of an i2c message -- interrupt handler will take care of
//   completing the message send.  When the i2c message is sent (or the send has failed)
//   the interrupt handler will send an internal_message of type MSGT_MASTER_SEND_COMPLETE if
//   the send was successful and an internal_message of type MSGT_MASTER_SEND_FAILED if the
//   send failed (e.g., if the slave did not acknowledge).  Both of these internal_messages
//   will have a length of 0.
// The subroutine must copy the msg to be sent from the "msg" parameter below into
//   the structure to which ic_ptr points [there is already a suitable buffer there].

unsigned char i2c_master_send(unsigned char length, unsigned char *msg,unsigned char destinataire)
{
    ic_ptr->slave_addr = destinataire; //Setting the slave address to which the message is being sent
    if (SSPSTAT & 0x4) return -1;
    ic_ptr->outbuflen = length;
    ic_ptr->outbufind = 0;
    for(char index = 0; index < length;index++)
    {

        ic_ptr->outbuffer[index] = msg[index];
    }
    ic_ptr->status = I2C_MASTER_SEND_STARTED;
    SSPCON2bits.SEN = 1; // Generating start condition
    
    return(0);
}



// Receiving in I2C Master mode [slave read]
// 		returns -1 if the i2c bus is busy
// 		return 0 otherwise
// Will start the receiving of an i2c message -- interrupt handler will take care of
//   completing the i2c message receive.  When the receive is complete (or has failed)
//   the interrupt handler will send an internal_message of type MSGT_MASTER_RECV_COMPLETE if
//   the receive was successful and an internal_message of type MSGT_MASTER_RECV_FAILED if the
//   receive failed (e.g., if the slave did not acknowledge).  In the failure case
//   the internal_message will be of length 0.  In the successful case, the
//   internal_message will contain the message that was received [where the length
//   is determined by the parameter passed to i2c_master_recv()].
// The interrupt handler will be responsible for copying the message received into

unsigned char i2c_master_recv(unsigned char length, unsigned char destinataire) {
    
    ic_ptr->slave_addr = destinataire;
    ic_ptr->status = I2C_MASTER_RECEIVE_STARTED;
    ic_ptr->buflen = length;
    ic_ptr->bufind = 0;
    SSPCON2bits.SEN = 1;
    return(0);
}

// this is the interrupt handler for i2c -- it is currently built for slave mode
// -- to add master mode, you should determine (at the top of the interrupt handler)
//    which mode you are in and call the appropriate subroutine.  The existing code
//    below should be moved into its own "i2c_slave_handler()" routine and the new
//    master code should be in a subroutine called "i2c_master_handler()"



void i2c_int_handler()
{
    switch(ic_ptr->status)
    {
        case I2C_MASTER_SEND_STARTED://Send Address and W/R bit
        {

            //SSPBUF = 0x67;
            //Test
            //PORTE = 0x1;
            SSPBUF = ((ic_ptr->slave_addr)<<1);
            ic_ptr->status = I2C_MASTER_ADD_SENT;
            break;
        }
        case I2C_MASTER_ADD_SENT://Send Data
        {
            //Test
            //PORTE = 0x2;
            if (SSPCON2bits.ACKSTAT == 0) //Slave ACK
            {

                //SSPBUF = ic_ptr->buffer[ic_ptr->outbufind];
                SSPBUF = ic_ptr->outbuffer[ic_ptr->outbufind];
                (ic_ptr->outbufind)++;
                if (ic_ptr->outbuflen == ic_ptr->outbufind)ic_ptr->status = I2C_MASTER_DATA_SENT;
            }
            else if (SSPCON2bits.ACKSTAT == 1) //Slave did not ACK
            {
            }
            break;

        }
        case I2C_MASTER_DATA_SENT: //Send Stop
        {

            if (SSPCON2bits.ACKSTAT == 0) //Slave ACK
            {
                SSPCON2bits.PEN = 1;
                ic_ptr->status = I2C_MASTER_IDLE;
                ic_ptr->outbufind = 0;
            }
            break;
        }
        case I2C_MASTER_IDLE:
        {
            //PORTE = 0x3;
            break;
        }
        //////////////////////////////////////////////////////Recepetion
        case I2C_MASTER_RECEIVE_STARTED:
        {
            
            SSPBUF = (((ic_ptr->slave_addr)<<1)|0x1);
            ic_ptr->status = I2C_MASTER_ADD_SENT_R;
            break;
        }
        case I2C_MASTER_ADD_SENT_R:
        {
            //PORTE = 0x1;
            if (SSPCON2bits.ACKSTAT == 0)SSPCON2bits.RCEN = 1;
            ic_ptr->status = I2C_MASTER_RECEIVE_DATA;
            break;
        }

        case I2C_MASTER_RECEIVE_DATA:
        {
            
            //PORTE = 0x2;
            ic_ptr->buffer[ic_ptr->bufind] = SSPBUF;
            (ic_ptr->bufind)++;
            if (ic_ptr->buflen == ic_ptr->bufind)
            {
                SSPCON2bits.ACKDT = 1;//Acknowledge
                SSPCON2bits.ACKEN = 1;//Start Acknowledge sequence
                ic_ptr->status = I2C_MASTER_DATA_RECEIVED;
                //PORTE = 0x3;
            }
            else
            {
                SSPCON2bits.ACKEN = 1;//Start Acknowledge sequence
                SSPCON2bits.ACKDT = 0;
                ic_ptr->status = I2C_MASTER_ADD_SENT_R;
            }
            break;
        }
        case I2C_MASTER_DATA_RECEIVED:
        {
            //PORTE = 0x3;
            //Send stop bit
            SSPCON2bits.PEN = 1;
            ic_ptr->bufind = 0;
            ic_ptr->status = I2C_MASTER_IDLE;
            ToMainHigh_sendmsg(ic_ptr->buflen, MSGT_I2C_DATA, (void *) ic_ptr->buffer);
            break;
        }






    }

}


// set up the data structures for this i2c code
// should be called once before any i2c routines are called

void init_i2c(i2c_comm *ic) {
    ic_ptr = ic;
    ic_ptr->buflen = 0;
    ic_ptr->bufind = 0;
    ic_ptr->outbuflen = 0;
    ic_ptr->event_count = 0;
    ic_ptr->status = I2C_IDLE;
    ic_ptr->error_count = 0;

}





