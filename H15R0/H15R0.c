/*
    BitzOS (BOS) V0.2.1 - Copyright (C) 2017-2020 Hexabitz
    All rights reserved

    File Name     : H15R0.c
    Description   : Source code for module H15R0.
										Bipolar 10V Analog Output Module.
		
		Required MCU resources :
		
			>> USARTs 1,2,3,4 for module ports.
			>> GPIOA 4  for Analog Output Module.
			>> for DAC out   0  --->> OP-AM  is -10  volt.
			>> for DAC out  3.1 --->> OP-AM  is +8.5 volt.
			
*/
	
/* Includes ------------------------------------------------------------------*/
#include "BOS.h"


/* Define UART variables */
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart5;

/* Module exported parameters ------------------------------------------------*/
module_param_t modParam[NUM_MODULE_PARAMS] = {{.paramPtr=NULL, .paramFormat=FMT_FLOAT, .paramName=""}};

/* Private variables ---------------------------------------------------------*/
float Vref = 3.3;                          // Vref of op-am
float MaxVoltage = 8.6;                    // maximum out of op-am
float MinVoltage = -10;                   // minimum out of op-am
float MaxDACout = 3.1;
float DACOut;
int DAC_MaxDigitalValue = 256;         //For right-aligned 8-bit resolution: DAC_MaxDigitalValue = 0xFF
uint8_t ByteVal;

/* Private function prototypes -----------------------------------------------*/	




/* Create CLI commands --------------------------------------------------------*/

//portBASE_TYPE demoCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
//portBASE_TYPE onCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
//portBASE_TYPE offCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
//portBASE_TYPE colorCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );

/* CLI command structure : off */


/*-----------------------------------------------------------*/
/* CLI command structure : color */


/*-----------------------------------------------------------*/
/* CLI command structure : RGB */
//const CLI_Command_Definition_t RGBCommandDefinition =
//{
//	( const int8_t * ) "rgb", /* The command string to type. */
//	( const int8_t * ) "RGB:\r\n Set RGB LED red (1st par.), green (2nd par.), and blue (3rd par.) values (0-255) at a specific intensity (0-100%) (4th par.)\r\n\r\n",
//	RGBCommand, /* The function to run. */
//	4 /* Four parameters are expected. */
//};
/*-----------------------------------------------------------*/
/* CLI command structure : toggle */


/*-----------------------------------------------------------*/

/* -----------------------------------------------------------------------
	|												 Private Functions	 														|
   ----------------------------------------------------------------------- 
*/


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
//	FreeRTOS_CLIRegisterCommand( &demoCommandDefinition );
//	FreeRTOS_CLIRegisterCommand( &onCommandDefinition );
//	FreeRTOS_CLIRegisterCommand( &offCommandDefinition );
//	FreeRTOS_CLIRegisterCommand( &colorCommandDefinition );
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

/* RGBledTask function */
void RGBledTask(void * argument)
{

  /* Infinite loop */
  for(;;)
  {
	
	}	

}
/*-----------------------------------------------------------*/

/* --- Pulse the RGB LED --- 
*/


/*-----------------------------------------------------------*/

/* -----------------------------------------------------------------------
	|																APIs	 																 	|
   ----------------------------------------------------------------------- 
*/

/* --- Analog output percentage ---
*/
Module_Status AnalogPercentage(float outputVoltage)
{
	DACOut = outputVoltage * MaxDACout / 100;
	ByteVal = (DACOut * DAC_MaxDigitalValue)/Vref;
	
  HAL_DAC_Start(&hdac,DAC1_CHANNEL_1);
 	HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_8B_R, ByteVal);
}

/*-----------------------------------------------------------*/

/* --- Analog output value ---
*/
Module_Status AnalogOutValue(float outputVoltage)
{
	DACOut = ( outputVoltage - MinVoltage) * MaxDACout / ( MaxVoltage - MinVoltage);
	ByteVal = (DACOut * DAC_MaxDigitalValue)/Vref;
  HAL_DAC_Start(&hdac,DAC1_CHANNEL_1);
 	HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_8B_R, ByteVal);

}
/*-----------------------------------------------------------*/

/* --- 
*/



/*-----------------------------------------------------------*/

/* -----------------------------------------------------------------------
	|															Commands																 	|
   ----------------------------------------------------------------------- 
*/




/*-----------------------------------------------------------*/




/*-----------------------------------------------------------*/




/*-----------------------------------------------------------*/



/*-----------------------------------------------------------*/




/*-----------------------------------------------------------*/


/************************ (C) COPYRIGHT HEXABITZ *****END OF FILE****/
