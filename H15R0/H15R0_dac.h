/*
 BitzOS (BOS) V0.2.9 - Copyright (C) 2017-2023 Hexabitz
 All rights reserved

  File Name          : H15R0_dac.h
  Description        : This file contains all the functions prototypes for
                       the DAC

*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __H15R0_dac_H
#define __H15R0_dac_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

extern DAC_HandleTypeDef hdac;	 
extern void MX_DAC_Init(void);
	 
#ifdef __cplusplus
}
#endif
#endif /*__H15R0_dac_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
