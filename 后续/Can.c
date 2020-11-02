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
** 函数名称: CAN_NVIC_Config
** 功能描述: 配置CAN的NVIC, 第1优先级组，0，0优先级
** 输　入:   无
** 输　出:   无
***************************************************************/

static void CAN_NVIC_Config(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure;

	 	/*中断设置*/
		NVIC_InitStructure.NVIC_IRQChannel = CAN_RX_IRQ;	   //CAN RX0中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			   //子优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void CAN2_NVIC_Config(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure;

	 	/*中断设置*/
		NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;	   //CAN RX1中断 ...................
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			   //子优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/***************************************************************
** 函数名称: CAN_Mode_Config
** 功能描述: CAN初始化结构体配置
** 输　入:   无
** 输　出:   无
***************************************************************/

static void CAN_Mode_Config(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	/***************CAN通信参数设置*******************/
	/* Enable CAN clock */
//  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);

	/*CAN寄存器初始化*/
//	CAN_DeInit(CANx);
//	CAN_StructInit(&CAN_InitStructure);

	/*CAN单元初始化*/
	CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  关闭时间触发通信模式使能
	CAN_InitStructure.CAN_ABOM=ENABLE;			   //MCR-ABOM  自动离线管理 
	CAN_InitStructure.CAN_AWUM=DISABLE;			   //MCR-AWUM  使用自动唤醒模式
	CAN_InitStructure.CAN_NART=DISABLE;			   //MCR-NART  禁止报文自动重传	  DISABLE-自动重传
	CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  接收FIFO 锁定模式  DISABLE-溢出时新报文会覆盖原有报文  
	CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  发送FIFO优先级 DISABLE-优先级取决于报文标示符 
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //正常工作模式  CAN_Mode_Normal     CAN_Mode_LoopBack
	CAN_InitStructure.CAN_SJW=CAN_SJW_2tq;		   //BTR-SJW 重新同步跳跃宽度 2个时间单元
	 
	/* ss=1 bs1=4 bs2=2 位时间宽度为(1+4+2) 波特率即为时钟周期tq*(1+4+2)  */
	CAN_InitStructure.CAN_BS1=CAN_BS1_4tq;		   //BTR-TS1 时间段1 占用了4个时间单元
	CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;		   //BTR-TS1 时间段2 占用了2个时间单元	
	
	/* CAN Baudrate = 1 MBps (1MBps已为stm32的CAN最高速率) (CAN 时钟频率为 APB 1 = 42 MHz) */
	CAN_InitStructure.CAN_Prescaler =6;		   ////BTR-BRP 波特率分频器  定义了时间单元的时间长度 42/(1+4+2)/6=1 Mbps
	CAN_Init(CANx, &CAN_InitStructure);
}

static void CAN2_Mode_Config(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	/***************CAN通信参数设置*******************/
	/* Enable CAN clock */

	/*CAN单元初始化*/
	CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  关闭时间触发通信模式使能
	CAN_InitStructure.CAN_ABOM=ENABLE;			   //MCR-ABOM  自动离线管理 
	CAN_InitStructure.CAN_AWUM=DISABLE;			   //MCR-AWUM  使用自动唤醒模式
	CAN_InitStructure.CAN_NART=DISABLE;			   //MCR-NART  禁止报文自动重传	  DISABLE-自动重传
	CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  接收FIFO 锁定模式  DISABLE-溢出时新报文会覆盖原有报文  
	CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  发送FIFO优先级 DISABLE-优先级取决于报文标示符 
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal ;  //正常工作模式  CAN_Mode_Normal     CAN_Mode_LoopBack
	CAN_InitStructure.CAN_SJW=CAN_SJW_2tq;		   //BTR-SJW 重新同步跳跃宽度 2个时间单元
	 
	/* ss=1 bs1=4 bs2=2 位时间宽度为(1+4+2) 波特率即为时钟周期tq*(1+4+2)  */
	CAN_InitStructure.CAN_BS1=CAN_BS1_4tq;		   //BTR-TS1 时间段1 占用了4个时间单元
	CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;		   //BTR-TS1 时间段2 占用了2个时间单元	
	
	/* CAN Baudrate = 1 MBps (1MBps已为stm32的CAN最高速率) (CAN 时钟频率为 APB 1 = 42 MHz) */
	CAN_InitStructure.CAN_Prescaler =6;		   ////BTR-BRP 波特率分频器  定义了时间单元的时间长度 42/(1+4+2)/6=1 Mbps
	CAN_Init(CAN2, &CAN_InitStructure);
}


/***************************************************************
** 函数名称: CAN_Filter_Config
** 功能描述: CAN过滤器配置
** 输　入:   无
** 输　出:   无
***************************************************************/

static void CAN_Filter_Config(void)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/*CAN筛选器初始化*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						//筛选器组14
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//工作在掩码模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//筛选器位宽为单个32位。
	
	//不筛选，所有ID的报文都接收进来
	CAN_FilterInitStructure.CAN_FilterIdHigh= 0x0000;		//要筛选的ID高位 
	CAN_FilterInitStructure.CAN_FilterIdLow= 0x0000; //要筛选的ID低位 
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;			//筛选器高16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x0000;			//筛选器低16位每位必须匹配
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;			//筛选器被关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//使能筛选器
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	/*CAN通信中断使能*/
	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
//	CAN_ITConfig(CANx, CAN_IT_FMP1, ENABLE);//............
}


