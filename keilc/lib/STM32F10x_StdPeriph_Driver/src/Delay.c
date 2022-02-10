

#include "stm32f10x.h"
//#include "CONFIG_STM32F10X.h"
#include "Delay.h"

static u8  fac_us=0;
static u16 fac_ms=0; 



void Fn_DELAY_Short (unsigned int _vrui_Time)
{
	while(_vrui_Time--);
}
void Fn_DELAY_Long  (unsigned int _vrui_Time)
{
	unsigned int vrui_i;
	while(_vrui_Time--)
	{
		for(vrui_i = 0; vrui_i < 1000; vrui_i++);
	}
}

//==============================delay===============================
void Fn_DELAY_Init (unsigned char _CLK)
{
	SysTick->CTRL&=0xfffffffb;        
	fac_us=_CLK/8;		    
	fac_ms=(u16)fac_us*1000;
}



void Fn_DELAY_ms (unsigned int _vrui_Time)
{	 		  	  		              //72M������,nms<=1864
	u32 temp;		   
	SysTick->LOAD=(u32)_vrui_Time*fac_ms;    //ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;               //��ռ�����
	SysTick->CTRL=0x01 ;              //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&(!(temp&(1<<16))));//�ȴ�ʱ�䵽��
	SysTick->CTRL=0x00;               //�رռ�����
	SysTick->VAL =0X00;               //��ռ�����	  	    
}   
void Fn_DELAY_us (unsigned long _vrui_Time)
{		
	u32 temp;	    	 
	SysTick->LOAD=_vrui_Time*fac_us;         //ʱ�����	  		 
	SysTick->VAL=0x00;                //��ռ�����
	SysTick->CTRL=0x01 ;              //��ʼ���� 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&(!(temp&(1<<16))));//�ȴ�ʱ�䵽��
	SysTick->CTRL=0x00;               //�رռ�����
	SysTick->VAL =0X00;               //��ռ�����	 
}
