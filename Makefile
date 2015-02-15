CFLAGS=-Os -std=c99 -pedantic -Wall -Wextra -mmcu=atmega8a -DF_CPU=8000000UL
LDFLAGS=-Wl,-O1,-s,--sort-common,--as-needed

all: lib test

lib:
	avr-gcc ${CFLAGS} -c USART.c -o USART.o

test: lib
	avr-gcc ${CFLAGS} ${LDFLAGS} USART.o USART_test.c -o USART_test.bin
	avr-objcopy -O ihex USART_test.bin USART_test.hex

flash: test
	avrdude -c usbasp -p m8 -U flash:w:USART_test.hex
