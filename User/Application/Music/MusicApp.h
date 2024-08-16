/**
*********************************************************************
*********
* @project_name :MusicClion
* @file : MusicApp.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2024/2/7 
*********************************************************************
*********
*/
//

#ifndef MUSICCLION_MUSICAPP_H
#define MUSICCLION_MUSICAPP_H
#include "Application.h"
#include "Music.h"
class MusicApp: public Application{
private:
    Music *musicPlayer;
public:
    MusicApp(String  appName,void * setIcon,InitFunction function);

    void start() override;

    void resume() override;

    void RUNNING() override;

    void backRUNNING() override;

    void pause() override;

    void Delete() override;

    ~MusicApp();
protected:
    String secondsToMinSec(uint16_t );
    void refreshWidgetInfoRealTime();
};


#endif //MUSICCLION_MUSICAPP_H
