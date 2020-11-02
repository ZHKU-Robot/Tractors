#ifndef __CAN_H
#define __CAN_H

#include "stm32f4xx.h"
#include <stdio.h>


extern CanTxMsg TxMessage,TxMessage2;			     //发送缓冲区
extern CanRxMsg RxMessage,RxMessage2;				 //接收缓冲区
extern uint8_t can1_rx_data_buf[12][6];	//存放按ID分组的can报文
extern uint8_t can2_rx_data_buf[12][6];


#define CANx                       	CAN1
#define CAN_CLK                     RCC_APB1Periph_CAN1
#define CAN_RX_IRQ									CAN1_RX0_IRQn
#define CAN_RX_IRQHandler					  CAN1_RX0_IRQHandler

#define CAN_RX_PIN                  GPIO_Pin_0
#define CAN_TX_PIN                  GPIO_Pin_1
#define CAN_TX_GPIO_PORT            GPIOD
#define CAN_RX_GPIO_PORT            GPIOD
#define CAN_TX_GPIO_CLK             RCC_AHB1Periph_GPIOD
#define CAN_RX_GPIO_CLK             RCC_AHB1Periph_GPIOD
#define CAN_AF_PORT                 GPIO_AF_CAN1
#define CAN_RX_SOURCE               GPIO_PinSource0
#define CAN_TX_SOURCE               GPIO_PinSource1


static void CAN_GPIO_Config(void);
static void CAN2_GPIO_Config(void);

static void CAN_NVIC_Config(void);
static void CAN2_NVIC_Config(void);

static void CAN_Mode_Config(void);
static void CAN2_Mode_Config(void);

static void CAN_Filter_Config(void);
static void CAN2_Filter_Config(void);

void CAN_Config(void);
void CAN2_Config(void);

void CAN_Send_Msg(CanTxMsg *TxMessage, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq, uint32_t id);
//void CAN2_Send_Msg(CanTxMsg *TxMessage2, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq, uint32_t id);

void Message_buffer(CanRxMsg* RxMessage, uint8_t buf[][6]);
void Message_buffer2(CanRxMsg* RxMessage2, uint8_t buf[][6]);


#endif
