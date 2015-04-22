#include <stdlib.h>
#include "motors.h"
#include "timer1_thread.h"
#include "timer0_thread.h"
#include "my_uart.h"
#include "maindefs.h"

void move(timer1_thread_struct *tptr){
    PORTA = 0x01;
    tptr->distLim = false;
    unsigned char test[2];
    test[0] = tptr->ctrl[0];
    test[1] = tptr->ctrl[1];
    TIMER1_ENABLE;
    sendUARTarr(2,test);
    tptr->cmCount = 0;
}

void moveDist(timer1_thread_struct *tptr, int dist){
    PORTA = 0x01;
    tptr->distLim = true;
    tptr->curMove = dist;
    unsigned char test[2];
    test[0] = tptr->ctrl[0];
    test[1] = tptr->ctrl[1];
    TIMER1_ENABLE;
    sendUARTarr(2,test);
    WriteTimer1(tptr->cmDist);
    tptr->cmCount = 0;
}

void turn(timer0_thread_struct *tptr, int angle){
    PORTA = 0x01;
    tptr->curMove = abs(angle);
    tptr->distLim = true;
    TIMER0_ENABLE;
    unsigned char test[2];
    test[0] = (angle < 0) ? tptr->ctrlR[0] : tptr->ctrlL[0];
    test[1] = (angle < 0) ? tptr->ctrlR[1] : tptr->ctrlL[1];
    sendUARTarr(2,test);
    WriteTimer0(tptr->cmDist);
    tptr->cmCount = 0;
}

void stop(timer1_thread_struct *tptr){
    PORTA = 0x00;
    unsigned char test[2] = {0,0};
    sendUARTarr(2,test);
    WriteTimer1(tptr->cmDist);
    TIMER0_DISABLE;
    TIMER1_DISABLE;
    tptr->cmCount = 0;
}