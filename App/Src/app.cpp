#include "main.h"
#include "cmsis_os.h"
#include "iev.h"
#include "gui/main_screen/MainPresenter.hpp"
#include <stdio.h>


Console console;
QuadrantState qstate;
extern SerialOut vcom;

void ledAliveTask(void const * argument){
  for(;;){
    HAL_GPIO_TogglePin(LD_USER2_GPIO_Port, LD_USER2_Pin);
    vTaskDelay(pdMS_TO_TICKS(200));    
    //osDelay(200);
  }
}

void consoleTask(void const * argument){  
  Help help;
  Rpm rpm;

  vcom.init();
  console.init(&vcom, "iEV>");

  console.addCommand(&help);
  console.addCommand(&rpm);

  for(;;){
    console.process();
    osDelay(100);
  }
}

void appTask(void const * argument){

  // load data from NVRAM

  qstate.gearRacio = 3000;


  osThreadDef(consoleProcess, consoleTask, osPriorityLow, 0, 512);
  osThreadCreate(osThread(consoleProcess), NULL);

  osThreadDef(aliveLed, ledAliveTask, osPriorityLow, 0, 128);
  osThreadCreate(osThread(aliveLed), NULL);

  while(1){
    
      

      osDelay(100);
  }
}

