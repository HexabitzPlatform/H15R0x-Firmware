/*
    BitzOS (BOS) V0.2.4 - Copyright (C) 2017-2021 Hexabitz
    All rights reserved

    File Name     : H15R0.c
    Description   : Source code for module H15R0.
										Bipolar 10V Analog Output Module.
		
		Required MCU resources :
		
			>> USARTs 1,2,3,5 for module ports.
			>> GPIOA 4  for Analog Output Module.
			>> for DAC out   0  --->> OP-AM  is -10  volt.
			>> for DAC out  3.1 --->> OP-AM  is +8.5 volt.
			
*/
	
/* Includes ------------------------------------------------------------------*/
#include "BOS.h"
#include "H15R0.h"

/* Define UART variables */
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart5;

/* Exported variables */
extern FLASH_ProcessTypeDef pFlash;
extern uint8_t numOfRecordedSnippets;

/* Module exported parameters ------------------------------------------------*/
module_param_t modParam[NUM_MODULE_PARAMS] = {{.paramPtr=NULL, .paramFormat=FMT_FLOAT, .paramName=""}};

/* Private variables ---------------------------------------------------------*/ 
float Vref = 3.3;                          // Vref of op-am float MaxVoltage = 8.6;                    
float MaxVoltage = 9.3;                    // maximum out of op-am
float MinVoltage = -9.6;                   // minimum out of op-am
float MaxDACout = 3.2;
float DACOut;
int DAC_MaxDigitalValue = 255;         //For right-aligned 8-bit resolution: DAC_MaxDigitalValue = 0xFF
uint8_t ByteVal;

/* Private function prototypes -----------------------------------------------*/
void RegisterModuleCLICommands(void);

/* Create CLI commands --------------------------------------------------------*/

portBASE_TYPE percentagevalueCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
portBASE_TYPE AnalogvalueCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );

/*-----------------------------------------------------------*/
/* CLI command structure : color */
const CLI_Command_Definition_t percentagevalueCommandDefinition =
{
	( const int8_t * ) "percentage", /* The command string to type. */
	( const int8_t * ) "percentage:\r\n Give a percentage value ( 0 to 100 )% of OP-AM between ( -10 , +10 ) volt (1st par.)\r\n\r\n",
	percentagevalueCommand, /* The function to run. */
	1 /* One parameter is expected. */
};
/*-----------------------------------------------------------*/
/* CLI command structure : RGB */
const CLI_Command_Definition_t AnalogvalueCommandDefinition =
{
	( const int8_t * ) "analogval", /* The command string to type. */
	( const int8_t * ) "analogval:\r\n Give a precise value of OP-AM between ( -10 , +10 ) volt (1st par.)\r\n\r\n",
	AnalogvalueCommand, /* The function to run. */
	1 /* One parameter is expected. */
};
/*-----------------------------------------------------------*/

/* -----------------------------------------------------------------------
	|												 Private Functions	 														|
   ----------------------------------------------------------------------- 
*/
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            PREDIV                         = 1
  *            PLLMUL                         = 6
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_USART3;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
	
	__HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV32;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	

	__SYSCFG_CLK_ENABLE();

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
	
}

