#include "iev.h"


static const double PI = 3.14159265358979323846f;    ///< PI

void Config::help(void) {
		console->xputs("usage: conf <option>\n");
		console->xputs("racio <float> - Rotation racio between wheel and motor");
		console->xputs("bat <0-100> - Battery level");
		console->xputs("wheel <float> - Wheel diameter in meters\n");

}

char Config::execute(void *ptr) {
char *p1 = (char*)ptr;
uint32_t operation = 0, bval;
double value;
	
	
	// check parameters
	if( p1 == NULL || *p1 == '\0'){
		help();
        console->print("racio: %.2f\n", qconfig.gearRacio);		
		console->print("wheel: %.8fm\n\n", qconfig.wheelCircumference);		
	}else{
		while( !(operation & OPT_DONE) ){
			if(isNextWord(&p1, "racio")){
				if(nextDouble(&p1, &value)){					 
					 OPT_SET_OPER(operation, OPT_READ);
					 OPT_SET_FLAG(operation, OPT_DONE);
				}				
			}

			if(isNextWord(&p1, "wheel")){
				if(nextDouble(&p1, &value)){
					OPT_SET_OPER(operation, OPT_WRITE);					 
					OPT_SET_FLAG(operation, OPT_DONE);
				}				
			}

			if(isNextWord(&p1, "bat")){
				if(nextInt(&p1, (int32_t*)&bval)){
					OPT_SET_OPER(operation, OPT_OPER3);					 
					OPT_SET_FLAG(operation, OPT_DONE);
				}				
			}

			if (*p1 == '\0'){
				OPT_SET_FLAG(operation, OPT_DONE);
			}else
			p1 = nextWord(p1);
		}

	
		if(xSemaphoreTake(qconfig.mutex, portMAX_DELAY) == pdPASS){
			switch(OPT_OPER(operation)){
				case OPT_READ:
					qconfig.gearRacio = value;
					break;
				case OPT_WRITE:			
					qconfig.wheelCircumference = value * PI;
					break;
				case OPT_OPER3:
					qconfig.data.battery = bval;
					break;
			}
			qconfig.updated= TRUE;
			xSemaphoreGive(qconfig.mutex);
		}		
    }
	return CMD_OK;
}