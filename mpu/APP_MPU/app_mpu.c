//version : 2019.12.30  --01  --ycy

#include "app_mpu.h"

//任务控制块与任务堆栈定义
OS_TCB MPUTaskTCB;																	//任务控制块	
__align(8) CPU_STK MPU_TASK_STK[MPU_STK_SIZE];			//任务堆栈


//变量定义



//任务创建函数
void mpu_task_create(void)
{
		OS_ERR err;
		OSTaskCreate((OS_TCB 		* )&MPUTaskTCB,									//任务控制块
								 (CPU_CHAR	* )"mpu task", 									//任务名字
                 (OS_TASK_PTR )mpu_task, 										//任务函数
                 (void			* )0,														//传递给任务函数的参数
                 (OS_PRIO	 		)MPU_TASK_PRIO,     					//任务优先级
                 (CPU_STK   * )&MPU_TASK_STK[0],						//任务堆栈基地址
                 (CPU_STK_SIZE)MPU_STK_SIZE/10,							//任务堆栈深度限位
                 (CPU_STK_SIZE)MPU_STK_SIZE,								//任务堆栈大小
                 (OS_MSG_QTY  )0,														//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  	)0,														//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   		* )0,														//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 		* )&err);												//存放该函数错误时的返回值
}


//任务函数
void mpu_task(void *p_arg)
{
		OS_ERR err;
		unsigned char dmp_sta = 1;
		CPU_SR_ALLOC();
		p_arg = p_arg;
	
		while(1)
		{
				OS_CRITICAL_ENTER();//进入临界区
				mpu_get_accelerometer(&aacx, &aacy, &aacz);
				mpu_get_gyroscope(&gyrox, &gyroy, &gyroz);
				OS_CRITICAL_EXIT();	//退出临界区	
				OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);
				OS_CRITICAL_ENTER();//进入临界区
				dmp_sta = mpu_dmp_getdata(&pitch, &roll, &yaw);
				if(dmp_sta == 0)
				{
						mpu_dmp_sbuf(nrf24l01_sbuf);
						//printf("pitch : %.1f, roll : %.1f, yaw : %.1f \r\n\r\n", pitch, roll, yaw);
				}
				OS_CRITICAL_EXIT();	//退出临界区	
				
				OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_PERIODIC,&err);
		}
}


