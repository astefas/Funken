#include <Timer.h>

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
  char *token = strtok_r(c, " ", &p);
  char *del = strtok_r(NULL, " ", &p);

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
