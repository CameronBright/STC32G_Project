#ifndef __SYS_H_
#define __SYS_H_

#include "stc32g.h"



typedef unsigned char u8;				//unsigned char			 	类型，用u8表示
typedef unsigned int u16;				//unsigned int 				类型，用u16表示
typedef unsigned long int u32;	//unsigned long int 	类型，用u32表示

extern int Left_moto1     ;//左风机PWM
extern int Right_moto2    ;//右
extern int Forwar_dmoto1  ;//前
extern int Backward_moto2 ;//后

#endif
