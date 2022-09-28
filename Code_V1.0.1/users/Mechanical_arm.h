#ifndef __Mechanical_arm_H
#define __Mechanical_arm_H
#include "sys.h"

void TIM4_PWM_Init (u16 arr,u16 psc);
u8 Calculation();
u8 Mechanical_Arm_Init();
u8 Carried_out();
#endif
