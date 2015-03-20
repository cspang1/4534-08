#ifndef __motors
#define __motors

#include "timer1_thread.h"

void move(timer1_thread_struct *);
void moveDist(timer1_thread_struct *, int);
void turn(timer1_thread_struct *, int);

#endif