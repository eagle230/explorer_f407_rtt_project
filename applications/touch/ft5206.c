/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-01     Nate_Wu       the first version
 */
#include "ft5206.h"
#include "touch.h"
#include "string.h"
#include <rtthread.h>
#include <rtdevice.h>
#define DBG_TAG "FT5026"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#define FT5206_I2C_BUS_NAME          "i2c1"  /* 传感器连接的I2C总线设备名称 */
#define FT5206_ADDR                  0x38    /* 从机地址 */
static struct rt_i2c_bus_device *i2c_bus = RT_NULL;     /* I2C总线设备句柄 */
//0111 0000 =0x70   0111 0001 =0x71   011 1000
static void FT5206_WR_Reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t value)
{
    struct rt_i2c_msg msg = {0};
    rt_uint8_t buf[2];
    buf[0] = reg;
    buf[1] = value;

    msg.addr = FT5206_ADDR;
    msg.flags = RT_I2C_WR;
    msg.buf = buf;
    msg.len = 2;
   if (rt_i2c_transfer(bus, &msg, 1) != 1)
   {
       LOG_D("ft5426 write register error");
   }
}

static void FT5206_RD_Reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t *buf, rt_uint8_t len)
{
    struct rt_i2c_msg msg = {0};
    rt_uint8_t temp_reg;

    temp_reg = reg;

    msg.addr = FT5206_ADDR;
    msg.flags = RT_I2C_WR;
    msg.buf = &temp_reg;
    msg.len = 1;
    if (rt_i2c_transfer(bus, &msg, 1) != 1)
    {
        LOG_D("ft5426 write register error");
    }

    msg.addr = FT5206_ADDR;
    msg.flags = RT_I2C_RD;
    msg.buf = buf;
    msg.len = len;
    if (rt_i2c_transfer(bus, &msg, 1) != 1)
    {
        LOG_D("ft5426 read register error");
    }
}


//初始化FT5206触摸屏
//返回值:0,初始化成功;1,初始化失败
int FT5206_Init(void)
{
    rt_uint8_t temp[2];
    /* 查找I2C总线设备，获取I2C总线设备句柄 */
    i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(FT5206_I2C_BUS_NAME);

    if (i2c_bus == RT_NULL)
    {
        LOG_D("can't find %s device!\n", FT5206_I2C_BUS_NAME);
    }
    else
    {
        LOG_D("successed find %s device!\n", FT5206_I2C_BUS_NAME);
        temp[0] = 0;
        FT5206_WR_Reg(i2c_bus,FT_DEVIDE_MODE,temp[0]);   //进入正常操作模式
        FT5206_WR_Reg(i2c_bus,FT_ID_G_MODE,temp[0]);     //查询模式
        temp[0] = 22;                             //触摸有效值，22，越小越灵敏
        FT5206_WR_Reg(i2c_bus,FT_ID_G_THGROUP,temp[0]);  //设置触摸有效值
        temp[0] = 12;                             //激活周期，不能小于12，最大14
        FT5206_WR_Reg(i2c_bus,FT_ID_G_PERIODACTIVE,temp[0]);
        //读取版本号，参考值：0x3003
        FT5206_RD_Reg(i2c_bus,FT_ID_G_LIB_VERSION,&temp[0],2);
        LOG_D("temp[0]:%x,temp[1]:%x\r\n",temp[0],temp[1]);
        return 0;
    }
    return -1;
}

const rt_uint16_t FT5206_TPX_TBL[5]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,FT_TP4_REG,FT_TP5_REG};
//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
int FT5206_Scan(void)
{
    rt_uint8_t state;
    rt_uint8_t temp = 0;
    rt_uint8_t buf[4];
    int res;
    FT5206_RD_Reg(i2c_bus,FT_REG_NUM_FINGER,&state,1);//读取触摸点的状态
//    rt_kprintf("state : %02X\r\n",state);
    if((state & 0xF) && ((state & 0xF) < 6))
    {
        temp = 0xFF << (state & 0xF);//将点的个数转换为1的位数,匹配tp_dev.sta定义
        tp_dev.sta = (~temp) | TP_PRES_DOWN | TP_CATH_PRES;
        for(rt_uint8_t i = 0;i < 5;i++)
        {
            if(tp_dev.sta & (1 << i))   //触摸有效
            {
                FT5206_RD_Reg(i2c_bus,FT5206_TPX_TBL[i],buf,4); //读取XY坐标值
                if(tp_dev.dir & 0x01)//横屏
                {
                    tp_dev.y[i] = ((rt_uint16_t)(buf[0] & 0x0F) << 8) + buf[1];
                    tp_dev.x[i] = ((rt_uint16_t)(buf[2] & 0x0F) << 8) + buf[3];
                }
                else
                {
                    tp_dev.x[i] = 480 - (((rt_uint16_t)(buf[0] & 0x0F) << 8) + buf[1]);
                    tp_dev.y[i] = ((rt_uint16_t)(buf[2] & 0x0F) << 8) + buf[3];
                }
                if((buf[0] & 0xF0) != 0x80)
				{
					tp_dev.x[i] = tp_dev.y[i] = 0;//必须是contact事件，才认为有效
				}
                LOG_D("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
            }
        }
        res = 1;
        if(tp_dev.x[0] == 0 && tp_dev.y[0] == 0)
		{
			state=0; //读到的数据都是0,则忽略此次数据
		}
    }
    if((state & 0x1F) == 0)//无触摸点按下
    {
        if(tp_dev.sta & TP_PRES_DOWN) //之前是被按下的
        {
            tp_dev.sta &= ~(1<<7);    //标记按键松开
        }else                       //之前就没有被按下
        {
            tp_dev.x[0] = 0xffff;
            tp_dev.y[0] = 0xffff;
            tp_dev.sta &= 0XE0;   //清除点有效标记
        }
    }
    return res;
}
