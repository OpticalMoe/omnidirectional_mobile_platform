/********************************************************************************
* @file         RGB.c
* @author       Chen
* @version
* @date         2019/1/30
* @brief        RGB	   
******************************************************************************
* @attention    RGB1: PB9/8/7
*	            RGB2: PB0/1/2
*               RGB3: PA7/5/4
*******************************************************************************/

#include "RGB.h"
#include "delay.h"

/**
* @name     RGB_Init
* @brief    RGB��ʼ��
* @param    
* @retval			
*/
void RGB_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE);	 //ʹ��PB�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7;    //RGB1 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		                //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		                //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					                //�����趨������ʼ��
 GPIO_ResetBits(GPIOB,GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7);		    //RGB�����
     
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;    //RGB2 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		                //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		                //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					                //�����趨������ʼ��
 GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);		    //RGB�����
  
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5 | GPIO_Pin_4;    //RGB3 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		                //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		                //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					                //�����趨������ʼ��
 GPIO_ResetBits(GPIOA,GPIO_Pin_7 | GPIO_Pin_5 | GPIO_Pin_4);		    //RGB�����
}

/**
* @name 	Colour
* @brief  	ѡ����ɫ��
* @param    
* @retval			
*/
void Colour(unsigned char Number,unsigned char colour){
	if(Number == 1){
        if(colour & 0x0001) Red1 = 1;
            else Red1 = 0;
        if((colour >> 1) & 0x0001) Green1 = 1;
            else Green1 = 0;
        if((colour >> 2) & 0x0001) Blue1 = 1;
            else Blue1 = 0;
    }
    if(Number == 2){
        if(colour & 0x0001) Red2 = 1;
            else Red2 = 0;
        if((colour >> 1) & 0x0001) Green2 = 1;
            else Green2 = 0;
        if((colour >> 2) & 0x0001) Blue2 = 1;
            else Blue2 = 0;
    }
    if(Number == 3){
        if(colour & 0x0001) Red3 = 1;
            else Red3 = 0;
        if((colour >> 1) & 0x0001) Green3 = 1;
            else Green3 = 0;
        if((colour >> 2) & 0x0001) Blue3 = 1;
            else Blue3 = 0;
    }
}

/**
* @name 	Colour_All
* @brief  	ѡ����ɫ��
* @param    
* @retval			
*/
void Colour_All(unsigned char colour){
    if(colour & 0x0001)
    {Red1 = 1; Red2 = 1; Red3 = 1;}
        else 
        {Red1 = 0; Red2 = 0; Red3 = 0; }
    if((colour >> 1) & 0x0001) 
    {Green1 = 1; Green2 = 1; Green3 = 1;}
        else 
        {Green1 = 0; Green2 = 0; Green3 = 0;}
    if((colour >> 2) & 0x0001) 
    {Blue1 = 1; Blue2 = 1; Blue3 = 1;}
        else 
        {Blue1 = 0; Blue2 = 0; Blue3 = 0;}
   
}

/**
* @name 	RGB_Blinking
* @brief  	RGB����
* @param    Count ������colour ��ɫ��Time���ڣ�Percentageռ�ձ�
* @retval			
*/
void RGB_Blinking(unsigned char Count,unsigned char Number,unsigned char colour,unsigned short Time,unsigned char Percentage){
	if(Percentage > 99) Percentage = 99;
	else if(Percentage == 0) {
		delay_ms(Time); 
		return ;
	}
	while(Count--){
		Colour(Number,colour);
		delay_ms(Time * Percentage / 100);	
		Colour(Number,0x00);
		delay_ms(Time * (100 - Percentage) / 100);	
	}
}

/**
* @name 	  RGB_Blink
* @brief  	RGB����
* @param    Time_Sum ������ʱ��ms��colour ��ɫ��Time ���ms
* @retval			
*/
void RGB_Blink(unsigned short Time_Sum,unsigned char Number,unsigned char colour,unsigned short Time){
	unsigned short Count = Time_Sum / (Time * 2);
	while(Count--){
		Colour(Number,colour);
		delay_ms(Time);	
		Colour(Number,0x00);
		delay_ms(Time);	
	}
}

/**
* @name 	RGB_All_Blinking
* @brief  	RGB����
* @param    Count ������colour ��ɫ��Time���ڣ�Percentageռ�ձ�
* @retval			
*/
void RGB_All_Blinking(unsigned char Count,unsigned char colour,unsigned short Time,unsigned char Percentage){
	if(Percentage > 99) Percentage = 99;
	else if(Percentage == 0) {	
		Colour_All(0x00);
		delay_ms(Time); 
		return ;
	}
	while(Count--){
		Colour_All(colour);
		delay_ms(((Time * Percentage) > 100? Time * Percentage:100)/ 100);	
		Colour_All(0x00);
		delay_ms(((Time * (100 - Percentage)) > 100? (Time * (100 - Percentage)):100)/ 100);	
	}
}

/**
* @name 	  RGB_All_Blink
* @brief  	RGB����
* @param    Time_Sum ������ʱ��ms��colour ��ɫ��Time ���ms
* @retval			
*/
void RGB_All_Blink(unsigned short Time_Sum,unsigned char colour,unsigned short Time){
	unsigned short Count = Time_Sum / (Time * 2);
	while(Count--){
		Colour_All(colour);
		delay_ms(Time);	
		Colour_All(0x00);
		delay_ms(Time);	
	}
}
///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
