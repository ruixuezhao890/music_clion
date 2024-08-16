/**
*********************************************************************
*********
* @project_name :MusicClion
* @file : Application.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2024/2/7 
*********************************************************************
*********
*/
//

#ifndef MUSICCLION_APPLICATION_H
#define MUSICCLION_APPLICATION_H
#include "MyUsart.h"
#include "pageManage.h"
// Enum representing the states of an application
class ApplicationManage;
enum class AppState {
    RESUME,
    RUNNING,
    BGRUNNING,
    PAUSED,
    DELETE,
    NOTINGTODO
};
class Application {
protected:
    ApplicationManage* applicationManage;
    PAGE pageApplication;
    lv_obj_t *closeBtnParent;
    InitFunction function;
    bool bgRUNNING;
    String name;
    void * icon;
    AppState state;

public:
    Application();

    Application(String  appName);

    void *getIcon() const;

    void setIcon(void *icon);

    Application(String  appName,void * setIcon,InitFunction function);

    //如果更改父类必须在start函数中对closeBtnParent进行更改
    virtual void closeBtn();

    virtual void start()=0;

    virtual void resume()=0;

    virtual void RUNNING()=0;

    virtual void backRUNNING()=0;

    virtual void pause()=0;

    virtual void Delete()=0;

    bool isBgRunning() const;

    void setBgRunning(bool bgRunning);

    const String &getName() const;

    void setName(const String &name);

    AppState getState() const;

    void setState(AppState state);

    PAGE &getPageApplication() ;

    InitFunction getFunction() const;

    lv_obj_t *getCloseBtnParent() const;

    void setCloseBtnParent(lv_obj_t *closeBtnParent);

};


#endif //MUSICCLION_APPLICATION_H
