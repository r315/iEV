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

		if (xSemaphoreTake(qconfig.mutex, portMAX_DELAY) == pdPASS)
		{
			qconfig.data.rpm = rpm;
			qconfig.updated = TRUE;
			xSemaphoreGive(qconfig.mutex);
		}
	}

	return CMD_OK;
}
