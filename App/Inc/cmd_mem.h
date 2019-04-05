#ifndef _cmd_mem_h_
#define _cmd_mem_h_


#ifdef __cplusplus
extern "C" {
#endif

#include "console.h"

class CmdMem : public ConsoleCommand{
    Console *console;
public:
    CmdMem () : ConsoleCommand("mem") {}
    void init(void *params) { console = static_cast<Console*>(params); }
    char execute(void *ptr);
    void help(void);
    void dumpMem(uint32_t add, uint32_t cnt);
};


#ifdef __cplusplus
}
#endif

#endif