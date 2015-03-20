#include "motors.h"
#include "timer1_thread.h"

void move(timer1_thread_struct *tptr){
    unsigned char test[2] = {80,205};
    sendUARTarr(2,test);
    tptr->distLim = false;
    tptr->turning = false;
}

void moveDist(timer1_thread_struct *tptr, int dist){
    tptr->curMove = dist;
    unsigned char test[2] = {80,205};
    sendUARTarr(2,test);
    WriteTimer1(tptr->cmDist);
    tptr->turning = false;
    tptr->distLim = true;
}

void turn(timer1_thread_struct *tptr, int angle){
    // TODO
}