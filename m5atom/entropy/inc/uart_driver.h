/*
 * uart_driver.h
 *
 *  Created on: Nov 30, 2019
 *      Author: Pollock
 */

#ifndef INC_UART_DRIVER_H_
#define INC_UART_DRIVER_H_
#include <stdint.h> //for uint8_t
#include <stdio.h>

static const int RX_BUF_SIZE = (1024);
#define RD_BUF_SIZE (RX_BUF_SIZE)

#define TXD_PIN 			(GPIO_NUM_16)
#define RXD_PIN 			(GPIO_NUM_17)
#define MAIN_UART 			UART_NUM_2
#define BAUD_RATE 			(9600)
#define PATTERN_CHR_NUM    		(3)         /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/
#define ONLINE_TOKEN_BUFFER_LEN		20
#define ICT_VALIDATED_BILL_SIGNAL	0x81

void
uart_main_init (void);
int
uart_transmit (uint8_t data[] , size_t _len);
int
uart_get_cash_value (void);

#endif /* INC_UART_DRIVER_H_ */
