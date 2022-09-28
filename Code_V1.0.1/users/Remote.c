/********************************************************************************
* @file         Remote.c
* @author       Chen
* @version
* @date         2019/2/2
* @brief        遥控	   
******************************************************************************
* @attention    
*******************************************************************************/

#include "Remote.h"
#include "sys.h"

unsigned short int USART3_RX_BUF[36];		//接受数据缓冲
int CH[5];
int S[2]; 

/**
* @name 	USART3_DMA_Init
* @brief  	串口3DMA初始化
* @param    
* @retval			
*/
void USART3_DMA_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
        
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //使能UART3所在GPIOB的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //串口时钟配置	 
     
     
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;          
    GPIO_Init(GPIOB, &GPIO_InitStructure);	
     
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				            
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	       	  //推挽输出   ULTR_EN-->PB.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		        //IO口速度为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);					            //根据设定参数初始化PB.10
     
    USART_InitStructure.USART_BaudRate = 100000;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx;	//收模式
    USART_Init(USART3, &USART_InitStructure);
     
    //Usart3 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
    
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);  //使能串口3 DMA功能
    
    USART_ClearFlag(USART3, USART_FLAG_IDLE);
//    USART_ITConfig(USART3,USART_IT_TC,DISABLE);  //关闭发送中断 
//    USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);//关闭接收中断  
    USART_ITConfig(USART3,USART_IT_IDLE,ENABLE); //开启总线空闲中断 
    USART_Cmd(USART3, ENABLE);	      // 使能 USART3

    /* -------------- DMA ---------------------------------------*/
    DMA_InitTypeDef DMA_InitStructure;	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                      //使能DMA1时钟  
    DMA_DeInit(DMA1_Channel3);                                              //初始化DMA1通道3  串口3接收通道
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART3->DR);   //串口3接收数据的地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART3_RX_BUF;         //数据存放的复制
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      //数据传送方向  串口3作为数据源
    DMA_InitStructure.DMA_BufferSize = 18u;                                 //接收数据个数       
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //外围设备 串口3 地址不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // DMA存储地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外围设备 串口3 数据宽度 8bits
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //DMA存储 数据宽度 8bits
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         //循环获取数据
    //DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular （不停地传送）
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                   //串口3通道优先为中等
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            //关闭内存到内存
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel3, ENABLE);
}

/**
* @name 	USART3_IRQHandler
* @brief  	串口3DMA空闲中断
* @param    
* @retval   DMA连续接收数据并持续触发串口总线中断；循环获取数据
*/                       
void USART3_IRQHandler(void)                	
{	
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_ReceiveData(USART3);
    }
    else if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)		//串口3	总线空闲	
        {
            USART_ReceiveData(USART3);
            USART_ClearITPendingBit(USART3, USART_IT_IDLE);  //清除标记位

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
* @retval   DMA连续接收数据并持续触发串口总线中断；循环获取数据
*/                       
void Remote(void)                	
{	

    
}


///************************ (C) COPYRIGHT 2019  Chen *****END OF FILE****/