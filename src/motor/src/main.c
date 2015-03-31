#include "maindefs.h"
#include <stdio.h>
#include <plib/usart.h>
#include <plib/i2c.h>
#include <plib/timers.h>
#include "interrupts.h"
#include "messages.h"
#include "my_uart.h"
#include "my_i2c.h"
#include "uart_thread.h"
#include "timer1_thread.h"
#include "timer0_thread.h"
#include "portb_thread.h"
#include "motors.h"

//Setup configuration registers
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled (control is placed on SWDTEN bit))
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable bit (Reset on stack overflow/underflow disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode enabled)
#pragma config CP0 = OFF        // Code Protection bit (Program memory is not code-protected)
#pragma config FOSC = HSPLL     // Oscillator Selection bits (HS oscillator, PLL enabled and under software control)
#pragma config FOSC2 = ON       // Default/Reset System Clock Select bit (Clock selected by FOSC as system clock is enabled when OSCCON<1:0> = 00)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = ON        // Two-Speed Start-up (Internal/External Oscillator Switchover) Control bit (Two-Speed Start-up enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)
#pragma config CCP2MX = DEFAULT // CCP2 MUX bit (CCP2 is multiplexed with RC1)

void main(void) {
    signed char length;
    unsigned char msgtype;
    unsigned char last_reg_recvd;
    unsigned char baud, UARTConfig;
    uart_comm uc;
    i2c_comm ic;
    unsigned char msgbuffer[MSGLEN + 1];
    uart_thread_struct uthread_data; // info for uart_lthread
    timer1_thread_struct t1thread_data; // info for timer1_lthread
    timer0_thread_struct t0thread_data; // info for timer0_lthread

    // Setup PORTA for output
    PORTA = 0x0;	// clear the port
    LATA = 0x0;		// clear the output latch
    ADCON1 = 0x0F;	// turn off the A2D function on these pins
    CMCON = 0x07;	// turn the comparator off
    TRISA = 0x00;	// set RA3-RA0 to outputs

    // Setup PORTB for input
    PORTB = 0x0;	// clear the port
    LATB = 0x0;		// clear the output latch
    TRISB = 0xFF;	// set RB3-RB0 to inputs

    OSCCON = 0x82; // see datasheeet
    OSCTUNEbits.PLLEN = 1; // Makes the clock exceed the PIC's rated speed if the PLL is on

    // initialize my uart recv handling code
    init_uart_recv(&uc);

    // initialize the i2c code
    init_i2c(&ic);

    // init the timer1 lthread
    init_timer1_lthread(&t1thread_data);

    // init the timer1 lthread
    init_timer0_lthread(&t0thread_data);
    
    // init the UART lthread
    init_uart_lthread(&uthread_data);

    // initialize message queues before enabling any interrupts
    init_queues();
     
    // initialize Timers
    // Set Timer0's source as external (quadrature encoder)
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_EXT & T0_EDGE_RISE & T0_PS_1_1);
    OpenTimer1(TIMER_INT_ON & T1_PS_1_1 & T1_16BIT_RW & T1_SOURCE_EXT & T1_OSC1EN_OFF);
    TIMER0_DISABLE;
    TIMER1_DISABLE;

    // configure the hardware USART device
    UARTConfig = USART_TX_INT_ON & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_BRGH_LOW;
    baud = 77;
    OpenUSART(UARTConfig,baud);

    // Decide on the priority of the enabled peripheral interrupts
    // 0 is low, 1 is high
    // Timer1 interrupt
    IPR1bits.TMR1IP = 0;
    // USART RC interrupt
    IPR1bits.RCIP = 0;
    // USART TX interrupt
    IPR1bits.TXIP = 0;
    // I2C interrupt
    IPR1bits.SSPIP = 1;
    
    // configure the hardware i2c device as a slave (0x9E -> 0x4F) or (0x9A -> 0x4D)
    i2c_configure_slave(0x9A);

    // must specifically enable the I2C interrupts
    PIE1bits.SSPIE = 1;

    // Peripheral interrupts can have their priority set to high or low
    // enable high-priority interrupts and low-priority interrupts
    enable_interrupts();

    // This API call instructs the rover to move forward 168 cm
    //turn(&t0thread_data, 90);
    //122

    // loop forever
    while (1) {
        // Block while no messages in queue
        block_on_To_msgqueues();

        // Check high priority message queue
        length = ToMainHigh_recvmsg(MSGLEN, &msgtype, (void *) msgbuffer);
        if (length < 0) {
            // no message, check the error code to see if it is concern
            if (length != MSGQUEUE_EMPTY) {
                // This case be handled by your code.
            }
        } else {
            switch (msgtype) {
                case MSGT_TIMER0:
                {
                    timer0_lthread(&t0thread_data);
                    break;
                };
                case MSGT_I2C_DATA:
                {
                    last_reg_recvd = msgbuffer[0];
                    switch(last_reg_recvd){
                        case 0x01:
                        {
                            move(&t1thread_data);
                            break;
                        }
                        case 0x02:
                        {
                            turn(&t0thread_data, 270);
                            break;
                        }
                        case 0x03:
                        {
                            turn(&t0thread_data, 90);
                            break;
                        }
                        case 0x04:
                        {
                            stop(&t1thread_data);
                            break;
                        }
                    }
                    break;
                }
                case MSGT_I2C_DBG:
                {
                    // Here is where you could handle debugging, if you wanted
                    // keep track of the first byte received for later use (if desired)
                    last_reg_recvd = msgbuffer[0];
                    break;
                };
                case MSGT_I2C_RQST:
                {
                    // Generally, this is *NOT* how I recommend you handle an I2C slave request
                    // I recommend that you handle it completely inside the i2c interrupt handler
                    // by reading the data from a queue (i.e., you would not send a message, as is done
                    // now, from the i2c interrupt handler to main to ask for data).
                    //
                    // The last byte received is the "register" that is trying to be read
                    // The response is dependent on the register.
                    switch (last_reg_recvd) {
                        case 0xaa:
                        {
                            length = 2;
                            msgbuffer[0] = 0x55;
                            msgbuffer[1] = 0xAA;
                            break;
                        }
                        case 0xa8:
                        {
                            length = 1;
                            msgbuffer[0] = 0x3A;
                            break;
                        }
                        case 0xa9:
                        {
                            length = 1;
                            msgbuffer[0] = 0xA3;
                            break;
                        }
                    };
                    start_i2c_slave_reply(length, msgbuffer);
                    break;
                };
                default:
                {
                    // Your code should handle this error
                    break;
                };
            };
        }

        // Check the low priority queue
        length = ToMainLow_recvmsg(MSGLEN, &msgtype, (void *) msgbuffer);
        if (length < 0) {
            // no message, check the error code to see if it is concern
            if (length != MSGQUEUE_EMPTY) {
                // Your code should handle this situation
            }
        } else {
            switch (msgtype) {
                case MSGT_TIMER1:
                {
                    timer1_lthread(&t1thread_data);
                    break;
                };
                case MSGT_OVERRUN:
                case MSGT_UART_TX:
                case MSGT_UART_RC:
                {
                    uart_lthread(&uthread_data, msgtype, length, msgbuffer);
                    break;
                };
                default:
                {
                    // Your code should handle this error
                    break;
                };
            };
        }
    }

}