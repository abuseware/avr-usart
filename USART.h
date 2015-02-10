#pragma once

#define BAUD 9600 //Desired baud rate
#define USART_ECHO //Enable input echo
#define USART_BUFF_MAX 128 //RX and TX buffer size (must be < 254)


void usart_start(void);
void usart_stop(void);
void usart_write(char *data);
uint8_t usart_read(char *buff);
