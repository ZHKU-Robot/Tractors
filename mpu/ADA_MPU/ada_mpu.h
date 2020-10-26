//version : 2019.10.16  --01

#ifndef  __ADA_MPU_H
#define  __ADA_MPU_H

//ͷ�ļ�
#include "main.h"

//�궨��
#define  				ADA_MPU						1					//MPU�ӿ�ʹ�ܱ�־

//��������
extern	float 	pitch,roll,yaw; 						//ŷ����
extern	short 	aacx,aacy,aacz;							//���ٶȴ�����ԭʼ����
extern	short 	gyrox,gyroy,gyroz;					//������ԭʼ����

//��������
unsigned char mpu6050_init(void);																				//mpu��ʼ�����������õײ㺯��
unsigned char mpu_dmp_getdata(float *pitch, float *roll, float *yaw);		//��ȡmpu_dmp���ݺ�����pitch��roll��yaw��
unsigned char mpu_get_accelerometer(short *ax, short *ay, short *az);		//��ȡ���ٶ����ݺ���
unsigned char mpu_get_gyroscope(short *gx, short *gy, short *gz);				//��ȡ���������ݺ���

unsigned char mpu_dmp_sbuf(char *buf);																	//mpu_dmp����ת��Ϊ�ַ�������

#endif

