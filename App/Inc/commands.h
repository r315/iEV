#ifndef _commands_h_
#define _commands_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "console.h"

class Help : public ConsoleCommand {
	Console *console;
public:
    Help() : ConsoleCommand("help") {}	
	void init(void *params) { console = static_cast<Console*>(params); }

	void help(void) {
		console->puts("Available commands:\n\n");
		
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
		console->puts("usage rpm <0-20000>\n\n");
	}

	char execute(void *ptr);
};


#ifdef __cplusplus
}
#endif

#endif