#ifndef __WAVPLAY_H
#define __WAVPLAY_H
#include "sys.h"
#include "Music.h"
#include "AudioFileParser.h"
#define WAV_I2S_TX_DMA_BUFSIZE    8192		//����WAV TX DMA �����С(����192Kbps@24bit��ʱ��,��Ҫ����8192��Ų��Ῠ)
 
//RIFF��
typedef __packed struct
{
    u32 ChunkID;		   	//chunk id;����̶�Ϊ"RIFF",��0X46464952
    u32 ChunkSize ;		   	//���ϴ�С;�ļ��ܴ�С-8
    u32 Format;	   			//��ʽ;WAVE,��0X45564157
}ChunkRIFF ;
//fmt��
typedef __packed struct
{
    u32 ChunkID;		   	//chunk id;����̶�Ϊ"fmt ",��0X20746D66
    u32 ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size);����Ϊ:20.
    u16 AudioFormat;	  	//��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM
	u16 NumOfChannels;		//ͨ������;1,��ʾ������;2,��ʾ˫����;
	u32 SampleRate;			//������;0X1F40,��ʾ8Khz
	u32 ByteRate;			//�ֽ�����; 
	u16 BlockAlign;			//�����(�ֽ�); 
	u16 BitsPerSample;		//�����������ݴ�С;4λADPCM,����Ϊ4
//	u16 ByteExtraData;		//���ӵ������ֽ�;2��; ����PCM,û���������
}ChunkFMT;	   
//fact�� 
typedef __packed struct 
{
    u32 ChunkID;		   	//chunk id;����̶�Ϊ"fact",��0X74636166;
    u32 ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size);����Ϊ:4.
    u32 NumOfSamples;	  	//����������; 
}ChunkFACT;
//LIST�� 
typedef __packed struct 
{
    u32 ChunkID;		   	//chunk id;����̶�Ϊ"LIST",��0X74636166;
    u32 ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size);����Ϊ:4. 
}ChunkLIST;

//data�� 
typedef __packed struct 
{
    u32 ChunkID;		   	//chunk id;����̶�Ϊ"data",��0X5453494C
    u32 ChunkSize ;		   	//�Ӽ��ϴ�С(������ID��Size) 
}ChunkDATA;

//wavͷ
typedef __packed struct
{ 
	ChunkRIFF riff;	//riff��
	ChunkFMT fmt;  	//fmt��
//	ChunkFACT fact;	//fact�� ����PCM,û������ṹ��	 
	ChunkDATA data;	//data��		 
}__WaveHeader; 

//wav ���ſ��ƽṹ��
//typedef __packed struct
//{
//    u16 audioformat;			//��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM
//	u16 nchannels;				//ͨ������;1,��ʾ������;2,��ʾ˫����;
//	u16 blockalign;				//�����(�ֽ�);
//	u32 datasize;				//WAV���ݴ�С
//
//    u32 totsec ;				//���׸�ʱ��,��λ:��
//    u32 cursec ;				//��ǰ����ʱ��
//
//    u32 bitrate;	   			//������(λ��)
//	u32 samplerate;				//������
//	u16 bps;					//λ��,����16bit,24bit,32bit
//
//	u32 datastart;				//����֡��ʼ��λ��(���ļ������ƫ��)
//}__wavctrl;

class wavPlay:public AudioFileParser {
protected:

    uint16_t audioformat;			//��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM
    uint16_t nchannels;				//ͨ������;1,��ʾ������;2,��ʾ˫����;
    uint16_t blockalign;				//�����(�ֽ�);
    uint16_t bps;					//λ��,����16bit,24bit,32bit
    uint32_t fillnum;
    uint32_t datasize;				//WAV���ݴ�С
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
















