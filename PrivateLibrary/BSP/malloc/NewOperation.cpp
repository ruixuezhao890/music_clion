/**
*********************************************************************
*********
* @project_name :MusicClion
* @file : NewOperation.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2024/2/13 
*********************************************************************
*********
*/
//

#include "NewOperation.h"
#include "malloc.h"

void *operator new(size_t size){
    void *p = nullptr;
    p= mymalloc(SRAMEX,size);
    if (!p){
        throw std::bad_alloc();
    }
    return p;
}
void operator delete(void *pointer){
    if (pointer){
        myfree(SRAMEX,pointer);
    }
}

