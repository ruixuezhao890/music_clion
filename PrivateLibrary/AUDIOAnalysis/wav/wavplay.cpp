#include "wavplay.h"

#include "Music.h"
#include "MyUsart.h"
#include "delay.h" 
#include "malloc.h"
#include "ff.h"
#include "i2S2.h"
#include "wm8978.h"
#include "key.h"
#include "StatusList.h"
//1,֧��16λ/24λWAV�ļ�����
//2,��߿���֧�ֵ�192K/24bit��WAV��ʽ.
 
//__wavctrl wavctrl;		//WAV���ƽṹ��
vu8 wavtransferend=0;	//i2s������ɱ�־
vu8 wavwitchbuf=0;		//i2sbufxָʾ��־
u8 Playing_Finish_Flag=0;
void audio_msg_show(wavPlay * wavplay){
    Serial.println("title:%s",wavplay->getTitle());
    Serial.println("cursec:%d",wavplay->getCursec());
    Serial.println("totsec:%d",wavplay->getTotsec());
    Serial.println("samplerate:%d",wavplay->getSamplerate());

}
//WAV����ʱ,I2S DMA����ص�����
void wav_i2s_dma_tx_callback(void)
{
//    BaseType_t Result,xHigherPriorityTaskWoken;
//    if(__HAL_DMA_GET_FLAG(&I2S2_TXDMA_Handler,DMA_FLAG_TCIF0_4)!=RESET) //DMA�������
//    {
//        __HAL_DMA_CLEAR_FLAG(&I2S2_TXDMA_Handler, DMA_FLAG_TCIF0_4);
        u16 i;
        if (DMA1_Stream4->CR & (1 << 19)) {
            wavwitchbuf = 0;
#if 0
            if ((audiodev.status & 0X01) == 0) {
                for (i = 0; i < WAV_I2S_TX_DMA_BUFSIZE; i++)//��ͣ
                {
                    audiodev.i2sbuf1[i] = 0;//���0
                }
            }
#endif
        } else {
            wavwitchbuf = 1;
#if 0
            if ((audiodev.status & 0X01) == 0) {
                for (i = 0; i < WAV_I2S_TX_DMA_BUFSIZE; i++)//��ͣ
                {
                    audiodev.i2sbuf2[i] = 0;//���0
                }
            }

#endif
        }
        wavtransferend = 1;

}
////�õ���ǰ����ʱ��
////fx:�ļ�ָ��
////wavx:wav���ſ�����
//void wav_get_curtime(FIL*fx,__wavctrl *wavx)
//{
//	long long fpos;
// 	wavx->totsec=wavx->datasize/(wavx->bitrate/8);	//�����ܳ���(��λ:��)
//	fpos=fx->fptr-wavx->datastart; 					//�õ���ǰ�ļ����ŵ��ĵط�
//	wavx->cursec=fpos*wavx->totsec/wavx->datasize;	//��ǰ���ŵ��ڶ�������?
//}

