# USART library for Atmel AVR
This library provides light API for using USART in text mode.

# How to use it?

## void usart_start(void (\*receive_cb)(void))
Enable AVR hardware USART with baud rate from USART.h and function from
parameter as a callback for every complete packet.

Complete packet is every line ending with **\r\n** (*0xDA*) or **\n** (*0xA*).

You MUST enable global interrupts!

## void usart\_stop(void)
Disable USART and free memory. All ongoing operations will be terminated!

## uint8_t usart_receive(uint8_t\* data);
Get packet and it's length (as return value).
 
    uint8_t buffer[64];
    uint8_t len = usart_receive(buffer);

## uint8_t usart_send(const uint8_t\* data, const uint8_t len);
Add packet to queue, or return error.

    if(!usart_send("OK\r\n", 4) {
      return 1;
    }

**Error 1:** data_lenght above free buffer space.

**Be sure to start USART before!**

# Packet format

    Hello world!\r\n

or

    Hello world!\n

# Credits
Artur "Licho" Kaleta - Creator and maintainer