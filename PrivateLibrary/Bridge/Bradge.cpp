#include "Bradge.h"
#include "MyUsart.h"

#include "lcd.h"
#include "hal.h"
#include "lcd.h"
#include "key.h"
#include "lvgl.h"
#include "sram.h"
#include "malloc.h"
#include "ff.h"
#include "Music.h"
#include "norflash.h"
#include "fonts.h"
//#include "lv_port_indev.h"
//#include "lv_port_disp.h"
//#include "wm8978.h"
#include "i2S2.h"

void setup(){
    SRAM_Init();

    my_mem_init(SRAMIN);
    my_mem_init(SRAMCCM);
    my_mem_init(SRAMEX);

}
void  loop(){
//    lv_task_handler();
//    HAL_Delay(5);
}