#include "maindefs.h"
#include <stdio.h>
#include <plib/usart.h>
#include <plib/i2c.h>
#include <plib/timers.h>
#include "interrupts.h"
#include "messages.h"
#include "my_uart.h"
#include "my_i2c.h"
#include "uart_thread.h"
#include "timer1_thread.h"
#include "timer0_thread.h"
#include "parser.h"
#include "brain.h"

//Setup configuration registers
#ifdef __USE18F45J10
// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled (control is placed on SWDTEN bit))
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable bit (Reset on stack overflow/underflow disabled)
#ifndef __XC8
// Have to turn this off because I don't see how to enable this in the checkboxes for XC8 in this IDE
#pragma config XINST = ON       // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode enabled)
#else
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode enabled)
#endif

// CONFIG1H
#pragma config CP0 = OFF        // Code Protection bit (Program memory is not code-protected)

// CONFIG2L
#pragma config FOSC = HSPLL     // Oscillator Selection bits (HS oscillator, PLL enabled and under software control)
#pragma config FOSC2 = ON       // Default/Reset System Clock Select bit (Clock selected by FOSC as system clock is enabled when OSCCON<1:0> = 00)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = ON        // Two-Speed Start-up (Internal/External Oscillator Switchover) Control bit (Two-Speed Start-up enabled)

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = DEFAULT // CCP2 MUX bit (CCP2 is multiplexed with RC1)

#else

#ifdef __USE18F2680
#pragma config OSC = IRCIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (VBOR set to 2.1V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config BBSIZ = 1024     // Boot Block Size Select bits (1K words (2K bytes) Boot Block)
#ifndef __XC8
// Have to turn this off because I don't see how to enable this in the checkboxes for XC8 in this IDE
#pragma config XINST = ON       // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode enabled)
#endif

#else

#ifdef __USE18F26J50

// PIC18F26J50 Configuration Bit Settings

// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer (Disabled - Controlled by SWDTEN bit)
#pragma config PLLDIV = 3       // PLL Prescaler Selection bits (Divide by 3 (12 MHz oscillator input))
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset  (Disabled)
#pragma config XINST = ON       // Extended Instruction Set (Enabled)

// CONFIG1H
#pragma config CPUDIV = OSC1    // CPU System Clock Postscaler (No CPU system clock divide)
#pragma config CP0 = OFF        // Code Protect (Program memory is not code-protected)

// CONFIG2L
#pragma config OSC = HSPLL      // Oscillator (HS+PLL, USB-HS+PLL)
#pragma config T1DIG = OFF      // T1OSCEN Enforcement (Secondary Oscillator clock source may not be selected)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator (High-power operation)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = ON        // Internal External Oscillator Switch Over Mode (Enabled)

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Postscaler (1:32768)

// CONFIG3L
#pragma config DSWDTOSC = T1OSCREF// DSWDT Clock Select (DSWDT uses T1OSC/T1CKI)
#pragma config RTCOSC = T1OSCREF// RTCC Clock Select (RTCC uses T1OSC/T1CKI)
#pragma config DSBOREN = OFF    // Deep Sleep BOR (Disabled)
#pragma config DSWDTEN = OFF    // Deep Sleep Watchdog Timer (Disabled)
#pragma config DSWDTPS = G2     // Deep Sleep Watchdog Postscaler (1:2,147,483,648 (25.7 days))

// CONFIG3H
#pragma config IOL1WAY = ON     // IOLOCK One-Way Set Enable bit (The IOLOCK bit (PPSCON<0>) can be set once)
#pragma config MSSP7B_EN = MSK7 // MSSP address masking (7 Bit address masking mode)

// CONFIG4L
#pragma config WPFP = PAGE_63   // Write/Erase Protect Page Start/End Location (Write Protect Program Flash Page 63)
#pragma config WPEND = PAGE_WPFP// Write/Erase Protect Region Select (valid when WPDIS = 0) (Page WPFP<5:0> through Configuration Words erase/write protected)
#pragma config WPCFG = OFF      // Write/Erase Protect Configuration Region (Configuration Words page not erase/write-protected)

