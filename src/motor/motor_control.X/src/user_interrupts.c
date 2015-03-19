// This is where the "user" interrupts handlers should go
// The *must* be declared in "user_interrupts.h"

#include "maindefs.h"
#ifndef __XC8
#include <timers.h>
#else
#include <plib/timers.h>
#endif
#include "user_interrupts.h"
#include "messages.h"
#include "my_uart.h"

// A function called by the interrupt handler
// This one does the action I wanted for this program on a timer0 interrupt

void timer0_int_handler() {
    unsigned int val;
    int length, msgtype;

    // toggle an LED
#ifdef __USE18F2680
    LATBbits.LATB0 = !LATBbits.LATB0;
#endif
}

// A function called by the interrupt handler
// This one does the action I wanted for this program on a timer1 interrupt

void timer1_int_handler() {
    unsigned int result;

    // read the timer and then send an empty message to main()
#ifdef __USE18F2680
    LATBbits.LATB1 = !LATBbits.LATB1;
#endif

    // reset the timer
    WriteTimer1(65390);

    unsigned char curPos[2];
    curPos[0] = PORTAbits.RA0;
    curPos[1] = PORTAbits.RA1;

    // Send message to main
    ToMainLow_sendmsg(2, MSGT_TIMER1, (void *)curPos);
}