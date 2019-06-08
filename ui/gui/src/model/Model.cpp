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
    invData_t invData = {};

    if(xQueueReceive(invDataQueue, &invData, 0) == pdPASS){
        // update ui
        modelListener->notifyRpmChange(invData.rpm);
        modelListener->notifyDistanceChange(invData.distance);
        modelListener->notifySpeedChange(invData.speed);
        modelListener->notifyBatteryChange(invData.battery);
        modelListener->notifyMotorTempChange(invData.motorTemp);
        modelListener->notifyControllerTempChange(invData.controllerTemp);
        modelListener->notifyMotorCurrentChange(invData.motorCurrent);
    }
#endif  
}

void Model::userSetSerialMode(uint8_t mode){
 #if !defined(SIMULATOR) 
    cfgData.mode = (mode == 1) ? Serial : Can;
#endif
}

