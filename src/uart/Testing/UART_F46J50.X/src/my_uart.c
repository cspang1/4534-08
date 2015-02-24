#include "maindefs.h"
#ifndef __XC8
#include <usart.h>
#else
#include <plib/usart.h>
#endif
#include "my_uart.h"

static uart_comm *uc_ptr;

void uart_trans_int_handler() {
    // Check the low priority queue
    unsigned char* data = NULL;
    unsigned char msgtype;
    unsigned char curIndex;
    signed char length = FromMainLow_recvmsg(MSGLEN, &msgtype, (void *) data);
    if (length < 0) {
        // no message, check the error code to see if it is concern
        if (length != MSGQUEUE_EMPTY) {
            // This will be populated when we implement error-checking
        }
        else{
            // This will be populated when we implement error-checking
        }
    } else {
        curIndex = 0;
        while(curIndex < length){
#ifdef __USE18F46J50
            while(Busy1USART());
            TXREG1 = data[curIndex];
#else
            while(BusyUSART());
            TXREG = data[curIndex];
#endif
            curIndex++;
        }
    }

    PIE1bits.TXIE = 0;
}

void uart_recv_int_handler() {
#ifdef __USE18F26J50
    if (DataRdy1USART()) {
        uc_ptr->buffer[uc_ptr->buflen] = Read1USART();
#else
#ifdef __USE18F46J50
    if (DataRdy1USART()) {
        uc_ptr->buffer[uc_ptr->buflen] = Read1USART();
#else
    if (DataRdyUSART()) {
        uc_ptr->buffer[uc_ptr->buflen] = ReadUSART();
#endif
#endif

        uc_ptr->buflen++;
        // check if a message should be sent
        if (uc_ptr->buflen == MAXUARTBUF) {
            ToMainLow_sendmsg(uc_ptr->buflen, MSGT_UART_RC, (void *) uc_ptr->buffer);
            uc_ptr->buflen = 0;
        }
    }
#ifdef __USE18F26J50
    if (USART1_Status.OVERRUN_ERROR == 1) {
#else
#ifdef __USE18F46J50
    if (USART1_Status.OVERRUN_ERROR == 1) {
#else
    if (USART_Status.OVERRUN_ERROR == 1) {
#endif
#endif
        // we've overrun the USART and must reset
        // send an error message for this
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
        ToMainLow_sendmsg(0, MSGT_OVERRUN, (void *) 0);
    }
}

void init_uart_recv(uart_comm *uc) {
    uc_ptr = uc;
    uc_ptr->buflen = 0;
}

BOOL sendUART(unsigned char length, unsigned char* data){
    signed char retval = ToMainLow_sendmsg(length, MSGT_UART_TX, (void *)data);
    if (retval < 0) {
        // This will be populated when we implement error-checking
        return false;
    }
    return true;
}