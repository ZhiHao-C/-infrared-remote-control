#include <REGX52.H>



unsigned int IR_time;
unsigned char IR_state;
unsigned char IR_Data[4];//�����ĸ��ֽ������洢address ~address command ~command
unsigned char IR_pData;//��Χ0~31
unsigned char IR_Dataflag;//���ݴ洢���־
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


void timing_init()  //��ʱ��0��ʼ��
{
	TMOD=0x01;
	TF0=0;
	EA=0;           //�������жϺ���
	TL0 = 0;		//���ö�ʱ��ֵ
	TH0 = 0;		//���ö�ʱ��ֵ
	TR0=0;
}


void count_start(unsigned f) //����һ��ʼ����
{
	TR0=f;
}

void outinterrupt_init()     //�ⲿ�ж�0��ʼ��
{
	IT0=1;
	EX0=1;
	EA=1;
	IE0=0;
	PX0=1;
}

void timesetcount(unsigned int v)//���ó�ֵ
{
	TH0=v/256;
	TL0=v%256;
}

unsigned int get_timecount()//�õ����˶�����
{
	unsigned int c=0;
	c=((c|TH0)<<8)|TL0;
	return c; 
}



void itpt() interrupt 0
{
	if(IR_state==0)
	{
		timesetcount(0);//��TH0��TL0����Ϊ0
		count_start(1);//��ʼ����
		IR_state=1;
	}
	else if(IR_state==1)
	{
		
		IR_time=get_timecount();
		timesetcount(0);
		if(IR_time>=12441-500&&IR_time<12441+500)//�͵�ƽ9ms�������ߵ�ƽ4.5msΪ��ʼ��־��ת�͵�ƽʱ�����ж�ͬʱ��
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
		if(IR_time>=1032-500&&IR_time<1032+500)//�͵�ƽ560us�ߵ�ƽ560us����0
		{
			IR_Data[IR_pData/8]=IR_Data[IR_pData/8]&~(0x01<<(IR_pData%8));//out�ڷ������ݴӵ͵��ߴ洢
			IR_pData++;
		}
		else if(IR_time>=2073-500&&IR_time<2073+500)//�͵�ƽ560us�ߵ�ƽ1690us����1
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
			if((IR_Data[0]==~IR_Data[1])&&(IR_Data[2]==~IR_Data[3]))//��ַ���������ж�
			{
				
				IR_address=IR_Data[0];//IR_Data��0��Ϊ��ַ�룬IR_Data��1��Ϊ��ַ�뷴��
				IR_command=IR_Data[2];//IR_Data��2��Ϊ�����룬IR_Data��3��Ϊ�����뷴��
				IR_Dataflag=1;
			}
			count_start(0);
			IR_state=0;
		}
	}
}
