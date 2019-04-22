#include "iev.h"

void CmdCan::help(void){
    console->print("Usage: can [option] \n\n");    
    console->print("\t send, send test\n\n");
}


char CmdCan::execute(void *ptr)
{
	char *p1;
	p1 = (char *)ptr;
	uint32_t rpm, operation = 0;
	// check parameters
	if (p1 == NULL || *p1 == '\0' || !nextInt(&p1, (int32_t *)&rpm)){
		help();
		//console.print("rpm:%u", qdata.)
	}
	
    while( !(operation & OPT_DONE) ){
        if(isNextWord(&p1, "send")){
            CAN_SendTestMessage();				 
            OPT_SET_OPER(operation, OPT_OPER1);
            OPT_SET_FLAG(operation, OPT_DONE);								
        }

        if (*p1 == '\0'){
            OPT_SET_FLAG(operation, OPT_DONE);
        }else
        p1 = nextWord(p1);
    }

    switch(OPT_OPER(operation)){
            case OPT_OPER1:
                
                break;
            case OPT_OPER2:			
                
                break;
            case OPT_OPER3:
                
                break;
    }

	return CMD_OK;
}
