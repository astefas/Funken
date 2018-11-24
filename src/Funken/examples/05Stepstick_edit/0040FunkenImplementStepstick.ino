#define STEP_DELAY_SHOW 100
#define STEP_PIN 5
#define DIR_PIN 6
#define ENABLE_PIN 4

#define M0 7
#define M1 8
#define M2 9

unsigned long tdelta = 0;
int extrusionSpeed = 500;
long stepsPerRev = 200;

long currentStep = 0;
long stepToGo = 0;

boolean isForward = true;

boolean allowedToExtrude = true;

void FunkenSetupImplementStepstick() {
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

  fnk.listenTo("ENABLE", enableEx);
  fnk.listenTo("DISABLE", disableEx);
  fnk.listenTo("GO", go);
  //fnk.listenTo("FORWARD", ffw);
  //fnk.listenTo("BACKWARD", rv);
}

void go(char *c) {
  char *token = fnk.getToken(c);
  char *val = fnk.getArgument(c);

  int degree = atoi(val);
  long steps = (stepsPerRev * degree) / 360;
  
  stepToGo += steps;
}

void enableEx(char *c) {
  char *token = fnk.getToken(c);

  digitalWrite(ENABLE_PIN, 0);
}

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

  unsigned long dt = micros() - tdelta;
  if (dt < STEP_DELAY_SHOW) {
    delayMicroseconds(STEP_DELAY_SHOW - dt);
  }

  tdelta = micros();

  if (isForward) currentStep += 1;
  else currentStep -= 1;
}

void stepping() {
  long steppingDelta = stepToGo - currentStep;

  if (steppingDelta > 0) {
    if (!isForward) forward();
    step(extrusionSpeed);
  } else if (steppingDelta < 0) {
    if (isForward) backward();
    step(extrusionSpeed);
  }
}
