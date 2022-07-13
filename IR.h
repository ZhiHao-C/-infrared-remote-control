#ifndef __IR_H__
#define __IR_H__

void timing_init();
void count_start(unsigned f);
void outinterrupt_init();
void timesetcount(unsigned int v);
unsigned int get_timecount();




unsigned char IR_getdataflag();

unsigned char IR_getrepeatflag();


unsigned char getaddress();


unsigned char getcommand();




#endif