const int maxNumPins = 8;
int routedPins[maxNumPins];
int routedStates[maxNumPins];
int prevRoutedStates[maxNumPins];
String routedFunctions[maxNumPins];    

void FunkenSetupImplementRouting() {
  fnk.listenTo("RT", funkenRouteSignal);
  fnk.listenTo("CLRT", funkenClearRouting);
}


/*
   TYPICAL: CLROUTE
*/
void funkenClearRouting(char *c) {
  char *token = fnk.getToken(c);

  for (int i = 0; i < maxNumPins; i++) {
    routedPins[i] = 0;
    routedFunctions[i] = "";
    routedStates[i] = 0;
    prevRoutedStates[i] = 0;
  }
}


/*
   TYPICAL: ROUTE 2 0 TEST 500 or ROUTE [pin] [pin state] [routed function] [routed argument]
*/
void funkenRouteSignal(char *c) {
  char *token = fnk.getToken(c);
  char *pin = fnk.getArgument(c);
  char *state = fnk.getArgument(c);
  char *function = fnk.getArgument(c);
  char *funcArgument = fnk.getArgument(c);

  for (int i = 0; i < maxNumPins; i++) {
    if (routedPins[i] == 0) {
      // routed pin
      routedPins[i] = atoi(pin);
      pinMode(routedPins[i], INPUT_PULLUP);

      // routed state
      routedStates[i] = atoi(state);

      // routed command
      String routedComm = String(function);
      routedComm += " ";
      routedComm += String(funcArgument);
      routedFunctions[i] = routedComm;
      break;
    }
  }
}


void routeSignals() {
  for (int i = 0; i < maxNumPins; i++) {
    if (routedPins[i] != 0) {
      int pinState = digitalRead(routedPins[i]);
      if (pinState == routedStates[i] && pinState != prevRoutedStates[i]) { 
        fnk.process(routedFunctions[i]);
      }
      prevRoutedStates[i] = pinState;
    }
  }
}
