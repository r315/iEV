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

typedef enum {Serial, Can} Mode_t;

typedef struct {
    uint32_t rpm;
    uint32_t distance;
    uint32_t speed;
    uint16_t battery;
    uint8_t motorTemp;
    uint8_t controllerTemp;
    uint16_t motorCurrent;
}invData_t;

typedef struct {
    double gearRacio;
    double totalDistance;
    double wheelCircumference;
    uint8_t updated;
    invData_t invData;
    SemaphoreHandle_t mutex;
    Mode_t mode;
}SystemConfiguration_t;

extern QueueHandle_t invDataQueue;
extern SystemConfiguration_t cfgData;
extern StdOut uart;

#ifdef __cplusplus
}
#endif

#endif