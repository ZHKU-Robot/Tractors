#include "Can.h"
 int TEST;
static void CAN_GPIO_Config(void)
{
	/*	Init	*/
	GPIO_InitTypeDef GPIO_InitStructure;
 
  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(CAN_TX_GPIO_CLK|CAN_RX_GPIO_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN1, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN1, DISABLE);

	  /* Connect CAN pins to AF9 */


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
	
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);

}

static void CAN2_GPIO_Config(void)
{
	/*	Init	*/
	GPIO_InitTypeDef GPIO_InitStructure;
 
  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

  RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN2, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN2, DISABLE);


	  /* Configure CAN TX pins */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);

}


/***************************************************************
** ��������: CAN_NVIC_Config
** ��������: ����CAN��NVIC, ��1���ȼ��飬0��0���ȼ�
** �䡡��:   ��
** �䡡��:   ��
***************************************************************/

static void CAN_NVIC_Config(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure;

	 	/*�ж�����*/
		NVIC_InitStructure.NVIC_IRQChannel = CAN_RX_IRQ;	   //CAN RX0�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			   //�����ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void CAN2_NVIC_Config(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure;

	 	/*�ж�����*/
		NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;	   //CAN RX1�ж� ...................
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			   //�����ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/***************************************************************
** ��������: CAN_Mode_Config
** ��������: CAN��ʼ���ṹ������
** �䡡��:   ��
** �䡡��:   ��
***************************************************************/

static void CAN_Mode_Config(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	/***************CANͨ�Ų�������*******************/
	/* Enable CAN clock */
//  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);

	/*CAN�Ĵ�����ʼ��*/
//	CAN_DeInit(CANx);
//	CAN_StructInit(&CAN_InitStructure);

	/*CAN��Ԫ��ʼ��*/
	CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  �ر�ʱ�䴥��ͨ��ģʽʹ��
	CAN_InitStructure.CAN_ABOM=ENABLE;			   //MCR-ABOM  �Զ����߹��� 
	CAN_InitStructure.CAN_AWUM=DISABLE;			   //MCR-AWUM  ʹ���Զ�����ģʽ
	CAN_InitStructure.CAN_NART=DISABLE;			   //MCR-NART  ��ֹ�����Զ��ش�	  DISABLE-�Զ��ش�
	CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  ����FIFO ����ģʽ  DISABLE-���ʱ�±��ĻḲ��ԭ�б���  
	CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  ����FIFO���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ�� 
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //��������ģʽ  CAN_Mode_Normal     CAN_Mode_LoopBack
	CAN_InitStructure.CAN_SJW=CAN_SJW_2tq;		   //BTR-SJW ����ͬ����Ծ��� 2��ʱ�䵥Ԫ
	 
	/* ss=1 bs1=4 bs2=2 λʱ����Ϊ(1+4+2) �����ʼ�Ϊʱ������tq*(1+4+2)  */
	CAN_InitStructure.CAN_BS1=CAN_BS1_4tq;		   //BTR-TS1 ʱ���1 ռ����4��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;		   //BTR-TS1 ʱ���2 ռ����2��ʱ�䵥Ԫ	
	
	/* CAN Baudrate = 1 MBps (1MBps��Ϊstm32��CAN�������) (CAN ʱ��Ƶ��Ϊ APB 1 = 42 MHz) */
	CAN_InitStructure.CAN_Prescaler =6;		   ////BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� 42/(1+4+2)/6=1 Mbps
	CAN_Init(CANx, &CAN_InitStructure);
}

static void CAN2_Mode_Config(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	/***************CANͨ�Ų�������*******************/
	/* Enable CAN clock */

	/*CAN��Ԫ��ʼ��*/
	CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  �ر�ʱ�䴥��ͨ��ģʽʹ��
	CAN_InitStructure.CAN_ABOM=ENABLE;			   //MCR-ABOM  �Զ����߹��� 
	CAN_InitStructure.CAN_AWUM=DISABLE;			   //MCR-AWUM  ʹ���Զ�����ģʽ
	CAN_InitStructure.CAN_NART=DISABLE;			   //MCR-NART  ��ֹ�����Զ��ش�	  DISABLE-�Զ��ش�
	CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  ����FIFO ����ģʽ  DISABLE-���ʱ�±��ĻḲ��ԭ�б���  
	CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  ����FIFO���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ�� 
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal ;  //��������ģʽ  CAN_Mode_Normal     CAN_Mode_LoopBack
	CAN_InitStructure.CAN_SJW=CAN_SJW_2tq;		   //BTR-SJW ����ͬ����Ծ��� 2��ʱ�䵥Ԫ
	 
	/* ss=1 bs1=4 bs2=2 λʱ����Ϊ(1+4+2) �����ʼ�Ϊʱ������tq*(1+4+2)  */
	CAN_InitStructure.CAN_BS1=CAN_BS1_4tq;		   //BTR-TS1 ʱ���1 ռ����4��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;		   //BTR-TS1 ʱ���2 ռ����2��ʱ�䵥Ԫ	
	
	/* CAN Baudrate = 1 MBps (1MBps��Ϊstm32��CAN�������) (CAN ʱ��Ƶ��Ϊ APB 1 = 42 MHz) */
	CAN_InitStructure.CAN_Prescaler =6;		   ////BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� 42/(1+4+2)/6=1 Mbps
	CAN_Init(CAN2, &CAN_InitStructure);
}


/***************************************************************
** ��������: CAN_Filter_Config
** ��������: CAN����������
** �䡡��:   ��
** �䡡��:   ��
***************************************************************/

static void CAN_Filter_Config(void)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/*CANɸѡ����ʼ��*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						//ɸѡ����14
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//����������ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//ɸѡ��λ��Ϊ����32λ��
	
	//��ɸѡ������ID�ı��Ķ����ս���
	CAN_FilterInitStructure.CAN_FilterIdHigh= 0x0000;		//Ҫɸѡ��ID��λ 
	CAN_FilterInitStructure.CAN_FilterIdLow= 0x0000; //Ҫɸѡ��ID��λ 
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;			//ɸѡ����16λÿλ����ƥ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x0000;			//ɸѡ����16λÿλ����ƥ��
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;			//ɸѡ����������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//ʹ��ɸѡ��
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	/*CANͨ���ж�ʹ��*/
	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
//	CAN_ITConfig(CANx, CAN_IT_FMP1, ENABLE);//............
}


