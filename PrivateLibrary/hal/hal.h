/**
*********************************************************************
*********
* @project_name :MusicClion
* @file : hal.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2024/1/27 
*********************************************************************
*********
*/
//

#ifndef MUSICCLION_HAL_H
#define MUSICCLION_HAL_H



#include "sys.h"

class hal {
public:
   static uint8_t nowVolume;
    hal();
    void begin();
    void setMW8978SPKvol(uint8_t  setVal);
};

extern hal HardwareManage;

#endif //MUSICCLION_HAL_H
