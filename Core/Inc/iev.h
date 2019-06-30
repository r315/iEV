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
#define LED_TIME                400
#define BATTERY_VOLTAGE_SCALE   0.1

#define LED3_TOGGLE             HAL_GPIO_TogglePin(LD_USER3_GPIO_Port, LD_USER3_Pin);

/**
 * Symbols defined on the assumption that the gear racio
 * of the vehicle allows 5000rpm@120km/h
 * */
#define RPM_TS      ((double)UPDATE_RATE/1000.0)
#define RPM_REF     5000
#define SPEED_REF   120

/**
 * Can messages id's
 */
#define CAN_MSG_SIZE        10
#define CAN_MSG_01          0x601
#define CAN_MSG_02          0x602

typedef enum {Serial, Can} Mode_t;

typedef struct {
    uint32_t rpm;    
    uint16_t battery;
    uint8_t motorTemp;
    uint8_t controllerTemp;
    uint16_t motorCurrent;
}invData_t;

typedef struct {
    double distance;
    double gearRacio;
    double wheelCircumference;
    double batteryVoltage;
    uint32_t speed;
    uint8_t updated;
    invData_t *invData;
    SemaphoreHandle_t mutex;
    Mode_t mode;
    struct TM_struct tm;
}SystemData_t;

extern QueueHandle_t dispData;
extern SystemData_t cfgData;
extern invData_t invData;
extern StdOut uart;



#ifdef __cplusplus
}
#endif

#endif