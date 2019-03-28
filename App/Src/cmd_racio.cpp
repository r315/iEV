#include "iev.h"


char Racio::execute(void *ptr) {
	char *p1;
	int32_t val;
	p1 = (char*)ptr;
	
		// check parameters
	if( p1 == NULL || *p1 == '\0' || !nextInt(&p1, &val)){
		help();
        console->print("racio:%u\n", qstate.gearRacio);		
	}else{

    qstate.gearRacio = val;
    }
	return CMD_OK;
}