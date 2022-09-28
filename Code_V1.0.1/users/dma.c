#include "dma.h"

extern unsigned short int USART3_RX_BUF[36]; 
    
//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量 
void Usart3_DMA_Init(void)
{
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
