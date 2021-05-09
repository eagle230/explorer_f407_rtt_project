/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-01     Nate_Wu       the first version
 */
#include "touch.h"
#define DBG_TAG "TP"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "lcd.h"
static rt_thread_t tsk_tp_handle = RT_NULL;
tp_dev_struct tp_dev =
{
    .init = NULL,
    .scan = NULL,
    .x = {0},
    .y = {0},
    .sta = 0,
    .dir = 0,
};
int regist_tp_dev(tp_dev_struct *dev)
{
	if((dev->init = NULL)||(dev->scan == NULL))
	{
	    rt_kprintf("regist_tp_dev failed!\r\n");
		return -1;
	}
	tp_dev.init = dev->init;
	tp_dev.scan = dev->scan;
	rt_kprintf("regist_tp_dev successed!\r\n");
	return 0;
}
static void tsk_tp_scan(void *parameter)
{

    while(1)
    {
//        tp_dev.scan();
        FT5206_Scan();
		for(rt_uint8_t t=0;t<5;t++)
		{
			if((tp_dev.sta)&(1<<t))
			{
				//printf("X坐标:%d,Y坐标:%d\r\n",tp_dev.x[0],tp_dev.y[0]);

					if(tp_dev.x[t]>(lcddev.width-30)&&tp_dev.y[t]<20)
					{
						LCD_Clear(WHITE);//清屏
					 	POINT_COLOR=BLUE;//设置字体为蓝色
						LCD_ShowString(400 - 30,0,200,32,32,"clear");//显示清屏区域
					  	POINT_COLOR=RED;//设置画笔蓝色
					}
			}
		}

		rt_thread_delay(30);
    }

}
//触摸屏初始化
int tp_init(rt_uint8_t u8dir)
{
#if 1
//	tp_dev.init();
    FT5206_Init();
	tsk_tp_handle = rt_thread_create("tp", tsk_tp_scan, RT_NULL, 512, 20, 10);
    if (tsk_tp_handle != RT_NULL)
    {
    	rt_thread_startup(tsk_tp_handle);
		rt_kprintf("rt_thread_create successed!\r\n");
    }
	else
	{
	    rt_kprintf("rt_thread_create failed!\r\n");
		return -1;
	}
    return 0;
#endif


}
