 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Menna Saeed
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
#include "avr/io.h" /* To use the UART Registers */
#include "common_macros.h" /* To use the macros like SET_BIT */

/*------------------------------------------------------------------------------
 *                         Types Declaration                                   *
 ------------------------------------------------------------------------------*/
typedef enum
{
	BIT_5,BIT_6,BIT_7,BIT_8,BIT_9
}Uart_bitData;

typedef enum
{
	NO_PARITY,RESERVED,EVEN_PARITY,ODD_PARITY
}Uart_parityBit;

typedef enum
{
	ONE_STOP_BIT,TWO_STOP_BIT
}Uart_stopBit;

typedef enum
{
	BAUD_RATE_10=10,BAUD_RATE_300=300,BAUD_RATE_600=600,BAUD_RATE_1200=1200,BAUD_RATE_2400=2400,BAUD_RATE_4800=4800,
	BAUD_RATE_9600=9600,BAUD_RATE_14400=14400,BAUD_RATE_19200=19200,BAUD_RATE_38400=38400,BAUD_RATE_57600=57600,
	BAUD_RATE_115200=115200,BAUD_RATE_128000=128000,BAUD_RATE_256000=256000,
}Uart_baudRate;
/*-------------------------------------------------------------------------------
 * [Structure Name]: Uart_ConfigType
 *
 * [Description]: This structure is responsible for maintaining information about the UART configuration
 ----------------------------------------------------------------------------------*/
typedef struct
{
	/*
	 * bitData: a struct member of type Uart_bitData to indicate the number of data bits to be sent/received via UART
	 */
				Uart_bitData bitData;
	/*
	 * parityBit: a struct member of type Uart_parityBit to define the required parity mode
	 */
				Uart_parityBit parityBit;
	/*
	 * stopBit: a struct member of type Uart_stopBit to define the required number of stop bits to be sent
	 */
				Uart_stopBit  stopBit;
	/*
	 * baudRate:  a struct member of type Uart_baudRate to define the required baud rate
	 */
				Uart_baudRate baudRate;
}Uart_ConfigType;

/*-------------------------------------------------------------------------------
 *                       Functions Prototypes                          		 *
--------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------
 * [Function Name]: UART_init
 *
 * [Description]:  Function responsible for Initialize the UART device by:
 * 						1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 						2. Enable the UART.
 * 						3. Setup the UART baud rate.
 *
 * [Args]:        Config_Ptr: a constant pointer to struct that defines the UART required configuration
 *
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/
void UART_init(const Uart_ConfigType * Config_Ptr);
/*-------------------------------------------------------------------------------
 * [Function Name]: UART_sendByte
 *
 * [Description]:  Function responsible for send byte to another UART device.
 *
 * [Args]:        data: a uint8 variable indicates the required data to be sent
 *
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/
void UART_sendByte(const uint8 data);
/*-------------------------------------------------------------------------------
 * [Function Name]: UART_sendByte
 *
 * [Description]:  Function responsible for receive byte from another UART device.
 *
 * [Args]:        void
 *
 * [Returns]:      UDR: UART Data register value
 *
 ----------------------------------------------------------------------------------*/
uint8 UART_recieveByte(void);
/*-------------------------------------------------------------------------------
 * [Function Name]: UART_sendString
 *
 * [Description]:  Send the required string through UART to the other UART device.
 *
 * [Args]:        str: a pointer to a uint8 constant data
 *
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/
void UART_sendString(const uint8 *Str);
/*-------------------------------------------------------------------------------
 * [Function Name]: UART_receiveString
 *
 * [Description]:  Receive the required string until the '#' symbol through UART from the other UART device.
 *
 * [Args]:        str: a pointer to a uint8 data
 *
 * [Returns]:      Void
 *
 ----------------------------------------------------------------------------------*/
void UART_receiveString(uint8 *Str);

#endif /* UART_H_ */