// CONFIG4H
#pragma config WPDIS = OFF      // Write Protect Disable bit (WPFP<5:0>/WPEND region ignored)

#else

#ifdef __USE18F46J50

// PIC18F46J50 Configuration Bit Settings

// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer (Disabled - Controlled by SWDTEN bit)
#pragma config PLLDIV = 3       // PLL Prescaler Selection bits (Divide by 3 (12 MHz oscillator input))
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset (Disabled)
#ifndef __XC8
// Have to turn this off because I don't see how to enable this in the checkboxes for XC8 in this IDE
#pragma config XINST = ON       // Extended Instruction Set (Enabled)
#else
#pragma config XINST = OFF      // Extended Instruction Set (Disabled)
#endif

// CONFIG1H
#pragma config CPUDIV = OSC1    // CPU System Clock Postscaler (No CPU system clock divide)
#pragma config CP0 = OFF        // Code Protect (Program memory is not code-protected)

// CONFIG2L
#pragma config OSC = HSPLL      // Oscillator (HS+PLL, USB-HS+PLL)
#pragma config T1DIG = OFF      // T1OSCEN Enforcement (Secondary Oscillator clock source may not be selected)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator (High-power operation)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = ON        // Internal External Oscillator Switch Over Mode (Enabled)

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Postscaler (1:32768)

// CONFIG3L
#pragma config DSWDTOSC = T1OSCREF// DSWDT Clock Select (DSWDT uses T1OSC/T1CKI)
#pragma config RTCOSC = T1OSCREF// RTCC Clock Select (RTCC uses T1OSC/T1CKI)
#pragma config DSBOREN = OFF    // Deep Sleep BOR (Disabled)
#pragma config DSWDTEN = OFF    // Deep Sleep Watchdog Timer (Disabled)
#pragma config DSWDTPS = G2     // Deep Sleep Watchdog Postscaler (1:2,147,483,648 (25.7 days))

// CONFIG3H
#pragma config IOL1WAY = ON     // IOLOCK One-Way Set Enable bit (The IOLOCK bit (PPSCON<0>) can be set once)
#pragma config MSSP7B_EN = MSK7 // MSSP address masking (7 Bit address masking mode)

// CONFIG4L
#pragma config WPFP = PAGE_63   // Write/Erase Protect Page Start/End Location (Write Protect Program Flash Page 63)
#pragma config WPEND = PAGE_WPFP// Write/Erase Protect Region Select (valid when WPDIS = 0) (Page WPFP<5:0> through Configuration Words erase/write protected)
#pragma config WPCFG = OFF      // Write/Erase Protect Configuration Region (Configuration Words page not erase/write-protected)

// CONFIG4H
#pragma config WPDIS = OFF      // Write Protect Disable bit (WPFP<5:0>/WPEND region ignored)

#else

Something is messed up.
The PIC selected is not supported or the preprocessor directives are wrong.

#endif
#endif
#endif
#endif



#define _XTAL_FREQ 12000000
void Delay1Second()
{
    char i;
    for(i=0;i<100;i++)
    {
         __delay_ms(10);
    }
}
#define SIZEOFCOMMANDS 26
#define littleF 35
#define midF 55
#define bigF 60

