# ESP8266Basic-web-update-ota

folder espbasic 3.0 is ultimate basic and find what modify and how install

Inspired from esp8266 webupdater arduino examples (to add web update)

You can compile webupdate mini and flash to esp8266 ... after connect with esp8266 over wifi ...

and open http://192.168.4.1/update and upload ESP8266Basic.ino.generic.bin over the air

You can upload over the air again WebUpdater.ino.generic.bin

You can modify code and with Arduino menu sketch >> export compiled binary and load with http://192.168.4.1/update

https://docs.google.com/document/d/1NMZvnnjZ5XNzkykINgYS_Ql6mQ3nJEAzYudbmScdiIg/pub is ESP8266 Basic Language Reference

For ESP Basic 2.0 aXX

HTTP://ESP8266BASIC.COM

# But parameters differs ... for example if want to light the led of gpio2 

# need to write po 2 1 and not io(po,2,1) so parameters need to have only a space

---------------------------------------------------------------------------------------------------------------------------

You can upload espbasic in classic mode same as https://github.com/costycnc/costycnc-espbasic-adjust-errors

Also you can upload espbasic original from https://github.com/esp8266/basic

some examples wifi mode https://github.com/costycnc/costycnc-WIFI_AP-WIFI_STA-WIFI_AP_STA-esp8266
