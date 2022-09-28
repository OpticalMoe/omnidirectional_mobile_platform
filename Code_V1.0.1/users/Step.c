/*****************************************************************************
* @file      Step.c
* @author    Chen
* @version
* @date      2017/11/17
* @brief     步进电机控制程序		   
******************************************************************************
* @attention
*	  /    	 	   \ 
*    1				2
*   /        	  	 \
*          
*        -- 0 --     
*	DIR=0 -> 逆时针转
* 频率决定方向；脉冲数决定距离
*
*		STEP: A0 / A6 / B6
*       DIR : B3 / 4  / 5
*       MS  : B12/ 13 / 14
*       EN  : B15 低电平有效
*******************************************************************************/

	
#include "Step.h"
#include "S_curve.h"
#include <math.h>

#include "printf.h"

const unsigned short int Automatic_reload_value = 1000 - 1;	//自动重装载值  max 65us 15kHz
const unsigned short int Frequency = 8 - 1;	//8分频  9MHz

unsigned char Debug_Step = 0;		//串口调试标志

signed char Flag_Dir[3] ={1};									//方向标志
unsigned long int Step[3] ={0};								//步数
float Coefficient[3] = {1};										//三个轮间系数关系
unsigned short int ARR[3] = {1000,1000,1000};	//自动重载值
unsigned short int ARR_Fixed = 0;							//方向修正
unsigned long int Max_Pulse = 100;						//最大脉冲数
unsigned char Label = 0;											//标号，记录脉冲数最大的轮
unsigned char Status = 0;											//状态标记 停-加速-减速-匀速
const float Lenght_Pulse = 0.054664;					//系数，转换成实际距离//0.113883;0.054664;  
unsigned short int Ascending_step = 0;				//升频步长

/**
* @name 	    Query_Step_Remain
* @brief  		查询剩余步数
* @param      
* @retval			
*/
unsigned long int Query_Step_Remain(void){
		return Step[Label];
}

/**
* @name 	    Query_Step_Run
* @brief  		查询执行步数
* @param      
* @retval			
*/
unsigned long int Query_Step_Run(void){
		return Max_Pulse - Step[Label];
}

