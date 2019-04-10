#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "iev.h"

#define RPM_QUEUE_LENGTH 2
#define RPM_QUEUE_ITEM_SIZE sizeof(QuadrantData)

QuadrantData qdata;

#define FRAMES_SECOND 60

Model::Model() : modelListener(0)
{
    qdataQueue = xQueueCreate( RPM_QUEUE_LENGTH, RPM_QUEUE_ITEM_SIZE );
    if(qdataQueue == NULL){
        //console.log("Fail to create qdataQueue\n");
    }else{
        //console.log("qdataQueue created\n");
    }
    tickCount = 0;
    distance = (double)qstate.distance;
}

// Called every 15.66ms
void Model::tick()
{
    if(xQueueReceive(qdataQueue, &qdata, 0) == pdPASS){
        // update ui
        modelListener->notifyRpmChange(qdata.rpm);
    }

    
    distance += (double)qdata.rpm / qstate.gearRacio;
    modelListener->notifyDistanceChange(distance);
    modelListener->notifySpeedChange(qdata.rpm / qstate.gearRacio);
}
