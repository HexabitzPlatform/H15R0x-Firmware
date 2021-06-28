/*
 File Name          : H15R0_dac.c
 Description        : This file provides code for the configuration
                      of the DAC instances.

  */

/* Includes ------------------------------------------------------------------*/
#include "BOS.h"

DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac_ch1;
/* DAC init function */
void MX_DAC_Init(void)
{
  DAC_ChannelConfTypeDef sConfig;
  __DAC1_CLK_ENABLE();

  /* DAC Initialization */
  hdac.Instance = DAC;
  HAL_DAC_Init(&hdac);

  /* DAC channel OUT1 config */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);
}

/** Enable DMA controller clock */
void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Ch2_3_DMA2_Ch1_2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Ch2_3_DMA2_Ch1_2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Ch2_3_DMA2_Ch1_2_IRQn);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