/**
* @name 	    Step_Init
* @brief  		初始化步进电机所有相关设置
* @param      
* @retval			
*/
void Step_Init(void){  	
	GPIO_InitTypeDef GPIO_InitStructure;		
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
	TIM_OCInitTypeDef  TIM_OCInitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;	
	
/********************* 初始化GPIO **********************************/	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
    
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
     GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	                // JTAG DISABLE,SWD ENABLE
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |\
                                   GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;	//PB端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	                //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 		            //IO口速度为2MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 				    //根据设定参数初始化PA.1/PA.7
	 GPIO_SetBits(GPIOB,GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);				    //PA.1/PA.7输出高
	 GPIO_SetBits(GPIOB,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);	//PA.1/PA.7输出高

/********************* 初始化定时器 **********************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 	//使能定时器1时钟
 	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);  
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_6; 					
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 					
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);			

	TIM_TimeBaseStructure.TIM_Period = Automatic_reload_value;	//自动重装载值	
	TIM_TimeBaseStructure.TIM_Prescaler = Frequency;						//分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 						//初始化定时器2
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//初始化定时器3
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 						//初始化定时器4

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 						//TIMx_CNT<TIMx_CCR1 时有效电平
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //输出使能	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//高电平有效
	TIM_OCInitStructure.TIM_Pulse = 0x0005;												//初始脉宽长度为0
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);											//根据T指定的参数初始化外设TIM2 OC1
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); 						//使能TIM2在CCR1上的预装载寄存器,即TIM2_CCR1的预装载值在更新事件到来时才能被传送至当前寄存器中。 
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);											//根据T指定的参数初始化外设TIM3 OC1
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 						//使能TIM3在CCR1上的预装载寄存器,即TIM3_CCR1的预装载值在更新事件到来时才能被传送至当前寄存器中。 
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);											//根据T指定的参数初始化外设TIM4 OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); 						//使能TIM4在CCR1上的预装载寄存器,即TIM4_CCR1的预装载值在更新事件到来时才能被传送至当前寄存器中。 

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  					//TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  			//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  					//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  			//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  													//初始化NVIC寄存器
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  					//TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  			//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  													//初始化NVIC寄存器

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx更新中断标志
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);				//使能TIM更新中断源
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);				//使能TIM更新中断源
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);				//使能TIM更新中断源
	
	TIM_Cmd(TIM2, DISABLE);  					//失能
	TIM_Cmd(TIM3, DISABLE);  					//失能
	TIM_Cmd(TIM4, DISABLE);  					//失能
	Calculation(0,0,0);
}

/**
* @name 	    Calculation
* @brief  		计算   向右->x, 向前->y，angle:顺时针为正，角度值
* @param      float x, float y, float angle
* @retval			
*/
void Calculation(float x, float y, float angle) {
	u8 i = 0;
	signed long int Value_Pulse[3] = {0};		//计算值		
	
  angle = angle * 1.6;					//转换成实际角度值
  //计算
  Value_Pulse[0] = (long int)((x  + angle) / Lenght_Pulse + 0.5);
  Value_Pulse[1] = (long int)((-x  * 0.5 - y * 0.86 + angle) / Lenght_Pulse + 0.5);//y*=0.866
  Value_Pulse[2] = (long int)((-x  * 0.5 + y * 0.86 + angle) / Lenght_Pulse + 0.5);
	
	//执行//停
	TIM_Cmd(TIM2, DISABLE);  					//失能
	TIM_Cmd(TIM3, DISABLE);  					//失能
	TIM_Cmd(TIM4, DISABLE);  					//失能		
	//设定方向
	Flag_Dir[0] = Flag_Dir[1] = Flag_Dir[2] = -1; 
	if(Value_Pulse[0] > 0) Flag_Dir[0] =DIR_0 = 1;	//设定方向		
	else DIR_0 = 0;
	if(Value_Pulse[1] < 0) Flag_Dir[1] = DIR_1 = 1;	//设定方向
	else DIR_1 = 0;
	if(Value_Pulse[2] > 0) Flag_Dir[2] = DIR_2 = 1;	//设定方向
	else DIR_2 = 0;
	//记录步数
	for(i = 0;i < 3;i++){
		Step[i] = Value_Pulse[i] < 0 ? -Value_Pulse[i] : Value_Pulse[i];	//取绝对值
	}
	//记录最大脉冲数电机标号
	if(Step[0] > Step[1]){
		if(Step[0] > Step[2]) Label = 0;
		else Label = 2;
	}
	else if(Step[2] > Step[1]) Label = 2;
			 else Label = 1;
	Max_Pulse = Step[Label];		//记录最大脉冲数
	//计算系数
	for(i = 0;i < 3;i++){
		Coefficient[i] = (float)Step[i] / (float)Step[Label];		//计算三个轮间系数关系
	}	
	//计算最大脉冲数是否够加减速
	if(Max_Pulse < S_curve_Inquire_Ascending_steps()) Ascending_step = (Max_Pulse / 2 - 3200) > 0 ? (Max_Pulse / 2 - 3200):Max_Pulse / 2;
	else Ascending_step = S_curve_Inquire_Ascending_steps();
	
	//调试输出
	if(Debug_Step){		
		printf("Raw data:\t%.0f\t%.0f\t%.0f\r\n",x,y,angle / 1.66);
		printf("Value_Pulse:\t%ld\t%ld\t%ld\r\n",Value_Pulse[0],Value_Pulse[1],Value_Pulse[2]);
		printf("Step:       \t%ld\t%ld\t%ld\r\n",Step[0],Step[1],Step[2]);
		printf("Max_Step:\t%ld\r\n",Step[Label]);	
		printf("Coefficient:\t%f\t%f\t%f\r\n",Coefficient[0],Coefficient[1],Coefficient[2]);
		printf("\r\n");
	}
}

/**
* @name 	  Start  
* @brief  	使能定时器	
* @param      
* @retval			
*/
void Start(void){
	TIM_Cmd(TIM2, DISABLE);  					//失能
	TIM_Cmd(TIM3, DISABLE);  					//失能
	TIM_Cmd(TIM4, DISABLE);  					//失能
	Enable_Stepper();									//使能电机
	ARR[0] = ARR[1] = ARR[2] = 9000000 / (Query_Frequency(0)) + 0.5;
	TIM_SetAutoreload(TIM2,ARR[0]);//设置TIMx自动重装载寄存器值		 
	TIM_SetAutoreload(TIM3,ARR[1]);//设置TIMx自动重装载寄存器值		
	TIM_SetAutoreload(TIM4,ARR[2]);//设置TIMx自动重装载寄存器值	
	TIM_SetCounter(TIM2, 0x0000);
	TIM_SetCounter(TIM3, 0x0000);
	TIM_SetCounter(TIM4, 0x0000);
	if(Step[0]) TIM_Cmd(TIM2, ENABLE);  					//使能
	if(Step[1]) TIM_Cmd(TIM3, ENABLE);  					//使能
	if(Step[2]) TIM_Cmd(TIM4, ENABLE);  					//使能
}

/**
* @name 	  Stop  
* @brief  	失能定时器	
* @param      
* @retval			
*/
void Stop(void){
	Step[Label] = 0;
	TIM_Cmd(TIM2, DISABLE);  					//失能
	TIM_Cmd(TIM3, DISABLE);  					//失能
	TIM_Cmd(TIM4, DISABLE);  					//失能
}

/**
* @name 	  Enable_Stepper
* @brief  	步进电机使能
* @param      
* @retval			
*/
void Enable_Stepper(void){
	EN = 0;
}

/**
* @name 	  Disenable_Stepper
* @brief  	步进电机失能
* @param      
* @retval			
*/
void Disenable_Stepper(void){
	EN = 1;
}

/**
* @name 	  Enable_Debug
* @brief  	使能调试
* @param      
* @retval			
*/
void Enable_Debug_Step(void){
	Debug_Step = 1;
}

/**
* @name 	  Disenable_Debug
* @brief  	失能调试
* @param      
* @retval			
*/
void Disable_Debug_Step(void){
	Debug_Step = 0;
}

/**
* @name 	  Subdivision
* @brief  	细分
* @param    Subdivision 细分
* @retval		0->失败 1->成功
*/
unsigned char Subdivision(unsigned char Subdivision){
	switch(Subdivision){
		case 0:				//不细分
			MS_0 = 0;
			MS_1 = 0;
			MS_2 = 0;
			break;
		case 2:				//1/2细分
			MS_0 = 1;
			MS_1 = 0;
			MS_2 = 0;
			break;
		case 4:				//1/4细分
			MS_0 = 0;
			MS_1 = 1;
			MS_2 = 0;
			break;
		case 8:				//1/8细分
			MS_0 = 1;
			MS_1 = 1;
			MS_2 = 0;
			break;
		case 16:			//1/16细分
			MS_0 = 1;
			MS_1 = 1;
			MS_2 = 1;
			break;
		default:			//1/16细分
			MS_0 = 1;
			MS_1 = 1;
			MS_2 = 1;
		return 0;
	}
	return 1;
}

/**
* @name 	  TIM2_IRQHandler
* @brief  	定时器2中断服务程序
* @param      
* @retval			
*/
void TIM2_IRQHandler(void){   //TIM2中断
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  					//检查TIM2更新中断发生与否
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  					//清除TIMx更新中断标志 
			if(!(--Step[0])) TIM_Cmd(TIM2, DISABLE);   							//步数走完，关闭定时器
			if((Max_Pulse - Step[Label]) < (Ascending_step - 2)) {	//加速
				ARR[0] = 9000000 / (Coefficient[0] * Query_Frequency(Max_Pulse - Step[Label] + 1)) + 0.5;
				TIM_SetAutoreload(TIM2,ARR[0]);												//设置TIMx自动重装载寄存器值	
			}
			else if(Step[Label] < (Ascending_step - 2)){ 						//减速
						ARR[0] = 9000000 / (Coefficient[0] * Query_Frequency(Step[Label] + 1)) + 0.5;
						TIM_SetAutoreload(TIM2,ARR[0]);										//设置TIMx自动重装载寄存器值	
					}	
		}
}

