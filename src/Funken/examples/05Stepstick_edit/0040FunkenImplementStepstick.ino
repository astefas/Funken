// ???
#define STEP_DELAY_SHOW 100

// A4988 BASE PINS
#define STEP_PIN 5
#define DIR_PIN 6
#define ENABLE_PIN 4

// A4988 STEPSTICK MICROSTEPPING PINS
#define MS1 7
#define MS2 8
#define MS3 9

unsigned long tdelta = 0;
int rpm = 60;
long stepsPerRev = 200;
int steppingDelay;

long currentStep = 0;
long stepToGo = 0;

boolean isForward = true;
boolean isContinuous = false;


void FunkenSetupImplementStepstick() {
  // SETUP PINS FOR THE STEPSTICK DRIVER
  pinMode(STEP_PIN, OUTPUT);
  digitalWrite(STEP_PIN, LOW);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(DIR_PIN, HIGH);

  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);

  //digitalWrite(M0, LOW);
  //digitalWrite(M1, LOW);
  //digitalWrite(M2, HIGH);

  // DEFINE FUNKEN TOKENS AND CALLBACKS
  fnk.listenTo("ENABLE", enableEx);
  fnk.listenTo("DISABLE", disableEx);
  fnk.listenTo("GOREL", goRelative);
  fnk.listenTo("GOABS", goAbsolute);
  fnk.listenTo("SETRPM", setRPM);
  fnk.listenTo("CONTROT", continuousRotation);
  fnk.listenTo("MICROSTEP", setMicroStepMode); // not implemented

  // COMPUTE MICROSECONDS DELAY BETWEEN STEPS
  steppingDelay = int(1000000 / (stepsPerRev * (rpm / 60.0)));
}

/*
   TYPICAL: MICROSTEP 0 or MICROSTEP [0(full) or 1(half) or 2(quarter) or 3(eighth) or 4(sixteenth)]
*/
void setMicroStepMode(char *c) {
  char *token = fnk.getToken(c);
  char *val = fnk.getArgument(c);
}

/*
   TYPICAL: CONTROT 1 or CONTROT [-1(backward) or 0(stop) or 1(forward)]
*/
void continuousRotation(char *c) {
  char *token = fnk.getToken(c);
  char *val = fnk.getArgument(c);

  int dir = atoi(val);
  if (dir == 0) {
    isContinuous = false;
    stepToGo = currentStep;
  } else {
    if (dir >= 0) {
      isForward = true;
    } else {
      isForward = false;
    }
    isContinuous = true;
  }
}

/*
   TYPICAL: SETRPM 60 or SETRPM [int]
*/
void setRPM(char *c) {
  char *token = fnk.getToken(c);
  char *val = fnk.getArgument(c);

  rpm = atoi(val);
  steppingDelay = int(1000000 / (stepsPerRev * (rpm / 60.0)));
}

/*
   TYPICAL: GOABS 90 or GOABS [int]
*/
void goAbsolute(char *c) {
  char *token = fnk.getToken(c);
  char *val = fnk.getArgument(c);

  int degree = atoi(val);
  long steps = (stepsPerRev * degree) / 360;
  stepToGo = steps;

  isContinuous = false;
}

/*
   TYPICAL: GOREL 90 or GOREL [int]
*/
void goRelative(char *c) {
  char *token = fnk.getToken(c);
  char *val = fnk.getArgument(c);

  int degree = atoi(val);
  long steps = (stepsPerRev * degree) / 360;
  stepToGo = currentStep + steps;

  isContinuous = false;
}

/*
   TYPICAL: ENABLE [NO ATTRIBUTES]
*/
void enableEx(char *c) {
  char *token = fnk.getToken(c);

  digitalWrite(ENABLE_PIN, 0);
}

/*
   TYPICAL: DISABLE [NO ATTRIBUTES]
*/
void disableEx(char *c) {
  char *token = fnk.getToken(c);

  digitalWrite(ENABLE_PIN, 1);
}

void forward() {
  isForward = true;
  digitalWrite(DIR_PIN, HIGH);
}

void backward() {
  isForward = false;
  digitalWrite(DIR_PIN, LOW);
}

void step(long stepDelay) {
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(stepDelay);
  digitalWrite(STEP_PIN, LOW);

  //////////////////////////////////////////////////////// WHAT IS THIS?!?!
  unsigned long dt = micros() - tdelta;
  if (dt < STEP_DELAY_SHOW) {
    delayMicroseconds(STEP_DELAY_SHOW - dt);
  }
  tdelta = micros();
  ////////////////////////////////////////////////////////
  
  if (isForward) currentStep += 1;
  else currentStep -= 1;
}

void stepping() {
  if (isContinuous) {
    if (isForward) forward();
    else backward();
    step(steppingDelay);
  } else {
    long steppingDelta = stepToGo - currentStep;
    if (steppingDelta > 0) {
      if (!isForward) forward();
      step(steppingDelay);
    } else if (steppingDelta < 0) {
      if (isForward) backward();
      step(steppingDelay);
    }
  }
}
