/*
        GNU GPL v3
        (C) Tomas Kovacik [nail at nodomain dot sk]
        https://github.com/tomaskovacik/

	example for F-6188 module library
*/


#include "src/BK8000L.h"
#include <SoftwareSerial.h> //if using SW, with HW no need for this



#define resetBTPin 5

SoftwareSerial swSerial(11,12); //rxPin, txPin

BK8000L BT(&swSerial, resetBTPin); //in case of HW serial use for example: (&Serial1)

void setup(){
  BT.begin(9600); //or BT.begin(9600); for specific baudrate
  Serial.begin(115200);
  // getInitStates();
  Serial.println(BT.getName());
  swSerial.write("APT+SPP8888");
}

void loop(){
//should be call periodically, to get notifications from module, for example if someone calling...
  // Serial.println(BT.getNextEventFromBT());

  // if (BT.available() > 0) {
    char c = swSerial.read();
    Serial.print(c);

}