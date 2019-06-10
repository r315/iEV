#include <stdio.h>

#include "main.h"
#include "iev.h"
#include "gui/main_screen/MainPresenter.hpp"


#define IDLE_PRIORITY_TASK tskIDLE_PRIORITY
#define LOW_PRIORITY_TASK (tskIDLE_PRIORITY + 2)
#define NORMAL_PRIORITY_TASK (tskIDLE_PRIORITY + 5)
#define HIGH_PRIORITY_TASK (tskIDLE_PRIORITY + 7)
#define STACK_MINIMUM configMINIMAL_STACK_SIZE      // 128 * 4
#define STACK_MEDIUM (configMINIMAL_STACK_SIZE * 8) // 128 * 8 * 4

#define SECONDS_MINUTE 60
#define ITERATIONS_SECOND (1000/UPDATE_RATE)
#define SECONDS_HOUR 3600

#define DISP_QUEUE_ITEM_SIZE sizeof(SystemData_t)

#define CAN_MSG_SIZE 10
#define CAN_MSG_01   0x601
#define CAN_MSG_02   0x602

void GRAPHICS_MainTask(void);
void GRAPHICS_Init(void);
void GRAPHICS_HW_Init(void);

static const char prompt[] = "iEV>";

QueueHandle_t invDataQueue;
SystemData_t cfgData;

/**
 * Call back for processing received can messages
 * */
void CAN_MessageCallback(uint8_t *data){
BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    //if(header.FilterMatchIndex == CAN_RPM_MSG){
        // Should discart message after 100ms???
        if (xSemaphoreTakeFromISR(cfgData.mutex, &xHigherPriorityTaskWoken) == pdPASS){
            cfgData.invData.rpm = *((uint32_t*)data);
            xSemaphoreGiveFromISR(cfgData.mutex, &xHigherPriorityTaskWoken);
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

    invDataQueue = xQueueCreate(DISP_QUEUE_LENGTH, DISP_QUEUE_ITEM_SIZE);

    if (invDataQueue == NULL)
    {
        //console.log("Fail to create qdataQueue\n");
        return;
    }
    //console.log("qdataQueue created\n");

    cfgData.mutex = xSemaphoreCreateBinary();

    if (cfgData.mutex == NULL)
    {
        return;
    }
    
    xSemaphoreGive(cfgData.mutex);

    // Note that rpm_Ts may not be constant, so the exact 
    // rmp_Ts should be given on TM_xx functions calls
    TM_Init(RPM_TS, RPM_REF, SPEED_REF, &cfgData.tm);

    cfgData.mode = Can; //Serial;
    cfgData.speed = 0;
    cfgData.invData.rpm = 0;
    cfgData.invData.battery = 20;
    cfgData.invData.motorCurrent = 0;
    cfgData.invData.motorTemp = 20;
    cfgData.invData.controllerTemp = 24;    

    while (true)
    {
        /* Start by acquiring exclusive access */
        if (xSemaphoreTake(cfgData.mutex, pdMS_TO_TICKS(UPDATE_RATE)) == pdPASS)
        {
            #if 0
            double wheelRpm = cfgData.invData.rpm / cfgData.gearRacio;
            //get distance in respect to wheel revolutions in 100ms
            double distanceIteration = (wheelRpm / (SECONDS_MINUTE * ITERATIONS_SECOND)) * cfgData.wheelCircumference;
            
            cfgData.speed = distanceIteration * (SECONDS_HOUR / UPDATE_RATE);            

            distanceIteration /= 1000;

            if ((uint32_t)cfgData.distance != (uint32_t)(cfgData.distance + distanceIteration)){
                cfgData.updated = true;
            }

            cfgData.distance += distanceIteration;

            // If integer part of distance or configuration has changed update screen 
            if (cfgData.updated == true)
            {                
                // send to display
                xQueueSend(invDataQueue, &cfgData, pdMS_TO_TICKS(UPDATE_RATE));
                cfgData.updated = false;
            }
            
            #else
            // perform distance and speed calculation
            // TODO: calculate rpm_Ts
            double elapsedDistance = TM_ComputeDistance(cfgData.invData.rpm, RPM_TS, cfgData.tm);            

            if((int32_t)cfgData.distance != (uint32_t)(cfgData.distance + elapsedDistance)){
                cfgData.updated = true;
            }
            // add to total distance
            cfgData.distance += elapsedDistance;            
            // check if an display update must be performed
            if(cfgData.updated == true){
                cfgData.speed = TM_EstimateSpeed(elapsedDistance, RPM_TS, cfgData.tm);
                xQueueSend(invDataQueue, &cfgData, pdMS_TO_TICKS(UPDATE_RATE)); 
                cfgData.updated = false;
            }
            #endif
            xSemaphoreGive(cfgData.mutex);
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
            if (xSemaphoreTake(cfgData.mutex, portMAX_DELAY) == pdPASS)
            {
                cfgData.invData.rpm = (msg[2]<<8) | msg[3];
                cfgData.updated = TRUE;
                xSemaphoreGive(cfgData.mutex);
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
        switch(cfgData.mode){
            case Serial:
                BSP_LED_On(LED3);
                console.process();
                osDelay(100);
                break;

            case Can:
                //BSP_LED_Off(LED3);
                msg[i++] = uart.xgetchar();
                if(i == CAN_MSG_SIZE){
                    i = 0;
                    uint16_t id = (msg[0] << 8) | msg[1];
                    switch(id){
                        case CAN_MSG_01:
                            if (xSemaphoreTake(cfgData.mutex, pdMS_TO_TICKS(UPDATE_RATE)) == pdPASS)
                            {
                                cfgData.invData.rpm = (msg[2]<<8) | msg[3];
                                cfgData.invData.motorTemp = msg[4];
                                cfgData.invData.controllerTemp = msg[5];
                                cfgData.invData.motorCurrent = (msg[6]<<8) | msg[7];
                                cfgData.updated = true;
                                xSemaphoreGive(cfgData.mutex);
                            }
                            break;

                        case CAN_MSG_02:
                            break;

                    }                    
                }
                break;

            default:
                break;
        }
    }
}
#endif

extern "C" void appMain(void)
{

    // TODO: load data from intflash or ext flash

    cfgData.gearRacio = 5;
    cfgData.wheelCircumference = 1.928f; //16" wheel    
   

    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);

    /* Create tasks */
    xTaskCreate(graphicsTask, "Graphics Task", STACK_MEDIUM, NULL, NORMAL_PRIORITY_TASK, NULL);
    xTaskCreate(ledAliveTask, "Alive Led Task", STACK_MINIMUM, NULL, IDLE_PRIORITY_TASK, NULL);
    xTaskCreate(serialTask, "Serial Task", STACK_MEDIUM, NULL, NORMAL_PRIORITY_TASK, NULL);
    xTaskCreate(updateTask, "Update Task", STACK_MEDIUM, NULL, HIGH_PRIORITY_TASK, NULL);

    CAN_Init(CAN_MessageCallback);    

    /* Start scheduler */
    osKernelStart();

    while (true)
    {
        osDelay(100);
    }
}
