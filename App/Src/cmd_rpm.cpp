#include "console.h"

class Rpm : public ConsoleCommand {
	Console *console;
public:
    Rpm() : ConsoleCommand("rpm") {}	
	void init(void *params) { console = static_cast<Console*>(params); }

	void help(void) {
		console->puts("Sets RPM:\n\n");
	}

	char execute(void *ptr) {
		return CMD_OK;
	}	
};


