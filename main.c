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
//	timing_init(); //����
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
			LCD_ShowHexNum(1,1,address,2);//address����ң������ַ
			LCD_ShowHexNum(2,1,command,2);//command������ң������ֵ��Ӧ��ַ
			if(command==0x15)//ң�����ϰ���VOL-�ĵ�ַ
			{
				num--;
			}
			else if(command==0x09)//ң�����ϰ���VOL+�ĵ�ַ
			{
				num++;
			}
			LCD_ShowNum(1,4,num,3);
		}
	}
	
}


