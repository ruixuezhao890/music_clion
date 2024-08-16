/**
*********************************************************************
*********
* @project_name :MusicClion
* @file : ApplicationManage.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2024/2/7 
*********************************************************************
*********
*/
//

#include "ApplicationManage.h"
#include "NewOperation.h"
#include "lvgl.h"
#include "ui.h"
ApplicationManage* appManage;
Application* curApp;
void appIconStartCallBack(lv_event_t * e);
void ApplicationManage::addApplication(const String &appName, Application *app) {
    if(appName==app->getName()){
    apps.emplace(appName, app);
    PageMange->page_manage_add_page(&app->getPageApplication(),app->getName(),LV_SCR_LOAD_ANIM_NONE,app->getFunction());
    app->setCloseBtnParent(app->getPageApplication().page);
    app->start();
    DEBUG_PRINTLN("Application '" + appName + "' installed.");
    }else{
        DEBUG_PRINTLN("Application '" + appName + "' install_error.");
    }
}

void ApplicationManage::removeApplication(const String &appName) {
    auto it = apps.find(appName);
    if (it != apps.end()) {
        delete it->second; // Delete the dynamically allocated object
        apps.erase(it);
        DEBUG_PRINTLN("Application '" + appName + "' uninstalled.");
    } else {
        DEBUG_PRINTLN("Application '" + appName + "' not found for uninstallation.");
    }
}

void ApplicationManage::applicationHandle() {
    for (auto& pair : apps) {
        Application* app = pair.second;

        switch (app->getState()) {
            case AppState::RESUME:
                app->closeBtn();
                PageMange->page_manage_switch_page(app->getName());
                app->resume();
                app->setState(AppState::RUNNING);
                break;
            case AppState::RUNNING:
                app->RUNNING();
                // Do something when application is running
                break;
            case AppState::PAUSED:
                if (app->isBgRunning()){
                    app->setState(AppState::BGRUNNING);
                }
                app->pause();
                PageMange->page_manage_switch_page(HOMEPAGE);
                drawApplication();
                app->setState(AppState::NOTINGTODO);
                // Do something when application is paused
                break;
            case AppState::BGRUNNING:
                app->backRUNNING();
                break;
            case AppState::DELETE:
                app->pause();
                app->Delete();
                removeApplication(app->getName());
                // Do something when application is stopped
                break;
            case AppState::NOTINGTODO:
                continue;
            default:
                DEBUG_PRINTLN("Unknown state for " + app->getName());
                break;
        }
    }
}

void ApplicationManage::startApplication(const String &appName) {
    auto app=apps[appName];
    curApp=app;
    app->setState(AppState::RESUME);
}

void ApplicationManage::closeApplication(const String &appName) {
    auto app=apps[appName];
    curApp= nullptr;
    app->setState(AppState::PAUSED);
}

void ApplicationManage::deleteApplication(const String &appName) {
    auto app=apps[appName];
    app->setState(AppState::DELETE);
}

ApplicationManage::ApplicationManage() {
    PageMange=new pageManage(&homePage,ui_Screen1_screen_init);
}

ApplicationManage::~ApplicationManage() {
    delete PageMange;
    PageMange= nullptr;
}

void ApplicationManage::drawApplication() {
    int16_t i=0;uint16_t j=0;lv_obj_t * btn;
    for (auto& pair : apps) {
        if (pair.second->getIcon()){
            //todo 图片按钮
        }else{
           btn= lv_btn_create(ui_Panel2);
        }
        lv_obj_set_size(btn,ICONWIDTH,ICONHIGHT);
        auto tempX=lv_coord_t((i*ICONWIDTH)+((i+1)*ICONINTERVALX));
        auto tempY=lv_coord_t(j*ICONHIGHT+(j+1)*ICONINTERVALY);
        lv_obj_set_pos(btn,tempX,tempY);
        lv_obj_t * label= lv_label_create(btn);
//        lv_obj_set_style_text_font(label,&myFont16,LV_STATE_DEFAULT);
        lv_label_set_long_mode(label,LV_LABEL_LONG_SCROLL);
        lv_label_set_text(label,pair.first.c_str());
        lv_obj_set_style_text_color(label,lv_color_black(),LV_STATE_DEFAULT);
        lv_obj_align_to(label,btn,LV_ALIGN_CENTER,0,0);
        lv_obj_add_event_cb(btn, appIconStartCallBack, LV_EVENT_CLICKED, (void *) pair.second);
        i++;if (i>=5){  i=0;j++;}
    }
}

const std::map<String, Application *> &ApplicationManage::getApps() const {
    return apps;
}

pageManage *ApplicationManage::getPageMange() const {
    return PageMange;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
void appIconStartCallBack(lv_event_t * e){
    auto EventCode= lv_event_get_code(e);
    if (EventCode==LV_EVENT_CLICKED) {
        auto * code= (Application *)lv_event_get_user_data(e);
       appManage->startApplication(code->getName());
    }
}






