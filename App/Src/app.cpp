#include "main.h"
#include "cmsis_os.h"
#include "iev.h"
#include "gui/main_screen/MainPresenter.hpp"
#include <stdio.h>

#define LOW_PRIORITY_TASK (tskIDLE_PRIORITY + 2)
#define NORMAL_PRIORITY_TASK (tskIDLE_PRIORITY + 2)
#define HIGH_PRIORITY_TASK (tskIDLE_PRIORITY + 5)
#define STACK_MINIMUM configMINIMAL_STACK_SIZE
#define STACK_MEDIUM (configMINIMAL_STACK_SIZE * 8)


Console console;
volatile QuadrantState qstate;
extern SerialOut vcom;

void ledAliveTask(void *argument){
  for(;;){
    HAL_GPIO_TogglePin(LD_USER2_GPIO_Port, LD_USER2_Pin);
    vTaskDelay(pdMS_TO_TICKS(200));    
    //osDelay(200);
  }
}

void consoleTask(void *argument){  
  Help help;
  Rpm rpm;
  Racio racio;
  CmdMem mem;

  vcom.init();
  console.init(&vcom, "iEV>");

  console.addCommand(&help);
  console.addCommand(&rpm);
  console.addCommand(&racio);
  console.addCommand(&mem);

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

  //osThreadDef(consoleProcess, consoleTask, osPriorityLow, 0, 512);
  //osThreadCreate(osThread(consoleProcess), NULL);

  //osThreadDef(aliveLed, ledAliveTask, osPriorityLow, 0, 128);
  //osThreadCreate(osThread(aliveLed), NULL);

  while(1){
    
      

      osDelay(100);
  }
}