uint8_t wavPlay::audioAnalysis(uint8_t *FileName) {

    FIL*ftemp;
	u8 *buf;
    UINT br=0;
	u8 res=0;

	ChunkRIFF *riff;
	ChunkFMT *fmt;
	ChunkFACT *fact;
	ChunkDATA *data;
	ftemp=(FIL*)malloc(sizeof(FIL));
	buf=(uint8_t *)malloc(512);
	if(ftemp&&buf)	//�ڴ�����ɹ�
	{
		res=f_open(ftemp,(TCHAR*)FileName,FA_READ);//���ļ�
		if(res==FR_OK)
		{
			f_read(ftemp,buf,(UINT)512,&br);	//��ȡ512�ֽ�������
			riff=(ChunkRIFF *)buf;		//��ȡRIFF��
			if(riff->Format==0X45564157)//��WAV�ļ�
			{
				fmt=(ChunkFMT *)(buf+12);	//��ȡFMT��
				fact=(ChunkFACT *)(buf+12+8+fmt->ChunkSize);//��ȡFACT��
				if(fact->ChunkID==0X74636166||fact->ChunkID==0X5453494C)datastart=12+8+fmt->ChunkSize+8+fact->ChunkSize;//����fact/LIST���ʱ��(δ����)
				else datastart=12+8+fmt->ChunkSize;
				data=(ChunkDATA *)(buf+datastart);	//��ȡDATA��
				if(data->ChunkID==0X61746164)//�����ɹ�!
				{
					audioformat=fmt->AudioFormat;		//��Ƶ��ʽ
					nchannels=fmt->NumOfChannels;		//ͨ����
					samplerate=fmt->SampleRate;		//������
					bitrate=fmt->ByteRate*8;			//�õ�λ��
					blockalign=fmt->BlockAlign;		//�����
					bps=fmt->BitsPerSample;			//λ��,16/24/32λ

					datasize=data->ChunkSize;			//���ݿ��С
					datastart=datastart+8;		//��������ʼ�ĵط�.
                    totsec=datasize/(bitrate/8);
					printf("wavx->audioformat:%d\r\n",audioformat);
					printf("wavx->nchannels:%d\r\n",nchannels);
					printf("wavx->samplerate:%d\r\n",samplerate);
					printf("wavx->bitrate:%d\r\n",bitrate);
					printf("wavx->blockalign:%d\r\n",blockalign);
					printf("wavx->bps:%d\r\n",bps);
					printf("wavx->datasize:%d\r\n",datasize);
					printf("wavx->datastart:%d\r\n",datastart);
				}else res=3;//data����δ�ҵ�.
			}else res=2;//��wav�ļ�

		}else res=1;//���ļ�����
	}
	f_close(ftemp);
	free(ftemp);//�ͷ��ڴ�
	free(buf);
	return 0;
}

uint32_t wavPlay::audioFillBuffer(uint8_t *buf, uint16_t size, uint8_t bits) {
    u16 readlen=0;
    u32 bread;
    u16 i;
    u8 *p;
    if(bits==24)//24bit��Ƶ,��Ҫ����һ��
    {
        readlen=(size/4)*3;							//�˴�Ҫ��ȡ���ֽ���
        f_read(audiodev.file,audiodev.tbuf,readlen,(UINT*)&bread);	//��ȡ����
        p=audiodev.tbuf;
        for(i=0;i<size;)
        {
            buf[i++]=p[1];
            buf[i]=p[2];
            i+=2;
            buf[i++]=p[0];
            p+=3;
        }
        bread=(bread*4)/3;		//����Ĵ�С.
    }else
    {
        f_read(audiodev.file,buf,size,(UINT*)&bread);//16bit��Ƶ,ֱ�Ӷ�ȡ����
        if(bread<size)//����������,����0
        {
            for(i=bread;i<size-bread;i++)buf[i]=0;
        }
    }
    return bread;
}

uint8_t wavPlay::audioPlaySong(uint8_t *FileName) {
    uint8_t res=0;uint8_t t=0;
    uint8_t key;
//    res=f_open(audiodev.file,(TCHAR*)FileName,FA_READ);	//���ļ�
//ʹ�ò���ϵͳʱ�����ȥ������Ƶ�����ļ��г��ֲ��Ŵ���

    if (wavtransferend==1&&audiodev.playStatus==1){

        wavtransferend=0;
        if(fillnum!=WAV_I2S_TX_DMA_BUFSIZE)//���Ž���?
        {
            res=PlayStatus_Finish;
        }
        if(wavwitchbuf)fillnum=audioFillBuffer(audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE,bps);//���buf2
        else fillnum=audioFillBuffer(audiodev.i2sbuf1,WAV_I2S_TX_DMA_BUFSIZE,bps);//���buf1
#if 0
        while(1) {
            key = KEY_Scan(0);
            if (key == WKUP_PRES)//��ͣ
            {
                if (audiodev.status & 0X01)audiodev.status &= ~(1 << 0);
                else audiodev.status |= 0X01;
            }
            if (key == KEY2_PRES || key == KEY0_PRES)//��һ��/��һ��
            {
                res = key;
                break;
            }
           // audioGetCurtime(audiodev.file);
            //audio_msg_show(this);
            t++;
            if (t == 20) {
                t = 0;
                HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_10);
                delay_ms(5);
            }
            if ((audiodev.status & 0X01) == 0)delay_ms(10);
            else break;
        }
#endif
    }else{
//        Serial0<<"playStatus:"<<audiodev.playStatus<<endl;
//        Serial0<<"wavtransferend:"<<wavtransferend<<endl;
//        Serial0<<"res:"<<res<<endl;
    }
    audioGetCurtime(audiodev.file);
