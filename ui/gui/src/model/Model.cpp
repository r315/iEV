#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "iev.h"

#define RPM_QUEUE_LENGTH 1
#define RPM_QUEUE_ITEM_SIZE sizeof(QuadrantData)

QueueHandle_t rpmQueue;



Model::Model() : modelListener(0)
{
    rpmQueue = xQueueCreate( RPM_QUEUE_LENGTH, RPM_QUEUE_ITEM_SIZE );
    if(rpmQueue == NULL){
        console.log("Fail to create rpmQueue\n");
    }
}

void Model::tick()
{

}
