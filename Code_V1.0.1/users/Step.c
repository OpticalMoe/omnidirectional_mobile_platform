/*****************************************************************************
* @file      Step.c
* @author    Chen
* @version
* @date      2017/11/17
* @brief     ����������Ƴ���		   
******************************************************************************
* @attention
*	  /    	 	   \ 
*    1				2
*   /        	  	 \
*          
*        -- 0 --     
*	DIR=0 -> ��ʱ��ת
* Ƶ�ʾ���������������������
*
*		STEP: A0 / A6 / B6
*       DIR : B3 / 4  / 5
*       MS  : B12/ 13 / 14
*       EN  : B15 �͵�ƽ��Ч
*******************************************************************************/

	
#include "Step.h"
#include "S_curve.h"
#include <math.h>

#include "printf.h"

const unsigned short int Automatic_reload_value = 1000 - 1;	//�Զ���װ��ֵ  max 65us 15kHz
const unsigned short int Frequency = 8 - 1;	//8��Ƶ  9MHz

unsigned char Debug_Step = 0;		//���ڵ��Ա�־

signed char Flag_Dir[3] ={1};									//�����־
unsigned long int Step[3] ={0};								//����
float Coefficient[3] = {1};										//�����ּ�ϵ����ϵ
unsigned short int ARR[3] = {1000,1000,1000};	//�Զ�����ֵ
unsigned short int ARR_Fixed = 0;							//��������
unsigned long int Max_Pulse = 100;						//���������
unsigned char Label = 0;											//��ţ���¼������������
unsigned char Status = 0;											//״̬��� ͣ-����-����-����
const float Lenght_Pulse = 0.054664;					//ϵ����ת����ʵ�ʾ���//0.113883;0.054664;  
unsigned short int Ascending_step = 0;				//��Ƶ����

/**
* @name 	    Query_Step_Remain
* @brief  		��ѯʣ�ಽ��
* @param      
* @retval			
*/
unsigned long int Query_Step_Remain(void){
		return Step[Label];
}

/**
* @name 	    Query_Step_Run
* @brief  		��ѯִ�в���
* @param      
* @retval			
*/
unsigned long int Query_Step_Run(void){
		return Max_Pulse - Step[Label];
}

