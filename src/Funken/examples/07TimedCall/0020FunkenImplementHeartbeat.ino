#ifndef TIMER
#define TIMER
#include <Timer.h>
#endif

Timer t;
int timerID;

void FunkenSetupImplementArduinoHeartbeat() {
  fnk.listenTo("STAHB", startHeartbeat);
  fnk.listenTo("STPHB", stopHeartbeat);
}

/*
 * TYPICAL: STAHB [timeout]
 */
void startHeartbeat(char *c){
  char *token = fnk.getToken(c);
  char *del = fnk.getArgument(c);

  int timeout = atoi(del);

  byte debug = 0;

  if(del != NULL){
    debug = 1;
    timerID = t.every(timeout, sendHeartbeat);
  }
}

/*
 * TYPICAL: STPHB
 */
void stopHeartbeat(char *c){
  t.stop(timerID);
}

void heartbeatUpdate(){
  t.update();
}
