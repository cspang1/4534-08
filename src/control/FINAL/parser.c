#include "parser.h"

//static parser_t parser;
void parseSeq(char seq, parser_t* par)
{
    
    par->id = (seq & 0x40)>>6;
    par->val = (seq& 0x3E)>>1;

    
    /*
    if ((par->id) == 1)
    {
        par->PcurSeq = ((seq & 0x20)>>5);
        if (!(seq & 0x80)) par->Perr = 1;
        if ((par->PcurSeq) != (par->PexpectedSeq)) par->Perr =1;
        else
        {
            par->Perr =0;
            if (seq & 0x1) par->PexpectedSeq = 0;
            else
            {
                par->PexpectedSeq ++;
                par->PexpectedSeq = par->PexpectedSeq % 2;
            }
        }

    }
    else if ((par->id) == 0)
    {
        par->McurSeq = ((seq & 0x20)>>5);
        if (!(seq & 0x80)) par->Merr=1;
        if ((par->McurSeq) != (par->MexpectedSeq)) par->Merr =1;
        else
        {
            par->Merr =0;
            if (seq & 0x1) par->MexpectedSeq = 0;
            else
            {
                par->MexpectedSeq ++;
                par->MexpectedSeq = par->MexpectedSeq % 2;
            }
        }
    }
*/
}