static void CAN2_Filter_Config(void)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/*CANɸѡ����ʼ��*/
	CAN_FilterInitStructure.CAN_FilterNumber=14;						//ɸѡ����14
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//����������ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//ɸѡ��λ��Ϊ����32λ��
	
	//��ɸѡ������ID�ı��Ķ����ս���
	CAN_FilterInitStructure.CAN_FilterIdHigh= 0x0000;		//Ҫɸѡ��ID��λ 
	CAN_FilterInitStructure.CAN_FilterIdLow= 0x0000; //Ҫɸѡ��ID��λ 
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;			//ɸѡ����16λÿλ����ƥ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x0000;			//ɸѡ����16λÿλ����ƥ��
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment= CAN_Filter_FIFO0 ;			//ɸѡ����������FIFO1....................
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//ʹ��ɸѡ��
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	/*CANͨ���ж�ʹ��*/
//	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);//.....................
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}



/******************************************************************************* 
* ������        : CAN_Config 
* ����          : ��������CAN�Ĺ���
* ����          : ��
* ����ֵ        : �� 
*******************************************************************************/

void CAN_Config(void)
{
	
  CAN_GPIO_Config();
  CAN_Mode_Config();
  CAN_Filter_Config();  
	CAN_NVIC_Config();
  CAN_Transmit(CANx, &TxMessage);
	
}

void CAN2_Config(void)
{
	
  CAN2_GPIO_Config();
  CAN2_Mode_Config();
  CAN2_Filter_Config();  
	CAN2_NVIC_Config();
  CAN_Transmit(CAN2, &TxMessage2);
	
}


/******************************************************************************* 
* ������        : CAN_Send_Msg 
* ����          : ���õ���ֵ��ID�������ñ��ķ��ͳ�ȥ
* ����          : ���ͱ��Ľṹ��TxMessage��4���������ֵcmx_iq����׼ID
* ����ֵ        : �� 
*******************************************************************************/

void CAN_Send_Msg(CanTxMsg* TxMessage, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq, uint32_t id)
{
  
  TxMessage->StdId = id;						 //��׼֡ID	
  TxMessage->IDE=CAN_ID_STD;				 //��׼ģʽ
  TxMessage->RTR=CAN_RTR_DATA;			 //���͵�������֡
  TxMessage->DLC=8;							     //���ݳ���Ϊ8�ֽ�
	
	TxMessage->Data[0] = (uint8_t)(cm1_iq >> 8);
	TxMessage->Data[1] = (uint8_t)cm1_iq;
	TxMessage->Data[2] = (uint8_t)(cm2_iq >> 8);
	TxMessage->Data[3] = (uint8_t)cm2_iq;
	TxMessage->Data[4] = (uint8_t)(cm3_iq >> 8);
	TxMessage->Data[5] = (uint8_t)cm3_iq;
	TxMessage->Data[6] = (uint8_t)(cm4_iq >> 8);
	TxMessage->Data[7] = (uint8_t)cm4_iq;
  
	CAN_Transmit(CANx, TxMessage);  //����can����
	
}


