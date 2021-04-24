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
int main(void)
{
    LOG_D("Nate test project build on %s %s",__DATE__,__TIME__);
    udp_app_start();
    while (1)
    {
        rt_thread_mdelay(1000);
    }
    return RT_EOK;
}
