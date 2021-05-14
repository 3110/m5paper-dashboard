#include <M5EPD.h>
#include "common.h"
#include "rtc.h"

const char *TZ = "JST-9";
const char *NTP_SERVER = "ntp.nict.jp";
const char *SSID = "[your SSID]";
const char *PASSWORD = "[your password]";

const bool touchEnable = true;
const bool SDEnable = true;
const bool SerialEnable = true;
const bool BatteryADCEnable = true;
const bool I2CEnable = false;

RTC rtc(M5.RTC);

void setup(void)
{
    M5.begin(touchEnable, SDEnable, SerialEnable, BatteryADCEnable, I2CEnable);
    M5.EPD.SetRotation(90);
    M5.EPD.Clear(true);
    rtc.setRTC(TZ, NTP_SERVER, SSID, PASSWORD);
}

void loop(void)
{
    M5.update();
    SERIAL_PRINTF("DateTime: %s\n", rtc.getString());
    delay(1000);
}