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

static const char prompt[] = "iEV>";
Console console;

void ledAliveTask(void *argument){
  for(;;){
    BSP_LED_Toggle(LED_GREEN);    
    vTaskDelay(pdMS_TO_TICKS(400));
  }
}

void consoleTask(void *argument){  
  Help help;
  Rpm rpm;
  Racio racio;
  CmdMem mem;
  SDCard sd;

  vcom.init();
  console.init(&vcom, prompt);

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

void appTask(void const * argument){

  // load data from NVRAM

  qstate.gearRacio = 3000;

  xTaskCreate(ledAliveTask, "Alive Led", STACK_MINIMUM, NULL, LOW_PRIORITY_TASK, NULL);
  xTaskCreate(consoleTask, "Console", STACK_MEDIUM, NULL, NORMAL_PRIORITY_TASK, NULL);

  while(1){
      osDelay(100);
  }
}

