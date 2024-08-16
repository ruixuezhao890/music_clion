/**
*********************************************************************
*********
* @project_name :MusicClion
* @file : Application.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2024/2/7 
*********************************************************************
*********
*/
//

#include "Application.h"
#include "ApplicationManage.h"
//#include <utility>
void appIconStopCallBack(lv_event_t *e);
Application::Application(String appName): name(std::move(appName)), state(AppState::NOTINGTODO),bgRUNNING(false) {
   applicationManage=appManage;closeBtnParent= nullptr;
}
Application::Application(String appName, void *setIcon,InitFunction function): Application(appName){
    icon=setIcon;this->function=function;
}
const String &Application::getName() const {
    return name;
}

void Application::setName(const String &name) {
    Application::name = name;
}

AppState Application::getState() const {
    return state;
}

void Application::setState(AppState state) {
    Application::state = state;
}

bool Application::isBgRunning() const {
    return bgRUNNING;
}

void Application::setBgRunning(bool bgRunning) {
    bgRUNNING = bgRunning;
}

void *Application::getIcon() const {
    return icon;
}

void Application::setIcon(void *icon) {
    Application::icon = icon;
}

PAGE &Application::getPageApplication()  {
    return pageApplication;
}

InitFunction Application::getFunction() const {
    return function;
}

lv_obj_t *Application::getCloseBtnParent() const {
    return closeBtnParent;
}

void Application::setCloseBtnParent(lv_obj_t *closeBtnParent) {
    Application::closeBtnParent = closeBtnParent;
}

void Application::closeBtn() {
    DEBUG_PRINTLN("close btn init");
    if (closeBtnParent) {
        lv_obj_t *deleteBtn = lv_btn_create(closeBtnParent);
        lv_obj_set_pos(deleteBtn, 0, 0);
        lv_obj_move_foreground(deleteBtn);
        lv_obj_t *deleteLabel = lv_label_create(deleteBtn);
        lv_obj_set_style_text_color(deleteLabel, lv_color_black(), LV_STATE_DEFAULT);
        lv_label_set_text(deleteLabel, "close");
        lv_obj_align_to(deleteLabel, deleteBtn, LV_ALIGN_CENTER, 0, 0);
        lv_obj_add_event_cb(deleteBtn, appIconStopCallBack, LV_EVENT_CLICKED, (void *) this);
    }
}

void appIconStopCallBack(lv_event_t *e){
    auto EventCode= lv_event_get_code(e);
    if (EventCode==LV_EVENT_CLICKED) {
        auto *obj = (Application *) lv_event_get_user_data(e);
        obj->setState(AppState::PAUSED);
    }
}


