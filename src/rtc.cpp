#include <WiFi.h>
#include "common.h"
#include "rtc.h"

RTC::RTC(BM8563 &rtc) : rtc(rtc)
{
    rtc.begin();
}

RTC::~RTC()
{
}

void RTC::connect(const char *ssid, const char *password)
{
    SERIAL_PRINTF("Connecting to %s", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        SERIAL_PRINT(".");
    }
    SERIAL_PRINTLN(" CONNECTED");
}

void RTC::disconnect(void)
{
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

void RTC::setRTCDate(const struct tm &info)
{
    static rtc_date_t d;

    d.week = info.tm_wday;
    d.mon = info.tm_mon + 1;
    d.day = info.tm_mday;
    d.year = info.tm_year + 1900;

    rtc.setDate(&d);
}

void RTC::setRTCTime(const struct tm &info)
{
    static rtc_time_t t;

    t.hour = info.tm_hour;
    t.min = info.tm_min;
    t.sec = info.tm_sec;

    rtc.setTime(&t);
}

bool RTC::setRTC(const char *tz, const char *ntpServer,
                 const char *ssid, const char *password)
{
    static struct tm info;

    connect(ssid, password);
    configTzTime(tz, ntpServer);
    const bool succeeded = getLocalTime(&info);
    if (succeeded)
    {
        setRTCDate(info);
        setRTCTime(info);
        SERIAL_PRINTF("Succeeded to setRTC(%s, %s): %s",
                      tz, ntpServer, getString());
    }
    else
    {
        SERIAL_PRINTF("Failed to setRTC(%s, %s)", tz, ntpServer);
    }
    disconnect();
    return succeeded;
}

void RTC::getRTC(rtc_date_t &d, rtc_time_t &t)
{
    rtc.getDate(&d);
    rtc.getTime(&t);
}

const char *RTC::getString(void)
{
    static const char *wd[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    static const char *format = "%04d/%02d/%02d(%s) %02d:%02d:%02d";
    static char dateTime[] = "2021/05/15(Sun) 00:00:00";
    static rtc_date_t d;
    static rtc_time_t t;

    getRTC(d, t);
    snprintf(dateTime, sizeof(dateTime), format, d.year, d.mon, d.day,
             wd[d.week], t.hour, t.min, t.sec);
    return dateTime;
}
