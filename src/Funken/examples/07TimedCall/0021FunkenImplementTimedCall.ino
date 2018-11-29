#ifndef TIMER
#define TIMER
#include <Timer.h>
#endif

Timer tcall;
int tCallID;

void FunkenSetupImplementTimedCall() {
  fnk.listenTo("TIMED", timedCall);
}

/*
 * TYPICAL: STAHB [timeout]
 */
void timedCall(char *c){
  char *token = fnk.getToken(c);
  char *del = fnk.getArgument(c);

  int timeout = atoi(del);

  byte debug = 0;
  if(del != NULL){
    debug = 1;
    tCallID = t.after(timeout, sendCall);
  }
}

void timedCallUpdate(){
  t.update();
}

void sendCall(){
  t.stop(tCallID);
  sendTimedCall();
}
