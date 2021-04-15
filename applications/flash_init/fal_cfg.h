/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-14     Nate_Wu       the first version
 */
#ifndef APPLICATIONS_FLASH_INIT_FAL_CFG_H_
#define APPLICATIONS_FLASH_INIT_FAL_CFG_H_

#include <rtconfig.h>
#include <board.h>

/* ===================== Flash device Configuration ========================= */
extern struct fal_flash_dev flash_dev_w25q128;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &flash_dev_w25q128,                                                     \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table */
#define FAL_PART_TABLE                                                               \
{                                                                                    \
    {FAL_PART_MAGIC_WORD, "filesystem", FAL_USING_NOR_FLASH_DEV_NAME,           0, 1024*1024, 0}, \
    {FAL_PART_MAGIC_WORD,  "download", FAL_USING_NOR_FLASH_DEV_NAME,   1024*1024, 1024*1024, 0}, \
    {FAL_PART_MAGIC_WORD, "easyflash", FAL_USING_NOR_FLASH_DEV_NAME, 1024*1024*2, 1024*1024, 0}, \
}
#endif /* FAL_PART_HAS_TABLE_CFG */


#endif /* APPLICATIONS_FLASH_INIT_FAL_CFG_H_ */
