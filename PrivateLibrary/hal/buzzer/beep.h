#ifndef __BEEP_H
#define __BEEP_H
#include "sys.h"
#ifdef __cplusplus
extern "C"
{
#endif




//LED�˿ڶ���
#define BEEP PFout(8)	// ����������IO 

void BEEP_Init(void);//��ʼ��
#ifdef __cplusplus
}
#endif
#endif
