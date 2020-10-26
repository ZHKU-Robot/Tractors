//version : 2019.10.16  --01

#include "ada_mpu.h"

//��������
float 	pitch,roll,yaw; 						//ŷ����
short 	aacx,aacy,aacz;							//���ٶȴ�����ԭʼ����
short 	gyrox,gyroy,gyroz;					//������ԭʼ����

//mpu��ʼ�����������õײ㺯��
//����ֵ��0����ʼ���ɹ���  ��������ʼ��ʧ�ܣ�
unsigned char mpu6050_init(void)
{
		unsigned char sta = 0;
		sta =  MPU_Init();
		sta |= (mpu_dmp_init());
		return sta;
}

//��ȡmpu_dmp���ݺ�����pitch��roll��yaw��
//������pitch�������ǣ�  roll������ǣ�  yaw������ǣ�
//����ֵ��0����ȡ�ɹ���  ��������ȡʧ�ܣ�
unsigned char mpu_dmp_getdata(float *pitch, float *roll, float *yaw)
{
		return mpu_dmp_get_data(pitch, roll, yaw);
}

//��ȡ���ٶ����ݺ���
//������ax,ay,az��xyz��ļ��ٶ����ݣ�
//����ֵ��0����ȡ�ɹ���  ��������ȡʧ�ܣ�
unsigned char mpu_get_accelerometer(short *ax, short *ay, short *az)
{
		return MPU_Get_Accelerometer(ax, ay, az);
}

//��ȡ���������ݺ���
//������gx,gy,gz��xyz������������ݣ�
//����ֵ��0����ȡ�ɹ���  ��������ȡʧ�ܣ�
unsigned char mpu_get_gyroscope(short *gx, short *gy, short *gz)
{
		return MPU_Get_Gyroscope(gx, gy, gz);
}


/* 
 *	@description	:	��mpu_dmp����ת��Ϊ�ַ�������
 *	@param - buf  : �ַ������ݴ洢��
 *	@return				: �����Ƿ�ת���ɹ���0���ɹ��� 1��ʧ�ܣ�
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

