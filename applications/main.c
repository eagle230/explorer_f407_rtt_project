/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-13     RT-Thread    first version
 */

#include <rtthread.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "udp_multicast.h"
#include "ft5206.h"
#include "touch.h"
#include "lcd.h"
#include "weather.h"
int main(void)
{
    tp_dev_struct tp_dev = {0};
    LOG_D("Nate test project build on %s %s",__DATE__,__TIME__);
    udp_app_start();
    tp_dev.init = FT5206_Init;
    tp_dev.scan = FT5206_Scan;
    regist_tp_dev(&tp_dev);

    tp_init(1);
    LCD_Init();
 //   weather_app_start();
    LCD_ShowString(30,50,200,32,32,"nate test");
    LCD_ShowString(30,70,200,32,32,"TOUCH TEST");
	LCD_ShowString(400-30,0,200,32,32,"Clean");//��ʾ��������
    while (1)
    {
//        FT5206_Scan();
        rt_thread_mdelay(100);
    }
    return RT_EOK;
}
