/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-14     Nate_Wu       the first version
 */
#include <rtthread.h>
#include <easyflash.h>

static int user_ef_init(void)
{
    easyflash_init();
    return 0;
}
INIT_COMPONENT_EXPORT(user_ef_init);
