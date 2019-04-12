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
/* Application includes */
#include "main.h"
#include "console.h"
#include "commands.h"


typedef struct {
    uint32_t rpm;
}QuadrantData;

typedef struct {
    uint32_t gearRacio;
    uint32_t distance;
    
}QuadrantState;

extern QueueHandle_t qdataQueue;
extern volatile QuadrantState qstate; /* Not protected with mutex */
extern SerialOut uart;


#ifdef __cplusplus
}
#endif

#endif