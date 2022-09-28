#include "dma.h"

extern unsigned short int USART3_RX_BUF[36]; 
    
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ����� 
void Usart3_DMA_Init(void)
{
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
