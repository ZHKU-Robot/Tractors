#include "Tim.h"

void Tim6_Config(void)
{
	/*	 Init		*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	/*	CLK		*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	/*	TIM6	*/
	TIM_TimeBaseInitStruct.TIM_Period = 500-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 168-1;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	
	/*	TIM6_it		*/
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	
	/*	TIM6-->ENABLE		*/
	TIM_Cmd(TIM6, ENABLE);
}

void Tim12_Config(void)
{
	/*	Init  */
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	/*	CLK	 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	
	/*	GPIO	*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	/*	AF	*/
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM12);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM12);
	
	/*	 Tim12  */
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1;
	TIM_TimeBaseInitStruct.TIM_Period = 2500-1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseInitStruct);
	
	/*	PWM	*/
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;										
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;					
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OC1Init(TIM12, &TIM_OCInitStruct);
	TIM_OC2Init(TIM12, &TIM_OCInitStruct);
	
	/*	Preload-->Enable	*/
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM12, ENABLE);
	
	/*	Tim12-->Enable	*/
	TIM_Cmd(TIM12, ENABLE);
}
