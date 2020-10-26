//version : 2019.12.30  --01  --ycy

#include "app_mpu.h"

//������ƿ��������ջ����
OS_TCB MPUTaskTCB;																	//������ƿ�	
__align(8) CPU_STK MPU_TASK_STK[MPU_STK_SIZE];			//�����ջ


//��������



//���񴴽�����
void mpu_task_create(void)
{
		OS_ERR err;
		OSTaskCreate((OS_TCB 		* )&MPUTaskTCB,									//������ƿ�
								 (CPU_CHAR	* )"mpu task", 									//��������
                 (OS_TASK_PTR )mpu_task, 										//������
                 (void			* )0,														//���ݸ��������Ĳ���
                 (OS_PRIO	 		)MPU_TASK_PRIO,     					//�������ȼ�
                 (CPU_STK   * )&MPU_TASK_STK[0],						//�����ջ����ַ
                 (CPU_STK_SIZE)MPU_STK_SIZE/10,							//�����ջ�����λ
                 (CPU_STK_SIZE)MPU_STK_SIZE,								//�����ջ��С
                 (OS_MSG_QTY  )0,														//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  	)0,														//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   		* )0,														//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 		* )&err);												//��Ÿú�������ʱ�ķ���ֵ
}


//������
void mpu_task(void *p_arg)
{
		OS_ERR err;
		unsigned char dmp_sta = 1;
		CPU_SR_ALLOC();
		p_arg = p_arg;
	
		while(1)
		{
				OS_CRITICAL_ENTER();//�����ٽ���
				mpu_get_accelerometer(&aacx, &aacy, &aacz);
				mpu_get_gyroscope(&gyrox, &gyroy, &gyroz);
				OS_CRITICAL_EXIT();	//�˳��ٽ���	
				OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);
				OS_CRITICAL_ENTER();//�����ٽ���
				dmp_sta = mpu_dmp_getdata(&pitch, &roll, &yaw);
				if(dmp_sta == 0)
				{
						mpu_dmp_sbuf(nrf24l01_sbuf);
						//printf("pitch : %.1f, roll : %.1f, yaw : %.1f \r\n\r\n", pitch, roll, yaw);
				}
				OS_CRITICAL_EXIT();	//�˳��ٽ���	
				
				OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_PERIODIC,&err);
		}
}


