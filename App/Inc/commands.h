#ifndef _commands_h_
#define _commands_h_

#ifdef __cplusplus
extern "C" {


#include "console.h"
#include "cmd_mem.h"

/*  
  b15-b8     b7 b3   b2-b1    b0
|   size  | -      | OP    | start | */
#define OPT_READ  1
#define OPT_WRITE  2
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
		console->puts("Available commands:\n");
		
		for (uint8_t i = 0; i < console->getCmdListSize(); i++) {			
				console->print("\t%s\n", console->getCmdIndexed(i)->getName());
		}
		console->xputc('\n');
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
		console->puts("usage rpm <0-20000>");
	}

	char execute(void *ptr);
};


class Racio : public ConsoleCommand {
	Console *console;
public:
    Racio() : ConsoleCommand("racio") {}	
	void init(void *params) { console = static_cast<Console*>(params); }

	void help(void) {
		console->puts("usage racio <0-20000>");
	}

	char execute(void *ptr);
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


}
#endif

#endif