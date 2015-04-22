#ifndef __timer1_thread
#define __timer1_thread

#include "maindefs.h"

typedef struct __timer1_thread_struct {
    // "persistent" data for this "lthread" would go here
    unsigned int cmCount;
    unsigned char ctrl[2];
    unsigned int cmDist;
    unsigned int curMove;
    unsigned int tenCount;
    BOOL distLim;
    BOOL ready;
} timer1_thread_struct;

void init_timer1_lthread(timer1_thread_struct *);
int timer1_lthread(timer1_thread_struct *);

#endif