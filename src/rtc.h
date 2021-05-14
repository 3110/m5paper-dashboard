#ifndef RTC_H_
#define RTC_H_

#include <utility/BM8563.h>

class RTC
{
public:
    RTC(BM8563 &rtc);
    virtual ~RTC();
    virtual bool setRTC(const char *tz, const char *ntpServer,
                        const char *ssid, const char *password);
    virtual void getRTC(rtc_date_t &d, rtc_time_t &t);
    virtual const char *getString(void);

protected:
    virtual void connect(const char *ssid, const char *password);
    virtual void disconnect(void);
    virtual void setRTCDate(const struct tm &info);
    virtual void setRTCTime(const struct tm &info);

private:
    BM8563 rtc;
};
#endif // !RTC_H_