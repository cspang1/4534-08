#include "brain.h"
#include <plib/i2c.h>
/*
unsigned char csc(unsigned char x)
{
    return (1/x+x/6+7*x*x*x/360+31*x*x*x*x*x/15120);
}
unsigned char cosine(unsigned char x)
{
    return (1-x/2*x+x/2*x/3*x/4*x-x/2*x/3*x/4*x/5*x/6*x+x/2*x/3*x/4*x/5*x/6*x/7*x/8*x-x/2*x/3*x/4*x/5*x/6*x/7*x/8*x/9*x/10);
}
*/

unsigned char rounder(unsigned char x)
{
    if (((x/10) ==0) && ((x%10)<5)) return 0;
    else if (((x/10) ==0) && ((x%10)>=5))return 10;
    else if((x%10)<5) return x;
    else return ((x/10)*10+10);
}

void runDecision(brain* myBrain)
{
    const unsigned char ERROR =2;
    const unsigned char DISTANCE = 8;


    switch(myBrain->decision)
    {

        case REST:
        {
            
            myBrain->command = IDLES;
            myBrain->value = 0;
            myBrain->decision = GETPARALLEL;
            break;
        }

        case GETPARALLEL:
        {

            if (myBrain->s1 > myBrain->s2) myBrain->command = 2;
            else if (myBrain->s2 > myBrain->s1) myBrain->command = 3;
            else myBrain->command = 1;
            myBrain->switcher = myBrain->command;
            if ((myBrain->switcher == FRONT)&&(myBrain->command == RIGHT)) myBrain->decision = REST;
            else if ((myBrain->switcher == FRONT)&&(myBrain->command == RIGHT)) myBrain->decision = REST;
            myBrain->value = ALPHA;
        }




    }

}