void main(void) {
    char c;
    signed char length;
    signed char lengthl;
    unsigned char msgtype;
    unsigned char last_reg_recvd;
    uart_comm uc;
    i2c_comm ic;
    i2c_comm ic2;
    brain myBrain;
    unsigned char msgbuffer[MSGLEN + 1];
    unsigned char i;
    uart_thread_struct uthread_data; // info for uart_lthread
    timer1_thread_struct t1thread_data; // info for timer1_lthread
    timer0_thread_struct t0thread_data; // info for timer0_lthread
    parser_t parser;


#ifdef __USE18F2680
    OSCCON = 0xFC; // see datasheet
    // We have enough room below the Max Freq to enable the PLL for this chip
    OSCTUNEbits.PLLEN = 1; // 4x the clock speed in the previous line
#else
#ifdef __USE18F45J10
    OSCCON = 0x82; // see datasheeet
    OSCTUNEbits.PLLEN = 0; // Makes the clock exceed the PIC's rated speed if the PLL is on
#else
#ifdef __USE18F26J50
    OSCCON = 0xE0; // see datasheeet
    OSCTUNEbits.PLLEN = 1;
#else
#ifdef __USE18F46J50
    OSCCON = 0xE0; //see datasheet
    OSCTUNEbits.PLLEN = 1;
#else
    Something is messed up.
    The PIC selected is not supported or the preprocessor directives are wrong.
#endif
#endif
#endif
#endif

    // initialize my uart recv handling code
    init_uart_recv(&uc);

    // initialize the i2c code
    init_i2c(&ic);

    // init the timer1 lthread
    init_timer1_lthread(&t1thread_data);

    // initialize message queues before enabling any interrupts
    init_queues();

#ifndef __USE18F26J50
    // set direction for PORTB to output
    TRISB = 0x0;
    LATB = 0x0;
#endif

    // how to set up PORTA for input (for the V4 board with the PIC2680)
    /*
            PORTA = 0x0;	// clear the port
            LATA = 0x0;		// clear the output latch
            ADCON1 = 0x0F;	// turn off the A2D function on these pins
            // Only for 40-pin version of this chip CMCON = 0x07;	// turn the comparator off
            TRISA = 0x0F;	// set RA3-RA0 to inputs
     */

    // initialize Timers
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_128);

#ifdef __USE18F26J50
    // MTJ added second argument for OpenTimer1()
    OpenTimer1(TIMER_INT_ON & T1_SOURCE_FOSC_4 & T1_PS_1_8 & T1_16BIT_RW & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF,0x0);
#else
#ifdef __USE18F46J50
    OpenTimer1(TIMER_INT_ON & T1_SOURCE_FOSC_4 & T1_PS_1_8 & T1_16BIT_RW & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF,0x0);
#else
    OpenTimer1(TIMER_INT_ON & T1_PS_1_8 & T1_16BIT_RW & T1_SOURCE_INT & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF);
#endif
#endif

    unsigned char UARTConfig, baud;
    // configure the hardware USART device
#ifdef __USE18F46J50
    UARTConfig = USART_TX_INT_ON & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_LOW;
    baud = 12;
    Open1USART(UARTConfig, baud);
#else
    UARTConfig = USART_TX_INT_ON & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_BRGH_HIGH ;
    baud = 77;
    OpenUSART(UARTConfig,baud);
#endif

    // Decide on the priority of the enabled peripheral interrupts
    // 0 is low, 1 is high
    // Timer1 interrupt
    IPR1bits.TMR1IP = 0;
    // USART RX interrupt
    IPR1bits.RCIP = 0;
    // USART TX interrupt
    IPR1bits.TXIP = 0;
    // I2C interrupt
    IPR1bits.SSPIP = 1;

    // configure the hardware i2c device as a slave (0x9E -> 0x4F) or (0x9A -> 0x4D)
#if 1
    // Note that the temperature sensor Address bits (A0, A1, A2) are also the
    // least significant bits of LATB -- take care when changing them
    // They *are* changed in the timer interrupt handlers if those timers are
    //   enabled.  They are just there to make the lights blink and can be
    //   disabled.
    i2c_configure_master();
    
    
#else
    // If I want to test the temperature sensor from the ARM, I just make
    // sure this PIC does not have the same address and configure the
    // temperature sensor address bits and then just stay in an infinite loop
    i2c_configure_slave(0x9A);
#ifdef __USE18F2680
    LATBbits.LATB1 = 1;
    LATBbits.LATB0 = 1;
    LATBbits.LATB2 = 1;
#endif
    for (;;);
