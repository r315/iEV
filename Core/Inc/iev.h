#ifndef _iev_h_
#define _iev_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* board related includes */
#include "stm32f769i_discovery.h"
/* RTOS includes */
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
/* Application includes */
#include "main.h"
#include "console.h"
#include "commands.h"
#include "can.h"
#include "logger.h"

#define RPM_QUEUE_LENGTH        2
#define UPDATE_RATE             10

#define LED3_TOGGLE             HAL_GPIO_TogglePin(LD_USER3_GPIO_Port, LD_USER3_Pin);

typedef struct {
    uint32_t rpm;
    uint32_t distance;
    uint32_t speed;
    uint16_t battery;
}QuadrantData;

typedef struct {
    double gearRacio;
    double totalDistance;
    double wheelCircumference;
    uint8_t updated;
    QuadrantData data;
    SemaphoreHandle_t mutex;
}SystemConfiguration_t;

extern QueueHandle_t qdataQueue;
extern SystemConfiguration_t qconfig;
extern StdOut uart;

#ifdef __cplusplus
}
#endif

#endif