#include <Funken.h>

Funken fnk;
char *p;

boolean state = false;

void setup() {
  fnk.begin(57600,0, 2);

  FunkenSetupImplementEssentials();
  FunkenSetupImplementBasicArduino();
  FunkenSetupImplementArduinoHeartbeat();

  //fnk.process("STAHB 10");
}

void loop() {
  fnk.hark();
  heartbeatUpdate();
}

void sendHeartbeat(){
  fnk.respond("HB", String(millis()));
}