/* --- Save array topology and Command Snippets in Flash RO --- 
*/
uint8_t SaveToRO(void)
{
	BOS_Status result = BOS_OK; 
	HAL_StatusTypeDef FlashStatus = HAL_OK;
	uint16_t add = 2, temp = 0;
	uint8_t snipBuffer[sizeof(snippet_t)+1] = {0};
	
	HAL_FLASH_Unlock();
	
	/* Erase RO area */
	FLASH_PageErase(RO_START_ADDRESS);
	FlashStatus = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE); 
	if(FlashStatus != HAL_OK) {
		return pFlash.ErrorCode;
	} else {			
		/* Operation is completed, disable the PER Bit */
		CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
	}	
	
	/* Save number of modules and myID */
	if (myID)
	{
		temp = (uint16_t) (N<<8) + myID;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, RO_START_ADDRESS, temp);
		FlashStatus = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE); 
		if (FlashStatus != HAL_OK) {
			return pFlash.ErrorCode;
		} else {
			/* If the program operation is completed, disable the PG Bit */
			CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
		}			
	
	/* Save topology */
		for(uint8_t i=1 ; i<=N ; i++)
		{
			for(uint8_t j=0 ; j<=MaxNumOfPorts ; j++)
			{
				if (array[i-1][0]) {
					HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, RO_START_ADDRESS+add, array[i-1][j]);
					add += 2;
					FlashStatus = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE); 
					if (FlashStatus != HAL_OK) {
						return pFlash.ErrorCode;
					} else {
						/* If the program operation is completed, disable the PG Bit */
						CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
					}		
				}				
			}
		}
	}
	
	// Save Command Snippets
	int currentAdd = RO_MID_ADDRESS;
	for(uint8_t s=0 ; s<numOfRecordedSnippets ; s++) 
	{
		if (snippets[s].cond.conditionType) 
		{
			snipBuffer[0] = 0xFE;		// A marker to separate Snippets
			memcpy( (uint8_t *)&snipBuffer[1], (uint8_t *)&snippets[s], sizeof(snippet_t));
			// Copy the snippet struct buffer (20 x numOfRecordedSnippets). Note this is assuming sizeof(snippet_t) is even.
			for(uint8_t j=0 ; j<(sizeof(snippet_t)/2) ; j++)
			{		
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, currentAdd, *(uint16_t *)&snipBuffer[j*2]);
				FlashStatus = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE); 
				if (FlashStatus != HAL_OK) {
					return pFlash.ErrorCode;
				} else {
					/* If the program operation is completed, disable the PG Bit */
					CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
					currentAdd += 2;
				}				
			}			
			// Copy the snippet commands buffer. Always an even number. Note the string termination char might be skipped
			for(uint8_t j=0 ; j<((strlen(snippets[s].cmd)+1)/2) ; j++)
			{
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, currentAdd, *(uint16_t *)(snippets[s].cmd+j*2));
				FlashStatus = FLASH_WaitForLastOperation((uint32_t)HAL_FLASH_TIMEOUT_VALUE); 
				if (FlashStatus != HAL_OK) {
					return pFlash.ErrorCode;
				} else {
					/* If the program operation is completed, disable the PG Bit */
					CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
					currentAdd += 2;
				}				
			}				
		}	
	}
	
	HAL_FLASH_Lock();
	
	return result;
}

/* --- Clear array topology in SRAM and Flash RO --- 
*/
uint8_t ClearROtopology(void)
{
	// Clear the array 
	memset(array, 0, sizeof(array));
	N = 1; myID = 0;
	
	return SaveToRO();
}

/* --- H15R0 module initialization.
*/
void Module_Init(void)
{
	/* Peripheral clock enable */
	
	/* Array ports */
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART5_UART_Init();
	
	/* DAC output */
  MX_DAC_Init();
}
/*-----------------------------------------------------------*/

/* --- Register this module CLI Commands
*/
void RegisterModuleCLICommands(void)
{
	FreeRTOS_CLIRegisterCommand( &percentagevalueCommandDefinition );
	FreeRTOS_CLIRegisterCommand( &AnalogvalueCommandDefinition );
}
/*-----------------------------------------------------------*/

/* --- Get the port for a given UART.
*/
uint8_t GetPort(UART_HandleTypeDef *huart)
{
#ifdef H15R0
	     if (huart->Instance == USART5)
		return P1;
	else if (huart->Instance == USART2)
		return P2;
	else if (huart->Instance == USART3)
		return P3;
	else if (huart->Instance == USART1)
		return P4;
#endif

	return 0;
}
/*-----------------------------------------------------------*/
/* --- H15R0 message processing task. 
*/
Module_Status Module_MessagingTask(uint16_t code, uint8_t port, uint8_t src, uint8_t dst, uint8_t shift)
{
	Module_Status result = H15R0_OK;
	
	switch (code)
	{
		case CODE_H15R0_AnalogPercentage :
			AnalogPercentage(cMessage[port-1][shift]);
			break;
		
		case CODE_H15R0_AnalogOutValue :
				AnalogOutValue(cMessage[port-1][shift]);
			break;

		default:
			result = H15R0_ERR_UnknownMessage;
			break;
	}

	return result;
}
/*-----------------------------------------------------------*/


/* -----------------------------------------------------------------------
	|																APIs	 																 	|
   ----------------------------------------------------------------------- 
*/

