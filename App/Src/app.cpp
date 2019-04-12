#include "main.h"
#include "cmsis_os.h"
#include "iev.h"
#include "gui/main_screen/MainPresenter.hpp"
#include <stdio.h>

#define LOW_PRIORITY_TASK (tskIDLE_PRIORITY + 2)
#define NORMAL_PRIORITY_TASK (tskIDLE_PRIORITY + 5)
#define HIGH_PRIORITY_TASK (tskIDLE_PRIORITY + 7)
#define STACK_MINIMUM configMINIMAL_STACK_SIZE      // 128 * 4
#define STACK_MEDIUM (configMINIMAL_STACK_SIZE * 8) // 128 * 8 * 4

void GRAPHICS_MainTask(void);
void GRAPHICS_Init(void);
void GRAPHICS_HW_Init(void);

static const char prompt[] = "iEV>";
Console console;

void ledAliveTask(void *argument){
  for(;;){
    BSP_LED_Toggle(LED_GREEN);    
    vTaskDelay(pdMS_TO_TICKS(400));
  }
}

void graphicsTask(void *argument)
{  
  /* Graphic application, never return */
  GRAPHICS_MainTask();

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);   
  }
}

void consoleTask(void *argument){  
  Help help;
  Rpm rpm;
  Racio racio;
  CmdMem mem;
  SDCard sd;

  uart.init();
  console.init(&uart, prompt);

  console.addCommand(&help);
  console.addCommand(&rpm);
  console.addCommand(&racio);
  console.addCommand(&mem);
  console.addCommand(&sd);

  for(;;){
    console.process();
    osDelay(100);
  }
}

extern "C" void appMain(void){

  // load data from NVRAM

  qstate.gearRacio = 3000;

  /* Initialise the graphical hardware */
  GRAPHICS_HW_Init();

  /* Initialise the graphical stack engine */  
  GRAPHICS_Init();

  xTaskCreate(graphicsTask, "Graphics Task", STACK_MEDIUM, NULL, NORMAL_PRIORITY_TASK, NULL);
  xTaskCreate(ledAliveTask, "Alive Led Task", STACK_MINIMUM, NULL, LOW_PRIORITY_TASK, NULL);
  xTaskCreate(consoleTask, "Console Task", STACK_MEDIUM, NULL, NORMAL_PRIORITY_TASK, NULL);

  /* Start scheduler */
  osKernelStart();

  while(1){
      osDelay(100);
  }
}

