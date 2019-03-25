#ifndef _iev_h_
#define _iev_h_

#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "commands.h"


typedef struct {
    uint32_t rpm;
}QuadrantData;

typedef struct {
    uint32_t gearRacio;
    uint32_t distance;
    
}QuadrantState;

extern Console console;
extern QueueHandle_t qdataQueue;
extern QuadrantState qstate;

#endif