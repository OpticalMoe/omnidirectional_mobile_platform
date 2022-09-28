/********************************************************************************
* @file      S_curve.c
* @author    Chen
* @version
* @date      2017/11/17
* @brief     S曲线程序	   
******************************************************************************
* @attention
*******************************************************************************/

#include "S_curve.h"
#include "math.h"
#include "printf.h"

/*
max 15.384kHz  65us
*/

//S曲线
const unsigned short int Ascending_steps = 3200;	//升频步长
unsigned short int Highest_frequency = 15000; //最高频率k  15000
float Slope = 0.0015;  				//斜率a
signed short int Panning = 2500;    				//偏移b
unsigned short int Lowest_frequency = 150; 	//最低频率c
unsigned short int Upscale_Frequency[Ascending_steps] = {0};//升频频率表
unsigned char Debug_S_curve = 0;									//调试

/**
* @name 	    S_curve_Init
* @brief  		初始化
* @param      增益k,斜率a,平移b,初始增益c
* @retval
*/
void S_curve_Init(unsigned short int gain,float slope,unsigned short int panning,unsigned short int initial_gain){
	Highest_frequency = gain;      		//最高频率k
	Slope = slope;  									//斜率a
	Panning = panning;    						//偏移b
	Lowest_frequency = initial_gain; 	//最低频率c
}

/**
* @name 	    S_curve_Calculate_Init
* @brief  		S曲线初始化计算，方便以后查表
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
* @brief  		查询频率值
* @param      
* @retval			
*/
unsigned short int Query_Frequency(unsigned short int Step){
		return Upscale_Frequency[Step];
}


/**
* @name 	    S_curve_Set
* @brief  		设定最大、最小频率
* @param      最高频率k,最低频率c
* @retval
*/
void S_curve_Set(unsigned short int gain,unsigned short int initial_gain){
	Highest_frequency = gain;      		//最高频率k
	Lowest_frequency = initial_gain; 	//最低频率c
}

/**
* @name 	    S_curve_Inquire_Highest_frequency
* @brief  		查询最大频率
* @param      
* @retval			Highest_frequency 最大频率
*/
unsigned short int S_curve_Inquire_Highest_frequency(void){
	return Highest_frequency;      		//最高频率k
}

/**
* @name 	    S_curve_Inquire_Lowest_frequency
* @brief  		查询最小频率
* @param      
* @retval			Lowest_frequency 最低频率c
*/
unsigned short int S_curve_Inquire_Lowest_frequency(void){
	return Lowest_frequency; 	//最低频率c
}

/**
* @name 	    S_curve_Inquire_Ascending_steps
* @brief  		查询升频步长
* @param      
* @retval			Ascending_steps 升频步长
*/
unsigned short int S_curve_Inquire_Ascending_steps(void){
	return Ascending_steps; 	//升频步长
}

/**
* @name 	  Enable_Debug
* @brief  	使能调试
* @param      
* @retval			
*/
void Enable_Debug_S_curve(void){
	Debug_S_curve = 1;
}

/**
* @name 	  Disenable_Debug
* @brief  	失能调试
* @param      
* @retval			
*/
void Disable_Debug_S_curve(void){
	Debug_S_curve = 0;
}

/**
* @name 	    S_curve_Calculate
* @brief  		S曲线计算
* @param      Step 模型表中对应步数
* @retval			Frequency 频率
*/
unsigned short int S_curve_Calculate(unsigned int Step) {
	unsigned int Frequency = (unsigned int)((Highest_frequency - Lowest_frequency) * (1 / (1 + exp(-Slope * ((signed long int)Step - Panning)))) + Lowest_frequency);
  return Frequency;
}

///************************ (C) COPYRIGHT 2017  Chen *****END OF FILE****/