/**
* @name 	    Step_Init
* @brief  		��ʼ��������������������
* @param      
* @retval			
*/
void Step_Init(void){  	
	GPIO_InitTypeDef GPIO_InitStructure;		
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
	TIM_OCInitTypeDef  TIM_OCInitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;	
	
/********************* ��ʼ��GPIO **********************************/	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��
    
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
     GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	                // JTAG DISABLE,SWD ENABLE
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |\
                                   GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;	//PB�˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 	                //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 		            //IO���ٶ�Ϊ2MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 				    //�����趨������ʼ��PA.1/PA.7
	 GPIO_SetBits(GPIOB,GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);				    //PA.1/PA.7�����
	 GPIO_SetBits(GPIOB,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);	//PA.1/PA.7�����

/********************* ��ʼ����ʱ�� **********************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 	//ʹ�ܶ�ʱ��1ʱ��
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

	TIM_TimeBaseStructure.TIM_Period = Automatic_reload_value;	//�Զ���װ��ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler = Frequency;						//��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 								//ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 						//��ʼ����ʱ��2
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 						//��ʼ����ʱ��3
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 						//��ʼ����ʱ��4

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 						//TIMx_CNT<TIMx_CCR1 ʱ��Ч��ƽ
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //���ʹ��	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//�ߵ�ƽ��Ч
	TIM_OCInitStructure.TIM_Pulse = 0x0005;												//��ʼ������Ϊ0
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);											//����Tָ���Ĳ�����ʼ������TIM2 OC1
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); 						//ʹ��TIM2��CCR1�ϵ�Ԥװ�ؼĴ���,��TIM2_CCR1��Ԥװ��ֵ�ڸ����¼�����ʱ���ܱ���������ǰ�Ĵ����С� 
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);											//����Tָ���Ĳ�����ʼ������TIM3 OC1
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 						//ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���,��TIM3_CCR1��Ԥװ��ֵ�ڸ����¼�����ʱ���ܱ���������ǰ�Ĵ����С� 
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);											//����Tָ���Ĳ�����ʼ������TIM4 OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); 						//ʹ��TIM4��CCR1�ϵ�Ԥװ�ؼĴ���,��TIM4_CCR1��Ԥװ��ֵ�ڸ����¼�����ʱ���ܱ���������ǰ�Ĵ����С� 

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  					//TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  			//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  					//TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  			//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  													//��ʼ��NVIC�Ĵ���
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  					//TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  			//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  													//��ʼ��NVIC�Ĵ���

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx�����жϱ�־
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx�����жϱ�־
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx�����жϱ�־
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);				//ʹ��TIM�����ж�Դ
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);				//ʹ��TIM�����ж�Դ
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);				//ʹ��TIM�����ж�Դ
	
	TIM_Cmd(TIM2, DISABLE);  					//ʧ��
	TIM_Cmd(TIM3, DISABLE);  					//ʧ��
	TIM_Cmd(TIM4, DISABLE);  					//ʧ��
	Calculation(0,0,0);
}

/**
* @name 	    Calculation
* @brief  		����   ����->x, ��ǰ->y��angle:˳ʱ��Ϊ�����Ƕ�ֵ
* @param      float x, float y, float angle
* @retval			
*/
void Calculation(float x, float y, float angle) {
	u8 i = 0;
	signed long int Value_Pulse[3] = {0};		//����ֵ		
	
  angle = angle * 1.6;					//ת����ʵ�ʽǶ�ֵ
  //����
  Value_Pulse[0] = (long int)((x  + angle) / Lenght_Pulse + 0.5);
  Value_Pulse[1] = (long int)((-x  * 0.5 - y * 0.86 + angle) / Lenght_Pulse + 0.5);//y*=0.866
  Value_Pulse[2] = (long int)((-x  * 0.5 + y * 0.86 + angle) / Lenght_Pulse + 0.5);
	
	//ִ��//ͣ
	TIM_Cmd(TIM2, DISABLE);  					//ʧ��
	TIM_Cmd(TIM3, DISABLE);  					//ʧ��
	TIM_Cmd(TIM4, DISABLE);  					//ʧ��		
	//�趨����
	Flag_Dir[0] = Flag_Dir[1] = Flag_Dir[2] = -1; 
	if(Value_Pulse[0] > 0) Flag_Dir[0] =DIR_0 = 1;	//�趨����		
	else DIR_0 = 0;
	if(Value_Pulse[1] < 0) Flag_Dir[1] = DIR_1 = 1;	//�趨����
	else DIR_1 = 0;
	if(Value_Pulse[2] > 0) Flag_Dir[2] = DIR_2 = 1;	//�趨����
	else DIR_2 = 0;
	//��¼����
	for(i = 0;i < 3;i++){
		Step[i] = Value_Pulse[i] < 0 ? -Value_Pulse[i] : Value_Pulse[i];	//ȡ����ֵ
	}
	//��¼���������������
	if(Step[0] > Step[1]){
		if(Step[0] > Step[2]) Label = 0;
		else Label = 2;
	}
	else if(Step[2] > Step[1]) Label = 2;
			 else Label = 1;
	Max_Pulse = Step[Label];		//��¼���������
	//����ϵ��
	for(i = 0;i < 3;i++){
		Coefficient[i] = (float)Step[i] / (float)Step[Label];		//���������ּ�ϵ����ϵ
	}	
	//��������������Ƿ񹻼Ӽ���
	if(Max_Pulse < S_curve_Inquire_Ascending_steps()) Ascending_step = (Max_Pulse / 2 - 3200) > 0 ? (Max_Pulse / 2 - 3200):Max_Pulse / 2;
	else Ascending_step = S_curve_Inquire_Ascending_steps();
	
	//�������
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
* @brief  	ʹ�ܶ�ʱ��	
* @param      
* @retval			
*/
void Start(void){
	TIM_Cmd(TIM2, DISABLE);  					//ʧ��
	TIM_Cmd(TIM3, DISABLE);  					//ʧ��
	TIM_Cmd(TIM4, DISABLE);  					//ʧ��
	Enable_Stepper();									//ʹ�ܵ��
	ARR[0] = ARR[1] = ARR[2] = 9000000 / (Query_Frequency(0)) + 0.5;
	TIM_SetAutoreload(TIM2,ARR[0]);//����TIMx�Զ���װ�ؼĴ���ֵ		 
	TIM_SetAutoreload(TIM3,ARR[1]);//����TIMx�Զ���װ�ؼĴ���ֵ		
	TIM_SetAutoreload(TIM4,ARR[2]);//����TIMx�Զ���װ�ؼĴ���ֵ	
	TIM_SetCounter(TIM2, 0x0000);
	TIM_SetCounter(TIM3, 0x0000);
	TIM_SetCounter(TIM4, 0x0000);
	if(Step[0]) TIM_Cmd(TIM2, ENABLE);  					//ʹ��
	if(Step[1]) TIM_Cmd(TIM3, ENABLE);  					//ʹ��
	if(Step[2]) TIM_Cmd(TIM4, ENABLE);  					//ʹ��
}

/**
* @name 	  Stop  
* @brief  	ʧ�ܶ�ʱ��	
* @param      
* @retval			
*/
void Stop(void){
	Step[Label] = 0;
	TIM_Cmd(TIM2, DISABLE);  					//ʧ��
	TIM_Cmd(TIM3, DISABLE);  					//ʧ��
	TIM_Cmd(TIM4, DISABLE);  					//ʧ��
}

/**
* @name 	  Enable_Stepper
* @brief  	�������ʹ��
* @param      
* @retval			
*/
void Enable_Stepper(void){
	EN = 0;
}

/**
* @name 	  Disenable_Stepper
* @brief  	�������ʧ��
* @param      
* @retval			
*/
void Disenable_Stepper(void){
	EN = 1;
}

/**
* @name 	  Enable_Debug
* @brief  	ʹ�ܵ���
* @param      
* @retval			
*/
void Enable_Debug_Step(void){
	Debug_Step = 1;
}

/**
* @name 	  Disenable_Debug
* @brief  	ʧ�ܵ���
* @param      
* @retval			
*/
void Disable_Debug_Step(void){
	Debug_Step = 0;
}

/**
* @name 	  Subdivision
* @brief  	ϸ��
* @param    Subdivision ϸ��
* @retval		0->ʧ�� 1->�ɹ�
*/
unsigned char Subdivision(unsigned char Subdivision){
	switch(Subdivision){
		case 0:				//��ϸ��
			MS_0 = 0;
			MS_1 = 0;
			MS_2 = 0;
			break;
		case 2:				//1/2ϸ��
			MS_0 = 1;
			MS_1 = 0;
			MS_2 = 0;
			break;
		case 4:				//1/4ϸ��
			MS_0 = 0;
			MS_1 = 1;
			MS_2 = 0;
			break;
		case 8:				//1/8ϸ��
			MS_0 = 1;
			MS_1 = 1;
			MS_2 = 0;
			break;
		case 16:			//1/16ϸ��
			MS_0 = 1;
			MS_1 = 1;
			MS_2 = 1;
			break;
		default:			//1/16ϸ��
			MS_0 = 1;
			MS_1 = 1;
			MS_2 = 1;
		return 0;
	}
	return 1;
}

/**
* @name 	  TIM2_IRQHandler
* @brief  	��ʱ��2�жϷ������
* @param      
* @retval			
*/
void TIM2_IRQHandler(void){   //TIM2�ж�
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  					//���TIM2�����жϷ������
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  					//���TIMx�����жϱ�־ 
			if(!(--Step[0])) TIM_Cmd(TIM2, DISABLE);   							//�������꣬�رն�ʱ��
			if((Max_Pulse - Step[Label]) < (Ascending_step - 2)) {	//����
				ARR[0] = 9000000 / (Coefficient[0] * Query_Frequency(Max_Pulse - Step[Label] + 1)) + 0.5;
				TIM_SetAutoreload(TIM2,ARR[0]);												//����TIMx�Զ���װ�ؼĴ���ֵ	
			}
			else if(Step[Label] < (Ascending_step - 2)){ 						//����
						ARR[0] = 9000000 / (Coefficient[0] * Query_Frequency(Step[Label] + 1)) + 0.5;
						TIM_SetAutoreload(TIM2,ARR[0]);										//����TIMx�Զ���װ�ؼĴ���ֵ	
					}	
		}
}

/**
* @name 	  TIM3_IRQHandler
* @brief  	��ʱ��3�жϷ������
* @param      
* @retval			
*/
void TIM3_IRQHandler(void){   //TIM3�ж�
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  					//���TIM3�����жϷ������
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  					//���TIMx�����жϱ�־
			if(!(--Step[1])) TIM_Cmd(TIM3, DISABLE);   							//�������꣬�رն�ʱ��
			if((Max_Pulse - Step[Label]) < (Ascending_step - 2)) {	//����
				ARR[1] = 9000000 / (Coefficient[1] * Query_Frequency(Max_Pulse - Step[Label] + 1)) + 0.5;
				TIM_SetAutoreload(TIM3,ARR[1]);												//����TIMx�Զ���װ�ؼĴ���ֵ	
			}
			else if(Step[Label] < (Ascending_step - 2)){ 						//����
						ARR[1] = 9000000 / (Coefficient[1] * Query_Frequency(Step[Label] + 1)) + 0.5;
						TIM_SetAutoreload(TIM3,ARR[1]);										//����TIMx�Զ���װ�ؼĴ���ֵ	
					}	
		}
}

/**
* @name 	  TIM4_IRQHandler
* @brief  	��ʱ��4�жϷ������
* @param      
* @retval			
*/
void TIM4_IRQHandler(void){   //TIM4�ж�
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  					//���TIM4�����жϷ������
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  					//���TIMx�����жϱ�־ 
			if(!(--Step[2])) TIM_Cmd(TIM4, DISABLE);   							//�������꣬�رն�ʱ��
			if((Max_Pulse - Step[Label]) < (Ascending_step - 2)) {	//����
				ARR[2] = 9000000 / (Coefficient[2] * Query_Frequency(Max_Pulse - Step[Label] + 1)) + 0.5;
				TIM_SetAutoreload(TIM4,ARR[2]);												//����TIMx�Զ���װ�ؼĴ���ֵ	
			}
			else if(Step[Label] < (Ascending_step - 2)){ 						//����
						ARR[2] = 9000000 / (Coefficient[2] * Query_Frequency(Step[Label] + 1)) + 0.5;
						TIM_SetAutoreload(TIM4,ARR[2]);										//����TIMx�Զ���װ�ؼĴ���ֵ	
					}	
		}
}

///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
