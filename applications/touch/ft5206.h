/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-01     Nate_Wu       the first version
 */
#ifndef APPLICATIONS_TOUCH_FT5206_H_
#define APPLICATIONS_TOUCH_FT5206_H_


//与电容触摸屏连接的芯片引脚(未包含IIC引脚)
//IO操作函数
#define FT_RST                  PCout(13)   //FT5206复位引脚
#define FT_INT                  PBin(1)     //FT5206中断引脚

//I2C读写命令
#define FT_CMD_WR               0X70        //写命令
#define FT_CMD_RD               0X71        //读命令

//FT5206 部分寄存器定义
#define FT_DEVIDE_MODE          0x00        //FT5206模式控制寄存器
#define FT_REG_NUM_FINGER       0x02        //触摸状态寄存器

#define FT_TP1_REG              0X03        //第一个触摸点数据地址
#define FT_TP2_REG              0X09        //第二个触摸点数据地址
#define FT_TP3_REG              0X0F        //第三个触摸点数据地址
#define FT_TP4_REG              0X15        //第四个触摸点数据地址
#define FT_TP5_REG              0X1B        //第五个触摸点数据地址


#define FT_ID_G_LIB_VERSION     0xA1        //版本
#define FT_ID_G_MODE            0xA4        //FT5206中断模式控制寄存器
#define FT_ID_G_THGROUP         0x80        //触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE    0x88        //激活状态周期设置寄存器


int FT5206_Init(void);
int FT5206_Scan(void);

#endif /* APPLICATIONS_TOUCH_FT5206_H_ */