#endif

    // must specifically enable the I2C interrupts
    PIE1bits.SSPIE = 1;

    // configure the hardware USART device


    // Peripheral interrupts can have their priority set to high or low
    // enable high-priority interrupts and low-priority interrupts
    enable_interrupts();

    /* Junk to force an I2C interrupt in the simulator (if you wanted to)
    PIR1bits.SSPIF = 1;
    _asm
    goto 0x08
    _endasm;
     */

    // printf() is available, but is not advisable.  It goes to the UART pin
    // on the PIC and then you must hook something up to that to view it.
    // It is also slow and is blocking, so it will perturb your code's operation
    // Here is how it looks: printf("Hello\r\n");

    TRISE = 0x0;
    TRISE |= 0x4;
    ANCON0bits.PCFG7 = 1;
    TRISAbits.TRISA0 = 1;
    ANCON0bits.PCFG0 = 1;

    
    char count = 0;
    parser.McurSeq = 0;
    parser.MexpectedSeq = 0;
    parser.PcurSeq = 0;
    parser.PexpectedSeq = 0;
    parser.Merr = 0;
    parser.Perr = 0;
    parser.sysState = INIT;
    parser.superInt = 0;
    myBrain.column = 1;
    myBrain.command =0;
    myBrain.value = 0;
    myBrain.decision = REST;
    myBrain.o_theta = 0;
    myBrain.s1 = 0;
    myBrain.s2 = 0;
    myBrain.switcher = IDLES;
    myBrain.tilt = IDLES;
    myBrain.curPortion = UP;
    myBrain.prevPortion = UP;
    unsigned char rampBounc = 0;
