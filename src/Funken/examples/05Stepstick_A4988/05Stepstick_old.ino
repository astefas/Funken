#include <Funken.h>

Funken fnk;
char *p;

boolean state = false;

void setup() {
  fnk.begin(57600,0, 1);

  FunkenSetupImplementEssentials();
  FunkenSetupImplementBasicArduino();
  FunkenSetupImplementStepstick();
}

void loop() {
  fnk.hark();
  stepping();
}
