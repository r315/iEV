#ifndef _console_h_
#define _console_h_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdarg.h>

#include "serialout.h"
#include "console_command.h"

#define NO        0
#define YES       1

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

#define CONSOLE_MAX_COMMANDS 64
#define COMMAND_MAX_LEN		 64
#define HISTORY_SIZE		 5


	class Console {

		ConsoleCommand *cmdList[CONSOLE_MAX_COMMANDS];
		uint8_t cmdListSize;
		uint8_t executing;
		char line[COMMAND_MAX_LEN];
		uint8_t line_len;
		const char *prompt;
		SerialOut *out;
		
		char history[HISTORY_SIZE][COMMAND_MAX_LEN];
		void historyDump(void);
		void historyAdd(char *entry);
		char *historyBack(void);
		char *historyForward(void);
		char *historyGet(void);
		void historyClear(void);
		void changeLine(char *new_line);
		uint8_t hist_idx;
		uint8_t hist_cur;
		uint8_t hist_size;

	public:
		Console(void);
		Console(SerialOut *sp, const char *prt);

		void init(SerialOut *sp, const char *prt);

		char getLine(char *line, uint8_t max);
		char getLineNonBlocking(char *line, uint8_t *cur_len, uint8_t max);
		void process(void);

		void addCommand(ConsoleCommand *cmd);
		char parseCommand(char *line);
		char executeCommand(void *ptr);
		void xputc(char c);
		void puts(const char* str);
		char xgetchar(void);
		void gets(char* str);
		char getline(char *line, uint8_t max);
		void print(const char* str, ...);
		uint8_t kbhit(void);

		void log(const char* str, ...);

		uint8_t getCmdListSize(void) { return cmdListSize; }
		ConsoleCommand *getCmdIndexed(uint8_t idx) { return cmdList[idx]; } // security issues??
	};

#ifdef __cplusplus
}
#endif

#endif