 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Menna Saeed
 *
 *******************************************************************************/

#include "uart.h"

/*-------------------------------------------------------------------------------
 *                       Functions Definitions                           		 *
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
void UART_init(const Uart_ConfigType * Config_Ptr)
{
	/* Calculate the UBRR register value */
	uint16 ubrr_value =(F_CPU/((Config_Ptr -> baudRate)*8UL))-1;;

	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);

	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/ 
	UCSRB = (1<<RXEN) | (1<<TXEN);

	UCSRB=(UCSRB & 0xFB)|((Config_Ptr->bitData) & 0x04);
	
	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = 00 Disable parity bit
	 * USBS    = 0 One stop bit
	 * UCSZ1:0 = 11 For 8-bit data mode
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/ 	
	UCSRC |= (1<<URSEL);
	//Parity Mode
	UCSRC=(UCSRC&0XCF)|((Config_Ptr->parityBit & 0x03)<<4);
	//Character size: Bit data
	UCSRC=(UCSRC & 0xF9)|((Config_Ptr->bitData & 0x03)<<1);
	//Stop bit
	if (Config_Ptr -> stopBit == TWO_STOP_BIT){
			SET_BIT(UCSRC,USBS);
		}
		else{
			CLEAR_BIT(UCSRC,USBS);
		}
		CLEAR_BIT(UCSRC,UCPOL);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;
}
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

void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */

	while(BIT_IS_CLEAR(UCSRA,UDRE)){}
	/* Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now */
	UDR = data;
	/************************* Another Method *************************
		UDR = data;
		while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transimission is complete TXC = 1
		SET_BIT(UCSRA,TXC); // Clear the TXC flag
	 *******************************************************************/

}
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
uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */

	while(BIT_IS_CLEAR(UCSRA,RXC)){}
	/* Read the received data from the Rx buffer (UDR) and the RXC flag
	   will be cleared after read this data automatically */
	return UDR;
}
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
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}		
	*******************************************************************/
}
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
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}
