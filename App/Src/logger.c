
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#include "logger.h"

typedef struct _Log{
    const char *tag;
    const char *file;
    int line;
    const char *fmt;
}Log;

#define LOGGER_SIZE 5
#define LOGGER_ITEM_SIZE sizeof(const char*)

QueueHandle_t loggerQueue;

void logger_init(void){
    loggerQueue = xQueueCreate(LOGGER_SIZE, LOGGER_ITEM_SIZE);

}


void log_print(const char *tag, const char *file, int line, const char *fmt, ...){

    
}