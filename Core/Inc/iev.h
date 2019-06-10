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
#include "TripMeter.h"

#define DISP_QUEUE_LENGTH        5
#define UPDATE_RATE             10 //ms

#define LED3_TOGGLE             HAL_GPIO_TogglePin(LD_USER3_GPIO_Port, LD_USER3_Pin);

/**
 * Symbols defined on the assumption that the gear racio
 * of the vehicle allows 5000rpm@120km/h
 * */
#define RPM_TS      0.01   //10ms
#define RPM_REF     5000
#define SPEED_REF   120


typedef enum {Serial, Can} Mode_t;

typedef struct {
    uint32_t rpm;    
    uint16_t battery;
    uint8_t motorTemp;
    uint8_t controllerTemp;
    uint16_t motorCurrent;
}invData_t;

typedef struct {
    uint32_t speed;
    double distance;
    double gearRacio;
    double wheelCircumference;
    uint8_t updated;
    invData_t invData;
    SemaphoreHandle_t mutex;
    Mode_t mode;
    struct TM_struct tm;
}SystemData_t;

extern QueueHandle_t invDataQueue;
extern SystemData_t cfgData;
extern StdOut uart;



#ifdef __cplusplus
}
#endif

#endif