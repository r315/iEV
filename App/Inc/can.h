#ifndef _can_h_
#define _can_h_

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f7xx_hal.h"

#define CAN_INSTANCE    CAN1

HAL_StatusTypeDef CAN_Init(void (*cb)(uint8_t *data));
void CAN_SendTestMessage(void);

#ifdef __cplusplus
}
#endif

#endif