#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART.h"

void usart_rx(void) {
	uint8_t buff[USART_BUFFER_SIZE];
	uint8_t size;
	size = usart_receive(buff);
	usart_send(buff, size);
	usart_send((uint8_t*)"OK\r\n", 4);
}

int main(void) {
  _delay_ms(100);

  usart_start(&usart_rx);
  sei();

  while(1);
}
