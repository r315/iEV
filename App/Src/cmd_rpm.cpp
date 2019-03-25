#include "iev.h"


char Rpm::execute(void *ptr) {
	char *p1;
	QuadrantData qdata;
	p1 = (char*)ptr;
	
		// check parameters
	if( p1 == NULL || *p1 == '\0' || !nextInt(&p1, (int32_t*)&qdata.rpm)){
		help();
		return CMD_OK;
	}

	xQueueSend(qdataQueue, &qdata, portMAX_DELAY);

	return CMD_OK;
}	



