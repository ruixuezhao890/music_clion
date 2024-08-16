/**
*********************************************************************
*********
* @project_name :MusicClion
* @file : OSTask.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2024/2/4 
*********************************************************************
*********
*/
//
#include "ApplicationManage.h"
#include "MusicApp.h"
#include "hal.h"
#include "key.h"
#include "lcd.h"
#include "fonts.h"
#include "malloc.h"
#include "StatusList.h"
#include "Music.h"
#include "exfuns.h"
#include "OSTask.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "MyUsart.h"
#include "ui.h"
extern "C" void Music(){
    DEBUG_PRINTLN("MUSICTask");
    uint32_t flag;audiodev.playStatus=0; audiodev.status=PlayStatus_NULL;
    while (1){
       flag=osEventFlagsWait(MusicStauteHandle,EVENTBIT_ALL,osFlagsWaitAny,osWaitForever);
        if(flag&EVENTBIT_Next){
            Serial0<<"EVENTBIT_Next"<<endl;
            g_MusicPlayer.getManage()->audioVarRelease();
            g_MusicPlayer.deleteManage();
            g_MusicPlayer.setCurindex(g_MusicPlayer.getCurindex()+1);
            if (g_MusicPlayer.getCurindex()>=g_MusicPlayer.getTotwavnum()){
                g_MusicPlayer.setCurindex(0);
            }
            Serial<<"Curindex:"<<g_MusicPlayer.getCurindex()<<endl;
            audiodev.status=PlayStatus_Not;
        }
        if (flag&EVENTBIT_Play){
            Serial0<<"EVENTBIT_Play"<<endl;
            if (audiodev.playStatus){
                Music::audio_start();
                audiodev.status=PlayStatus_Playing;
            }else{
                Music::audio_stop();
                audiodev.status=PlayStatus_NULL;
            }
        }
        if (flag&EVENTBIT_Prev){
            Serial0<<"EVENTBIT_Prev"<<endl;
            g_MusicPlayer.getManage()->audioVarRelease();
            g_MusicPlayer.deleteManage();
            if (g_MusicPlayer.getCurindex()){
                g_MusicPlayer.setCurindex(g_MusicPlayer.getCurindex()-1);
            }else{
                g_MusicPlayer.setCurindex(g_MusicPlayer.getTotwavnum()-1);
            }
            audiodev.status=PlayStatus_Not;
        }
        if (flag&EVENTBIT_Forward){

        }
        if (audiodev.status==PlayStatus_Not){
            g_MusicPlayer.gainsPname();
            g_MusicPlayer.setCurType(f_typetell(g_MusicPlayer.getPname()));
            if (g_MusicPlayer.audioGreatType()){ continue;}
            audiodev.status=PlayStatus_TypeInit;
            Serial<<"great ok"<<endl;
        }
        if (audiodev.status==PlayStatus_TypeInit){
            if (g_MusicPlayer.getManage()->audioPlaySongInit(g_MusicPlayer.getPname())){
                Serial<<"InitFail"<<endl;
                audiodev.status=PlayStatus_Finish;
            }
            exflag=1;
            if (audiodev.playStatus){
                audiodev.status=PlayStatus_Playing;
                g_MusicPlayer.audio_start();
            }else{
                audiodev.status=PlayStatus_NULL;
                Music::audio_stop();
            }
        }
        if (audiodev.status==PlayStatus_Playing){
            uint8_t res=g_MusicPlayer.audioPlaySong();
            if (res){//出现错误
                audiodev.status=PlayStatus_Finish;
            }
        }
        if (audiodev.status==PlayStatus_Finish){
            Music::audio_stop();
//            g_MusicPlayer.setCurindex(g_MusicPlayer.getCurindex()+1);
            setMusicStatus(EVENTBIT_Next);
        }
    }
}
extern "C" void memoryRes(){
uint8_t ret=0;
    while (1){
//       ret=KEY_Scan(0);
//        if (ret==KEY1_PRES){
//            DEBUG_PRINTLN("Font Updata");
//            vTaskSuspendAll();
//            lcd_clear(WHITE);
//           auto res=fonts_update_font(20, 90, 16, (uint8_t *)"0:", RED);
//            if (res){
//                DEBUG_PRINTLN("updata error");
//            }else{
//                xTaskResumeAll();
//            }
//        }
        osDelay(50);
    }
}

extern "C" void APPManagerUpData(){
    HardwareManage.begin();

    DEBUG_PRINTLN("APPManagerUpData");

    ApplicationManage manage;appManage=&manage;
    manage.addApplication("MUSIC",new MusicApp("MUSIC", nullptr,ui_Music_page_init));
    manage.addApplication("MUSIC1",new MusicApp("MUSIC1", nullptr,ui_Music_page_init));
    manage.drawApplication();
    while (1){
        manage.applicationHandle();
        osDelay(5);
    }
}

uint8_t getNowVolume(){
    return hal::nowVolume;
}

