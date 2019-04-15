#include "iev.h"


/*  
  b15-b8     b7 b3   b2-b1    b0
|   size  | -      | OP    | start | */

#define MEM_OP_SET_OPER(o, n) o = ((o & ~(3 << OPT_DONE)) | (n << OPT_DONE))
#define MEM_OP_SET_RANGE(_o, _n) _o |= (_n << 8)


void readMem(uint32_t address, uint8_t *dst, uint32_t count){
uint8_t *pdata = (uint8_t*)address;
    while(count--){
        *dst++ = *pdata++;
    }
}

void writeMem(uint32_t address, uint8_t *data, uint32_t count){
uint8_t *pdata = (uint8_t*)address;
    while(count--){
        *pdata++ = *data++;
    }
}

void CmdMem::dumpMem(uint32_t add, uint32_t cnt){
uint8_t *pdata = (uint8_t*)add;

    for(uint32_t i = 0; i < cnt; i++){
        if((i & 0x0F) == 0){
            console->xputc('\n');
            console->print("%08X: ", add + i);
        }
        console->print("%02X ", *(pdata++) );
    }
    console->xputc('\n');
}

void CmdMem::help(void){
    console->print("Usage: mem [option] \n\n");  
    console->print("\t r <address>, Read starting from address\n");
    console->print("\t w, <address> <data>\n");
    console->print("\t n, number of bytes\n\n");
}

char CmdMem::execute(void *ptr){
uint32_t operation, n = 1;
char *p1, c;
uint32_t address, data;

    MEM_OP_SET_RANGE(operation,n); // set default size

	p1 = (char*)ptr;

	 // check parameters
    if( p1 == NULL || *p1 == '\0'){
        help();
        return CMD_OK;
    }

    while( !(operation & OPT_DONE) ){
        c = nextChar(&p1);
        switch(c){
            case 'r':                
                MEM_OP_SET_OPER(operation, OPT_READ);
            case 'w':
                if(!nextHex(&p1, &address)){
                    console->print("Invalid address\n");
                    return CMD_BAD_PARAM;
                }
                if(OPT_OPER(operation) != OPT_READ){
                    MEM_OP_SET_OPER(operation, OPT_WRITE);
                    if(!nextHex(&p1, &data)){
                        console->print("Invalid data\n");
                          return CMD_BAD_PARAM;
                    }
                }
                break;          

            case 'n':
                if(!nextInt(&p1, (int32_t*)&n)){
                    console->print("Invalid size\n");
                    return CMD_BAD_PARAM;
                }
                MEM_OP_SET_RANGE(operation, n&255);                    
                break;

            case '\n':
            case '\r':    
            case '\0':
                OPT_SET_FLAG(operation, OPT_DONE);
        }
    }

    switch(OPT_OPER(operation)){
        case OPT_READ:
            dumpMem(address, n);
            break;
        case OPT_WRITE:
            //console->print("Not implemented\n");
            writeMem(address, (uint8_t*)&data, n);
            break;
    }
    
    

    return CMD_OK;
}