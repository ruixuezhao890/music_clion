/**
*********************************************************************
*********
* @project_name :MusicClion
* @file : NewOperation.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2024/2/13 
*********************************************************************
*********
*/
//

#ifndef MUSICCLION_NEWOPERATION_H
#define MUSICCLION_NEWOPERATION_H
#include <new>

void *operator new(size_t size);
void operator delete(void *pointer);
#endif //MUSICCLION_NEWOPERATION_H
