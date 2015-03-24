#ifndef __motors
#define __motors

#include "timer1_thread.h"
#include "timer0_thread.h"

void move(timer1_thread_struct *);
void moveDist(timer1_thread_struct *, int);
void turn(timer0_thread_struct *, int);
void stop(timer1_thread_struct *);

#endif