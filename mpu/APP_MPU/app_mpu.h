//version : 2019.12.30  --01  --ycy

#ifndef __APP_MPU_H
#define __APP_MPU_H

#include "main.h"

//�궨��
#define		APP_MPU			1				//MPU�����־


//������ض��弰����
#define MPU_TASK_PRIO		4																	//�������ȼ�
#define MPU_STK_SIZE 		512																//�����ջ��С	
extern OS_TCB MPUTaskTCB;																	//������ƿ�	
extern CPU_STK MPU_TASK_STK[MPU_STK_SIZE];								//�����ջ
void mpu_task_create(void);																//���񴴽�����
void mpu_task(void *p_arg);																//������


#endif
