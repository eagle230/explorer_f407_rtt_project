/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-24     Nate_Wu       the first version
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <rtthread.h>
#include "udp_multicast.h"
#define DBG_TAG "udp_app"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
rt_thread_t tsk_udp_handle;
void Tsk_udp_app(void *parameter)
{
    LOG_D("entry tsk_udp_test");
    int socked = socket(AF_INET,SOCK_DGRAM,0);
    char group[16] = "224.0.0.50";
    if(socked < 0)
    {
        LOG_D("socket creat failed!");
        goto ERROR;
    }

    struct sockaddr_in local_addr;
    memset(&local_addr,0,sizeof(local_addr));

    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(9898);

    int ret = bind(socked,(struct sockaddr*)&local_addr,sizeof(local_addr));
    if(ret < 0)
    {
        LOG_D("bind failed !");
        goto ERROR;
    }

    struct ip_mreq mreq; // 多播地址结构体
    mreq.imr_multiaddr.s_addr = inet_addr(group);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    ret = setsockopt(socked,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
    if(ret < 0)
    {
        LOG_D("setsockopt failed !");
        goto ERROR;
    }
    else
    {
        LOG_D("setsockopt success\n");
    }
    char buf[1024];
    int length = 0;
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    while(1)
    {
        memset(buf, 0, sizeof(buf));
        length = recvfrom(socked,buf,sizeof(buf),0,(struct sockaddr*)&sender,&sender_len);
        buf[length] ='\0';
        LOG_D("%s %d : %s\n",inet_ntoa(sender.sin_addr),ntohs(sender.sin_port),buf);
    }
ERROR:
    LOG_D("Tsk_test_app  ERROR");
    setsockopt(socked, IPPROTO_IP, IP_DROP_MEMBERSHIP,&mreq, sizeof(mreq));
    close(socked);
}
void Tsk_test_app(void *parameter)
{
    while(1)
    {
        LOG_D("Tsk_test_app");
        rt_kprintf("Tsk_test_app\n");
        rt_thread_delay(1000);
    }
}
void udp_app_start(void)
{
    tsk_udp_handle = rt_thread_create("udp_test",Tsk_udp_app,RT_NULL,1024*2,30,5);
    if(tsk_udp_handle != RT_NULL)
    {
        LOG_D("creat udp_task success..\r\n");
        rt_thread_startup(tsk_udp_handle);
    }
    else {
        LOG_D("creat udp_task faild..\r\n");
    }
}
