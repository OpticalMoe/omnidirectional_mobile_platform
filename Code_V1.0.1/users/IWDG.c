#include "IWDG.h"
#include "stm32f10x.h"

void IWDG_Init()
{
	IWDG ->KR = 0x5555; 	//打开写保护
	IWDG->SR = 0X00;			//状态寄存器，bit0 为0时预分频因子可更新，bit1为0时重装载值可更新
	IWDG ->PR = 0X00; 		//预分频因子4
	IWDG -> RLR = 0Xfff;	//重装载值  中文参考手册 表83		此时20ms
	IWDG ->KR = 0XCCCC;   //打开看门狗
	
}

void weigou()
{	
		IWDG -> KR = 0XAAAA;			//weigou
}



