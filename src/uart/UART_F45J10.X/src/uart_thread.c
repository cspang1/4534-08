#include "maindefs.h"
#include <stdio.h>
#include "uart_thread.h"
#include "messages.h"

void init_uart_lthread(uart_thread_struct *uptr) {
}

// This is a "logical" thread that processes messages from the UART
// It is not a "real" thread because there is only the single main thread
// of execution on the PIC because we are not using an RTOS.
int uart_lthread(uart_thread_struct *uptr, int msgtype, int length, unsigned char *msgbuffer) {
    if (msgtype == MSGT_OVERRUN) {
    }
    /*************************************
     * To transmit a USART message, send a
     * ToMainLow message of msgtype MSGT_UART_TX
     * from wherever you want to send a message,
     * containing the data you want to send
     *///////////////////////////////////////
    else if(msgtype == MSGT_UART_TX){
        // Copy contents of msgbuffer
        int index = 0;
        for(index = 0; index < 10; index++) uptr->data[index] = msgbuffer[index];

        // Send message to interrupt
        signed char status = FromMainLow_sendmsg(length, msgtype, (void*)uptr->data);
        if(status < 0){
            // Handle error here
        }

        PIE1bits.TXIE = 1;
    }
    else if(msgtype == MSGT_UART_RC){

        // print the message (this assumes that the message
        // 		was a printable string)
        msgbuffer[length] = '\0'; // null-terminate the array as a string

        /*************************************************
         * Here is where you will send a ToMainLow message
         * to create a thread to process the UART message
         * OR: simply handle the transmission here
         *////////////////////////////////////////////////
    }
}