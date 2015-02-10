#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART.h"

int main(void){
  char buff[USART_BUFF_MAX];
  _delay_ms(100);
  sei();
  
  usart_start();
  while(1){
    if(!(usart_read(buff))){
      usart_write(buff);
    }
    _delay_ms(1000);
  }
  usart_stop();
}