static void CAN2_Filter_Config(void)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/*CAN筛选器初始化*/
	CAN_FilterInitStructure.CAN_FilterNumber=14;						//筛选器组14
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//工作在掩码模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//筛选器位宽为单个32位。
	
	//不筛选，所有ID的报文都接收进来
	CAN_FilterInitStructure.CAN_FilterIdHigh= 0x0000;		//要筛选的ID高位 
	CAN_FilterInitStructure.CAN_FilterIdLow= 0x0000; //要筛选的ID低位 
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;			//筛选器高16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0x0000;			//筛选器低16位每位必须匹配
	
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment= CAN_Filter_FIFO0 ;			//筛选器被关联到FIFO1....................
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//使能筛选器
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	/*CAN通信中断使能*/
//	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);//.....................
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
}



/******************************************************************************* 
* 函数名        : CAN_Config 
* 描述          : 完整配置CAN的功能
* 参数          : 无
* 返回值        : 无 
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
* 函数名        : CAN_Send_Msg 
* 描述          : 配置电流值和ID，并将该报文发送出去
* 参数          : 发送报文结构体TxMessage，4个具体电流值cmx_iq，标准ID
* 返回值        : 无 
*******************************************************************************/

void CAN_Send_Msg(CanTxMsg* TxMessage, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq, uint32_t id)
{
  
  TxMessage->StdId = id;						 //标准帧ID	
  TxMessage->IDE=CAN_ID_STD;				 //标准模式
  TxMessage->RTR=CAN_RTR_DATA;			 //发送的是数据帧
  TxMessage->DLC=8;							     //数据长度为8字节
	
	TxMessage->Data[0] = (uint8_t)(cm1_iq >> 8);
	TxMessage->Data[1] = (uint8_t)cm1_iq;
	TxMessage->Data[2] = (uint8_t)(cm2_iq >> 8);
	TxMessage->Data[3] = (uint8_t)cm2_iq;
	TxMessage->Data[4] = (uint8_t)(cm3_iq >> 8);
	TxMessage->Data[5] = (uint8_t)cm3_iq;
	TxMessage->Data[6] = (uint8_t)(cm4_iq >> 8);
	TxMessage->Data[7] = (uint8_t)cm4_iq;
  
	CAN_Transmit(CANx, TxMessage);  //发送can报文
	
}


void CAN2_Send_Msg(CanTxMsg* TxMessage2, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq, uint32_t id)
{
  
  TxMessage2->StdId = id;						 //标准帧ID	
  TxMessage2->IDE=CAN_ID_STD;				 //标准模式
  TxMessage2->RTR=CAN_RTR_DATA;			 //发送的是数据帧
  TxMessage2->DLC=8;							     //数据长度为8字节
	
	TxMessage2->Data[0] = (uint8_t)(cm1_iq >> 8);
	TxMessage2->Data[1] = (uint8_t)cm1_iq;
	TxMessage2->Data[2] = (uint8_t)(cm2_iq >> 8);
	TxMessage2->Data[3] = (uint8_t)cm2_iq;
	TxMessage2->Data[4] = (uint8_t)(cm3_iq >> 8);
	TxMessage2->Data[5] = (uint8_t)cm3_iq;
	TxMessage2->Data[6] = (uint8_t)(cm4_iq >> 8);
	TxMessage2->Data[7] = (uint8_t)cm4_iq;
  
	CAN_Transmit(CAN2, TxMessage2);  //发送can报文
	
}
/******************************************************************************* 
* 函数名        : Message_buffer 
* 描述          : 将接收到的can报文按ID分组按顺序保存进二维数组
* 参数          : 接收报文结构体RxMessage，目标二位数组buf[][6] 
* 返回值        : 无 
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
