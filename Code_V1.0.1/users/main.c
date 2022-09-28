/********************************************************************************
* @file      main.c
* @author    Chen
* @version	 1.0
* @date      2017/11/17
* @brief     ���������˲���������		   
******************************************************************************
* @attention
* 					 ע�⣡���� A0/A13/-A14/A15  B2/B3/B4 ��ֹʹ�ã�����
*******************************************************************************/

#include "stm32f10x.h"  
//#include "printf.h"
#include "delay.h"              
//#include "usart.h"            //����1
#include "usart3.h"           //����3
#include "RGB.h"                //RGBָʾ��
#include "key.h"                //����
#include "Buzzer.h"             //������
//#include "IWDG.h"             //���Ź�
#include "S_curve.h"            //S����
#include "Step.h"               //������� ����

unsigned char Count = 0;		//ת������
unsigned char Count_RGB = 0,Number_RGB = 0;    //LED���Ƽ���
unsigned char Debug = 0;		//Debug

/**
* @name 	  main
* @brief  	������
* @param    
* @retval			
*/
int main(void)
{
	Step_Init();					//��ʼ���������
	Subdivision(16);			    //ϸ��	
    
	delay_ms(500);	
//	printf_init();					//����1  ������ 57600		û����żУ��	ֹͣλ1
////	usart2_Init();					//����ң�������� ������100000	û����żУ��	ֹͣλ1
	USART3_Init();					//�������������� ������9600	û����żУ��	ֹͣλ1
	RGB_Init();						//��ʼ��RGB
	KEY_Init();						//��ʼ������
    Buzzer_Init();                  //��������ʼ��
	Disable_Debug_Step();			//ʧ�ܲ�������
	Disable_Debug_S_curve();	    //ʧ��S���ߵ���
//	S_curve_Calculate_Init();	    //S���߳�ʼ�����㣬�����Ժ���
	//	IWDG_Init();
	Enable_Stepper();				//ʹ�ܵ��

//************************************************//
    
    //�������Լ�
    Buzzer_Blink(600,100);
    
//	//RGB�Լ�	
//    for(Count_RGB = 1;Count_RGB < 9;Count_RGB++)
//        for(Number_RGB = 1;Number_RGB < 4;Number_RGB++)
//            RGB_Blinking(1,Number_RGB,Count_RGB,100,20);	//RGB�Լ�  T=100ms D=99%
//    Colour_All(0);       //�ص�

	//�趨�ٶ�
	if(!KEY_Start){					//���� ����
		RGB_All_Blink(200,4,20);		//���  T=500ms  ��/40ms  D=50%
        Buzzer_Blinking(1,200,99);
		S_curve_Set(S_curve_Inquire_Highest_frequency(),S_curve_Inquire_Lowest_frequency());	//17000Hz
	}
	else {										//Ĭ�� ����
		RGB_All_Blink(200,2,20);		//�̵�  T=500ms  ��/40ms  D=50%
        Buzzer_Blinking(1,100,99);
		S_curve_Set(10000,S_curve_Inquire_Lowest_frequency());//15000Hz
	}
	S_curve_Calculate_Init();	//S���߳�ʼ�����㣬�����Ժ���

//	//���ڴ�ӡ��׼����ɣ����Կ�ʼ����
//	printf("Ready to finish, you can start!\r\n");
	
	//�ȴ�����	
    while(!KEY_Start);      //�������ּ��
	while(KEY_Start);
    Buzzer_Blinking(1,100,99);
    
//************************************************//
    
	//������
	while(1){
		if(!Query_Step_Remain()){			//ִ����
		//ѡ��
			switch(++Count){
				case 1:
					Calculation(0,3000,0);	
					break;
				case 2:
					Calculation(5000,0,0);	
					break;
				case 3:
					Calculation(0,900,0);	
					break;
				case 4:
					Calculation(-800,0,0);	
					break;
				case 5:
					Calculation(0,900,0);	
					break;
				case 6:
					Calculation(800,0,0);	
					break;
				case 7:
					Calculation(0,900,0);	
					break;
                case 8:
					Calculation(-1500 - 125,0,-1);	
					break;
				case 9:
					Calculation(0,-1200 + 250,0);	
					break;
				
				default:							//�������
					Calculation(0,0,0);									
					Count = 0;		
					Disenable_Stepper();								//ʧ�ܵ��
					while(1) RGB_All_Blinking(1,2,1000,2);	//�̵Ʊ���
					break;
			}
			RGB_All_Blinking(1,4,15,99);	//״̬�� ������ T=50ms D=30%
            Buzzer_Blinking(1,30,99);	//״̬�� ������ T=50ms D=30%
			Start();					//�����������
		}
		else; 				//δִ����
	}
}

///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
