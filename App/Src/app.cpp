#include "main.h"
#include "cmsis_os.h"
#include "iev.h"
#include "gui/main_screen/MainPresenter.hpp"
#include <stdio.h>

#define IDLE_PRIORITY_TASK tskIDLE_PRIORITY
#define LOW_PRIORITY_TASK (tskIDLE_PRIORITY + 2)
#define NORMAL_PRIORITY_TASK (tskIDLE_PRIORITY + 5)
#define HIGH_PRIORITY_TASK (tskIDLE_PRIORITY + 7)
#define STACK_MINIMUM configMINIMAL_STACK_SIZE      // 128 * 4
#define STACK_MEDIUM (configMINIMAL_STACK_SIZE * 8) // 128 * 8 * 4

#define UPDATE_RATE 100

#define RPM_QUEUE_LENGTH 2
#define RPM_QUEUE_ITEM_SIZE sizeof(QuadrantData)

void GRAPHICS_MainTask(void);
void GRAPHICS_Init(void);
void GRAPHICS_HW_Init(void);

static const char prompt[] = "iEV>";

QueueHandle_t qdataQueue;
QuadrantConfiguration qconfig;

void updateTask(void *argument)
{
    TickType_t lastTime = xTaskGetTickCount();
    QuadrantData qdata;

    qdataQueue = xQueueCreate(RPM_QUEUE_LENGTH, RPM_QUEUE_ITEM_SIZE);
    if (qdataQueue == NULL)
    {
        //console.log("Fail to create qdataQueue\n");
        return;
    }
    //console.log("qdataQueue created\n");

    qconfig.mutex = xSemaphoreCreateBinary();

    if (qconfig.mutex == NULL)
    {
        return;
    }
    
    xSemaphoreGive(qconfig.mutex);    

    while (1)
    {
        /* Read rpm */
        if (xSemaphoreTake(qconfig.mutex, pdMS_TO_TICKS(UPDATE_RATE)) == pdPASS)
        {
            uint32_t curDistance = (uint32_t)qconfig.distance;
            // increment distance counter by wheel revolutions per frame
            qconfig.distance += (((double)qconfig.rpm / qconfig.gearRacio) / 10) * qconfig.wheelCircumference;

            // If integer part is changed update total distance counter
            if ((uint32_t)qconfig.distance != curDistance || qconfig.updated == TRUE)
            {
                qconfig.updated = FALSE;
                qdata.rpm = qconfig.rpm;
                qdata.speed = qconfig.rpm / qconfig.gearRacio;
                qdata.distance = (uint32_t)qconfig.distance;
                xQueueSend(qdataQueue, &qdata, pdMS_TO_TICKS(UPDATE_RATE));
            }
            xSemaphoreGive(qconfig.mutex);
        }
        vTaskDelayUntil(&lastTime, pdMS_TO_TICKS(UPDATE_RATE));
    }
}

void ledAliveTask(void *argument)
{
    TickType_t lastTime = xTaskGetTickCount();

    for (;;)
    {
        BSP_LED_Toggle(LED_GREEN);
        vTaskDelayUntil(&lastTime, pdMS_TO_TICKS(400));
        //vTaskDelay(pdMS_TO_TICKS(400));
    }
}

void graphicsTask(void *argument)
{
    /* Initialise the graphical hardware */
    GRAPHICS_HW_Init();
    /* Initialise the graphical stack engine */
    GRAPHICS_Init();
    /* Graphic application, never return */
    GRAPHICS_MainTask();

    /* Infinite loop */
    for (;;)
    {
        osDelay(100);
    }
}

void consoleTask(void *argument)
{
    Console console;
    Help help;
    Rpm rpm;
    Config cfg;
    CmdMem mem;
    SDCard sd;

    uart.init();
    console.init(&uart, prompt);

    console.addCommand(&help);
    console.addCommand(&rpm);
    console.addCommand(&cfg);
    console.addCommand(&mem);
    console.addCommand(&sd);

    for (;;)
    {
        console.process();
        osDelay(100);
    }
}

extern "C" void appMain(void)
{

    // TODO: load data from intflash or ext flash

    qconfig.gearRacio = 1;
    qconfig.wheelCircumference = 1.928f; //16" wheel

    /* Create tasks */
    xTaskCreate(graphicsTask, "Graphics Task", STACK_MEDIUM, NULL, NORMAL_PRIORITY_TASK, NULL);
    xTaskCreate(ledAliveTask, "Alive Led Task", STACK_MINIMUM, NULL, IDLE_PRIORITY_TASK, NULL);
    xTaskCreate(consoleTask, "Console Task", STACK_MEDIUM, NULL, NORMAL_PRIORITY_TASK, NULL);
    xTaskCreate(updateTask, "Update Task", STACK_MEDIUM, NULL, HIGH_PRIORITY_TASK, NULL);

    /* Start scheduler */
    osKernelStart();

    while (1)
    {
        osDelay(100);
    }
}