//    myBrain.stopped = 1;

    //unsigned char cal[SIZEOFCOMMANDS] = {FRONT,FRONT,FRONT,FRONT,FRONT,FRONT,LEFT,LEFT,FRONT,LEFT,LEFT,FRONT,FRONT,FRONT,FRONT,FRONT,FRONT,RIGHT,RIGHT,FRONT,RIGHT,RIGHT,FRONT,FRONT,FRONT,FRONT,FRONT,FRONT,0};
    //unsigned char val[SIZEOFCOMMANDS] = {bigF,bigF,bigF,bigF,bigF,littleF,45,45,midF,45,45,bigF,bigF,bigF,bigF,bigF,littleF,45,45,midF,45,45,bigF,bigF,bigF,bigF,bigF,littleF,0};
    unsigned char cal[SIZEOFCOMMANDS] = {FRONT,FRONT,FRONT,FRONT,FRONT,FRONT,LEFT,FRONT,FRONT,LEFT,FRONT,FRONT,FRONT,FRONT,FRONT,FRONT,LEFT,FRONT,LEFT,FRONT,FRONT,FRONT,FRONT,FRONT,FRONT,0};
    unsigned char val[SIZEOFCOMMANDS] = {littleF,bigF,bigF,bigF,bigF,bigF-2,0,midF,midF,0,littleF,bigF,bigF,bigF,bigF,bigF,0,midF,0,littleF,bigF,bigF,bigF,bigF,bigF,0};
    unsigned char ramps[SIZEOFCOMMANDS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    //unsigned char cal[SIZEOFCOMMANDS] = {FRONT,FRONT,FRONT,FRONT,FRONT,FRONT,LEFT,0};
    //unsigned char val[SIZEOFCOMMANDS] = {bigF,bigF,bigF,bigF,bigF,littleF,0,0};
    //unsigned char ramps[SIZEOFCOMMANDS] = {0,0,0,0,0,0,0,0};
    //unsigned char cal[SIZEOFCOMMANDS] = {FRONT,FRONT,0};
    //unsigned char val[SIZEOFCOMMANDS] = {bigF,bigF,0};
    unsigned char where = 0;
    // loop forever
    // This loop is responsible for "handing off" messages to the subroutines
    // that should get them.  Although the subroutines are not threads, but
    // they can be equated with the tasks in your task diagram if you
    // structure them properly.
    LATEbits.LE0 = 0;
    while (1) 
    {
        //LATEbits.LE0 = parser.superInt;
        switch(parser.sysState)
        {
            case INIT:
            {
                //LATEbits.LE0 = 0;
                lengthl = ToMainLow_recvmsg(1, &msgtype, (void *) msgbuffer);
                if (lengthl >= 1)
                {

                    if (msgbuffer[lengthl-1]== SUPERGO)
                    {
                        where = 0;
                        parser.sysState = IDLE;
                    }
                    else
                    {
                        parser.superInt = 1;
                        parser.superMSG = msgbuffer[lengthl-1];
                        parser.sysState = SUPERCORRECT;
                    }
                }
                break;
            }
            case IDLE:
            {
                //LATEbits.LE0 = 1;
                //PORTE = 0x3;
                Delay1Second();
                //PORTE = 0xFC;
                lengthl = ToMainLow_recvmsg(1, &msgtype, (void *) msgbuffer);
                if (lengthl > 0)
                {
                        parser.superInt = 1;
                        parser.superMSG = msgbuffer[lengthl-1];
                        parser.sysState = SUPERCORRECT;
                }
                else
                {
                    //LATEbits.LE1 = 0;
                    //LATEbits.LE0 = 1;
                    if ((where == 1 )||(where == 11))
                    {
                        parser.sysState = FROMSENSOR;
                    }
                    else
                    parser.sysState = TOMOTOR;
                    parser.superInt = 0;
                }
                //LATEbits.LE0 = 0;
                break;
            }
            case FROMSENSOR:
            {
                lengthl = ToMainLow_recvmsg(1, &msgtype, (void *) msgbuffer);
                if (lengthl > 0)
                {
                        parser.superInt = 1;
                        parser.superMSG = msgbuffer[lengthl-1];
                        parser.sysState = SUPERCORRECT;
                        //myBrain.littleCounter = 1;
                        break;
                }
                i2c_master_recv(2,SENSOR);
                do
                {
                    length = ToMainHigh_recvmsg(MSGLEN, &msgtype, (void *) msgbuffer);
                }
                while (length<=0 || (msgtype != MSGT_I2C_DATA));
                myBrain.s1 = msgbuffer[0];
                myBrain.s2 = msgbuffer[1];
                myBrain.s2 = myBrain.s2 - 2;
                parser.sysState = DECIDE;
                break;
            }
            case DECIDE:
            {

                if ((myBrain.s1 - myBrain.s2) == 0)
                {
                    myBrain.command = IDLES;
                    myBrain.value = 0;
                    myBrain.littleCounter ++;
                }
                /*else if ((myBrain.s1 - myBrain.s2) == -1)
                {
                    myBrain.command = IDLES;
                    myBrain.value = 0;
                }
                else if ((myBrain.s1 - myBrain.s2) == 1)
                {
                    myBrain.command = IDLES;
                    myBrain.value = 0;
                }

                else if ((myBrain.s1 - myBrain.s2) == -2)
                {
                    myBrain.command = IDLES;
                    myBrain.value = 0;
                }
                else if ((myBrain.s1 - myBrain.s2) == 2)
                {
                    myBrain.command = IDLES;
                    myBrain.value = 0;
                }
                */
                else if (myBrain.s1 > myBrain.s2)
                {
                    myBrain.command = RIGHT;
                    myBrain.value = ALPHA;
                }
                else if (myBrain.s2 > myBrain.s1)
                {
                    myBrain.command = LEFT;
                    myBrain.value = ALPHA;
                }

                parser.sysState = CORRECT;
                break;
            }
            case CORRECT:
            {

                LATEbits.LE1 = 1;
                unsigned char motor_msg=0;
                motor_msg |= (myBrain.command << 6);
                motor_msg |= myBrain.value;
                i2c_master_send(1,&(motor_msg),MOTOR);
                parser.sysState = WAITCORRECTION;
                break;
            }
            case WAITCORRECTION:
            {

                LATEbits.LATE0 = 1;
                Delay1Second();
                Delay1Second();
                while (PORTEbits.RE2 == 1);
                LATEbits.LATE0 = 0;

                if(myBrain.littleCounter == 0)
                parser.sysState = FROMSENSOR;
                else
                {
                    parser.sysState = TOMOTOR;
                    myBrain.littleCounter = 1;
                }
                break;
                
            }
            
            case SUPERCORRECT:
            {
                
                i2c_master_send(1,&(parser.superMSG),MOTOR);
                parser.sysState = FROMMOTOR;
                break;
            }
            
            case TOMOTOR:
            {
                
                LATEbits.LE1 = 0;
                unsigned char motor_msg=0;
                motor_msg |= (cal[where] << 6);
                motor_msg |= (val[where]);
                i2c_master_send(1,&(motor_msg),MOTOR);
                parser.sysState = FROMMOTOR;
                break;
            }
            case TOSUPERVISOR:
            {
               
                //unsigned char superMSG=0;
                //superMSG |= (cal[where] << 6);
                //superMSG |= (val[where]);
              

                unsigned char superMSG = 69;
                sendUART(1,&superMSG);
                do
                {
                    lengthl = ToMainLow_recvmsg(1, &msgtype, (void *) msgbuffer);
                }
                while((msgtype != MSGT_UART_TX)||(lengthl == 0));
                lengthl = 1;
                uart_lthread(&uthread_data, msgtype, lengthl, msgbuffer);
                unsigned char  i = 0;
                for (i = 0; i < SIZEOFCOMMANDS;i++)
                {
                    superMSG = ramps[i];
                    sendUART(1,&superMSG);
                    do
                    {
                        lengthl = ToMainLow_recvmsg(1, &msgtype, (void *) msgbuffer);
                    }
                    while((msgtype != MSGT_UART_TX)||(lengthl == 0));
                    lengthl = 1;
                    uart_lthread(&uthread_data, msgtype, lengthl, msgbuffer);

                }
                
                parser.sysState = INIT;
                break;
            }
            case FROMMOTOR:
            {
                Delay1Second();
                LATEbits.LATE1 = 1;
                while (PORTEbits.RE2 == 1)
                {
                    if (PORTAbits.RA0 == 1)
                    {
                        rampBounc ++;
                        LATEbits.LE0 = 1;
                    }
                    else
                    {
                        LATEbits.LE0 = 0;
                    }
                    lengthl = ToMainLow_recvmsg(1, &msgtype, (void *) msgbuffer);
                    if (lengthl > 0)
                    {
                        if (msgbuffer[lengthl-1] == 0x0)break;
                    }
                }
                LATEbits.LATE1 = 0;
                if (parser.superInt == 1)
                {
                    parser.sysState = FROMSUPERVISOR;
                }
                else if(parser.superInt == 0)
                {
                    parser.sysState = UPDATECOURS;
                }
                
                break;
            }
            case FROMSUPERVISOR:
            {
                //LATEbits.LE0 = 1;
                char signal = 0;
                lengthl = ToMainLow_recvmsg(1, &msgtype, (void *) msgbuffer);
                if (lengthl > 0)
                {
                    if (msgbuffer[lengthl-1] == ENDSUPER)
                    {
                        parser.superInt = 0;
                        parser.sysState = IDLE;
                        //LATEbits.LE0 = 0;
                    }
                    else if (parser.superMSG == PREVIOUS)
                    {
                        if (where >= 2)
                        {
                            where--;
                            where--;
                            parser.superInt = 0;
                            parser.sysState = TOMOTOR;
                        }
                        else signal = 1;
                        
                    }
                    else if (parser.superMSG == NEXT) 
                    {
                        if (where <= (SIZEOFCOMMANDS-3))
                        {
                            where++;
                            where++;
                            parser.superInt = 0;
                            parser.sysState = TOMOTOR;
                        }
                        else signal = 1;

                        
                    }
                    else if (parser.superMSG == RESTART) 
                    {
                        where = 0;
                        parser.superInt = 0;
                        parser.sysState = TOMOTOR;
                    }
                    else if (parser.superMSG == FINISH)
                    {
                        where = SIZEOFCOMMANDS -1;
                        parser.superInt = 0;
                        parser.sysState = TOSUPERVISOR;
                    }
                    else if (signal == 0)
                    {
                        parser.superMSG = msgbuffer[lengthl-1];
                        parser.sysState = SUPERCORRECT;
                    }
                    signal = 0;
                }
                break;
            }
            case UPDATECOURS:
            {

                ramps[where] = rampBounc;
                rampBounc = 0;
                where++;
                if (where >= SIZEOFCOMMANDS) 
                {
                    parser.sysState = TOSUPERVISOR;
                    break;
                }
                if ((cal[where]== FRONT)&&(cal[where-1]!= FRONT));
                parser.sysState = IDLE;
                break;
            }
        }
    }
}