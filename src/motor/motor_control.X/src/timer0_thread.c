#include "maindefs.h"
#include <stdio.h>
#include "timer0_thread.h"
#include "my_uart.h"

// This is a "logical" thread that processes messages from TIMER0
// It is not a "real" thread because there is only the single main thread
// of execution on the PIC because we are not using an RTOS.

void init_timer0_lthread(timer0_thread_struct *tptr){
    tptr->msgcount = 0;
}

int timer0_lthread(timer0_thread_struct *tptr, int msgtype, int length, unsigned char *msgbuffer) {
    unsigned int *msgval;
    msgval = (unsigned int *) msgbuffer;

    tptr->msgcount++;

    if((tptr->msgcount % 60) == 59) {
        unsigned char temp[2] = {199,72};
        sendUARTarr(2,temp);
    }

    
    // Here is where we would do something with the message

}