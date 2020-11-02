/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "Usart.h"
#include "Can.h"
#include "Control.h"
#include "Light.h"

#include "delay.h"
#include "uart3.h"
#include "JY901.h"
#include "math.h"
//#include "myiic.h"
//#include "mpu6050.h"
//#include "inv_mpu.h"
//#include "inv_mpu_dmp_motion_driver.h"

extern uint8_t recv_end_flag;
extern CanRxMsg RxMessage,RxMessage2;			
uint8_t can1_rx_data_buf[12][6],can2_rx_data_buf[12][6];

extern int NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8;
extern uint8_t Gyro_buff[30];
uint16_t Gyro_buff_counter = 0;
u8 Temp;
extern float pitch,roll,yaw;
extern int print_cnt,init_cnt;
extern struct SAngle 	stcAngle;
extern int pitch_angle,roll_angle,Yaw_angle;

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
void TIM6_DAC_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) == 1)
	{
		
		ReadEncode();
		Control();
		print_cnt++;
		init_cnt++;
		
//		pitch_angle=(int)((float)(stcAngle.Angle[0]/32768*180));
//		roll_angle =(int)((float)(stcAngle.Angle[1]/32768*180));
//		Yaw_angle  =(int)((float)(stcAngle.Angle[2]/32768*180));
//		
		CAN_Send_Msg(&TxMessage, NUM1, NUM2, NUM3, NUM4, 0x200);		//NUM1、NUM2为左右后轮电机
		CAN_Send_Msg(&TxMessage, NUM5, NUM6, NUM7,NUM8, 0x1FF);			//NUM7作为转向电机
		
//    CAN2_Send_Msg(&TxMessage2,0, 0, NUM7, NUM8, 0x1FF);
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}

void USART1_IRQHandler(void)
{

	if(USART_GetITStatus(USART1, USART_IT_IDLE))  //触发空闲中断
	{  
		USART1->SR;	//清USART_IT_IDLE标志，先读SR，然后读DR才能清除 
		USART1->DR; 
		DMA_Cmd(DMA2_Stream2,DISABLE);	//关闭DMA，关闭DMA才能清除DMA_FLAG_TCIF2标志
		DMA_ClearFlag(DMA2_Stream2,DMA_FLAG_TCIF2);	//清除DMA_FLAG_TCIF2传输完成标志
		DMA_Cmd(DMA2_Stream2,ENABLE);	//重新打开DMA
		recv_end_flag = 1;
	}   

}

void USART2_IRQHandler(void)
{
//	if(USART_GetITStatus(USART2, USART_IT_RXNE))
//	{
//		Gyro_buff[Gyro_buff_counter] = USART_ReceiveData(USART2);
//    Gyro_buff_counter++;
//		
//		if (Gyro_buff_counter == 21)
//		{
//			Gyro_buff_counter = 0;
//			if((Gyro_buff[2] == 0x52) && (Gyro_buff[13] == 0x53))Gyro_Read();
//		}
//	}
	
		if(USART_GetITStatus(USART2, USART_IT_RXNE))
	{
//		printf("speed=%d\r\n",ecode1);	
		USART2->SR;	//清USART_IT_IDLE标志，先读SR，然后读DR才能清除 
		USART2->DR; 
	}
}

void USART6_IRQHandler(void)
{

}

////JY901中断
//void UART7_IRQHandler(void)
//{
//  if(USART_GetITStatus(UART7, USART_IT_TXE) != RESET)
//  {   
//    USART_SendData(UART7, TxBuffer[TxCounter++]); 
//    USART_ClearITPendingBit(UART7, USART_IT_TXE);
//    if(TxCounter == count) USART_ITConfig(UART7, USART_IT_TXE, DISABLE);
//  }
//	else if(USART_GetITStatus(UART7, USART_IT_RXNE) != RESET)
//  {
//		CopeSerial2Data((unsigned char)UART7->DR);//处理数据
//		USART_ClearITPendingBit(UART7, USART_IT_RXNE);
//  }
//	
//	USART_ClearITPendingBit(UART7,USART_IT_ORE);
//}

//void USART3_IRQHandler(void)
//{
//  if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
//  {   
//    USART_SendData(USART3, TxBuffer[TxCounter++]); 
//    USART_ClearITPendingBit(USART2, USART_IT_TXE);
//    if(TxCounter == count) USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
//  }
//	else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//  {
//		CopeSerial2Data((unsigned char)USART2->DR);//处理数据
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//  }
//	
//	USART_ClearITPendingBit(USART2,USART_IT_ORE);
//}

void CAN1_RX0_IRQHandler(void)
{
	 if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);	//从邮箱中读出报文
	Message_buffer(&RxMessage, can1_rx_data_buf);	//将接收到的can报文按ID分组按顺序保存进二维数组
	CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);  
		}
}
//void CAN1_RX1_IRQHandler(void)
//{
//	CAN_Receive(CAN1, CAN_FIFO1, &RxMessage);	//从邮箱中读出报文
//	Message_buffer(&RxMessage, can1_rx_data_buf);	//将接收到的can报文按ID分组按顺序保存进二维数组
//	
//}

void CAN2_RX0_IRQHandler(void)
{
	 if (CAN_GetITStatus(CAN2, CAN_IT_FMP0) != RESET)
  {
	CAN_Receive(CAN2, CAN_FIFO0, &RxMessage2);	//从邮箱中读出报文
	Message_buffer2(&RxMessage2, can2_rx_data_buf);	//将接收到的can报文按ID分组按顺序保存进二维数组
	CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);  
  }
}

//void CAN2_RX0_IRQHandler(void)
//{
//	CAN_Receive(CAN2, CAN_FIFO1, &RxMessage2);	//从邮箱中读出报文
//	Message_buffer2(&RxMessage2, can2_rx_data_buf);	//将接收到的can报文按ID分组按顺序保存进二维数组
//	
//}





/*****************************END OF FILE*****************************************/
