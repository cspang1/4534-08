#include "maindefs.h"
#include <stdio.h>
#include "messages.h"
#include "timer1_thread.h"
#include "my_uart.h"

void init_timer1_lthread(timer1_thread_struct *tptr) {
    tptr->msgcount = 0;
    tptr->ctrl[0] = 182;
    tptr->ctrl[1] = 54;
    tptr->prevPos[0] = 0;
    tptr->prevPos[1] = 0;
    tptr->current_position = 0;
    tptr->direction = 0;
    tptr->cmDist = 65535-78;
    tptr->curMove = 0;
    tptr->turning = false;
    tptr->distLim = false;
}

// This is a "logical" thread that processes messages from TIMER1
// It is not a "real" thread because there is only the single main thread
// of execution on the PIC because we are not using an RTOS.

int timer1_lthread(timer1_thread_struct *tptr, int msgtype, int length, unsigned char *msgbuffer) {
    tptr->msgcount++;

    if(tptr->distLim){
        switch(tptr->turning){
            case false:
            {
                if ((tptr->msgcount % tptr->curMove) == tptr->curMove-1) {
                    unsigned char temp[2] = {0,0};
                    sendUARTarr(2,temp);
                }
                break;
            }
            case true:
            {
                /* TURN TEST */
                // Every 22.38385 cm
                if ((tptr->msgcount % 45) == 44) {
                    unsigned char temp[2] = {0,0};
                    sendUARTarr(2,temp);
                    tptr->msgcount = 0;
                }
                break;
            }
        }
        tptr->distLim = false;
    }
    WriteTimer1(tptr->cmDist);
}