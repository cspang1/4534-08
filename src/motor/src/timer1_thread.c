#include "maindefs.h"
#include <stdio.h>
#include "messages.h"
#include "timer1_thread.h"
#include "my_uart.h"

void init_timer1_lthread(timer1_thread_struct *tptr) {
    tptr->cmCount = 0;
    tptr->ctrl[0] = 80;
    tptr->ctrl[1] = 205;
    tptr->cmDist = DELTA_CM;
    tptr->curMove = 0;
    tptr->distLim = false;
}

int timer1_lthread(timer1_thread_struct *tptr){
    tptr->cmCount++;

    if (tptr->distLim && ((tptr->cmCount % tptr->curMove) == (tptr->curMove-1))){
        unsigned char temp[2] = {0,0};
        sendUARTarr(2,temp);
        TIMER1_DISABLE;
    }

    WriteTimer1(tptr->cmDist);
}