/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-15     Nate_Wu       the first version
 */
#include <rtthread.h>
#include <webclient.h>
#include "weather.h"
#include <cJSON.h>
#define DBG_TAG "weather_app"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
static rt_thread_t tsk_weather_handle = RT_NULL;

char str_name[12],str_time[24],str_temp[24];
static int weather_data_parse(const char *data)
{
    int ret = 0;
    cJSON *json_root,*json_result,*json_location,*json_now,*json_name,*json_time,*json_temp,*json_arry;
    char *p_name,*p_time,*p_temp;
    LOG_D("\r\nweather_data_parse\r\n");
    json_root = cJSON_Parse(data);
    if(json_root != RT_NULL)
    {
        json_result = cJSON_GetObjectItem(json_root,"results");
        if(!json_result)
        {
            LOG_D("json_result errors\r\n");
            goto exit;
        }
        int size = cJSON_GetArraySize(json_result);
        LOG_D("json_result size %d\r\n",size);
        json_arry = cJSON_GetArrayItem(json_result,0);
        json_location = cJSON_GetObjectItem(json_arry,"location");
        if(!json_location)
        {
            LOG_D("json_location errors\r\n");
            goto exit;
        }
        json_name = cJSON_GetObjectItem(json_location,"name");
        if(!json_name)
        {
            LOG_D("json_name errors\r\n");
            goto exit;
        }
        json_now = cJSON_GetObjectItem(json_arry,"now");
        if(!json_now)
        {
            LOG_D("json_now errors\r\n");
            goto exit;
        }
        json_temp = cJSON_GetObjectItem(json_now,"temperature");
        if(!json_temp)
        {
            LOG_D("json_temp errors\r\n");
        }
        json_time = cJSON_GetObjectItem(json_arry,"last_update");
        if(!json_time)
        {
            LOG_D("json_time errors\r\n");
            goto exit;
        }
        p_name = cJSON_GetStringValue(json_name);
        rt_sprintf(str_name, "%s",p_name);
        rt_kprintf("str_name1 : %s,len = %d\r\n",p_name,rt_strlen(p_name));
        p_time = cJSON_GetStringValue(json_time);
        rt_sprintf(str_time, "%s",p_time);

        p_temp = cJSON_GetStringValue(json_temp);
        rt_sprintf(str_temp, "%s",p_temp);

        LOG_D("str_name : %s\r\n",str_name);
        LOG_D("str_time : %s\r\n",str_time);
        LOG_D("str_temp : %s\r\n",str_temp);
    }
    else {
        ret = -1;
        LOG_D(" json parse failed ..");
    }
exit:
    cJSON_Delete(json_root);
    return ret;
}
static int webclient_get_comm1(const char *uri)
{
    struct webclient_session* session = RT_NULL;
    unsigned char *buffer = RT_NULL;
    int index, ret = 0;
    int bytes_read, resp_status;
    int content_length = -1;

    buffer = (unsigned char *) web_calloc(1,GET_RESP_BUFSZ);
    if (buffer == RT_NULL)
    {
        LOG_D("no memory for receive buffer.\n");
        ret = -RT_ENOMEM;
        goto __exit;

    }

    /* create webclient session and set header response size */
    session = webclient_session_create(GET_HEADER_BUFSZ);
    if (session == RT_NULL)
    {
        ret = -RT_ENOMEM;
        goto __exit;
    }

    /* send GET request by default header */
    if ((resp_status = webclient_get(session, uri)) != 200)
    {
        LOG_D("webclient GET request failed, response(%d) error.\n", resp_status);
        ret = -RT_ERROR;
        goto __exit;
    }

    LOG_D("webclient get response data: \n");

    content_length = webclient_content_length_get(session);
    if (content_length < 0)
    {
        LOG_D("webclient GET request type is chunked.\n");
        do
        {
            bytes_read = webclient_read(session, buffer, GET_RESP_BUFSZ);
            if (bytes_read <= 0)
            {
                break;
            }

            for (index = 0; index < bytes_read; index++)
            {
//                rt_kprintf("%c", buffer[index]);
            }
        } while (1);

//        rt_kprintf("\n");
    }
    else
    {
        int content_pos = 0;

        do
        {
            bytes_read = webclient_read(session, buffer,
                    content_length - content_pos > GET_RESP_BUFSZ ?
                            GET_RESP_BUFSZ : content_length - content_pos);
            if (bytes_read <= 0)
            {
                break;
            }

            for (index = 0; index < bytes_read; index++)
            {
//                rt_kprintf("%c", buffer[index]);
            }

            content_pos += bytes_read;
        } while (content_pos < content_length);

//        rt_kprintf("\n");
    }
    LOG_D("resp:%s",buffer);
    weather_data_parse((char*)buffer);
__exit:
    if (session)
    {
        webclient_close(session);
    }

    if (buffer)
    {
        web_free(buffer);
    }

    return ret;
}
int app_weather_get(void)
{
    char *uri = RT_NULL;

    uri = rt_malloc(URL_MAX_LEN);
    if(uri == RT_NULL)
    {
        LOG_D("no memory for create get request uri buffer.\n");
        return -RT_ENOMEM;
    }
    rt_sprintf(uri, WEATHER_GET_URI,"nanjing");

    webclient_get_comm1(uri);

    if (uri)
    {
        web_free(uri);
    }

    return RT_EOK;
}
static void tsk_weathe_app(void *parameter)
{
    char uri[URL_MAX_LEN] ={0};
    rt_sprintf(uri, WEATHER_GET_URI,"nanjing");
    while(1)
    {
        rt_thread_delay(1*60*1000);
        webclient_get_comm1(uri);
        LOG_D("tsk_weathe_app tsk_weathe_app!\r\n");
    }

}
int weather_app_start(void)
{
    tsk_weather_handle = rt_thread_create("weather_app", tsk_weathe_app, RT_NULL, 10*1024, 30, 10);
    if (tsk_weather_handle != RT_NULL)
    {
        rt_thread_startup(tsk_weather_handle);
        LOG_D("tsk_weather_handle successed!\r\n");
    }
    else
    {
        LOG_D("tsk_weather_handle failed!\r\n");
        return -1;
    }
    return 0;
}
int weather_get_test(int argc, char **argv)
{
    char *uri = RT_NULL;

    if (argc == 2)
    {
    	uri = rt_malloc(URL_MAX_LEN);
		if(uri == RT_NULL)
        {
		    LOG_D("no memory for create get request uri buffer.\n");
            return -RT_ENOMEM;
        }
		rt_sprintf(uri, WEATHER_GET_URI,argv[1]);
       
        webclient_get_comm1(uri);
    }
    else
    {
        LOG_D("web_get_test [location]  - webclient GET request test.\n");
        return -RT_ERROR;
    }

    if (uri)
    {
        web_free(uri);
    }

    return RT_EOK;
}

#ifdef FINSH_USING_MSH
#include <finsh.h>
MSH_CMD_EXPORT_ALIAS(weather_get_test, weather_get_test, weather get request test);
#endif /* FINSH_USING_MSH */
