#include "Control.h"
#include "math.h"

#include "delay.h"
#include "myiic.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"




int Angle_one,Angle1,Angle2,Angle3,SPEED1,SPEED2,SPEED3,sp1,sp2,sp3,ANgle1,anglemax=4000,anglemin;
static int flagg1=0,flagg2=0;

//MPU6050数据
extern float pitch,roll,yaw; 		//欧拉角
extern short aacx,aacy,aacz;		//加速度传感器原始数据
extern short gyrox,gyroy,gyroz;	//陀螺仪原始数据
extern short temp;		
extern float pitch_First,roll_First,yaw_First; 
extern int print_cnt;

extern uint16_t Expect_Turncode;
extern char dmp_init_flag;
extern int TurnCode_Angle,TurnCode_Speed,TurnLastCode,TurnRoundCountQ,TurnCodeMiss,TurnRoundCountE;
extern int ExpectYawAngle, YawMaxAngle, YawMinAngle;		//期望转向角度及角度限制

void ReadEncode(void)
{
	TurnCode_Angle = (can1_rx_data_buf[4][0]<<8) | can1_rx_data_buf[4][1];
	TurnCode_Speed = (can1_rx_data_buf[4][2]<<8) | can1_rx_data_buf[4][3];
	
	ecode1 = (can1_rx_data_buf[0][2]<<8) | can1_rx_data_buf[0][3] ;
	ecode2 = (can1_rx_data_buf[1][2]<<8) | can1_rx_data_buf[1][3] ;
	if(ecode1 == -3) ecode1 = 0 ;
	if(ecode2 == -3) ecode2 = 0 ;
	
	if(dmp_init_flag)
	{
			ExpectYawAngle = 3000 + (int)((8191.00/720)*yaw);
			if(ExpectYawAngle > YawMaxAngle) ExpectYawAngle = YawMaxAngle;
			if(ExpectYawAngle < YawMinAngle) ExpectYawAngle = YawMinAngle;
	}
	
}



	 

int Underpan_PID (int16_t R_value,int16_t C_value,char number)
{                                                                                                                                                                                                                                                                                                                                                                                            
	float P = 7, I = 0, D = 0;
	float C_new=0,C_old=0,C_miss=0;
	int CurrentValue=0;
	C_old = RC_DiffValue[number-1];
	C_new = R_value - C_value;
	C_miss = C_new - C_old;
	RC_DiffValue[number-1] = R_value - C_value;
	RC_IntegralValue[number-1] += RC_DiffValue[number-1];
	CurrentValue = (int)((P*RC_DiffValue[number-1]) + (I*RC_IntegralValue[number-1]) + (D*C_miss));
	if(CurrentValue > 4000) CurrentValue = 4000;
	if(CurrentValue < -4000) CurrentValue = -4000;
	return CurrentValue;
}

int Yaw_Current_PID(int16_t R_value,int16_t C_value)
{

	float P = 0, I = 0, D = 0;
	int C_new=0,C_old=0;
	float C_miss=0;
	int CurrentValue=0;	
	C_old = Yaw_DiffValue[1];
	C_new = R_value - C_value;
	C_miss = C_new - C_old;
	Yaw_DiffValue[1] = R_value - C_value;
	Yaw_IntegralValue[1] += Yaw_DiffValue[1];
	if(Yaw_IntegralValue[1] > 6000)Yaw_IntegralValue[1] = 6000;
	else if (Yaw_IntegralValue[1] <-6000)Yaw_IntegralValue[1] = -6000;
	CurrentValue = (int)((P*Yaw_DiffValue[1]) + (I*Yaw_IntegralValue[1]) + (D*C_miss));
	//CurrentValue += 600; 
	if(CurrentValue > 30000) CurrentValue = 30000;
	if(CurrentValue < -30000) CurrentValue = -30000;
	return CurrentValue;
}

int Yaw_Speed_PID(int16_t R_value,int16_t C_value)
{

	float P = 5, I = 6, D = 0;
	int C_new=0,C_old=0;
	float C_miss=0;
	int CurrentValue=0;	
	C_old = Yaw_DiffValue[1];
	C_new = R_value - C_value;
	C_miss = C_new - C_old;
	Yaw_DiffValue[1] = R_value - C_value;
	Yaw_IntegralValue[1] += Yaw_DiffValue[1];
	if(Yaw_IntegralValue[1] > 6000)Yaw_IntegralValue[1] = 6000;
	else if (Yaw_IntegralValue[1] <-6000)Yaw_IntegralValue[1] = -6000;
	CurrentValue = (int)((P*Yaw_DiffValue[1]) + (I*Yaw_IntegralValue[1]) + (D*C_miss));
	//CurrentValue += 600; 
	if(CurrentValue > 30000) CurrentValue = 30000;
	if(CurrentValue < -30000) CurrentValue = -30000;
	return CurrentValue;
}


int Yaw_Location_PID(int16_t R_value,int16_t C_value)
{
	//float P = 4, I = 0, D = 0;
	float P = 0.5, I = 0, D = 0;
	float C_new=0,C_old=0,C_miss=0;
	int CurrentValue=0;
	C_old = Yaw_DiffValue[2];
	C_new = R_value - C_value;
	C_miss = C_new - C_old;
	Yaw_DiffValue[2] = R_value - C_value;
	if(Yaw_IntegralValue[2] > 50)Yaw_IntegralValue[2] = 50;
	else if (Yaw_IntegralValue[2] < -50)Yaw_IntegralValue[2] = -50;
	CurrentValue = (int)((P*Yaw_DiffValue[2]) + (I*Yaw_IntegralValue[2]) + (D*C_miss));
	if(CurrentValue > 200) CurrentValue = 200;
	if(CurrentValue < -200) CurrentValue = -200;
	return CurrentValue;
}






void Control(void)
{
/***********************************************/	

	
		if(dmp_init_flag)
		{
					
//						expect_revolutions[0] = ((1024 - ch1)-(1024 - ch0)-(1024-ch2))*10 ;     // 1  
//						expect_revolutions[1] = -((1024 - ch1) +(1024 - ch0)+(1024-ch2))*10 ;   // 2  

						expect_revolutions[0] =   (pitch*20 + yaw *10)*3;
						expect_revolutions[0] = - (pitch*20 - yaw *10)*3;
			
						
			
						NUM1 = Underpan_PID (expect_revolutions[0],ecode1,1);
						NUM2 = Underpan_PID (expect_revolutions[1],ecode2,2);
						NUM3 = Underpan_PID (expect_revolutions[2],ecode3,3);
						NUM4 = Underpan_PID (expect_revolutions[3],ecode4,4);	
		}
						NUM5 = Yaw_Speed_PID(Yaw_Location_PID((ExpectYawAngle),TurnCode_Angle), TurnCode_Speed);
}



















