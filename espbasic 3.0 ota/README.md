install arduino arduino-1.6.5-r5-windows https://www.arduino.cc/en/software/OldSoftwareReleases

add board http://arduino.esp8266.com/stable/package_esp8266com_index.json

and install 2.3.0

from tools board select esp8266

put folders ESP8266Basic and libraries in documents/arduino 

modified ESP8266Basic.ino at line :

70     #include <ESP8266HTTPUpdateServer.h>

235    ESP8266HTTPUpdateServer httpUpdater;

1226   httpUpdater.setup(server);

original https://github.com/esp8266/Basic/tree/NewWebSockets
