/**
*********************************************************************
*********
* @project_name :MusicClion
* @file : MusicApp.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2024/2/7 
*********************************************************************
*********
*/
//
#include "ui.h"
#include "hal.h"
#include "key.h"
#include "MusicApp.h"
#include "StatusList.h"
#include "cmsis_os2.h"
static void ChangeImg();
void MusicApp::start() {
    pageApplication.ScreenOrientation=1;//设置竖屏
    musicPlayer->begin();
}

void MusicApp::resume() {
    audiodev.playStatus=0;
    setMusicStatus(EVENTBIT_DMAFinish);//触发一次读取歌曲最开始的数据

    DEBUG_PRINTLN("resume");
}

void MusicApp::RUNNING() {
    if (audiodev.playStatus==PlayStatus_Playing||audiodev.playStatus==PlayStatus_TypeInit){
        refreshWidgetInfoRealTime();
    }
    uint8_t key=KEY_Scan(0);
    if (key==KEY0_PRES){
        Serial<<"press1"<<endl;
            setMusicStatus(EVENTBIT_Next);
    }
    if (key==KEY2_PRES){
        Serial<<"press2"<<endl;
            setMusicStatus(EVENTBIT_Prev);
    }
    if (key==WKUP_PRES){
        Serial<<"press3"<<endl;
            audiodev.playStatus=!audiodev.playStatus;
            setMusicStatus(EVENTBIT_Play);
    }
    ChangeImg();
}

void MusicApp::backRUNNING() {
    DEBUG_PRINTLN("backRUNNING");
}

void MusicApp::pause() {
    audiodev.playStatus=0;
    setMusicStatus(EVENTBIT_Play);
    if (!bgRUNNING){
        delete musicPlayer->getManage();
        musicPlayer->setManage(nullptr);
        audiodev.status=PlayStatus_Not;
    }
    DEBUG_PRINTLN("pause");
}

void MusicApp::Delete() {
    DEBUG_PRINTLN("Delete");
}

MusicApp::MusicApp(String  appName,void * setIcon,InitFunction function): Application(appName,setIcon,function) {
    musicPlayer=&g_MusicPlayer;
}

MusicApp::~MusicApp() {

}

void MusicApp::refreshWidgetInfoRealTime() {
        if (exflag) {
            exflag=0;
            lv_slider_set_value(ui_VolumeSlider, hal::nowVolume, LV_ANIM_ON);//音量条
            lv_slider_set_range(ui_Playback_progressSlider,0,(uint16_t )musicPlayer->getManage()->getTotsec());
            auto res = secondsToMinSec(musicPlayer->getManage()->getTotsec());
            lv_label_set_text(ui_AllTimeLabel, res.c_str());
            lv_label_set_text(ui_SongTitle, (char *) musicPlayer->getManage()->getTitle());
            lv_label_set_text(ui_SongWrite, (char *) musicPlayer->getManage()->getArtist());
            DEBUG_PRINTLN("info:");
            DEBUG_PRINTLN((char *) musicPlayer->getManage()->getTitle());
            DEBUG_PRINTLN((char *) musicPlayer->getManage()->getArtist());
        }
       auto res=secondsToMinSec(musicPlayer->getManage()->getCursec());
        lv_label_set_text(ui_CurTimeLabel1, res.c_str());
        uint16_t get=(uint16_t)musicPlayer->getManage()->getCursec();
        lv_slider_set_value(ui_Playback_progressSlider,get,LV_ANIM_ON);
    }


String MusicApp::secondsToMinSec(uint16_t total_seconds) {
    String temp;
    int minutes = total_seconds / 60;
    int seconds = total_seconds % 60;
    return temp+minutes+':'+seconds;
}

//void MusicApp::closeBtn(lv_obj_t *parent) {
//
//}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
                                            //FUNCTION//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ChangePauseButton(lv_event_t *e ){
    lv_obj_t * target = lv_event_get_target(e);
    audiodev.playStatus=!audiodev.playStatus;
    setMusicStatus(EVENTBIT_Play);

}
void ui_event_PausBtn( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);
    if ( event_code == LV_EVENT_CLICKED) {
        ChangePauseButton( e );
    }
}
void ui_event_PreBtn( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
    if ( event_code == LV_EVENT_CLICKED) {
        audiodev.playStatus=1;
        setMusicStatus(EVENTBIT_Prev);
        setMusicStatus(EVENTBIT_Play);
    }
}
void ui_event_NextBtn( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
    if ( event_code == LV_EVENT_CLICKED) {
        audiodev.playStatus=1;
        setMusicStatus(EVENTBIT_Next);
        setMusicStatus(EVENTBIT_Play);
    }
}
void ui_event_ModeLabel( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
    if ( event_code == LV_EVENT_CLICKED) {
//        ChangeMode( e );
    }
}
void ui_event_VoiceButton( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
    if ( event_code == LV_EVENT_CLICKED) {
        _ui_flag_modify( ui_VolumeSlider, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_TOGGLE);
    }
}

void ui_event_VolumeSlider( lv_event_t * e){
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
    if(event_code==LV_EVENT_VALUE_CHANGED){
       uint8_t ret=lv_slider_get_value(target);
        HardwareManage.setMW8978SPKvol(ret);
    }
}
static void ChangeImg(){
    if(audiodev.playStatus){
        lv_obj_set_style_bg_img_src(ui_PausBtn,&ui_img_930994846, LV_PART_MAIN);
    }else{

        lv_obj_set_style_bg_img_src(ui_PausBtn,&ui_img_264944503, LV_PART_MAIN);
    }
}