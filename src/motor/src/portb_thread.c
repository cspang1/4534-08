#include "maindefs.h"
#include <stdio.h>
#include "portb_thread.h"
#include "messages.h"

// Initializer for UART thread
void init_portb_lthread(portb_thread_struct *bptr){
    bptr->dist = 0;
    bptr->msgcount = 0;
}

// This is a "logical" thread that processes messages from the UART
// It is not a "real" thread because there is only the single main thread
// of execution on the PIC because we are not using an RTOS.
int portb_lthread(portb_thread_struct *bptr) {
    bptr->msgcount++;
    //if((bptr->msgcount % 10785) == 10784)
    LATA = !PORTA;
}
