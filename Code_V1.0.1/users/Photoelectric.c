/********************************************************************************
* @file      Photoelectric.c
* @author    Chen
* @version
* @date      2017/11/17
* @brief     光电开关	   
******************************************************************************
* @attention	黑色-> 指示灯灭-> 1
*						 橙  黄  绿  蓝  紫  灰  白  黑  棕  红 
*						 B12 B14 A8  B1  A12 A11 B15 B7  B13  B8
*******************************************************************************/
 
#include "stm32f10x.h"
#include "Photoelectric.h"
#include "delay.h"

unsigned char Flag_Photoelectric = 0xC0;		//光电管状态标志
								    
/**
* @name 	  Photoelectric_Init
* @brief  	光电开关初始化
* @param      
* @retval			
*/
void Photoelectric_Init(void) //IO初始化
{ 
	GPIO_InitTypeDef GPIO_InitStructure;		
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
//	NVIC_InitTypeDef NVIC_InitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_12 | GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_12 | GPIO_Pin_14; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化 
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成下拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化 
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;				 			//端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 			 //根据设定参数初始化GPIOB.0
	 GPIO_SetBits(GPIOB,GPIO_Pin_7 | GPIO_Pin_8);					 //PB.7/8输出高
}

/**
* @name 	  Photoelectric_Start_OUT
* @brief  	配置开始光电管为输出模式
* @param    
* @retval			
*/
void Photoelectric_Start_OUT(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTE时钟	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 	//LED-->PB.0 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		//IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 		//根据设定参数初始化GPIOB.0
	GPIO_SetBits(GPIOB,GPIO_Pin_3);						 				//PB.0 输出高
}

/**
* @name 	  Ducted_fan_Open
* @brief  	开涵道风扇
* @param    
* @retval			
*/
void Ducted_fan_Open(void){
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);						 //PB.8 输出低
}

/**
* @name 	  Ducted_fan_Off
* @brief  	关涵道风扇
* @param    
* @retval			
*/
void Ducted_fan_Off(void){
	GPIO_SetBits(GPIOB,GPIO_Pin_8);						 //PB.8 输出低
}

/**
* @name 	  Servo_Control
* @brief  	控制舵机
* @param    
* @retval			
*/
void Servo_Control(signed char Angle){
	GPIO_SetBits(GPIOB,GPIO_Pin_7);						 //PB.8 输出低
	delay_us(1500 + Angle * 11);
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);						 //PB.8 输出低
	delay_us(500);
}

/**
* @name 	  Servo_Self_test
* @brief  	舵机&风机自检
* @param    
* @retval			
*/
void Servo_Self_test(void){
	signed char Servo_Angle = -90;
	Servo_Control(-90);
	Ducted_fan_Open();
	for(Servo_Angle = -90;Servo_Angle < 90;Servo_Angle++){
		Servo_Control(Servo_Angle);
		delay_ms(5);
	}
	Ducted_fan_Off();
	Servo_Control(0);
}

/**
* @name 	  Servo_Sweep_field
* @brief  	舵机&风机扫场
* @param    
* @retval			
*/
void Servo_Sweep_field(void){
	signed char Servo_Angle = -90;
	Servo_Control(-90);
	Ducted_fan_Open();
	delay_ms(500);
	for(Servo_Angle = -90;Servo_Angle < 90;Servo_Angle++){
		Servo_Control(Servo_Angle);
		delay_ms(10);
	}
	delay_ms(200);	
	Ducted_fan_Off();
	Servo_Control(0);
}
///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
