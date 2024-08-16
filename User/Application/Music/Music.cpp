/**
*********************************************************************
*********
* @project_name :MusicClion
* @file : Music.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2024/1/27 
*********************************************************************
*********
*/
//

#include "Music.h"
#include "wm8978.h"
#include "exfuns.h"
#include "i2S2.h"
#include "key.h"
#include "NewOperation.h"
#include "AudioFileParser.h"
#include "StatusList.h"
#include "MyUsart.h"
__audiodev audiodev;
uint8_t ret;
Music g_MusicPlayer(PATH);
Music::Music() {

}

Music::Music(String path) {
    manage= nullptr;
    _path=path;
    totwavnum=0;
}

void Music::begin() {
    uint8_t res;
    uint32_t temp;
    WM8978_ADDA_Cfg(1,0);	//开启DAC
    WM8978_Input_Cfg(0,0,0);//关闭输入通道
    WM8978_Output_Cfg(1,0);	//开启DAC输出
    res=f_opendir(&wavdir,_path.c_str());
    if (res!=FR_OK)//打开音乐文件夹
    {
        PromptMessage="Failed to open folder:0:/MUSIC:";
        PromptMessage+=res;
        return;
    }
    totwavnum= getAllMusciNum(_path); //得到总有效文件数
    Serial0<<"allNUm:"<<totwavnum<<endl;
    if(totwavnum== 0)//音乐文件总数为0
    {
        PromptMessage="No music files/Num==0";
        return;
    }
    wavfileinfo=(FILINFO*)malloc(sizeof(FILINFO));	//申请内存
    pname=(uint8_t *)malloc(_MAX_LFN*2+1);//为带路径的文件名分配内存
    memset(pname,0,_MAX_LFN*2+1);
    wavoffsettbl=(uint32_t *)malloc(4*totwavnum);				//申请4*totwavnum个字节的内存,用于存放音乐文件off block索引
    if (!wavfileinfo||!pname||!wavoffsettbl)//内存分配出错
    {
        PromptMessage="Memory allocation failed";
        return;
    }
    res=f_opendir(&wavdir,_path.c_str()); //打开目录
    if(res==FR_OK)
    {
        curindex=0;//当前索引为0
        while(1)//全部查询一遍
        {
             temp=wavdir.dptr;								//记录当前index
            res=f_readdir(&wavdir,wavfileinfo);       		//读取目录下的一个文件
            if(res!=FR_OK||wavfileinfo->fname[0]==0)break;	//错误了/到末尾了,退出
            res=f_typetell((uint8_t *)wavfileinfo->fname);
            if((res&0XF0)==0X40||(res&0XF0)==0X41)//取高四位,看看是不是音乐文件
            {
                wavoffsettbl[curindex]=temp;//记录索引
                curindex++;
            }
        }
    }else{
        PromptMessage="Failed to open folder:0:/MUSIC";
        PromptMessage+=res;
        return;
    }
    res= f_closedir(&wavdir);
    if (res!=FR_OK){PromptMessage="close failed";
                    PromptMessage+=res;
            return;
    }
    curindex=0;audiodev.status=PlayStatus_Not;
    PromptMessage="READ OK";

}

uint16_t Music::getAllMusciNum(String path) {
    u8 res;
    u16 rval=0;
    DIR tdir;	 		//临时目录
    FILINFO* tfileinfo;	//临时文件信息
    tfileinfo=(FILINFO*)malloc(sizeof(FILINFO));//申请内存
    res=f_opendir(&tdir,(const TCHAR*)path.c_str()); //打开目录
    if(res==FR_OK&&tfileinfo)
    {
        while(1)//查询总的有效文件数
        {
            res=f_readdir(&tdir,tfileinfo);       			//读取目录下的一个文件
            if(res!=FR_OK||tfileinfo->fname[0]==0)break;	//错误了/到末尾了,退出
            res=f_typetell((u8*)tfileinfo->fname);
            if((res&0XF0)==0X40)//取高四位,看看是不是音乐文件
            {
                rval++;//有效文件数增加1
            }
        }
    }
    free(tfileinfo);//释放内存
    return rval;
}

u16 Music::getTotwavnum() const {
    return totwavnum;
}

const String &Music::getPromptMessage() const {
    return PromptMessage;
}

Music::~Music() {
    delete manage;
    free(wavfileinfo);
    free(pname);
    free(wavoffsettbl);
    pname= nullptr;
    wavfileinfo= nullptr;
    wavoffsettbl= nullptr;
}

void Music::audioPlay() {
//    g_MusicPlayer.gainsPname();
//    g_MusicPlayer.audioPlaySong();
//    g_MusicPlayer.audioControl();
}

void Music::gainsPname() {
    Serial<<"curindex"<<curindex<<endl;
    uint8_t res= f_opendir(&wavdir,_path.c_str());
    if (res==FR_OK){
        dir_sdi(&wavdir,wavoffsettbl[curindex]);
        res=f_readdir(&wavdir,wavfileinfo);       				//读取目录下的一个文件
        if(res!=FR_OK||wavfileinfo->fname[0]==0)return;			//错误了/到末尾了,退出
        strcpy((char*)pname,"0:/MUSIC/");						//复制路径(目录)
        strcat((char*)pname,(const char*)wavfileinfo->fname);	//将文件名接在后面
    }
     res= f_closedir(&wavdir);
    if (res!=FR_OK){
        PromptMessage="close fail";
    }
//    Serial<<"gainsPname";Serial.println("%s",pname);
}

uint8_t Music::audioPlaySong() {

    return manage->audioPlaySong(pname);

}

void Music::audioControl() {

    if(ret==KEY2_PRES)		//上一曲
    {
        if(curindex)curindex--;
        else curindex=totwavnum-1;
    }else if(ret==KEY0_PRES)//下一曲
    {
        curindex++;
        if(curindex>=totwavnum)curindex=0;//到末尾的时候,自动从头开始
    }else {
        PromptMessage="KEY error";
        Serial<<PromptMessage;
    };	//产生了错误
}

u8 *Music::getPname() const {
    return pname;
}

void Music::audio_start(void) {
//    audiodev.status=3<<0;//开始播放+非暂停
    I2S_Play_Start();
}

void Music::audio_stop(void) {
//    audiodev.status=0;
    I2S_Play_Stop();
}

u16 Music::getCurindex() const {
    return curindex;
}

u16 Music::getCurType() const {
    return curType;
}

void Music::setCurType(u16 curType) {
    Music::curType = curType;
}

AudioFileParser *Music::getManage() const {
    return manage;
}

uint8_t Music::audioGreatType() {
    Serial0.println(curType);
    switch (curType) {
        case T_WAV:{

            manage=new wavPlay;
            break;
        }
        case T_MP3:{

            manage=new mp3Play;
            break;
        }
        default://其他文件,自动跳转到下一曲
        {
            PromptMessage = "can't play:";
            PromptMessage += (*pname);
            setMusicStatus(EVENTBIT_Next);
            return 1;
        }
    }
    return 0;
}

void Music::setCurindex(u16 curindex) {
    Music::curindex = curindex;
}

void Music::deleteManage() {

    delete manage;
    manage= nullptr;

}

void Music::setManage(AudioFileParser *manage) {
    Music::manage = manage;
}

