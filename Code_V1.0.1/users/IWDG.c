#include "IWDG.h"
#include "stm32f10x.h"

void IWDG_Init()
{
	IWDG ->KR = 0x5555; 	//��д����
	IWDG->SR = 0X00;			//״̬�Ĵ�����bit0 Ϊ0ʱԤ��Ƶ���ӿɸ��£�bit1Ϊ0ʱ��װ��ֵ�ɸ���
	IWDG ->PR = 0X00; 		//Ԥ��Ƶ����4
	IWDG -> RLR = 0Xfff;	//��װ��ֵ  ���Ĳο��ֲ� ��83		��ʱ20ms
	IWDG ->KR = 0XCCCC;   //�򿪿��Ź�
	
}

void weigou()
{	
		IWDG -> KR = 0XAAAA;			//weigou
}



