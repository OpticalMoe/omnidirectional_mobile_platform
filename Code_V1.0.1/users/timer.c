#include "timer.h"
#include "led.h"
#include "usart.h"

void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//?????3??
 	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB  |RCC_APB2Periph_AFIO, ENABLE);  //??GPIO???AFIO????????
		
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3?????  TIM3_CH2->B5    
																																		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9; 					//TIM_CH2|TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  			//??????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//???GPIO
	
	
	//???TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; 					//???????????????????????????
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					 //??????TIMx??????????? 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					//??????:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 				//??TIM_TimeBaseInitStruct?????????TIMx???????
	
	//TIM4 Channe1 2 PWM
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				//???????:TIM????????2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//????:TIM??????? 						//??T??????????TIM3 OC1
	TIM_OCInitStructure.TIM_Pulse = 0x0000;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	//TIM4 Channe2 2 PWM
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				//???????:TIM????????2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//????:TIM??????? 						//??T??????????TIM3 OC1
	TIM_OCInitStructure.TIM_Pulse = 0x0000;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	//TIM4 Channe3 2 PWM
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				//???????:TIM????????2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//????:TIM??????? 						//??T??????????TIM3 OC1
	TIM_OCInitStructure.TIM_Pulse = 0x0000;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	//TIM4 Channe4 2 PWM
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				//???????:TIM????????2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//????:TIM??????? 						//??T??????????TIM3 OC1
	TIM_OCInitStructure.TIM_Pulse = 0x0000;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  
	TIM_Cmd(TIM4, ENABLE);  
}
/*
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//?????3??
 	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB  |RCC_APB2Periph_AFIO, ENABLE);  //??GPIO???AFIO????????
		
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3?????  TIM3_CH2->B5    
																																		
	
	//????????????,??TIM3 CH2?PWM????	GPIOB.5 GPIOB.4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 					//TIM_CH2|TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  			//??????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//???GPIO
	
	
	//???TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; 					//???????????????????????????
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					 //??????TIMx??????????? 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					//??????:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM??????
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 				//??TIM_TimeBaseInitStruct?????????TIMx???????
	
	//???TIM3 Channe1 2 PWM??	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				//???????:TIM????????2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//????:TIM???????
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  						//??T??????????TIM3 OC1

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  
	TIM_Cmd(TIM4, ENABLE);  //??TIM3
}
*/
/*
void TIM6_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM6, ENABLE);  //使能TIMx					 
}
//定时器3中断服务程序
void TIM6_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
		LED=!LED;
		}
}
*/
