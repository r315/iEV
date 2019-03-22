#include "main.h"
#include "cmsis_os.h"
#include "console.hpp"
#include <stdio.h>

extern SerialOut vcom;

void ledTask(void const * argument){

  for(;;){
    HAL_GPIO_TogglePin(LD_USER1_GPIO_Port, LD_USER1_Pin);
    osDelay(200);    
  }
}

void consoleTask(void const * argument){
  Console console;

  vcom.init();
  console.init(&vcom, "iEV>");   

  for(;;){
    console.process();
    osDelay(100);
  }
}

void appTask(void const * argument){

    osThreadDef(consoleProcess, consoleTask, osPriorityLow, 0, 512);
  osThreadCreate(osThread(consoleProcess), NULL);

    osThreadDef(aliveLed, ledTask, osPriorityLow, 0, 128);
  osThreadCreate(osThread(aliveLed), NULL);

  while(1){
      osDelay(100);
  }

}