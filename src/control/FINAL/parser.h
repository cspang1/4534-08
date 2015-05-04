/* 
 * File:   parser.h
 * Author: Youssef Ben Jemia
 *
 * Created on February 13, 2015, 1:02 PM
 */

#ifndef PARSER_H
#define	PARSER_H
#define SID 0
#define MID 1
#define ENDSUPER 0x0E
#define SUPERGO 0x0F
#define RESTART 0x0D
#define PREVIOUS 0x0C
#define NEXT 0x0B
#define FINISH 0x0A

typedef enum {IDLE,FROMSENSOR,TOMOTOR,BLOCK,TOSUPERVISOR,DECIDE,FROMMOTOR,SUPERCORRECT,UPDATECOURS,FROMSUPERVISOR,INIT,CORRECT,WAITCORRECTION,DEBUG}state;

typedef struct parser
{
    char MexpectedSeq;
    char McurSeq;
    char Merr;
    char PexpectedSeq;
    char PcurSeq;
    char Perr;
    char id;
    char val;
    state sysState;
    unsigned char ONOFF;
    unsigned char superMSG;
    unsigned char superInt;
}parser_t;

void parseSeq(char,parser_t*);
void runStateMachine(parser_t*);
#endif	/* PARSER_H */