void CAN2_Send_Msg(CanTxMsg* TxMessage2, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq, uint32_t id)
{
  
  TxMessage2->StdId = id;						 //��׼֡ID	
  TxMessage2->IDE=CAN_ID_STD;				 //��׼ģʽ
  TxMessage2->RTR=CAN_RTR_DATA;			 //���͵�������֡
  TxMessage2->DLC=8;							     //���ݳ���Ϊ8�ֽ�
	
	TxMessage2->Data[0] = (uint8_t)(cm1_iq >> 8);
	TxMessage2->Data[1] = (uint8_t)cm1_iq;
	TxMessage2->Data[2] = (uint8_t)(cm2_iq >> 8);
	TxMessage2->Data[3] = (uint8_t)cm2_iq;
	TxMessage2->Data[4] = (uint8_t)(cm3_iq >> 8);
	TxMessage2->Data[5] = (uint8_t)cm3_iq;
	TxMessage2->Data[6] = (uint8_t)(cm4_iq >> 8);
	TxMessage2->Data[7] = (uint8_t)cm4_iq;
  
	CAN_Transmit(CAN2, TxMessage2);  //����can����
	
}
/******************************************************************************* 
* ������        : Message_buffer 
* ����          : �����յ���can���İ�ID���鰴˳�򱣴����ά����
* ����          : ���ձ��Ľṹ��RxMessage��Ŀ���λ����buf[][6] 
* ����ֵ        : �� 
*******************************************************************************/

void Message_buffer(CanRxMsg* RxMessage, uint8_t buf[][6])
{
	static uint8_t i;
	
	if(0x201 == RxMessage->StdId)
	{
		for(i = 0; i < 4; i++)
			buf[0][i] = RxMessage->Data[i];
	}
	else if(0x202 == RxMessage->StdId)
	{
		for(i = 0; i < 4; i++)
			buf[1][i] = RxMessage->Data[i];
	}
	else if(0x203 == RxMessage->StdId)
	{
		for(i = 0; i < 4; i++)
			buf[2][i] = RxMessage->Data[i];
	}
	else if(0x204 == RxMessage->StdId)
	{
		for(i = 0; i < 4; i++)
			buf[3][i] = RxMessage->Data[i];
	}
	else if(0x205 == RxMessage->StdId)  //Y
	{
		for(i = 0; i < 6; i++)
			buf[4][i] = RxMessage->Data[i];  
	}
	else if(0x206 == RxMessage->StdId)  //P
	{
		for(i = 0; i < 6; i++)
			buf[5][i] = RxMessage->Data[i];  
	}
	else if(0x207 == RxMessage->StdId)
	{
		for(i = 0; i < 6; i++)
			buf[6][i] = RxMessage->Data[i];
	}
	else if(0x208 == RxMessage->StdId)
	{
		for(i = 0; i < 6; i++)
			buf[7][i] = RxMessage->Data[i];
	}
//	 if(0x401 == RxMessage->StdId)
//	{
//		for(i = 0; i < 8; i++)
//			buf[8][i] = RxMessage->Data[i];
//		
//	}
//	else if(0x402 == RxMessage->StdId)
//	{
//		for(i = 0; i < 8; i++)
//			buf[9][i] = RxMessage->Data[i];
//	}
//		else if(0x405 == RxMessage->StdId)
//	{
//		for(i = 0; i < 5; i++)
//			buf[10][i] = RxMessage->Data[i];
//	}

}

void Message_buffer2(CanRxMsg* RxMessage2, uint8_t buf[][6])
{
	static uint8_t i;
	
	
	 if(0x401 == RxMessage2->StdId)
	{
		for(i = 0; i < 8; i++)
			buf[8][i] = RxMessage2->Data[i];
		
	}
	else if(0x402 == RxMessage2->StdId)
	{
		for(i = 0; i < 8; i++)
			buf[9][i] = RxMessage2->Data[i];
	}
		else if(0x405 == RxMessage2->StdId)
	{
		for(i = 0; i < 5; i++)
			buf[10][i] = RxMessage2->Data[i];
	}else if(0x207 == RxMessage2->StdId)
	{
		for(i = 0; i < 6; i++)
			buf[6][i] = RxMessage2->Data[i];
	}
	else if(0x208 == RxMessage2->StdId)
	{
		for(i = 0; i < 6; i++)
			buf[7][i] = RxMessage2->Data[i];
	}

}
