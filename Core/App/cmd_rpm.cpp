#include "iev.h"

char Rpm::execute(void *ptr)
{
	char *p1;
	p1 = (char *)ptr;
	uint32_t rpm, operation = 0;
	// check parameters
	if (p1 == NULL || *p1 == '\0' || !nextInt(&p1, (int32_t *)&rpm))
	{
		help();
		//console.print("rpm:%u", qdata.)
	}
	else
	{

		if (xSemaphoreTake(cfgData.mutex, portMAX_DELAY) == pdPASS)
		{
			invData.rpm = rpm;
			cfgData.updated = TRUE;
			xSemaphoreGive(cfgData.mutex);
		}
	}

	return CMD_OK;
}
