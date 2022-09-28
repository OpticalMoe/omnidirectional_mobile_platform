/********************************************************************************
* @file      Photoelectric.c
* @author    Chen
* @version
* @date      2017/11/17
* @brief     ��翪��   
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

#define Speed_Switch PBin(13)	// PB13 �ᴥ���أ��ٶ�ѡ�񣬰���-> �ߵ�ƽ-> ����

#define Servo PBout(7)			// PB7
#define Ducted_Fan PBout(8)	// PB8

void Photoelectric_Init(void) ;				//���ܳ�ʼ��
void Ducted_fan_Open(void);						//�����
void Ducted_fan_Off(void);						//�ط��
void Servo_Control(signed char Angle);//���ƶ���Ƕ�
void Servo_Self_test(void);						//���&����Լ�
void Servo_Sweep_field(void);					//ɨ��
void Photoelectric_Start_OUT(void);		//���ÿ�ʼ����Ϊ���ģʽ
unsigned char Read_Photoelectric(void);//������״̬
		    
#endif

///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
