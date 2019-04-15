#include "iev.h"


static const double PI = 3.14159265358979323846f;    ///< PI

void Config::help(void) {
		console->puts("usage: conf <option>\n");
		console->puts("racio <float> - Rotation racio between wheel and motor");
		console->puts("wheel <float> - Wheel diameter in meters\n");
}

char Config::execute(void *ptr) {
char *p1 = (char*)ptr;
uint32_t operation = 0;
double value;
	
	
	// check parameters
	if( p1 == NULL || *p1 == '\0'){
		help();
        console->print("racio: %f\n", qconfig.gearRacio);		
		console->print("wheel: %.8fm\n\n", qconfig.wheelCircumference);		
	}else{

		while( !(operation & OPT_DONE) ){
			if(isNextWord(&p1, "racio")){
				if(nextDouble(&p1, &value)){
					 qconfig.gearRacio = value;
					 OPT_SET_FLAG(operation, OPT_DONE);
				}				
			}

			if(isNextWord(&p1, "wheel")){
				if(nextDouble(&p1, &value)){
					 qconfig.wheelCircumference = value * PI;
					 OPT_SET_FLAG(operation, OPT_DONE);
					 OPT_SET_OPER(operation, OPT_READ);
				}				
			}			

		}

		if(xSemaphoreTake(qconfig.mutex, portMAX_DELAY) == pdPASS){
			if(OPT_OPER(operation) == OPT_READ){
				qconfig.gearRacio = value;
			}else{
				qconfig.rpm = value;
			}
			qconfig.updated= TRUE;
			xSemaphoreGive(qconfig.mutex);
		}   
    }
	return CMD_OK;
}