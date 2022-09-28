#include "stm32f10x.h"                  // Device header
#include "delay.h"

void delay_us(u32 i)
{
	u32 now;
	SysTick->LOAD = 9*i;
	SysTick->CTRL = 0X01;
	
	SysTick->VAL=0;		   	 
	do
	{
		now=SysTick->CTRL;		  
	}
	while((now&0x01)&&(!(now&(1<<16))));	
	SysTick->CTRL=0;
	SysTick->VAL=0;	
}


void delay_ms(u32 i)
{
	u32 now;
	SysTick->LOAD = 9000*i;
	SysTick->CTRL = 0X01;
	
	SysTick->VAL=0;		   	 
	do
	{
		now=SysTick->CTRL;		   
	}
	while((now&0x01)&&(!(now &(1<<16))));	
	SysTick->CTRL=0;
	SysTick->VAL=0;	
}

