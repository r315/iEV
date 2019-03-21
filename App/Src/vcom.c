
#include <stdint.h>
#include <stdio.h>
#include <stdout.h>

//#include "board.h"
//#include "usbserial.h"

#include "usbd_cdc_if.h"
#include "strfunc.h"
#include "fifo.h"

#define VCOM_FIFO_SIZE 512

static fifo_t txfifo;
static fifo_t rxfifo;

void vc_init(void){
    rxfifo.size = VCOM_FIFO_SIZE;
	txfifo.size = VCOM_FIFO_SIZE;
	fifo_init(&txfifo);
	fifo_init(&rxfifo);
    
    fifo_flush(&txfifo);
	fifo_flush(&rxfifo);    
}   

void vc_putchar(char c){
    //while(!fifo_put(&txfifo, c));
    while(CDC_Transmit_HS(&c, 1) != USBD_OK);
}

void vc_puts(const char* str){
    while(*str)
        vc_putchar(*str++);
}

uint8_t vc_kbhit(void){
    return fifo_avail(&rxfifo);
}

char vc_getchar(void){
    char c;
    while(!fifo_get(&rxfifo, (uint8_t*)&c));
    return c;
}

uint8_t vc_getCharNonBlocking(char *c){
   return fifo_get(&rxfifo, (uint8_t*)c);
}

StdOut vcom = {
    .init = vc_init,
    .xgetchar = vc_getchar,
    .xputchar = vc_putchar,
    .puts = vc_puts,
    .getCharNonBlocking = vc_getCharNonBlocking,
    .kbhit = vc_kbhit
};
