#ifndef __WAVPLAY_H
#define __WAVPLAY_H
#include "sys.h"
#include "Music.h"
#include "AudioFileParser.h"
#define WAV_I2S_TX_DMA_BUFSIZE    8192		//定义WAV TX DMA 数组大小(播放192Kbps@24bit的时候,需要设置8192大才不会卡)
 
//RIFF块
typedef __packed struct
{
    u32 ChunkID;		   	//chunk id;这里固定为"RIFF",即0X46464952
    u32 ChunkSize ;		   	//集合大小;文件总大小-8
    u32 Format;	   			//格式;WAVE,即0X45564157
}ChunkRIFF ;
//fmt块
typedef __packed struct
{
    u32 ChunkID;		   	//chunk id;这里固定为"fmt ",即0X20746D66
    u32 ChunkSize ;		   	//子集合大小(不包括ID和Size);这里为:20.
    u16 AudioFormat;	  	//音频格式;0X01,表示线性PCM;0X11表示IMA ADPCM
	u16 NumOfChannels;		//通道数量;1,表示单声道;2,表示双声道;
	u32 SampleRate;			//采样率;0X1F40,表示8Khz
	u32 ByteRate;			//字节速率; 
	u16 BlockAlign;			//块对齐(字节); 
	u16 BitsPerSample;		//单个采样数据大小;4位ADPCM,设置为4
//	u16 ByteExtraData;		//附加的数据字节;2个; 线性PCM,没有这个参数
}ChunkFMT;	   
//fact块 
typedef __packed struct 
{
    u32 ChunkID;		   	//chunk id;这里固定为"fact",即0X74636166;
    u32 ChunkSize ;		   	//子集合大小(不包括ID和Size);这里为:4.
    u32 NumOfSamples;	  	//采样的数量; 
}ChunkFACT;
//LIST块 
typedef __packed struct 
{
    u32 ChunkID;		   	//chunk id;这里固定为"LIST",即0X74636166;
    u32 ChunkSize ;		   	//子集合大小(不包括ID和Size);这里为:4. 
}ChunkLIST;

//data块 
typedef __packed struct 
{
    u32 ChunkID;		   	//chunk id;这里固定为"data",即0X5453494C
    u32 ChunkSize ;		   	//子集合大小(不包括ID和Size) 
}ChunkDATA;

//wav头
typedef __packed struct
{ 
	ChunkRIFF riff;	//riff块
	ChunkFMT fmt;  	//fmt块
//	ChunkFACT fact;	//fact块 线性PCM,没有这个结构体	 
	ChunkDATA data;	//data块		 
}__WaveHeader; 

//wav 播放控制结构体
//typedef __packed struct
//{
//    u16 audioformat;			//音频格式;0X01,表示线性PCM;0X11表示IMA ADPCM
//	u16 nchannels;				//通道数量;1,表示单声道;2,表示双声道;
//	u16 blockalign;				//块对齐(字节);
//	u32 datasize;				//WAV数据大小
//
//    u32 totsec ;				//整首歌时长,单位:秒
//    u32 cursec ;				//当前播放时长
//
//    u32 bitrate;	   			//比特率(位速)
//	u32 samplerate;				//采样率
//	u16 bps;					//位数,比如16bit,24bit,32bit
//
//	u32 datastart;				//数据帧开始的位置(在文件里面的偏移)
//}__wavctrl;

class wavPlay:public AudioFileParser {
protected:

    uint16_t audioformat;			//音频格式;0X01,表示线性PCM;0X11表示IMA ADPCM
    uint16_t nchannels;				//通道数量;1,表示单声道;2,表示双声道;
    uint16_t blockalign;				//块对齐(字节);
    uint16_t bps;					//位数,比如16bit,24bit,32bit
    uint32_t fillnum;
    uint32_t datasize;				//WAV数据大小
    FIL  tempFil;
    uint8_t i2s1[WAV_I2S_TX_DMA_BUFSIZE]={0};
    uint8_t i2s2[WAV_I2S_TX_DMA_BUFSIZE]={0};
    uint8_t tbuf[WAV_I2S_TX_DMA_BUFSIZE]={0};

protected:

    void audioGetName(uint8_t *);

    uint8_t audioAnalysis(uint8_t *FileName) override;

    uint32_t audioFillBuffer(uint8_t *buf, uint16_t size, uint8_t bits) override;

    void audioGetCurtime(FIL*fx) override;
public:
    wavPlay();

    uint8_t audioPlaySong(uint8_t *FileName) override;

    uint8_t audioPlaySongInit(uint8_t *FileName) override;

    void audioVarRelease() override;

    ~wavPlay() override;
};
#endif
















