/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-01     Nate_Wu       the first version
 */
#ifndef APPLICATIONS_TOUCH_TOUCH_H_
#define APPLICATIONS_TOUCH_TOUCH_H_
#include "ft5206.h"
#include <rtthread.h>
#include <rtdevice.h>
#define TP_PRES_DOWN 0x80  //触屏被按下
#define TP_CATH_PRES 0x40  //有按键按下了
#define CT_MAX_TOUCH  5    //电容屏支持的点数,固定为5点

//触摸屏控制器
typedef struct
{
    int (*init)(void);           //初始化触摸屏控制器
    int (*scan)(void);             //扫描触摸屏.0,屏幕扫描
    rt_uint16_t x[CT_MAX_TOUCH];        //当前坐标
    rt_uint16_t y[CT_MAX_TOUCH];        //电容屏有最多5组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用
                                //x[4],y[4]存储第一次按下时的坐标.
    rt_uint8_t  sta;                    //笔的状态
                                //b7:按下1/松开0;
                                //b6:0,没有按键按下;1,有按键按下.
                                //b5:保留
                                //b4~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)

//新增的参数,当触摸屏的左右上下完全颠倒时需要用到.
//b0:0,竖屏(适合左右为X坐标,上下为Y坐标的TP)
//   1,横屏(适合左右为Y坐标,上下为X坐标的TP)
//b1~6:保留.
//b7:0,电阻屏
//   1,电容屏
    rt_uint8_t dir;
}tp_dev_struct;

extern tp_dev_struct tp_dev;
extern int tp_init(rt_uint8_t u8dir);
extern int regist_tp_dev(tp_dev_struct *dev);


#endif /* APPLICATIONS_TOUCH_TOUCH_H_ */
