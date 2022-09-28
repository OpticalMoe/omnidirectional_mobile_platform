/********************************************************************************
* @file      main.c
* @author    Chen
* @version	 1.0
* @date      2017/11/17
* @brief     理工杯机器人参赛车程序		   
******************************************************************************
* @attention
* 					 注意！！！ A0/A13/-A14/A15  B2/B3/B4 禁止使用！！！
*******************************************************************************/

#include "stm32f10x.h"  
//#include "printf.h"
#include "delay.h"              
//#include "usart.h"            //串口1
#include "usart3.h"           //串口3
#include "RGB.h"                //RGB指示灯
#include "key.h"                //按键
#include "Buzzer.h"             //蜂鸣器
//#include "IWDG.h"             //看门狗
#include "S_curve.h"            //S曲线
#include "Step.h"               //步进电机 驱动

unsigned char Count = 0;		//转向点计数
unsigned char Count_RGB = 0,Number_RGB = 0;    //LED闪灯计数
unsigned char Debug = 0;		//Debug

/**
* @name 	  main
* @brief  	主函数
* @param    
* @retval			
*/
int main(void)
{
	Step_Init();					//初始化步进电机
	Subdivision(16);			    //细分	
    
	delay_ms(500);	
//	printf_init();					//串口1  波特率 57600		没有奇偶校验	停止位1
////	usart2_Init();					//接受遥控器数据 波特率100000	没有奇偶校验	停止位1
	USART3_Init();					//接受陀螺仪数据 波特率9600	没有奇偶校验	停止位1
	RGB_Init();						//初始化RGB
	KEY_Init();						//初始化按键
    Buzzer_Init();                  //蜂鸣器初始化
	Disable_Debug_Step();			//失能步进调试
	Disable_Debug_S_curve();	    //失能S曲线调试
//	S_curve_Calculate_Init();	    //S曲线初始化计算，方便以后查表
	//	IWDG_Init();
	Enable_Stepper();				//使能电机

//************************************************//
    
    //蜂鸣器自检
    Buzzer_Blink(600,100);
    
//	//RGB自检	
//    for(Count_RGB = 1;Count_RGB < 9;Count_RGB++)
//        for(Number_RGB = 1;Number_RGB < 4;Number_RGB++)
//            RGB_Blinking(1,Number_RGB,Count_RGB,100,20);	//RGB自检  T=100ms D=99%
//    Colour_All(0);       //关灯

	//设定速度
	if(!KEY_Start){					//按下 高速
		RGB_All_Blink(200,4,20);		//红灯  T=500ms  闪/40ms  D=50%
        Buzzer_Blinking(1,200,99);
		S_curve_Set(S_curve_Inquire_Highest_frequency(),S_curve_Inquire_Lowest_frequency());	//17000Hz
	}
	else {										//默认 低速
		RGB_All_Blink(200,2,20);		//绿灯  T=500ms  闪/40ms  D=50%
        Buzzer_Blinking(1,100,99);
		S_curve_Set(10000,S_curve_Inquire_Lowest_frequency());//15000Hz
	}
	S_curve_Calculate_Init();	//S曲线初始化计算，方便以后查表

//	//串口打印“准备完成，可以开始！”
//	printf("Ready to finish, you can start!\r\n");
	
	//等待启动	
    while(!KEY_Start);      //按键松手检测
	while(KEY_Start);
    Buzzer_Blinking(1,100,99);
    
//************************************************//
    
	//主函数
	while(1){
		if(!Query_Step_Remain()){			//执行完
		//选点
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
				
				default:							//完成任务
					Calculation(0,0,0);									
					Count = 0;		
					Disenable_Stepper();								//失能电机
					while(1) RGB_All_Blinking(1,2,1000,2);	//绿灯爆闪
					break;
			}
			RGB_All_Blinking(1,4,15,99);	//状态灯 闪蓝灯 T=50ms D=30%
            Buzzer_Blinking(1,30,99);	//状态灯 闪蓝灯 T=50ms D=30%
			Start();					//启动步进电机
		}
		else; 				//未执行完
	}
}

///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
