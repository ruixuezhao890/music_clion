/**
*********************************************************************
*********
* @project_name :MusicClion
* @file : StatusList.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2024/2/4 
*********************************************************************
*********
*/
//
#include "StatusList.h"
#include "MyUsart.h"
uint8_t exflag=0;
uint32_t setMusicStatus(const uint32_t set){
    return osEventFlagsSet(MusicStauteHandle,set);
}
osStatus_t WaitSemaphore(){
    osStatus_t status = osSemaphoreAcquire(infoSyncSemaphore, 30);
    return status;
}
void ReleaseSemaphore(){
    osStatus_t status = osSemaphoreRelease(infoSyncSemaphore);
    if (status != osOK) {
        DEBUG_PRINTLN("Release error");
    }
}


