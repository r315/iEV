#ifndef _commands_h_
#define _commands_h_

#ifdef __cplusplus
extern "C" {


#include "console.h"

/*  
  b15-b8     b7 b3   b2-b1    b0
|   size  | -      | OP    | start | */
#define OPT_READ  	1
#define OPT_WRITE  	2
#define OPT_OPER1  	1
#define OPT_OPER2  	2
#define OPT_OPER3	3
#define OPT_DONE (1 << 0)
#define OPT_SET_FLAG(_o, _f) _o |= _f
#define OPT_SET_OPER(o, n) o = ((o & ~(3 << OPT_DONE)) | (n << OPT_DONE))
#define OPT_OPER(x) ((x>>OPT_DONE) & 3)

class Help : public ConsoleCommand {
	Console *console;
public:
    Help() : ConsoleCommand("help") {}	
	void init(void *params) { console = static_cast<Console*>(params); }

	void help(void) {
		console->xputs("Available commands:\n");
		
		for (uint8_t i = 0; i < console->getCmdListSize(); i++) {			
				console->print("\t%s\n", console->getCmdIndexed(i)->getName());
		}
		console->xputchar('\n');
	}

	char execute(void *ptr) {
		help();
		return CMD_OK;
	}	
};



class Rpm : public ConsoleCommand {
	Console *console;
public:
    Rpm() : ConsoleCommand("rpm") {}	
	void init(void *params) { console = static_cast<Console*>(params); }

	void help(void) {
		console->xputs("usage rpm <0-20000>");
	}

	char execute(void *ptr);
};


class Config : public ConsoleCommand {
	Console *console;
public:
    Config() : ConsoleCommand("conf") {}	
	void init(void *params) { console = static_cast<Console*>(params); }

	void help(void);

	char execute(void *ptr);
};

class CmdMem : public ConsoleCommand{
    Console *console;
public:
    CmdMem () : ConsoleCommand("mem") {}
    void init(void *params) { console = static_cast<Console*>(params); }
    char execute(void *ptr);
    void help(void);
    void dumpMem(uint32_t add, uint32_t cnt);
};

class SDCard : public ConsoleCommand {
	Console *console;
public:
    SDCard() : ConsoleCommand("sd") {}	
	void init(void *params) { console = static_cast<Console*>(params); }
	void help(void);
	char execute(void *ptr);
	void fatFs_Init(void);	
};


class CmdCan : public ConsoleCommand{
    Console *console;
public:
    CmdCan () : ConsoleCommand("can") {}
    void init(void *params) { console = static_cast<Console*>(params); }
    char execute(void *ptr);
    void help(void);
};

}
#endif

#endif