/********************************************************************************
* @file         Remote.c
* @author       Chen
* @version
* @date         2019/2/2
* @brief        ң��	   
******************************************************************************
* @attention    
*******************************************************************************/

#include "Remote.h"
#include "sys.h"

unsigned short int USART3_RX_BUF[36];		//�������ݻ���
int CH[5];
int S[2]; 

/**
* @name 	USART3_DMA_Init
* @brief  	����3DMA��ʼ��
* @param    
* @retval			
*/
void USART3_DMA_Init(void){
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

    /* -------------- DMA ---------------------------------------*/
    DMA_InitTypeDef DMA_InitStructure;	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                      //ʹ��DMA1ʱ��  
    DMA_DeInit(DMA1_Channel3);                                              //��ʼ��DMA1ͨ��3  ����3����ͨ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART3->DR);   //����3�������ݵĵ�ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART3_RX_BUF;         //���ݴ�ŵĸ���
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      //���ݴ��ͷ���  ����3��Ϊ����Դ
    DMA_InitStructure.DMA_BufferSize = 18u;                                 //�������ݸ���       
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //��Χ�豸 ����3 ��ַ������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // DMA�洢��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //��Χ�豸 ����3 ���ݿ�� 8bits
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //DMA�洢 ���ݿ�� 8bits
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         //ѭ����ȡ����
    //DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular ����ͣ�ش��ͣ�
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                   //����3ͨ������Ϊ�е�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            //�ر��ڴ浽�ڴ�
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel3, ENABLE);
}

/**
* @name 	USART3_IRQHandler
* @brief  	����3DMA�����ж�
* @param    
* @retval   DMA�����������ݲ������������������жϣ�ѭ����ȡ����
*/                       
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

/**
* @name 	Remote
* @brief  	
* @param    
* @retval   DMA�����������ݲ������������������жϣ�ѭ����ȡ����
*/                       
void Remote(void)                	
{	

    
}


///************************ (C) COPYRIGHT 2019  Chen *****END OF FILE****/