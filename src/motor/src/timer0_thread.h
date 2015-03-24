#ifndef __timer0_thread
#define __timer0_thread

typedef struct __timer0_thread_struct {
    // "persistent" data for this "lthread" would go here
    unsigned int cmCount;
    unsigned char ctrl[2];
    unsigned int cmDist;
    unsigned int curMove;
    BOOL distLim;
} timer0_thread_struct;

void init_timer0_lthread(timer0_thread_struct *);
int timer0_lthread(timer0_thread_struct *);

#endif