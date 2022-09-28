#include "usart.h"
#include "delay.h"


u8 USART2_RX_BUF[200];		//接受数据缓冲

void usart2_Init()		//*****************
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//**********
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//*************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //开时钟
		
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;		//TX**********
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; 	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//****************
		
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;	//RX***********
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		//模拟输入		
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//************
	
	USART_InitStructure.USART_BaudRate = 9600;		//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据帧长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//停止位1位
	USART_InitStructure.USART_Parity = USART_Parity_No;		//没有奇偶检验位
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//硬件流失能  硬件流即 DMA
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;	//发送接受 使能
	USART_Init(USART2,&USART_InitStructure);						//************
	
	USART_Cmd(USART2, ENABLE);		//串口1使能，打开串口		***************
	
	//配置中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);			//组1，1位抢断优先级，3位响应优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢断优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//响应优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	//打开串口1的全局中断请求***************
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//
	NVIC_Init(&NVIC_InitStructure); 
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//打开串口中断服务			***************
	
}

int Buf_number;
void USART2_IRQHandler(void)			//*****************
{
	u8 k = 0;
	USART_ClearFlag(USART2,USART_FLAG_TC);	//清除发送完成标志位*******************
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=Bit_RESET)	//**************
	{
		k = USART_ReceiveData(USART2);			//************
		USART2_RX_BUF[Buf_number] = k;

	}
}

