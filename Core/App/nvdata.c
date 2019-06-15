#include <string.h>
#include "nvdata.h"

static NVData nvd;

/**
 * Each sector is divided in blocks with size NVDATA_BLOCK_SIZE, which corresponds to
 * n data bytes + 1 byte for status
 * On each write of one byte or more, a new block is written after the current block.
 * When the sector is fully written, it is erased and the operation repeats
 * */
uint8_t NVData_Init() {
	uint8_t *ptr = (uint8_t*)(NVDATA_SECTOR_START + NVDATA_SIZE);
	uint8_t *lastWritten = NULL;

	nvd.freeBlock = NULL;
	memset(nvd.data, 0xff, NVDATA_SIZE);

	NVDATA_SECTOR_INIT;

	do {
		if (*ptr == NVDATA_VALID) {
			lastWritten = ptr;
		}
		else if (*ptr == NVDATA_EMPTY) {
			//set freeblock to the found empty block start
			nvd.freeBlock = ptr - NVDATA_SIZE;
			if (lastWritten != NULL) {
				//get data from previous block, if has valid data
				NVDATA_SECTOR_READ(nvd.data, (uint8_t*)(lastWritten - NVDATA_SIZE), NVDATA_SIZE);
				return 1;
			}
			return 0;
		}
	} while ((uint32_t)(ptr += NVDATA_BLOCK_SIZE) < NVDATA_SECTOR_END);
	// no data found initialize sector
	NVDATA_SECTOR_ERASE(NVDATA_SECTOR_START);
	nvd.freeBlock = (uint8_t*)NVDATA_SECTOR_START;
	return 0;
}

uint32_t NVData_Restore(void* data, uint16_t count) {
	uint8_t *dst = (uint8_t*)data;

	if (count > NVDATA_SIZE)
		count = NVDATA_SIZE;

	if (nvd.freeBlock == NULL)
		return 0;

	memcpy(dst, nvd.data, count);

	return count;
}

uint32_t NVData_Save(void* data, uint16_t count) {
	uint8_t *src = (uint8_t*)data;

	if (count > NVDATA_SIZE)
		count = NVDATA_SIZE;

	if (nvd.freeBlock == NULL)
		return 0;

	if ((uint32_t)(nvd.freeBlock + NVDATA_BLOCK_SIZE) >= NVDATA_SECTOR_END) {
		NVDATA_SECTOR_ERASE(NVDATA_SECTOR_START);
		nvd.freeBlock = (uint8_t*)NVDATA_SECTOR_START;
	}

	memcpy(nvd.data, src, count);
	nvd.status = NVDATA_VALID;
	//write blocks
	NVDATA_SECTOR_WRITE(nvd.freeBlock, nvd.data, NVDATA_BLOCK_SIZE);

	nvd.freeBlock += NVDATA_BLOCK_SIZE;

	return count;
}
