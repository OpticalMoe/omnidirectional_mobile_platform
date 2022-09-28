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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM6, ENABLE);  //ʹ��TIMx					 
}
//��ʱ��3�жϷ������
void TIM6_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		LED=!LED;
		}
}
*/
