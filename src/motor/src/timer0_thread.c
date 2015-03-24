#include "maindefs.h"
#include <stdio.h>
#include "timer0_thread.h"
#include "messages.h"
#include "my_uart.h"

void init_timer0_lthread(timer0_thread_struct *tptr){
    tptr->cmCount = 0;
    tptr->ctrl[0] = 80;
    tptr->ctrl[1] = 205;
    tptr->cmDist = DELTA_CM;
    tptr->curMove = 0;
    tptr->distLim = false;
}

int timer0_lthread(timer0_thread_struct *tptr){
    tptr->cmCount++;

    if (tptr->distLim && ((tptr->cmCount % tptr->curMove) == (tptr->curMove-1))){
        unsigned char temp[2] = {0,0};
        sendUARTarr(2,temp);
        TIMER0_DISABLE;
    }

    WriteTimer0(tptr->cmDist);
}