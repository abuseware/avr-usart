#pragma once
#include <avr/io.h>
#include "ringbuffer/ringbuffer.h"

#define BAUD 9600 //Desired baud rate
#define USART_BUFFER_SIZE 32 //Desired buffer size

//Don't edit!
void usart_start(void (*receive_cb)(void));
void usart_stop(void);

uint8_t usart_receive(uint8_t* data);
uint8_t usart_send(const uint8_t* data, const uint8_t len);
