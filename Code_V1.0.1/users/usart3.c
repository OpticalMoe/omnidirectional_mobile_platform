#include "sys.h"
#include "usart3.h"	  

unsigned short int USART3_RX_BUF[36];		//接受数据缓冲
int CH[5];
int S[2]; 

void USART3_Init(void){
  	
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

}

/*------------------
DMA连续接收数据并持续触发串口总线中断
-----------------*/
//DMA mode 设置为DMA_Mode_Circular
//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         //循环获取数据
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