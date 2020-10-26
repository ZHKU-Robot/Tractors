//version : 2019.12.30  --01  --ycy

#ifndef __APP_MPU_H
#define __APP_MPU_H

#include "main.h"

//宏定义
#define		APP_MPU			1				//MPU任务标志


//任务相关定义及声明
#define MPU_TASK_PRIO		4																	//任务优先级
#define MPU_STK_SIZE 		512																//任务堆栈大小	
extern OS_TCB MPUTaskTCB;																	//任务控制块	
extern CPU_STK MPU_TASK_STK[MPU_STK_SIZE];								//任务堆栈
void mpu_task_create(void);																//任务创建函数
void mpu_task(void *p_arg);																//任务函数


#endif
