CFLAGS=-Os -std=c99 -pedantic -mmcu=atmega8a -DF_CPU=8000000UL

all: lib test

lib:
	avr-gcc ${CFLAGS} -c USART.c -o USART.o

test: lib
	avr-gcc ${CFLAGS} USART.o USART_test.c -o USART_test.bin
	avr-objcopy -O ihex USART_test.bin USART_test.hex

flash: test
	avrdude -c usbasp -p m8 -U flash:w:USART_test.hex
