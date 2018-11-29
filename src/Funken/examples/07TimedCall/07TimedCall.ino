#include <Funken.h>

Funken fnk;
char *p;

boolean state = false;

void setup() {
  fnk.begin(57600,0, 2);

  FunkenSetupImplementEssentials();
  FunkenSetupImplementBasicArduino();
  FunkenSetupImplementArduinoHeartbeat();
  FunkenSetupImplementTimedCall();
}

void loop() {
  fnk.hark();
  heartbeatUpdate();
  timedCallUpdate();
}

void sendHeartbeat(){
  fnk.respond("HB", String(millis()));
}

void sendTimedCall(){
  Serial.println("TIMED");
}
