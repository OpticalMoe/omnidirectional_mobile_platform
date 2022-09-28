/********************************************************************************
* @file      S_curve.c
* @author    Chen
* @version
* @date      2017/11/17
* @brief     S���߳���	   
******************************************************************************
* @attention
*******************************************************************************/

#include "S_curve.h"
#include "math.h"
#include "printf.h"

/*
max 15.384kHz  65us
*/

//S����
const unsigned short int Ascending_steps = 3200;	//��Ƶ����
unsigned short int Highest_frequency = 15000; //���Ƶ��k  15000
float Slope = 0.0015;  				//б��a
signed short int Panning = 2500;    				//ƫ��b
unsigned short int Lowest_frequency = 150; 	//���Ƶ��c
unsigned short int Upscale_Frequency[Ascending_steps] = {0};//��ƵƵ�ʱ�
unsigned char Debug_S_curve = 0;									//����

/**
* @name 	    S_curve_Init
* @brief  		��ʼ��
* @param      ����k,б��a,ƽ��b,��ʼ����c
* @retval
*/
void S_curve_Init(unsigned short int gain,float slope,unsigned short int panning,unsigned short int initial_gain){
	Highest_frequency = gain;      		//���Ƶ��k
	Slope = slope;  									//б��a
	Panning = panning;    						//ƫ��b
	Lowest_frequency = initial_gain; 	//���Ƶ��c
}

/**
* @name 	    S_curve_Calculate_Init
* @brief  		S���߳�ʼ�����㣬�����Ժ���
* @param      
* @retval			
*/
void S_curve_Calculate_Init(void){
	unsigned short int Step = 0;
	for(Step = 0;Step < Ascending_steps;Step++){
		Upscale_Frequency[Step] = (unsigned short int)((Highest_frequency - Lowest_frequency) * (1 / (1 + exp(-Slope * ((signed int)Step - Panning)))) + Lowest_frequency);
		if(Debug_S_curve) printf("NO%d:\t%d\r\n",Step,Query_Frequency(Step));
	}
}

/**
* @name 	    Query_Frequency
* @brief  		��ѯƵ��ֵ
* @param      
* @retval			
*/
unsigned short int Query_Frequency(unsigned short int Step){
		return Upscale_Frequency[Step];
}


/**
* @name 	    S_curve_Set
* @brief  		�趨�����СƵ��
* @param      ���Ƶ��k,���Ƶ��c
* @retval
*/
void S_curve_Set(unsigned short int gain,unsigned short int initial_gain){
	Highest_frequency = gain;      		//���Ƶ��k
	Lowest_frequency = initial_gain; 	//���Ƶ��c
}

/**
* @name 	    S_curve_Inquire_Highest_frequency
* @brief  		��ѯ���Ƶ��
* @param      
* @retval			Highest_frequency ���Ƶ��
*/
unsigned short int S_curve_Inquire_Highest_frequency(void){
	return Highest_frequency;      		//���Ƶ��k
}

/**
* @name 	    S_curve_Inquire_Lowest_frequency
* @brief  		��ѯ��СƵ��
* @param      
* @retval			Lowest_frequency ���Ƶ��c
*/
unsigned short int S_curve_Inquire_Lowest_frequency(void){
	return Lowest_frequency; 	//���Ƶ��c
}

/**
* @name 	    S_curve_Inquire_Ascending_steps
* @brief  		��ѯ��Ƶ����
* @param      
* @retval			Ascending_steps ��Ƶ����
*/
unsigned short int S_curve_Inquire_Ascending_steps(void){
	return Ascending_steps; 	//��Ƶ����
}

/**
* @name 	  Enable_Debug
* @brief  	ʹ�ܵ���
* @param      
* @retval			
*/
void Enable_Debug_S_curve(void){
	Debug_S_curve = 1;
}

/**
* @name 	  Disenable_Debug
* @brief  	ʧ�ܵ���
* @param      
* @retval			
*/
void Disable_Debug_S_curve(void){
	Debug_S_curve = 0;
}

/**
* @name 	    S_curve_Calculate
* @brief  		S���߼���
* @param      Step ģ�ͱ��ж�Ӧ����
* @retval			Frequency Ƶ��
*/
unsigned short int S_curve_Calculate(unsigned int Step) {
	unsigned int Frequency = (unsigned int)((Highest_frequency - Lowest_frequency) * (1 / (1 + exp(-Slope * ((signed long int)Step - Panning)))) + Lowest_frequency);
  return Frequency;
}

///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
