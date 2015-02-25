 #include "maindefs.h"
#include <stdio.h>
#include "messages.h"
#include "timer1_thread.h"
#include "my_uart.h"

void init_timer1_lthread(timer1_thread_struct *tptr) {
    tptr->msgcount = 0;
    tptr->even = 0;
    tptr->buffer[0] = 200;
    tptr->buffer[1] = 75;
}

// This is a "logical" thread that processes messages from TIMER1
// It is not a "real" thread because there is only the single main thread
// of execution on the PIC because we are not using an RTOS.

int timer1_lthread(timer1_thread_struct *tptr, int msgtype, int length, unsigned char *msgbuffer) {
    tptr->msgcount++;
    // Every second (6 timer1 interrupts), send a UART transmission message to
    if ((tptr->msgcount % 6) == 5) {
        tptr->even ^= 0x01;
        switch(tptr->even){
            case 0:
                tptr->buffer[0] = 0;
                tptr->buffer[1] = 0;
                break;
            case 1:
                tptr->buffer[0] = 200;
                tptr->buffer[1] = 72;
                break;
        }

        sendUART(2, tptr->buffer);
    }
}