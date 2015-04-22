#include "maindefs.h"
#include <stdio.h>
#include "timer0_thread.h"
#include "messages.h"
#include "my_uart.h"

void init_timer0_lthread(timer0_thread_struct *tptr){
    tptr->cmCount = 0;
    tptr->ctrlL[0] = 0x20;
    tptr->ctrlL[1] = 0xE0;
    tptr->ctrlR[0] = 0x92;
    tptr->ctrlR[1] = 0x5F;
    tptr->cmDist = 65506;
    tptr->curMove = 0;
    tptr->distLim = false;
    tptr->ready = true;
}

int timer0_lthread(timer0_thread_struct *tptr){
    tptr->cmCount++;

    if (tptr->distLim && ((tptr->cmCount % tptr->curMove) == (tptr->curMove-1))){
        unsigned char temp[2] = {0,0};
        sendUARTarr(2,temp);
        TIMER0_DISABLE;
        PORTA = 0x00;
        tptr->ready = true;
        tptr->cmCount = 0;
    }

    WriteTimer0(tptr->cmDist);
}