/**
* @name 	  TIM3_IRQHandler
* @brief  	定时器3中断服务程序
* @param      
* @retval			
*/
void TIM3_IRQHandler(void){   //TIM3中断
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  					//检查TIM3更新中断发生与否
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  					//清除TIMx更新中断标志
			if(!(--Step[1])) TIM_Cmd(TIM3, DISABLE);   							//步数走完，关闭定时器
			if((Max_Pulse - Step[Label]) < (Ascending_step - 2)) {	//加速
				ARR[1] = 9000000 / (Coefficient[1] * Query_Frequency(Max_Pulse - Step[Label] + 1)) + 0.5;
				TIM_SetAutoreload(TIM3,ARR[1]);												//设置TIMx自动重装载寄存器值	
			}
			else if(Step[Label] < (Ascending_step - 2)){ 						//减速
						ARR[1] = 9000000 / (Coefficient[1] * Query_Frequency(Step[Label] + 1)) + 0.5;
						TIM_SetAutoreload(TIM3,ARR[1]);										//设置TIMx自动重装载寄存器值	
					}	
		}
}

/**
* @name 	  TIM4_IRQHandler
* @brief  	定时器4中断服务程序
* @param      
* @retval			
*/
void TIM4_IRQHandler(void){   //TIM4中断
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  					//检查TIM4更新中断发生与否
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  					//清除TIMx更新中断标志 
			if(!(--Step[2])) TIM_Cmd(TIM4, DISABLE);   							//步数走完，关闭定时器
			if((Max_Pulse - Step[Label]) < (Ascending_step - 2)) {	//加速
				ARR[2] = 9000000 / (Coefficient[2] * Query_Frequency(Max_Pulse - Step[Label] + 1)) + 0.5;
				TIM_SetAutoreload(TIM4,ARR[2]);												//设置TIMx自动重装载寄存器值	
			}
			else if(Step[Label] < (Ascending_step - 2)){ 						//减速
						ARR[2] = 9000000 / (Coefficient[2] * Query_Frequency(Step[Label] + 1)) + 0.5;
						TIM_SetAutoreload(TIM4,ARR[2]);										//设置TIMx自动重装载寄存器值	
					}	
		}
}

///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
