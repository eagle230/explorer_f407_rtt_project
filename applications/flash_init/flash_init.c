/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-13     Nate_Wu       the first version
 */
#include <sfud.h>
#include "spi_flash_sfud.h"
#include "drv_spi.h"
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(hspi->Instance == SPI1)
    {
        /* USER CODE BEGIN SPI3_MspInit 0 */

        /* USER CODE END SPI3_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**SPI3 GPIO Configuration
        PC11     ------> SPI3_MISO
        PB3 (JTDO-TRACESWO)     ------> SPI3_SCK
        PB5     ------> SPI3_MOSI
        */

        GPIO_InitStruct.Pin = GPIO_PIN_3 |GPIO_PIN_4| GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USER CODE BEGIN SPI3_MspInit 1 */

        /* USER CODE END SPI3_MspInit 1 */
    }
}
static int rt_hw_spi_flash_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    rt_hw_spi_device_attach("spi1", "spi10", GPIOB, GPIO_PIN_14);

    struct rt_spi_device * spi_dev_com1;
    spi_dev_com1 = (struct rt_spi_device *)rt_device_find("spi10");
    if (!spi_dev_com1)
    {
        rt_kprintf("[SFUD] spi sample run failed! cant't find device!\n");
        return RT_NULL;
    }
    /* config spi */
    {
        struct rt_spi_configuration cfg;
        cfg.data_width = 8;
        cfg.mode = RT_SPI_MODE_0 | RT_SPI_MSB; /* SPI Compatible: Mode 0. */
        cfg.max_hz = 20 * 1000000;             /* max 10M */
        rt_spi_configure(spi_dev_com1, &cfg);
    }

    if (RT_NULL == rt_sfud_flash_probe(FAL_USING_NOR_FLASH_DEV_NAME, "spi10"))
    {
        return -RT_ERROR;
    };
    return RT_EOK;
}

INIT_DEVICE_EXPORT(rt_hw_spi_flash_init);
