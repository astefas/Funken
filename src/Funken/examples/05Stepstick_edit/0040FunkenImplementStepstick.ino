#define STEP_DELAY_SHOW 100
#define STEP_PIN 5
#define DIR_PIN 6
#define ENABLE_PIN 4

#define M0 7
#define M1 8
#define M2 9

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

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

  //digitalWrite(M0, LOW);
  //digitalWrite(M1, LOW);
  //digitalWrite(M2, HIGH);

  // DEFINE FUNKEN TOKENS AND CALLBACKS
  fnk.listenTo("ENABLE", enableEx);
  fnk.listenTo("DISABLE", disableEx);
  fnk.listenTo("GOREL", goRelative);
  fnk.listenTo("GOABS", goAbsolute); // not implemented
  fnk.listenTo("SETRPM", setRPM);
  fnk.listenTo("CONTROT", continuousRotation);

  // COMPUTE MICROSECONDS DELAY BETWEEN STEPS
  steppingDelay = int(1000000/(stepsPerRev*(rpm/60.0)));
}

/*
   TYPICAL: CONTROT 1 or CONTROT [-1 or 1]
*/
void continuousRotation(char *c) {
  char *token = fnk.getToken(c);
  char *val = fnk.getArgument(c);

  int dir = atoi(val);
  if (dir >= 0) isForward = true;
  else isForward = false;

  isContinuous = true;
}


/*
   TYPICAL: SETRPM 60 or SETRPM [int]
*/
void setRPM(char *c) {
  char *token = fnk.getToken(c);
  char *val = fnk.getArgument(c);

  rpm = atoi(val);
  steppingDelay = int(1000000/(stepsPerRev*(rpm/60.0)));
}


void goAbsolute(char *c) {
  char *token = fnk.getToken(c);
  char *val = fnk.getArgument(c);
}

/*
   TYPICAL: GOREL 90 or GOREL [int]
*/
void goRelative(char *c) {
  char *token = fnk.getToken(c);
  char *val = fnk.getArgument(c);

  int degree = atoi(val);
  long steps = (stepsPerRev * degree) / 360;
  stepToGo += steps;

  isContinuous = false;
}


/*
   TYPICAL: ENABLE
*/
void enableEx(char *c) {
  char *token = fnk.getToken(c);

  digitalWrite(ENABLE_PIN, 0);
}

/*
   TYPICAL: DISABLE
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
  //delayMicroseconds(stepDelay);

  //////////////////////////////////////////////////////// WHAT IS THIS?!?!
  unsigned long dt = micros() - tdelta;
  if (dt < STEP_DELAY_SHOW) {
    delayMicroseconds(STEP_DELAY_SHOW - dt);
  }

  tdelta = micros();

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
