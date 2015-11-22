MCU=atmega328p
FREQ=16000000UL

CC=avr-gcc
AVRDUDE=avrdude

CFLAGS=-O3 -Wl,-s -std=c11 -pedantic -mmcu=$(MCU) -DF_CPU=$(FREQ)

OBJ=USART_test.o USART.o ringbuffer/ringbuffer.o

.PHONY: all clean flash main

all: $(HDR) $(OBJ) USART_test

clean:
	-rm -f *.o USART_test ringbuffer/*.o

flash:
	$(AVRDUDE) -c usbasp -p m328p -U flash:w:USART_test

USART_test:
	$(CC) $(CFLAGS) -o $@ $(OBJ)
