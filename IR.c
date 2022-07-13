#include <REGX52.H>



unsigned int IR_time;
unsigned char IR_state;
unsigned char IR_Data[4];//定义四个字节用来存储address ~address command ~command
unsigned char IR_pData;//范围0~31
unsigned char IR_Dataflag;//数据存储完标志
unsigned char IR_repeatflag;
unsigned char IR_address;
unsigned char IR_command;

unsigned char IR_getdataflag()
{
	if(IR_Dataflag)
	{
		IR_Dataflag=0;
		return 1;
	}
	else 
	{
		return 0;
	}
}


unsigned char IR_getrepeatflag()
{
	if(IR_repeatflag)
	{
		IR_repeatflag=0;
		return 1;
	}
	else 
	{
		return 0;
	}
}

unsigned char getaddress()
{
	return IR_address;
}

unsigned char getcommand()
{
	return IR_command;
}


void timing_init()  //定时器0初始化
{
	TMOD=0x01;
	TF0=0;
	EA=0;           //不调用中断函数
	TL0 = 0;		//设置定时初值
	TH0 = 0;		//设置定时初值
	TR0=0;
}


void count_start(unsigned f) //输入一开始计数
{
	TR0=f;
}

void outinterrupt_init()     //外部中断0初始化
{
	IT0=1;
	EX0=1;
	EA=1;
	IE0=0;
	PX0=1;
}

void timesetcount(unsigned int v)//设置初值
{
	TH0=v/256;
	TL0=v%256;
}

unsigned int get_timecount()//得到计了多少下
{
	unsigned int c=0;
	c=((c|TH0)<<8)|TL0;
	return c; 
}



void itpt() interrupt 0
{
	if(IR_state==0)
	{
		timesetcount(0);//将TH0和TL0设置为0
		count_start(1);//开始计数
		IR_state=1;
	}
	else if(IR_state==1)
	{
		
		IR_time=get_timecount();
		timesetcount(0);
		if(IR_time>=12441-500&&IR_time<12441+500)//低电平9ms再来个高电平4.5ms为开始标志（转低电平时触发中断同时）
		{
			IR_state=2;
		}
		else if(IR_time>=10368-500&&IR_time<10368+500)
		{
			IR_repeatflag=1;
			count_start(0);
			IR_state=0;
		}
//		else 
//		{
//			timesetcount(0);
//			count_start(0);
//			IR_state=0;
//		}
	}
	else if(IR_state==2)
	{
		IR_time=get_timecount();
		timesetcount(0);
		if(IR_time>=1032-500&&IR_time<1032+500)//低电平560us高电平560us代表0
		{
			IR_Data[IR_pData/8]=IR_Data[IR_pData/8]&~(0x01<<(IR_pData%8));//out口发送数据从低到高存储
			IR_pData++;
		}
		else if(IR_time>=2073-500&&IR_time<2073+500)//低电平560us高电平1690us代表1
		{
			IR_Data[IR_pData/8]=IR_Data[IR_pData/8]|(0x01<<(IR_pData%8));
			IR_pData++;
		}
//		else 
//		{
//			IR_pData=0;
//			IR_state=1;
//		}
		if(IR_pData>=32)
		{
			IR_pData=0;
			if((IR_Data[0]==~IR_Data[1])&&(IR_Data[2]==~IR_Data[3]))//地址码命令码判断
			{
				
				IR_address=IR_Data[0];//IR_Data【0】为地址码，IR_Data【1】为地址码反码
				IR_command=IR_Data[2];//IR_Data【2】为命令码，IR_Data【3】为命令码反码
				IR_Dataflag=1;
			}
			count_start(0);
			IR_state=0;
		}
	}
}
