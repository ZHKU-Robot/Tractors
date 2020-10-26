//version : 2019.10.16  --01

#include "ada_mpu.h"

//变量定义
float 	pitch,roll,yaw; 						//欧拉角
short 	aacx,aacy,aacz;							//加速度传感器原始数据
short 	gyrox,gyroy,gyroz;					//陀螺仪原始数据

//mpu初始化函数，调用底层函数
//返回值：0（初始化成功）  其他（初始化失败）
unsigned char mpu6050_init(void)
{
		unsigned char sta = 0;
		sta =  MPU_Init();
		sta |= (mpu_dmp_init());
		return sta;
}

//获取mpu_dmp数据函数（pitch，roll，yaw）
//参数：pitch（俯仰角）  roll（横滚角）  yaw（航向角）
//返回值：0（读取成功）  其他（读取失败）
unsigned char mpu_dmp_getdata(float *pitch, float *roll, float *yaw)
{
		return mpu_dmp_get_data(pitch, roll, yaw);
}

//获取加速度数据函数
//参数：ax,ay,az（xyz轴的加速度数据）
//返回值：0（读取成功）  其他（读取失败）
unsigned char mpu_get_accelerometer(short *ax, short *ay, short *az)
{
		return MPU_Get_Accelerometer(ax, ay, az);
}

//获取陀螺仪数据函数
//参数：gx,gy,gz（xyz轴的陀螺仪数据）
//返回值：0（读取成功）  其他（读取失败）
unsigned char mpu_get_gyroscope(short *gx, short *gy, short *gz)
{
		return MPU_Get_Gyroscope(gx, gy, gz);
}


/* 
 *	@description	:	将mpu_dmp数据转化为字符串数据
 *	@param - buf  : 字符串数据存储区
 *	@return				: 数据是否转换成功（0：成功， 1：失败）
 */
unsigned char mpu_dmp_sbuf(char *buf)
{
		if(buf[0] == 0)
		{
				memset((char*)buf, 0, sizeof(buf));
				sprintf(buf, "A#%.1f#%.1f#%.1f#", pitch, roll, yaw);
				//printf(buf);printf("\r\n\r\n");
				return 0;
		}
		else 
				return 1;
}

