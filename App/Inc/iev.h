#ifndef _iev_h_
#define _iev_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* board related includes */
#include "stm32f769i_discovery.h"
/* RTOS includes */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
/* Application includes */
#include "main.h"
#include "console.h"
#include "commands.h"


typedef struct {
    uint32_t rpm;
    uint32_t distance;
    uint32_t speed;
}QuadrantData;

typedef struct {
    double gearRacio;
    double distance;    
    double wheelCircumference;
    uint32_t rpm;
    uint8_t updated;
    SemaphoreHandle_t mutex;
}QuadrantConfiguration;

extern QueueHandle_t qdataQueue;
extern QuadrantConfiguration qconfig;
extern SerialOut uart;


#ifdef __cplusplus
}
#endif

#endif