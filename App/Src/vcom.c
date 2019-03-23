
#include <stdint.h>
#include <stdio.h>
#include <serialout.h>

#include "main.h"
#include "stm32f7xx_hal.h"
//#include "usbd_cdc_if.h"
#include "FreeRTOS.h"
#include "queue.h"
//#include "board.h"
//#include "usbserial.h"
//#include "strfunc.h"
//#include "fifo.h"

#define VCOM_QUEUE_LENGTH 10
#define VCOM_QUEUE_ITEM_SIZE 1

UART_HandleTypeDef huart1;

QueueHandle_t vcQueue;


/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
void vc_init(void){
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX | USART_CR1_RXNEIE;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&huart1);
  vcQueue = xQueueCreate( VCOM_QUEUE_LENGTH, VCOM_QUEUE_ITEM_SIZE );
  uint8_t data = 55;
  if(vcQueue != NULL){
    NVIC_SetPriority(USART1_IRQn, NVIC_PRIORITYGROUP_0);
    NVIC_EnableIRQ(USART1_IRQn);  

  }  
}

void vc_putchar(char c){
  HAL_UART_Transmit(&huart1, (uint8_t *)&c, 1, 0xFFFF);
}

void vc_puts(const char* str){
  int size = 0;
  while(*(str + size))
    size++;
   HAL_UART_Transmit(&huart1, (uint8_t *)str, size, 0xFFFF);
}

uint8_t vc_kbhit(void){
  char c;
  return (xQueuePeek(vcQueue, &c, 0) == pdPASS) ? 1 : 0;
}

/**
 * Blocking call
 * */
char vc_getchar(void){
    char c;
    xQueueReceive(vcQueue, &c, portMAX_DELAY);    
    return c;
}

uint8_t vc_getCharNonBlocking(char *c){
   return (xQueueReceive(vcQueue, c, 0) == pdPASS) ? 1 : 0;
}

SerialOut vcom = {
    .init = vc_init,
    .xgetchar = vc_getchar,
    .xputchar = vc_putchar,
    .puts = vc_puts,
    .getCharNonBlocking = vc_getCharNonBlocking,
    .kbhit = vc_kbhit
};



/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  uint8_t data = USART1->RDR;
  xQueueSendToBackFromISR(vcQueue, &data, pdFALSE);  
  HAL_GPIO_TogglePin(LD_USER1_GPIO_Port, LD_USER1_Pin);
}