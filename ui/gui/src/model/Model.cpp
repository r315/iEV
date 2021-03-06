#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "iev.h"

#define FRAMES_SECOND 60
#define SECONDS_MINUTE 60


Model::Model() : modelListener(0)
{
}

// Called every frame (15.66ms)
void Model::tick()
{   
#if !defined(SIMULATOR) 
    SystemData_t cfgData = {};

    if(xQueueReceive(dispData, &cfgData, 0) == pdPASS){
        // update ui
        modelListener->notifySpeedChange(cfgData.speed);
        modelListener->notifyRpmChange(cfgData.invData->rpm);
        modelListener->notifyDistanceChange(cfgData.distance);
        modelListener->notifyBatteryLevelChange(cfgData.invData->battery);
        modelListener->notifyBatteryVoltageChange(cfgData.batteryVoltage);
        modelListener->notifyMotorTempChange(cfgData.invData->motorTemp);
        modelListener->notifyControllerTempChange(cfgData.invData->controllerTemp);
        modelListener->notifyMotorCurrentChange(cfgData.invData->motorCurrent);
    }
#endif  
}

void Model::userSetSerialMode(uint8_t mode){
#if !defined(SIMULATOR) 
    cfgData.mode = (mode == 1) ? Serial : Can;
#endif
}

