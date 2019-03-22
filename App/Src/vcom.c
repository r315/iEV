
#include <stdint.h>
#include <stdio.h>
#include <serialout.h>

#include "stm32f7xx_hal.h"
//#include "usbd_cdc_if.h"

//#include "board.h"
//#include "usbserial.h"

#include "strfunc.h"
#include "fifo.h"

#define VCOM_FIFO_SIZE 512

static fifo_t txfifo;
static fifo_t rxfifo;

UART_HandleTypeDef huart1;
//DMA_HandleTypeDef hdma_usart1_rx;
//DMA_HandleTypeDef hdma_usart1_tx;


//HAL_DMA_RegisterCallback();
//HAL_UART_Transmit_DMA

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
void vc_init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&huart1);
}

void vc_putchar(char c){
  HAL_UART_Transmit(&huart1, (uint8_t *)&c, 1, 0xFFFF);
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

SerialOut vcom = {
    .init = vc_init,
    .xgetchar = vc_getchar,
    .xputchar = vc_putchar,
    .puts = vc_puts,
    .getCharNonBlocking = vc_getCharNonBlocking,
    .kbhit = vc_kbhit
};
