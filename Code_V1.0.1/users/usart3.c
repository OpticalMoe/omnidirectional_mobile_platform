#include "sys.h"
#include "usart3.h"	  

unsigned short int USART3_RX_BUF[36];		//�������ݻ���
int CH[5];
int S[2]; 

void USART3_Init(void){
  	
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
        
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //ʹ��UART3����GPIOB��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //����ʱ������	 
     
     
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;          
    GPIO_Init(GPIOB, &GPIO_InitStructure);	
     
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				            
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	       	  //�������   ULTR_EN-->PB.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		        //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);					            //�����趨������ʼ��PB.10
     
    USART_InitStructure.USART_BaudRate = 100000;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx;	//��ģʽ
    USART_Init(USART3, &USART_InitStructure);
     
    //Usart3 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);  //ʹ�ܴ���3 DMA����
    
    USART_ClearFlag(USART3, USART_FLAG_IDLE);
//    USART_ITConfig(USART3,USART_IT_TC,DISABLE);  //�رշ����ж� 
//    USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);//�رս����ж�  
    USART_ITConfig(USART3,USART_IT_IDLE,ENABLE); //�������߿����ж� 
    USART_Cmd(USART3, ENABLE);	      // ʹ�� USART3

}

/*------------------
DMA�����������ݲ������������������ж�
-----------------*/
//DMA mode ����ΪDMA_Mode_Circular
//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         //ѭ����ȡ����
void USART3_IRQHandler(void)                	
{	
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_ReceiveData(USART3);
    }
    else if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)		//����3	���߿���	
        {
            USART_ReceiveData(USART3);
            USART_ClearITPendingBit(USART3, USART_IT_IDLE);  //������λ

            CH[0] = (USART3_RX_BUF[0] | (USART3_RX_BUF[1] << 8)) & 0x07ff;        //!< Channel 0
            CH[1] = ((USART3_RX_BUF[1] >> 3) | (USART3_RX_BUF[2] << 5)) & 0x07ff; //!< Channel 1
            CH[2] = ((USART3_RX_BUF[2] >> 6) | (USART3_RX_BUF[3] << 2) |          //!< Channel 2
                    (USART3_RX_BUF[4] << 10)) & 0x07ff;
            CH[3] = ((USART3_RX_BUF[4] >> 1) | (USART3_RX_BUF[5] << 7)) & 0x07ff; //!< Channel 3
            CH[4] = (USART3_RX_BUF[16] | (USART3_RX_BUF[17] << 8)) & 0x07ff;      //!< Channel 4

            S[0] = ((USART3_RX_BUF[5] >> 4) & 0x0003);                            //!< Switch left
            S[1] = ((USART3_RX_BUF[5] >> 4) & 0x000C) >> 2;                       //!< Switch right

            CH[0] -= 1024;
            CH[1] -= 1024;
            CH[2] -= 1024;
            CH[3] -= 1024;
            CH[4] -= 1024;
        }
}