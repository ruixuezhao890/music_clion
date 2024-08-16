/**
*********************************************************************
*********
* @project_name :MusicClion
* @file : ApplicationManage.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2024/2/7 
*********************************************************************
*********
*/
//

#ifndef MUSICCLION_APPLICATIONMANAGE_H
#define MUSICCLION_APPLICATIONMANAGE_H
#include "MyUsart.h"
#include "pageManage.h"
#include "Application.h"
#include <map>
#define ICONWIDTH 60
#define ICONHIGHT 60
#define ICONINTERVALY 15
#define ICONINTERVALX 15
class ApplicationManage final {
private:
    std::map<String, Application*> apps; // Change std::string to String

private:
    pageManage * PageMange;
    PAGE homePage={};
public:

    ApplicationManage();

    ~ApplicationManage();

    void addApplication(const String& appName, Application* app) ;

    void drawApplication();

    void removeApplication(const String& appName); // Change std::string to String

    void startApplication(const String & appName);

    void closeApplication(const String & appName);

    void deleteApplication(const String & appName);

    void applicationHandle();
    pageManage *getPageMange() const;
    const std::map<String, Application *> &getApps() const;
};
extern ApplicationManage* appManage;
extern Application* curApp;
#endif //MUSICCLION_APPLICATIONMANAGE_H
