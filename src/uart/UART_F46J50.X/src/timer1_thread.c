 #include "maindefs.h"
#include <stdio.h>
#include "messages.h"
#include "timer1_thread.h"

void init_timer1_lthread(timer1_thread_struct *tptr) {
    tptr->msgcount = 0;
}

// This is a "logical" thread that processes messages from TIMER1
// It is not a "real" thread because there is only the single main thread
// of execution on the PIC because we are not using an RTOS.

int timer1_lthread(timer1_thread_struct *tptr, int msgtype, int length, unsigned char *msgbuffer) {
    signed char retval;

    unsigned char ctrl[2] = {200,72};
    int even = 0;

    tptr->msgcount++;
    // Every second (6 timer1 interrupts), send a UART transmission message to
    //if ((tptr->msgcount % 6) == 5) {
        if(ReadTimer1()%2 == 0) even = 0;
        else even = 1;
        switch(even){
            case 0:
                ctrl[0] = 0;
                ctrl[1] = 0;
                break;
            case 1:
                ctrl[0] = 200;
                ctrl[1] = 72;
                break;
        }

        retval = ToMainLow_sendmsg(sizeof (tptr->msgcount), MSGT_UART_TX, (void *) &ctrl);
        if (retval < 0) {
            // We would handle the error here
        }
    // }
}