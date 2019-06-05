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
    QuadrantData qdata = {};

    if(xQueueReceive(qdataQueue, &qdata, 0) == pdPASS){
        // update ui
        modelListener->notifyRpmChange(qdata.rpm);
        modelListener->notifyDistanceChange(qdata.distance);
        modelListener->notifySpeedChange(qdata.speed);
        modelListener->notifyBatteryChange(qdata.battery);
    }
#endif  
}

void Model::userSetSerialMode(uint8_t mode){
 #if !defined(SIMULATOR) 
    qconfig.mode = (mode == 1) ? Serial : Can;
#endif
}

