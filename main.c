#include <REGX52.H>
#include "delay.h"
#include "LCD1602.h"
#include "IR.h"
unsigned char num;
unsigned int time;


unsigned char address;
unsigned char command;

void main ()
{
	LCD_Init();
//	timing_init(); //测试
//	count_start(1);
//	delay_ms(1);
//	time=get_timecount();
	timing_init();
	outinterrupt_init();
	while (1)
	{
		if(IR_getdataflag())
		{
			address=getaddress();
			command=getcommand();
			LCD_ShowHexNum(1,1,address,2);//address代表遥控器地址
			LCD_ShowHexNum(2,1,command,2);//command代表按下遥控器键值对应地址
			if(command==0x15)//遥控器上按键VOL-的地址
			{
				num--;
			}
			else if(command==0x09)//遥控器上按键VOL+的地址
			{
				num++;
			}
			LCD_ShowNum(1,4,num,3);
		}
	}
	
}


