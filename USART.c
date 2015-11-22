#include <avr/interrupt.h>

#include "USART.h"
#include <util/setbaud.h>

#include "ringbuffer/ringbuffer.h"

//Handler for user defined callback
static void (*usart_receive_cb)(void);

//Buffers
static ring_buffer_t* rx_buff;
static ring_buffer_t* tx_buff;

//Data receive interrupt
ISR(USART_RX_vect) {
  uint8_t buff = UDR0;

	ring_buffer_write(rx_buff, buff);

  //Whole packet received?
  if(buff == '\n') {
    usart_receive_cb();
  }
}

//Data send interrupt
ISR(USART_UDRE_vect) {
	uint8_t buff;

	//Fill registry with data byte
	if(!ring_buffer_read(tx_buff, &buff)) {
		UDR0 = buff;
	} else {
		UCSR0B &= ~(1<<UDRIE0);
  }
}

//Start USART with user defined callback
void usart_start(void (*receive_cb)(void)) {
  //Set USART parameters (from setbaud.h)
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;
  #if USE_2X
  UCSR0A |=  (1<<U2X0);
  #else
  UCSR0A &= ~(1<<U2X0);
  #endif

  //Promote callback
  usart_receive_cb = receive_cb;

  //Enable USART
  UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
  UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0);

	rx_buff = ring_buffer_create(USART_BUFFER_SIZE);
  tx_buff = ring_buffer_create(USART_BUFFER_SIZE);
}

//Disable USART and destroy transmission buffer
void usart_stop(void) {
  UCSR0B &= ~( (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0) | (1<<UDRIE0));
	ring_buffer_destroy(rx_buff);
  ring_buffer_destroy(tx_buff);
}

uint8_t usart_receive(uint8_t* data) {
  uint8_t buff;
  uint8_t i = 0;
  while(!ring_buffer_read(rx_buff, &buff)) {
    data[i++] = buff;
  }

  return i;
}

//Put packet into ring buffer
uint8_t usart_send(const uint8_t* data, const uint8_t len) {
  uint8_t ret = 0;
  for(uint8_t i = 0; i < len; i++) {
    if(ring_buffer_write(tx_buff, data[i])) {
      ret = 1;
      break;
    }
  }

  UCSR0B |= (1<<UDRIE0);
  return ret;
}