/* --- Analog output percentage ---
*/
Module_Status AnalogPercentage(float outputVoltage)
{
	if ( outputVoltage >= 0 && outputVoltage <= 100 )
	{
	DACOut = outputVoltage * MaxDACout / 100;
	ByteVal = (DACOut * (DAC_MaxDigitalValue+1))/Vref;
  HAL_DAC_Start(&hdac,DAC1_CHANNEL_1);
 	HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_8B_R, ByteVal);
		
		return H15R0_OK;
	}
	else
	{
		return H15R0_ERR_WrongParams;
	}
}

/*-----------------------------------------------------------*/

/* --- Analog output value ---
*/
Module_Status AnalogOutValue(float outputVoltage)
{
//	if ( outputVoltage > -10 && outputVoltage < 10 )
//	{
		
	DACOut = ( outputVoltage - MinVoltage) * MaxDACout / ( MaxVoltage - MinVoltage);
	ByteVal = (DACOut * (DAC_MaxDigitalValue+1))/Vref;
  HAL_DAC_Start(&hdac,DAC1_CHANNEL_1);
 	HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_8B_R, ByteVal);
	
//		return H15R0_OK;
//	}
//	else
//	{
//		return H15R0_ERR_WrongParams;
//	}
}
/*-----------------------------------------------------------*/

/* -----------------------------------------------------------------------
	|															Commands																 	|
   ----------------------------------------------------------------------- 
*/
portBASE_TYPE percentagevalueCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString )
{
	Module_Status result = H15R0_OK;
	
	int8_t *pcParameterString1; portBASE_TYPE xParameterStringLength1 = 0;
	uint8_t outputVoltage = 0;
	static const int8_t *pcOKMessage = ( int8_t * ) "OP-AM  is at percentage value  %d%%\r\n";
	static const int8_t *pcWrongoutputVoltageMessage = ( int8_t * ) "Wrong outputVoltage!\n\r";
	
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	
	/* Obtain the 1st parameter string. */
	pcParameterString1 = ( int8_t * ) FreeRTOS_CLIGetParameter
								(
									pcCommandString,		/* The command string itself. */
									1,						/* Return the first parameter. */
									&xParameterStringLength1	/* Store the parameter string length. */
								);
	outputVoltage = ( uint8_t ) atol( ( char * ) pcParameterString1 );
	
	result = AnalogPercentage(outputVoltage);
	
	/* Respond to the command */
	if (result == H15R0_OK)
		sprintf( ( char * ) pcWriteBuffer, ( char * ) pcOKMessage, outputVoltage);
	else if (result == H15R0_ERR_WrongParams)
		strcpy( ( char * ) pcWriteBuffer, ( char * ) pcWrongoutputVoltageMessage);
	
	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}
/*-----------------------------------------------------------*/
portBASE_TYPE AnalogvalueCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString )
{
	Module_Status result = H15R0_OK;
	
	int8_t *pcParameterString1; portBASE_TYPE xParameterStringLength1 = 0;
	uint8_t outputVoltage = 0;
	static const int8_t *pcOKMessage = ( int8_t * ) "OP-AM  is at precise value %d%%\r\n";
	static const int8_t *pcWrongoutputVoltageMessage = ( int8_t * ) "Wrong outputVoltage!\n\r";
	
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	
	/* Obtain the 1st parameter string. */
	pcParameterString1 = ( int8_t * ) FreeRTOS_CLIGetParameter
								(
									pcCommandString,		/* The command string itself. */
									1,						/* Return the first parameter. */
									&xParameterStringLength1	/* Store the parameter string length. */
								);
	outputVoltage = ( uint8_t ) atol( ( char * ) pcParameterString1 );
	
	result = AnalogOutValue(outputVoltage);
	
	/* Respond to the command */
	if (result == H15R0_OK)
		sprintf( ( char * ) pcWriteBuffer, ( char * ) pcOKMessage, outputVoltage);
	else if (result == H15R0_ERR_WrongParams)
		strcpy( ( char * ) pcWriteBuffer, ( char * ) pcWrongoutputVoltageMessage);
	
	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}
/*-----------------------------------------------------------*/



/************************ (C) COPYRIGHT HEXABITZ *****END OF FILE****/
