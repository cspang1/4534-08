/* 
 * File:   brain.h
 * Author: Youssef Ben Jemia
 *
 * Created on April 4, 2015, 5:02 AM
 */

#ifndef BRAIN_H
#define	BRAIN_H

// We choose ALPHA to be 66 degrees since cosine66 = 0.4
// When .4*10=4. This will give the best average error. Also this will avoid overflow
// knowing the width of the course
// The worst case error for .4 is .4 inches and that happens 2/5 of the times
// .2 2/5 of the times and 0 1/5 of the times
// We actually choose 78 for the same reason but better overflow avoidance

#define ALPHA 5 //Tilt Angle. The unit is 3 degrees. ALPHA =  78degrees
#define COSALPHA 15 //Cosine Alpha *10
//#define ERROR 1 //Tolerance in inches
//#define DISTANCE 12// Safe distance from the wall in inches
#define PHI 2 //Predef distance that the rover will go diagonally.
#define LEFT 3
#define RIGHT 2
#define FRONT 1
#define IDLES 0
//#define IDLES 4


typedef enum {REST,GETPARALLEL,TURNRIGHT,TURNLEFT,MOVEBACKWARD,MOVEFORWARD,FINALIZE} decisionState;
typedef enum {STARTING,UP,DOWN,MOVL,MOVR} portionState;
typedef struct __brain
{
    decisionState decision; //Holds the state of the decision
    char s1; //Sensor 1 value
    char s2; //Sensor 2 value
    unsigned char s3;
    unsigned char s4;
    unsigned char o_theta; //Distance of center of rover from the wall
    unsigned char column;//The column at which the rover is
    unsigned char value;
    unsigned char command;
    unsigned char switcher;
    unsigned char tilt;
    portionState curPortion;
    portionState prevPortion;
    unsigned char littleCounter;


} brain;

void runDecision(brain*);



#endif	/* BRAIN_H */

