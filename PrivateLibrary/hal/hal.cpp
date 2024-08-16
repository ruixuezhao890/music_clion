/**
*********************************************************************
*********
* @project_name :MusicClion
* @file : hal.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2024/1/27 
*********************************************************************
*********
*/
//

#include "hal.h"
#include "key.h"
#include "beep.h"
#include "exfuns.h"
#include "fonts.h"
#include "lcd.h"
#include "lvgl.h"
#include "lv_port_indev.h"
#include "lv_port_disp.h"
#include "wm8978.h"
#include "Music.h"
#include "MyUsart.h"
#include "norflash.h"
hal HardwareManage;
uint8_t hal::nowVolume;
hal::hal() {
    nowVolume=50;
}

void hal::begin() {
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
//    DEBUG_PRINTLN("lvgl OK");
//    norflash_init();
//    ftinfo.lvgl_FontRomanaddr=15821617;
    WM8978_Init();
    WM8978_HPvol_Set(40,40);		//耳机音量设置
    WM8978_SPKvol_Set(nowVolume);			//喇叭音量设置
    BEEP_Init();
    KEY_Init();

//    DEBUG_PRINTLN("OK");

}

void hal::setMW8978SPKvol(uint8_t setVal) {
    if (setVal>=63){
        setVal=63;
    }
    if (setVal<=0){
        setVal=1;
    }
    nowVolume=setVal;
    WM8978_SPKvol_Set(setVal);
}
