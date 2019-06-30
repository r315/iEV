#include <stdio.h>

#include "main.h"
#include "iev.h"
#include "gui/main_screen/MainPresenter.hpp"
#include "nvdata.h"


#define IDLE_PRIORITY_TASK tskIDLE_PRIORITY
#define LOW_PRIORITY_TASK (tskIDLE_PRIORITY + 2)
#define NORMAL_PRIORITY_TASK (tskIDLE_PRIORITY + 5)
#define HIGH_PRIORITY_TASK (tskIDLE_PRIORITY + 7)
#define STACK_MINIMUM configMINIMAL_STACK_SIZE      // 128 * 4
#define STACK_MEDIUM (configMINIMAL_STACK_SIZE * 8) // 128 * 8 * 4

#define SECONDS_MINUTE      60
#define ITERATIONS_SECOND   (1000/UPDATE_RATE)
#define SECONDS_HOUR        3600

#define DISP_QUEUE_ITEM_SIZE sizeof(SystemData_t)

void GRAPHICS_MainTask(void);
void GRAPHICS_Init(void);
void GRAPHICS_HW_Init(void);

static const char prompt[] = "iEV>";

QueueHandle_t dispData;
SystemData_t cfgData;
invData_t invData;

/**
 * Call back for processing received can messages
 * */
void CAN_MessageCallback(uint8_t *data){
BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    //if(header.FilterMatchIndex == CAN_RPM_MSG){
        // Should discart message after 100ms???
        if (xSemaphoreTakeFromISR(cfgData.mutex, &xHigherPriorityTaskWoken) == pdPASS){
            invData.rpm = *((uint32_t*)data);
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

    dispData = xQueueCreate(DISP_QUEUE_LENGTH, DISP_QUEUE_ITEM_SIZE);

    if (dispData == NULL)
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
    invData.rpm = 0;
    invData.battery = 20;
    invData.motorCurrent = 0;
    invData.motorTemp = 20;
    invData.controllerTemp = 24;    

    TM_ComputeDistance(invData.rpm, RPM_TS, cfgData.tm);

    cfgData.invData = &invData;
    cfgData.updated = true;

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
                xQueueSend(dispData, &cfgData, pdMS_TO_TICKS(UPDATE_RATE));
                cfgData.updated = false;
            }
            
            #else
            // perform distance and speed calculation
            double elapsedDistance = TM_ComputeDistance(invData.rpm, RPM_TS, cfgData.tm);            
            int partialDistance =  (int)((cfgData.distance + elapsedDistance) * 10);

            if(partialDistance != (int)(cfgData.distance * 10)){ // check if the 0.1 dial has changed
                cfgData.updated = true;
                cfgData.distance += elapsedDistance;      
                NVData_Save(&cfgData.distance, sizeof(cfgData.distance));
            }else{
                // add to total distance
                cfgData.distance += elapsedDistance;            
            }
            // check if an display update must be performed
            if(cfgData.updated == true){
                cfgData.speed = TM_EstimateSpeed(elapsedDistance, RPM_TS, cfgData.tm);
                cfgData.batteryVoltage = invData.battery * BATTERY_VOLTAGE_SCALE;
                xQueueSend(dispData, &cfgData, pdMS_TO_TICKS(UPDATE_RATE)); 
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
        BSP_LED_Toggle(LED2);
        vTaskDelayUntil(&lastTime, pdMS_TO_TICKS(LED_TIME));
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
                BSP_LED_Off(LED3);
                msg[i++] = uart.xgetchar();
                if(i == CAN_MSG_SIZE){
                    i = 0;
                    uint16_t id = (msg[0] << 8) | msg[1];
                    switch(id){
                        case CAN_MSG_01:
                            if (xSemaphoreTake(cfgData.mutex, pdMS_TO_TICKS(UPDATE_RATE)) == pdPASS)
                            {
                                invData.rpm = (msg[2]<<8) | msg[3];
                                invData.motorTemp = msg[4];
                                invData.controllerTemp = msg[5];
                                invData.motorCurrent = (msg[6]<<8) | msg[7];
                                invData.battery = (msg[8]<<8) | msg[9];
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

extern "C" void APP_Start(void)
{
    if(NVData_Init()){
        NVData_Restore(&cfgData.distance, sizeof(cfgData.distance));
    }
    else{
    cfgData.gearRacio = 5;
    cfgData.wheelCircumference = 1.928f; //16" wheel    
        cfgData.distance = 0;
    }   

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
