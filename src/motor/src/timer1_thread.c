#include "maindefs.h"
#include "messages.h"
#include "timer1_thread.h"
#include "my_uart.h"

void init_timer1_lthread(timer1_thread_struct *tptr) {
    tptr->cmCount = 0;
    tptr->ctrl[0] = 90; //880
    tptr->ctrl[1] = 215; //205
    tptr->cmDist = 65458;
    tptr->curMove = 0;
    tptr->distLim = false;
    tptr->ready = true;
}

int timer1_lthread(timer1_thread_struct *tptr){
    tptr->cmCount++;

    if (tptr->distLim && ((tptr->cmCount % tptr->curMove) == (tptr->curMove-1))){
        unsigned char temp[2] = {0,0};
        sendUARTarr(2,temp);
        TIMER1_DISABLE;
        PORTA = 0x00;
        tptr->ready = true;
        tptr->cmCount = 0;
    }

    WriteTimer1(tptr->cmDist);
}