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
float MinVoltage = -9.9;                   // minimum out of op-am
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
//portBASE_TYPE RGBCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
//portBASE_TYPE toggleCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
//portBASE_TYPE pulseColorCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
//portBASE_TYPE pulseRGBCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
//portBASE_TYPE sweepCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
//portBASE_TYPE dimCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );

/* CLI command structure : demo */


/*-----------------------------------------------------------*/
/* CLI command structure : on */


/*-----------------------------------------------------------*/
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
/* CLI command structure : pulseColor */


/*-----------------------------------------------------------*/
/* CLI command structure : pulseRGB */


/*-----------------------------------------------------------*/
/* CLI command structure : sweep */


/*-----------------------------------------------------------*/
/* CLI command structure : dim */


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
	
	/* Create the RGB LED task */
}

/*-----------------------------------------------------------*/

/* --- H15R0 message processing task. 
*/



/*-----------------------------------------------------------*/

/* --- Register this module CLI Commands 
*/
void RegisterModuleCLICommands(void)
{
//	FreeRTOS_CLIRegisterCommand( &demoCommandDefinition );
//	FreeRTOS_CLIRegisterCommand( &onCommandDefinition );
//	FreeRTOS_CLIRegisterCommand( &offCommandDefinition );
//	FreeRTOS_CLIRegisterCommand( &colorCommandDefinition );
//	FreeRTOS_CLIRegisterCommand( &RGBCommandDefinition );
//	FreeRTOS_CLIRegisterCommand( &toggleCommandDefinition );
//	FreeRTOS_CLIRegisterCommand( &pulseColorCommandDefinition );
//	FreeRTOS_CLIRegisterCommand( &pulseRGBCommandDefinition );
//	FreeRTOS_CLIRegisterCommand( &sweepCommandDefinition );
//	FreeRTOS_CLIRegisterCommand( &dimCommandDefinition );
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
Module_Status Module_MessagingTask(uint16_t code, uint8_t port, uint8_t src, uint8_t dst, uint8_t shift)
{
//	Module_Status result = H26R0_OK;
//  uint32_t period = 0;
//  uint32_t timeout = 0;
//	
//	switch (code)
//	{
//		case (CODE_H26R0_SET_RATE):
//			SetHX711Rate(cMessage[port-1][shift]);
//			break;
//		
//		case (CODE_H26R0_STREAM_PORT_GRAM):
//			period = ( (uint32_t) cMessage[port-1][1+shift] << 24 ) + ( (uint32_t) cMessage[port-1][2+shift] << 16 ) + ( (uint32_t) cMessage[port-1][3+shift] << 8 ) + cMessage[port-1][4+shift];
//			timeout = ( (uint32_t) cMessage[port-1][5+shift] << 24 ) + ( (uint32_t) cMessage[port-1][6+shift] << 16 ) + ( (uint32_t) cMessage[port-1][7+shift] << 8 ) + cMessage[port-1][8+shift];
//			StreamGramToPort(cMessage[port-1][shift], cMessage[port-1][9+shift], cMessage[port-1][10+shift], period, timeout);
//			break;
//		
//		case (CODE_H26R0_STREAM_PORT_KGRAM):
//			period = ( (uint32_t) cMessage[port-1][1+shift] << 24 ) + ( (uint32_t) cMessage[port-1][2+shift] << 16 ) + ( (uint32_t) cMessage[port-1][3+shift] << 8 ) + cMessage[port-1][4+shift];
//			timeout = ( (uint32_t) cMessage[port-1][5+shift] << 24 ) + ( (uint32_t) cMessage[port-1][6+shift] << 16 ) + ( (uint32_t) cMessage[port-1][7+shift] << 8 ) + cMessage[port-1][8+shift];
//			StreamKGramToPort(cMessage[port-1][shift], cMessage[port-1][9+shift], cMessage[port-1][10+shift], period, timeout);
//			break;
//		
//    case (CODE_H26R0_STREAM_PORT_OUNCE):
//			period = ( (uint32_t) cMessage[port-1][1+shift] << 24 ) + ( (uint32_t) cMessage[port-1][2+shift] << 16 ) + ( (uint32_t) cMessage[port-1][3+shift] << 8 ) + cMessage[port-1][4+shift];
//			timeout = ( (uint32_t) cMessage[port-1][5+shift] << 24 ) + ( (uint32_t) cMessage[port-1][6+shift] << 16 ) + ( (uint32_t) cMessage[port-1][7+shift] << 8 ) + cMessage[port-1][8+shift];
//			StreamOunceToPort(cMessage[port-1][shift], cMessage[port-1][9+shift], cMessage[port-1][10+shift], period, timeout);
//			break;
//		
//		case (CODE_H26R0_STREAM_PORT_POUND):
//			period = ( (uint32_t) cMessage[port-1][1+shift] << 24 ) + ( (uint32_t) cMessage[port-1][2+shift] << 16 ) + ( (uint32_t) cMessage[port-1][3+shift] << 8 ) + cMessage[port-1][4+shift];
//			timeout = ( (uint32_t) cMessage[port-1][5+shift] << 24 ) + ( (uint32_t) cMessage[port-1][6+shift] << 16 ) + ( (uint32_t) cMessage[port-1][7+shift] << 8 ) + cMessage[port-1][8+shift];
//			StreamPoundToPort(cMessage[port-1][shift], cMessage[port-1][9+shift], cMessage[port-1][10+shift], period, timeout);
//			break;
//		
//		case (CODE_H26R0_STOP):
//			global_mode=IDLE_CASE;
//			PowerDown();
//			xTimerStop( xTimer, portMAX_DELAY );
//			break;
//		
//		case (CODE_H26R0_SAMPLE_GRAM):
//			if (cMessage[port-1][shift] == 1)
//				H26R0_Weight1=SampleGram(cMessage[port-1][shift]);
//			else
//				H26R0_Weight2=SampleGram(cMessage[port-1][shift]);
//			break;
//			
//		case (CODE_H26R0_SAMPLE_KGRAM):
//			if (cMessage[port-1][shift] == 1)
//				H26R0_Weight1=SampleKGram(cMessage[port-1][shift]);
//			else
//				H26R0_Weight2=SampleKGram(cMessage[port-1][shift]);	
//			break;
//			
//		case (CODE_H26R0_SAMPLE_OUNCE):
//			if (cMessage[port-1][shift] == 1)
//				H26R0_Weight1=SampleOunce(cMessage[port-1][shift]);
//			else
//				H26R0_Weight2=SampleOunce(cMessage[port-1][shift]);	
//			break;
//			
//		case (CODE_H26R0_SAMPLE_POUND):
//			if (cMessage[port-1][shift] == 1)
//				H26R0_Weight1=SamplePound(cMessage[port-1][shift]);
//			else
//				H26R0_Weight2=SamplePound(cMessage[port-1][shift]);
//			break;
//			
//		case (CODE_H26R0_ZEROCAL):
//				ZeroCal(cMessage[port-1][shift]);
//			break;
//			
//		case (CODE_H26R0_STREAM_RAW):
//			period = ( (uint32_t) cMessage[port-1][1+shift] << 24 ) + ( (uint32_t) cMessage[port-1][2+shift] << 16 ) + ( (uint32_t) cMessage[port-1][3+shift] << 8 ) + cMessage[port-1][4+shift];
//			timeout = ( (uint32_t) cMessage[port-1][5+shift] << 24 ) + ( (uint32_t) cMessage[port-1][6+shift] << 16 ) + ( (uint32_t) cMessage[port-1][7+shift] << 8 ) + cMessage[port-1][8+shift];
//			StreamRawToPort(cMessage[port-1][shift], cMessage[port-1][9+shift], cMessage[port-1][10+shift], period, timeout);
//			H26R0_Weight2=Average(cMessage[port-1][shift],1);	
//			break;
//			
//		case (CODE_H26R0_SAMPLE_RAW):
//			H26R0_Weight2=Average(cMessage[port-1][shift],1);	
//			break;
//		
//		case (CODE_H26R0_STREAM_FORMAT):
//			if (cMessage[port-1][shift] == 0)
//				H26R0_DATA_FORMAT = FMT_UINT32;
//			else
//				H26R0_DATA_FORMAT = FMT_FLOAT;
//			break;
//			
//		default:
//			result = H26R0_ERR_UnknownMessage;
//			break;
//	}			

return 0;	
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

/* --- RGB LED basic color sweep --- 
*/


/*-----------------------------------------------------------*/

/* --- RGB LED fine color sweep --- 
*/


/*-----------------------------------------------------------*/

/* --- Dim the RGB LED --- 
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

/* --- Toggle RGB LED ---
*/



/*-----------------------------------------------------------*/

/* --- Set RGB colors on LED (continuously) using PWM intensity modulation. 
*/



/*-----------------------------------------------------------*/

/* --- Set LED color from a predefined color list (continuously) 
				using PWM intensity modulation. 
*/



/*-----------------------------------------------------------*/

/* --- Activate RGB LED dim mode using one of the basic colors. Set repeat to -1 for periodic signals --- 
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



/*-----------------------------------------------------------*/



/*-----------------------------------------------------------*/



/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/



/*-----------------------------------------------------------*/


/************************ (C) COPYRIGHT HEXABITZ *****END OF FILE****/
