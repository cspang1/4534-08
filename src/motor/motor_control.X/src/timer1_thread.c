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
    quad_lookup_init(tptr->quadpos);
}

// This is a "logical" thread that processes messages from TIMER1
// It is not a "real" thread because there is only the single main thread
// of execution on the PIC because we are not using an RTOS.

int timer1_lthread(timer1_thread_struct *tptr, int msgtype, int length, unsigned char *msgbuffer) {
    tptr->msgcount++;
    char index = msgbuffer[0] + (msgbuffer[1]<<1) + (tptr->prevPos[0]<<3) + (tptr->prevPos[1]<<2);

    tptr->prevPos[0] = msgbuffer[0];
    tptr->prevPos[1] = msgbuffer[1];
    /* Every 10
    if ((tptr->msgcount % 2500) == 2499) {
        sendUARTbyte('Q');
    }*/
}

void quad_lookup_init(char* table){
    table[0] = 0;
    table[1] = -1;
    table[2] = 1;
    table[3] = -2;
    table[4] = 1;
    table[5] = 0;
    table[6] = -2;
    table[7] = -1;
    table[8] = -1;
    table[9] = -2;
    table[10] = 0;
    table[11] = 1;
    table[12] = -2;
    table[13] = 1;
    table[14] = -1;
    table[15] = 0;
    table[16] = 0;
    table[17] = -1;
    table[18] = 1;
    table[19] = 2;
    table[20] = 1;
    table[21] = 0;
    table[22] = 2;
    table[23] = -1;
    table[24] = -1;
    table[25] = 2;
    table[26] = 0;
    table[27] = 1;
    table[28] = 2;
    table[29] = 1;
    table[30] = -1;
    table[31] = 0;
}