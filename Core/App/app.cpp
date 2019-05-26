#include "main.h"
#include "iev.h"
#include "gui/main_screen/MainPresenter.hpp"
#include <stdio.h>

#define IDLE_PRIORITY_TASK tskIDLE_PRIORITY
#define LOW_PRIORITY_TASK (tskIDLE_PRIORITY + 2)
#define NORMAL_PRIORITY_TASK (tskIDLE_PRIORITY + 5)
#define HIGH_PRIORITY_TASK (tskIDLE_PRIORITY + 7)
#define STACK_MINIMUM configMINIMAL_STACK_SIZE      // 128 * 4
#define STACK_MEDIUM (configMINIMAL_STACK_SIZE * 8) // 128 * 8 * 4

#define SECONDS_MINUTE 60
#define ITERATIONS_SECOND (1000/UPDATE_RATE)
#define SECONDS_HOUR 3600

#define RPM_QUEUE_ITEM_SIZE sizeof(QuadrantData)

#define CAN_MSG_SIZE 10
#define CAN_MSG_01   0x601
#define CAN_MSG_02   0x602

void GRAPHICS_MainTask(void);
void GRAPHICS_Init(void);
void GRAPHICS_HW_Init(void);

static const char prompt[] = "iEV>";

QueueHandle_t qdataQueue;
SystemConfiguration_t qconfig;

/**
 * 
 * */
void CAN_MessageCallback(uint8_t *data){
BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    //if(header.FilterMatchIndex == CAN_RPM_MSG){
        // Should discart message after 100ms???
        if (xSemaphoreTakeFromISR(qconfig.mutex, &xHigherPriorityTaskWoken) == pdPASS){
            qconfig.data.rpm = *((uint32_t*)data);
            xSemaphoreGiveFromISR(qconfig.mutex, &xHigherPriorityTaskWoken);
        }
    //}
}

/**
 *  @brief High priority task for handling constant periodicity calculation
 * \param   *argument - not used
 * */
void updateTask(void *argument)
{
    TickType_t lastTime = xTaskGetTickCount();

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

    while (true)
    {
        /* Read rpm */
        if (xSemaphoreTake(qconfig.mutex, pdMS_TO_TICKS(UPDATE_RATE)) == pdPASS)
        {
            double wheelRpm = qconfig.data.rpm / qconfig.gearRacio;
            //get distance in respect to wheel revolutions in 100ms
            double distanceIteration = (wheelRpm / (SECONDS_MINUTE * ITERATIONS_SECOND)) * qconfig.wheelCircumference;
            // add to total distance
            uint32_t curDistance = (uint32_t)qconfig.totalDistance;
            qconfig.totalDistance += distanceIteration;

            // If integer part of distance or configuration has changed update screen 
            if ((uint32_t)qconfig.totalDistance != curDistance || qconfig.updated == true)
            {
                qconfig.updated = false;
                qconfig.data.speed = distanceIteration * (SECONDS_HOUR / UPDATE_RATE);
                qconfig.data.distance = (uint32_t)(qconfig.totalDistance/1000); // display in km                
                // send to display
                xQueueSend(qdataQueue, &qconfig.data, pdMS_TO_TICKS(UPDATE_RATE));
            }
            xSemaphoreGive(qconfig.mutex);
        }
        vTaskDelayUntil(&lastTime, pdMS_TO_TICKS(UPDATE_RATE));
    }
}


/**
 *  @brief Low priority task for toggling Alive LED
 * \param   *argument - not used
 * */
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


/**
 *  @brief normal priority task for display management
 * \param   *argument - not used
 * */
void graphicsTask(void *argument)
{
    /* Initialise the graphical hardware */
    GRAPHICS_HW_Init();
    /* Initialise the graphical stack engine */
    GRAPHICS_Init();
    /* Graphic application, never return */
    GRAPHICS_MainTask();
}


/**
 *  @brief normal priority task for processing CAN messages from serial port
 * \param   *argument - not used
 * */
#if 0
void serialTask(void *argument)
{   
    uint8_t msg[20], i; 
    uart.init();

    i = 0;
    while(1){
        msg[i++] = uart.xgetchar();
        if(i == CAN_MSG_SIZE){
            i = 0;
            if(msg[0] != CAN_MSG_01 >> 8)
                continue;
            if (xSemaphoreTake(qconfig.mutex, portMAX_DELAY) == pdPASS)
            {
                qconfig.data.rpm = (msg[2]<<8) | msg[3];
                qconfig.updated = TRUE;
                xSemaphoreGive(qconfig.mutex);
            }
        }
    }
}
#else
/**
 *  @brief normal priority task for system management commands
 * \param   *argument - not used
 * */
void serialTask(void *argument)
{    
    Console console;
    Help help;
    Rpm rpm;
    Config cfg;
    CmdMem mem;
    SDCard sd;
    CmdCan can;
    uint8_t msg[20], i = 0; 

    uart.init();
    console.init(&uart, prompt);

    console.addCommand(&help);
    console.addCommand(&rpm);
    console.addCommand(&cfg);
    console.addCommand(&mem);
    console.addCommand(&sd);
    console.addCommand(&can);
 
    console.print("\rCPU %uMHz\n%s", SystemCoreClock/1000000, prompt);

    for (;;)
    {
        switch(qconfig.mode){
            case Serial:
                BSP_LED_On(LED3);
                console.process();
                osDelay(100);
                break;

            case Can:
                BSP_LED_Off(LED3);
                msg[i++] = uart.xgetchar();
                if(i == CAN_MSG_SIZE){
                    i = 0;
                    if(msg[0] != CAN_MSG_01 >> 8)
                        break;
                    if (xSemaphoreTake(qconfig.mutex, portMAX_DELAY) == pdPASS)
                    {
                        qconfig.data.rpm = (msg[2]<<8) | msg[3];
                        qconfig.updated = TRUE;
                        xSemaphoreGive(qconfig.mutex);
                    }
                }
            default:
                break;
        }
    }
}
#endif

extern "C" void appMain(void)
{

    // TODO: load data from intflash or ext flash

    qconfig.gearRacio = 1;
    qconfig.wheelCircumference = 1.928f; //16" wheel
    qconfig.data.rpm = 518;
    qconfig.data.battery = 50;
    qconfig.mode = Can; //Serial;

    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    /* Create tasks */
    xTaskCreate(graphicsTask, "Graphics Task", STACK_MEDIUM, NULL, NORMAL_PRIORITY_TASK, NULL);
    xTaskCreate(ledAliveTask, "Alive Led Task", STACK_MINIMUM, NULL, IDLE_PRIORITY_TASK, NULL);
    xTaskCreate(serialTask, "Messages Task", STACK_MEDIUM, NULL, NORMAL_PRIORITY_TASK, NULL);
    xTaskCreate(updateTask, "Update Task", STACK_MEDIUM, NULL, HIGH_PRIORITY_TASK, NULL);

    CAN_Init(CAN_MessageCallback);    

    /* Start scheduler */
    osKernelStart();

    while (true)
    {
        osDelay(100);
    }
}