//    audio_msg_show(this);
    if (res==PlayStatus_Finish){
        f_close(audiodev.file);
        audioVarRelease();
    }
    return res;
}

uint8_t wavPlay::audioPlaySongInit(uint8_t *FileName) {
    uint8_t res;
//    Serial.println("fileName:%s",FileName);
    audiodev.file=&tempFil;
    audiodev.i2sbuf1=i2s1;
    audiodev.i2sbuf2=i2s2;
    audiodev.tbuf=tbuf;
    audioGetName(FileName);
    if(audiodev.file&&audiodev.i2sbuf1&&audiodev.i2sbuf2&&audiodev.tbuf){

        res= audioAnalysis(FileName);
//        Serial<<"res0:"<<res<<endl;
        if (res==0){
            if(bps==16)
            {
                WM8978_I2S_Cfg(2,0);	//�����ֱ�׼,16λ���ݳ���
                I2S2_Init(I2S_STANDARD_PHILIPS,I2S_MODE_MASTER_TX,I2S_CPOL_LOW,I2S_DATAFORMAT_16B_EXTENDED);	//�����ֱ�׼,��������,ʱ�ӵ͵�ƽ��Ч,16λ��չ֡����
            }else if(bps==24)
            {
                WM8978_I2S_Cfg(2,2);	//�����ֱ�׼,24λ���ݳ���
                I2S2_Init(I2S_STANDARD_PHILIPS,I2S_MODE_MASTER_TX,I2S_CPOL_LOW,I2S_DATAFORMAT_24B);	//�����ֱ�׼,��������,ʱ�ӵ͵�ƽ��Ч,24λ����
            }
            I2S2_SampleRate_Set(samplerate);//���ò�����
            I2S2_TX_DMA_Init(audiodev.i2sbuf1,audiodev.i2sbuf2,WAV_I2S_TX_DMA_BUFSIZE/2); //����TX DMA
            i2s_tx_callback=wav_i2s_dma_tx_callback;			//�ص�����ָwav_i2s_dma_callback
            Music::audio_stop();
            res=f_open(audiodev.file,(TCHAR*)FileName,FA_READ);	//���ļ�
//            Serial<<"res1:"<<res<<endl;
            if(res==0) {
                f_lseek(audiodev.file, datastart);        //�����ļ�ͷ
                fillnum = audioFillBuffer(audiodev.i2sbuf1, WAV_I2S_TX_DMA_BUFSIZE, bps);
                fillnum = audioFillBuffer(audiodev.i2sbuf2, WAV_I2S_TX_DMA_BUFSIZE, bps);
                Music::audio_start();
            }else{
                res=0xFF;
            }
        }else{
            res=0xFF;
        }
    }else{
        res=0xFF;
    }
//    f_close(audiodev.file);
//    Serial<<"res2:"<<res<<endl;
    return res;
}

void wavPlay::audioVarRelease() {
    f_close(audiodev.file);
    audiodev.tbuf=nullptr;
    audiodev.i2sbuf1=nullptr;
    audiodev.i2sbuf2=nullptr;
    audiodev.file=nullptr;
}

wavPlay::~wavPlay() {

}

void wavPlay::audioGetCurtime(FIL *fx) {
    long long fpos;
    totsec=datasize/(bitrate/8);	//�����ܳ���(��λ:��)
    fpos=fx->fptr-datastart; 					//�õ���ǰ�ļ����ŵ��ĵط�
    cursec=fpos*totsec/datasize;	//��ǰ���ŵ��ڶ�������?
}

void wavPlay::audioGetName(uint8_t* pname) {
    String substring = "0:/MUSIC/";
    String substring1 = ".wav";
    String tempPname=(char *)pname;
    int index = tempPname.indexOf(substring);
    if (index != -1) {
        tempPname.remove(index, substring.length());
    }
    index=tempPname.indexOf(substring1);
    if (index!=-1){
        tempPname.remove(index,substring1.length());
        strcpy((char *)title,tempPname.c_str());
    }
}

wavPlay::wavPlay() {

}
