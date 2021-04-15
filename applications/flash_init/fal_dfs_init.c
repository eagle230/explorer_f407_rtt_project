/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-14     Nate_Wu       the first version
 */
#include <fal.h>
#include <dfs.h>
#include <dfs_fs.h>
#include <dfs_file.h>
#include "dfs_elm.h"
#define DBG_TAG "fal_dfs"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#define FILE_SYSTEM_PARTITION "filesystem"
static int fal_dfs_init(void)
{
    int ret;
    fal_init();
    fal_blk_device_create(FILE_SYSTEM_PARTITION);

    ret = dfs_mount(FILE_SYSTEM_PARTITION, "/", "elm", 0, 0);
    if(ret == -1)
    {
        LOG_D("have no system file,making file system...");
        ret = dfs_mkfs("elm",FILE_SYSTEM_PARTITION);
        if(ret == 0)
        {
            LOG_D("dfs_mkfs successed!");
            ret = dfs_mount(FILE_SYSTEM_PARTITION, "/", "elm", 0, 0);
            if(ret == -1)
            {
                LOG_D("dfs_mount faild!");
            }
            else
            {
                LOG_D("dfs_mount successed!");
                mkdir("/usr",0x777);
                mkdir("/sys",0x777);
            }
        }
        else
        {
            LOG_D("dfs_mkfs faild!");
        }
    }
    else
    {
        LOG_D("already have system file!");
        mkdir("/usr",0x777);
        mkdir("/sys",0x777);
    }
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(fal_dfs_init);
