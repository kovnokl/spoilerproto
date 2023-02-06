#include "DeviceInfo.h"

Spoilerpos pos1(500, 500);
Spoilerpos pos2(1000, 1000);

HardwareSerial vSerial(1);
HardwareSerial hSerial(2);

Motors mots(&vSerial, &hSerial, 5, 23);