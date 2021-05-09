/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-15     Nate_Wu       the first version
 */
#ifndef APPLICATIONS_WEATHER_APP_WEATHER_H_
#define APPLICATIONS_WEATHER_APP_WEATHER_H_

#define GET_HEADER_BUFSZ               1024
#define GET_RESP_BUFSZ                 1024
#define URL_MAX_LEN					   256
#define WEATHER_GET_URI                  "https://api.seniverse.com/v3/weather/now.json?key=Sfu0cnHxMOS421TeI&location=%s&language=zh-Hans&unit=c"

extern int weather_app_start(void);
#endif /* APPLICATIONS_WEATHER_APP_WEATHER_H_ */
