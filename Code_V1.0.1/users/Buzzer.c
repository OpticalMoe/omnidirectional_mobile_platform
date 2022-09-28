/********************************************************************************
* @file         Buzzer.c
* @author       Chen
* @version
* @date         2019/1/31
* @brief        Buzzer	   
******************************************************************************
* @attention    PA15 JTDI�ӿڣ�����ʧ��JTAG��ʹ��SWD��
*               RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
*               GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);// JTAG DISABLE,SWD ENABLE
*******************************************************************************/

#include "delay.h"
#include "Buzzer.h"

/**
* @name 	Buzzer_Init
* @brief  	Buzzer��ʼ��
* @param    
* @retval			
*/
void Buzzer_Init(void){ 
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);	 //ʹ��PA�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;    //Buzzer�� �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		                //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		                //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);					                //�����趨������ʼ��
    GPIO_ResetBits(GPIOA,GPIO_Pin_15);		    //Buzzer�����
}

/**
* @name 	Buzzer_Blinking
* @brief  	RGB����
* @param    Count ������Time���ڣ�Percentageռ�ձ�
* @retval			
*/
void Buzzer_Blinking(unsigned char Count,unsigned short Time,unsigned char Percentage){
	if(Percentage > 99) Percentage = 99;
	else if(Percentage == 0) {
		delay_ms(Time); 
		return ;
	}
	while(Count--){
		Buzzer = 1;
		delay_ms(((Time * Percentage) > 100? Time * Percentage:100)/ 100);	
		Buzzer = 0;
		delay_ms(((Time * (100 - Percentage)) > 100? (Time * (100 - Percentage)):100)/ 100);
	}
}

/**
* @name 	Buzzer_Blink
* @brief  	RGB����
* @param    Time_Sum ��ʱ��ms��Time ���ms
* @retval			
*/
void Buzzer_Blink(unsigned short Time_Sum,unsigned short Time){
	unsigned short Count = Time_Sum / (Time * 2);
	while(Count--){
		Buzzer = 1;
		delay_ms(Time);	
		Buzzer = 0;
		delay_ms(Time);	
	}
}

///************************ (C) COPYRIGHT 2019  Chen *****END OF FILE****/