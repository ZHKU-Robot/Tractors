#ifndef __CONTROL_H
#define __CONTROL_H

#include "stm32f4xx.h"
#include "Usart.h"
#include "Light.h"
#include "_Machinenary_Shop_GPIO.h"

extern uint8_t recv_end_flag;
extern int16_t ecode1, ecode2, ecode3, ecode4;
extern int16_t YawLocationCode,YawActualECode,YawGiveECode,PitchLocationCode,PitchActualECode,PitchGiveECode;
extern CanRxMsg RxMessage;
extern uint8_t can1_rx_data_buf[12][6],can2_rx_data_buf[12][6];
extern int NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8,NUM9;
extern uint8_t Gyro_buff[30];
extern float Pitch ,  Roll , Yaw ,Yaw_LocaDiffValue;
//extern int Yaw ;
extern float wx , wy , wz;
extern int RC_DiffValue[8],RC_IntegralValue[8];
extern int Yaw_DiffValue[5],Yaw_IntegralValue[5];
extern int Pitch_DiffValue[5],Pitch_IntegralValue[5];
extern int expect_revolutions[8];
extern int UnderpanFollowSpeed;
extern int ExpectYawAngle,YawMaxAngle,YawMinAngle,MissAngle;
extern int Yaw_Delay;
extern int Contro_error_flag;//1 means error
extern int Done_init_flag;//1 means done
extern int Position,PitchLocaIdeal;
extern float ChassisSpeedFilter_OutputBuff[12];
extern int ChassisSpeedFilter_InputBuff[12],EcodeSpeedIdeal[4];
extern int RammerTotalMove,RammerLocaMove,RammerModeFlag,RammerLastLoca,RammerCurMove,RammerStuckFlag,RammerBackFlag;
extern int RammerMode,RammerStuckCount;
extern int RammerMode,RammerStuckCount,TurnCode,TurnLastCode,TurnCodeMiss,TurnRoundCountQ,TurnRoundCountE;
extern  int FuckFlag,FuckCount,LastNum5,FuckTime,NUM5Miss;
extern int Angle1_Diff_Value,Angle1_Integra_Value,AnExpCurrnt;
extern int AnExpect,LineFollowSpeed;
extern int16_t Rescue_Current[2],Rescue_Anglecode[2],Rescue_Speedcode[2],Rescue_Angleflag,Rescue_ExpAngle[2],Rescue_DiffValue[2][2],Resucue_IntegralValue[2][2];
extern int16_t Tongs_Current,Tongs_Anglecode,Tongs_Speedcode,Tongs_Exp_Angle,Tongs_DiffValue[2],Tongs_IntegralValue[2];
extern int16_t Extend_Current,Extend_Anglecode,Extend_Speedcode,Extend_Exp_Angle,Extend_mark,Extend_DiffValue[2],Extend_IntegralValue[2];
extern int16_t Catapult_delay;


void ReadEncode(void);
void Gyro_Read(void);
void Control(void);
int Underpan_PID (int16_t ecode,int16_t target,char number);
void Safe_detection(void);
int Yaw_Current_PID(int16_t R_value,int16_t C_value);

#endif
