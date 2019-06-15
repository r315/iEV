#pragma once
#ifndef _nvdata_h_
#define _nvdata_h_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


#include "stm32f7xx_hal.h"	
#define NVDATA_SIZE 8	// sizeof double
#define NVDATA_BLOCK_SIZE (NVDATA_SIZE + 1)
#define NVDATA_EMPTY 0xFF
#define NVDATA_VALID 0xCC

#define SRAM_NVDATA

#if defined(SRAM_NVDATA)
#define NVDATA_SECTOR_SIZE      0x100 //4k
#define NVDATA_SECTOR_START     BKPSRAM_BASE
#define NVDATA_SECTOR_END       (NVDATA_SECTOR_START + NVDATA_SECTOR_SIZE)

#define NVDATA_SECTOR_INIT \
{ \
    __HAL_RCC_PWR_CLK_ENABLE(); /* Enable the power interface clock */ \
    HAL_PWR_EnableBkUpAccess(); /* enable access to the backup domain */ \
    __HAL_RCC_BKPSRAM_CLK_ENABLE(); /* Enable the backup SRAM clock */ \
}
#define NVDATA_SECTOR_ERASE(x) memset((void*)x, 0xff, NVDATA_SECTOR_SIZE)

static inline void NVDATA_SECTOR_READ(uint8_t *dst, uint8_t *src, uint32_t count){
    while(count--){
        *dst++ = *src++;
    }
}

static inline void NVDATA_SECTOR_WRITE(uint8_t *dst, uint8_t *src, uint32_t count){
    while(count--){
        *dst++ = *src++;
    }
}
#endif


typedef struct _NVData {
	uint8_t *freeBlock;
	uint8_t data[NVDATA_SIZE];
	uint8_t status;
}NVData;

uint8_t NVData_Init();
uint32_t NVData_Restore(void* data, uint16_t count);
uint32_t NVData_Save(void* data, uint16_t count);


#ifdef __cplusplus
}
#endif

#endif