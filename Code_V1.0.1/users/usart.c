#include "usart.h"
#include "delay.h"


u8 USART2_RX_BUF[200];		//�������ݻ���

void usart2_Init()		//*****************
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//**********
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//*************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //��ʱ��
		
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;		//TX**********
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; 	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//****************
		
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;	//RX***********
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		//ģ������		
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//************
	
	USART_InitStructure.USART_BaudRate = 9600;		//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����֡����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ1λ
	USART_InitStructure.USART_Parity = USART_Parity_No;		//û����ż����λ
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//Ӳ����ʧ��  Ӳ������ DMA
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;	//���ͽ��� ʹ��
	USART_Init(USART2,&USART_InitStructure);						//************
	
	USART_Cmd(USART2, ENABLE);		//����1ʹ�ܣ��򿪴���		***************
	
	//�����ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);			//��1��1λ�������ȼ���3λ��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//�������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	//�򿪴���1��ȫ���ж�����***************
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//
	NVIC_Init(&NVIC_InitStructure); 
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//�򿪴����жϷ���			***************
	
}

int Buf_number;
void USART2_IRQHandler(void)			//*****************
{
	u8 k = 0;
	USART_ClearFlag(USART2,USART_FLAG_TC);	//���������ɱ�־λ*******************
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=Bit_RESET)	//**************
	{
		k = USART_ReceiveData(USART2);			//************
		USART2_RX_BUF[Buf_number] = k;

	}
}

