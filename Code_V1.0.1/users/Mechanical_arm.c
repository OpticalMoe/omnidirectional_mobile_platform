#include "Mechanical_arm.h"
#include "led.h"
#include "math.h"
#include "key.h"
#include "time.h"
#include "delay.h"
#include "usart.h"
#include "printf.h"

struct Dobot{
	float Angle_BigArm,Angle_SmallArm;
	float X,Y,Z;
}New,Last;
float Length_BigArm = 131.9,Length_SmallArm = 143;
float Limit_BigArm_Min,Limit_BigArm_Max,Limit_SmallArm_Min,Limit_SmallArm_Max;

u8 Mechanical_Arm_Init(){
	unsigned int millis = 0;
	u8 i = 20;
	LED = 0;
	delay_ms(1000);
	TIM4_PWM_Init (2999,71);//3ms
	TIM4->CCR1=1500;	
	TIM4->CCR2=2200;	
	TIM4->CCR3=1650;
	TIM4->CCR4=1500;
	delay_ms (100);	
	while(KEY_BigArm == 0);
	while(millis < 2000){
		TIM4->CCR1=1200 - millis * 0.5;	
		if(KEY_BigArm == 0) break;
		delay_ms(5);
		millis++;
	}	
	Limit_BigArm_Max = (1200 - millis * 0.5) / 11.11 + 68.2;
	Limit_BigArm_Min = Limit_BigArm_Max - 133.4;
	LED = !LED;

	while(KEY_SmallArm == 0);
	millis = 0;
	while(millis < 3000){
		TIM4->CCR2=2200 + millis * 0.5;	
		if(KEY_SmallArm == 0) break;
		delay_ms(3);
		millis++;
	}
	Limit_SmallArm_Min = (2200 + millis * 0.5) / 11.11 -243;
	Limit_SmallArm_Max = Limit_SmallArm_Min + 114;
	
	Last.Angle_BigArm = Limit_BigArm_Max;
	Last.Angle_SmallArm = Limit_SmallArm_Min;
	New.Angle_BigArm = Last.Angle_BigArm;
	New.Angle_SmallArm = Last.Angle_SmallArm;
	
	printf("\t\r\nLast.Angle_BigArm:%f",Last.Angle_BigArm);
	printf("\t\r\nLast.Angle_SmallArm:%f",Last.Angle_SmallArm);
	
	printf("\r\n\r\nLimit_BigArm_Min:%f",Limit_BigArm_Min);
	printf("\t\r\nLimit_BigArm_Max:%f",Limit_BigArm_Max);
	printf("\t\r\nLimit_SmallArm_Min:%f",Limit_SmallArm_Min);
	printf("\t\r\nLimit_SmallArm_Max:%f",Limit_SmallArm_Max);
		
	while(i--){
		LED = !LED;
		delay_ms(50);
	}
	LED = 0;
	return 0;
}

u8 Calculation(){
	float Angle_Bottom_L,Angle_Vertex,Angle;
	float Angle_Gear_Vertex,Angle_Gear_Bottom;
	u8 Flag = 0;
	Angle_Bottom_L = acos(sqrt(pow(Length_BigArm,2) + pow(New.X,2) + pow(New.Z,2) - pow(Length_SmallArm,2)) / (2 * Length_BigArm * sqrt(pow(New.X,2) + pow(New.Z,2))));
	Angle_Vertex = acos(sqrt(pow(Length_BigArm,2) + pow(Length_SmallArm,2) - pow(New.X,2) - pow(New.Z,2) ) / (2 * Length_BigArm * Length_SmallArm));
	Angle = acos(New.X / sqrt(pow(New.X,2) + pow(New.Z,2)));

	Angle_Gear_Bottom = 57.3 * (Angle + Angle_Bottom_L);
	Angle_Gear_Vertex = 180 - 57.3 * (Angle_Bottom_L + Angle_Vertex + Angle);
	
	if(Angle_Gear_Vertex < Limit_SmallArm_Min) Flag += 1;
	if(Angle_Gear_Vertex > Limit_SmallArm_Max) Flag += 2;
	if(Angle_Gear_Bottom < Limit_BigArm_Min) Flag += 4;
	if(Angle_Gear_Bottom > Limit_BigArm_Max) Flag += 8;
	if(Angle_Vertex * 57.3 < 45) Flag += 16;
	if(Angle_Vertex * 57.3 > 150) Flag += 32;
	

	if(!Flag){
		New.Angle_BigArm = Angle_Gear_Bottom;
		New.Angle_SmallArm = Angle_Gear_Vertex;
	}
	else printf("\r\n\r\nError:%d",Flag);
		
	printf("\r\n\r\nNew.X:%f",New.X);
	printf("\t\r\nNew.Z:%f",New.Z);

	printf("\r\nAngle_Vertex:%f",Angle_Vertex);
	printf("\r\nAngle_Gear_Bottom:%f",Angle_Gear_Bottom);
	printf("\t\r\nAngle_Gear_Vertex:%f",Angle_Gear_Vertex);
	
	return Flag;	
}

u8 Carried_out(){
	char Threshold = 1;
	u8 Flag = 0;
	float Angle_BigArm = Last.Angle_BigArm, Angle_SmallArm = Last.Angle_SmallArm;
	if(New.Angle_BigArm - Last.Angle_BigArm > Threshold) Last.Angle_BigArm += Threshold;		
	else if(New.Angle_BigArm - Last.Angle_BigArm < -Threshold) Last.Angle_BigArm -= Threshold;		
	else Last.Angle_BigArm = New.Angle_BigArm;

	if(New.Angle_SmallArm - Last.Angle_SmallArm > Threshold) Last.Angle_SmallArm += Threshold;		
	else if(New.Angle_SmallArm - Last.Angle_SmallArm < -Threshold) Last.Angle_SmallArm -= Threshold;		
	else Last.Angle_SmallArm = New.Angle_SmallArm;
	
	if(Last.Angle_BigArm != Angle_BigArm) {
		TIM4->CCR1 = (Limit_BigArm_Max * 11.11 + 724) - Last.Angle_BigArm * 11.11;	
//		printf("\r\n\r\nLast.Angle_BigArm:%f",Last.Angle_BigArm);
		printf("\r\n\r\nTIM4->CCR1:%d",TIM4->CCR1);
		Flag++;
	}
	if(Last.Angle_SmallArm != Angle_SmallArm) {
		TIM4->CCR2 = ((Limit_SmallArm_Min + 25) * 11.11 + 2145) + Last.Angle_SmallArm * 11.11;
//		printf("\r\nLast.Angle_SmallArm:%f",Last.Angle_SmallArm);
		printf("\r\nTIM4->CCR2:%d",TIM4->CCR2);
		Flag++;
	}
	return Flag;//0 -> Ö´ÐÐÍê
 }
