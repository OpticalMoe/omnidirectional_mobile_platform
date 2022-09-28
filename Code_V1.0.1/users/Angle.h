#ifndef __Angle_H
#define __Angle_H	 
#include "sys.h"

unsigned char Angle_Init(void);						//陀螺仪初始化
unsigned short Query_Angle_Initial(void);	//查询初始化角度的原始数据值
float Query_Angle_Init(void);							//查询初始化角度
float Query_Angle(void);									//查询角度

#endif
