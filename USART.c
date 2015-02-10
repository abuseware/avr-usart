#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h> 

#include "USART.h"
#include <util/setbaud.h>

static char rx_buff[USART_BUFF_MAX];
static char tx_buff[USART_BUFF_MAX+3];
volatile static uint8_t rx_buff_pos = 0;
volatile static uint8_t tx_buff_pos = 0;

ISR(USART_RXC_vect){
  if(rx_buff_pos <= USART_BUFF_MAX){
    rx_buff[rx_buff_pos] = UDR;
    #ifdef USART_ECHO
    UDR = rx_buff[rx_buff_pos];
    #endif
    rx_buff_pos++;
  }
}

ISR(USART_UDRE_vect){
  if(tx_buff[tx_buff_pos] != 0){
    UDR = tx_buff[tx_buff_pos++];
  }else{
    UCSRB &= ~(1<<UDRIE);
    tx_buff_pos = 0;
  }
}

void usart_start(void){
  UBRRH = UBRRH_VALUE;
  UBRRL = UBRRL_VALUE;
  #if USE_2X
    UCSRA |=  (1<<U2X);
  #else
    UCSRA &= ~(1<<U2X);
  #endif

  UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
  UCSRB = (1<<TXEN) | (1<<RXEN) | (1<<RXCIE);
}

void usart_stop(void){
  UCSRB &= ~( (1<<TXEN) | (1<<RXEN) | (1<<RXCIE) | (1<<UDRIE));
}

void usart_write(char *data){
  while(!((UCSRA & (1<<UDRIE))));
  strncpy(tx_buff, data, USART_BUFF_MAX);
  strcat(tx_buff, "\r\n");
  UCSRB |= (1<<UDRIE);
}
    
uint8_t usart_read(char *buff){
  if(rx_buff_pos > 0){
    for(uint8_t i = 0; i <= rx_buff_pos; i++){
      if(rx_buff[i] == '\r' && rx_buff[i+1] == '\n'){
        buff[i] = 0;
        strncpy(rx_buff, &rx_buff[i+2], USART_BUFF_MAX);
        rx_buff_pos = 0;
        return 0;
      }else{
        buff[i] = rx_buff[i];
      }
    }
  }
  return 1;
}

