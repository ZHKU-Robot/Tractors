//version : 2019.10.16  --01

#ifndef  __ADA_MPU_H
#define  __ADA_MPU_H

//头文件
#include "main.h"

//宏定义
#define  				ADA_MPU						1					//MPU接口使能标志

//变量声明
extern	float 	pitch,roll,yaw; 						//欧拉角
extern	short 	aacx,aacy,aacz;							//加速度传感器原始数据
extern	short 	gyrox,gyroy,gyroz;					//陀螺仪原始数据

//函数声明
unsigned char mpu6050_init(void);																				//mpu初始化函数，调用底层函数
unsigned char mpu_dmp_getdata(float *pitch, float *roll, float *yaw);		//获取mpu_dmp数据函数（pitch，roll，yaw）
unsigned char mpu_get_accelerometer(short *ax, short *ay, short *az);		//获取加速度数据函数
unsigned char mpu_get_gyroscope(short *gx, short *gy, short *gz);				//获取陀螺仪数据函数

unsigned char mpu_dmp_sbuf(char *buf);																	//mpu_dmp数据转换为字符串函数

#endif

