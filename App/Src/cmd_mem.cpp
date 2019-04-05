#include "cmd_mem.h"


/*  
  b15-b8     b7 b3   b2-b1    b0
|   size  | -      | OP    | start | */
#define MEM_OP_READ  1
#define MEM_OP_WRITE  2
#define MEM_OP_START (1 << 0)
#define MEM_OP_SET_FLAG(_o, _f) _o |= _f
#define MEM_OP_SET_OPER(o, n) o = ((o & ~(3 << MEM_OP_START)) | (n << MEM_OP_START))
#define MEM_OP_SET_RANGE(_o, _n) _o |= (_n << 8)
#define MEM_OPER(x) ((x>>MEM_OP_START) & 3)

enum {
    READY = 0,
    ERROR,
    READ,
    WRITE,
    NOP
};

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
    console->print("\t n, number of bytes\n");
}

char CmdMem::execute(void *ptr){
uint32_t operation, n = 1;
char *p1, c;
uint32_t address;

    MEM_OP_SET_RANGE(operation,n); // set default size

	p1 = (char*)ptr;

	 // check parameters
    if( p1 == NULL || *p1 == '\0'){
        help();
        return CMD_OK;
    }

    while( !(operation & MEM_OP_START) ){
        c = nextChar(&p1);
        switch(c){
            case 'r':                
                MEM_OP_SET_OPER(operation, MEM_OP_READ);
            case 'w':
                if(!nextHex(&p1, &address)){
                    console->print("Invalid address\n");
                    return CMD_BAD_PARAM;
                }
                if(MEM_OPER(operation) != MEM_OP_READ){
                    MEM_OP_SET_OPER(operation, MEM_OP_WRITE);
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
                MEM_OP_SET_FLAG(operation, MEM_OP_START);
        }
    }

    switch(MEM_OPER(operation)){
        case MEM_OP_READ:
            dumpMem(address, n);
            break;
        case MEM_OP_WRITE:
            console->print("Not implemented\n");
            break;
    }
    
    

    return CMD_OK;
}