#ifndef _iev_h_
#define _iev_h_

#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "console.h"

typedef struct {
    uint32_t rpm;
}QuadrantData;

extern Console console;

#endif