/********************************************************************************
* @file      Photoelectric.c
* @author    Chen
* @version
* @date      2017/11/17
* @brief     光电开关   
******************************************************************************
* @attention
*******************************************************************************/

#ifndef __Photoelectric_H
#define __Photoelectric_H	 

#define Photoelectric_Start PBin(3) //PB15

#define Photoelectric_0 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)		//PA12
#define Photoelectric_1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)		//PB12
#define Photoelectric_2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)		//PB14
#define Photoelectric_3 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)			//PA8
#define Photoelectric_4 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)	  	//PB1
#define Photoelectric_5 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)		//PA11

#define Speed_Switch PBin(13)	// PB13 轻触开关，速度选择，按下-> 高电平-> 高速

#define Servo PBout(7)			// PB7
#define Ducted_Fan PBout(8)	// PB8

void Photoelectric_Init(void) ;				//光电管初始化
void Ducted_fan_Open(void);						//开风机
void Ducted_fan_Off(void);						//关风机
void Servo_Control(signed char Angle);//控制舵机角度
void Servo_Self_test(void);						//舵机&风机自检
void Servo_Sweep_field(void);					//扫场
void Photoelectric_Start_OUT(void);		//配置开始光电管为输出模式
unsigned char Read_Photoelectric(void);//读光电管状态
		    
#endif

///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
