#ifndef __PID_H
#define __PID_H

#include "stm32f4xx.h"

extern float Pitch_max, K_Time, L, K, PitchSpeed_last, dk;
extern int PitchSpeed;

#endif
