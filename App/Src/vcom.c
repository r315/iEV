
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

  HAL_UART_Transmit(&huart1, (uint8_t *)"Teste\n", 6, 0xFFFF);
}

void vc_putchar(char c){
    //while(!fifo_put(&txfifo, c));
    //while(CDC_Transmit_HS(&c, 1) != USBD_OK);
    putchar(c);
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

/* USER CODE BEGIN 1 */
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  
  return ch;
}
/* USER CODE END 1 */
