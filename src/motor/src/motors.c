#include "motors.h"
#include "timer1_thread.h"
#include "timer0_thread.h"
#include "my_uart.h"
#include "maindefs.h"

void move(timer1_thread_struct *tptr){
    unsigned char test[2];
    test[0] = tptr->ctrl[0];
    test[1] = tptr->ctrl[1];
    TIMER1_ENABLE;
    sendUARTarr(2,test);
}

void moveDist(timer1_thread_struct *tptr, int dist){
    tptr->distLim = true;
    tptr->curMove = dist;
    unsigned char test[2];
    test[0] = tptr->ctrl[0];
    test[1] = tptr->ctrl[1];
    TIMER1_ENABLE;
    sendUARTarr(2,test);
    WriteTimer1(tptr->cmDist);
}

void turn(timer0_thread_struct *tptr, int angle){
    tptr->curMove = angle;
    tptr->distLim = true;
    TIMER0_ENABLE;
    unsigned char test[2];
    test[0] = tptr->ctrl[0];
    test[1] = tptr->ctrl[1];
    sendUARTarr(2,test);
    WriteTimer0(tptr->cmDist);
}

//64 -> 16 = 48
//192 -> 13 = 179

void stop(timer1_thread_struct *tptr){
    unsigned char test[2] = {0,0};
    sendUARTarr(2,test);
    WriteTimer1(tptr->cmDist);
    TIMER0_DISABLE;
    TIMER1_DISABLE;
}