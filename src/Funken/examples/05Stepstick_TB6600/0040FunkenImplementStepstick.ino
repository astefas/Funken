//
#define STEP_DELAY_SHOW 100

// BASE PINS
#define STEP_PIN 7
#define DIR_PIN 6
#define ENABLE_PIN 5

unsigned long tdelta = 0;
double rpm = 30;
long pulse = 800;
long gearRatio = 1;
long stepsPerRev = pulse * gearRatio;
long steppingDelay;

long currentStep = 0;
long stepToGo = 0;

boolean isEnabled = false;
boolean isForward = true;
boolean isContinuous = false;

void FunkenSetupImplementStepstick() {

  // SETUP PINS FOR THE STEPPER DRIVER
  /// STEP_PIN
  pinMode(STEP_PIN, OUTPUT);
  digitalWrite(STEP_PIN, LOW);
  /// DIR_PIN
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(DIR_PIN, HIGH);
  /// ENABLE PIN
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, HIGH);


  // DEFINE FUNKEN TOKENS AND CALLBACKS
  fnk.listenTo("EN", enableEx);
  fnk.listenTo("DIS", disableEx);
  fnk.listenTo("GOREL", goRelative);
  fnk.listenTo("GOABS", goAbsolute);
  fnk.listenTo("ABS", setAbsolute);
  fnk.listenTo("RPM", setRPM);
  fnk.listenTo("CROT", continuousRotation);
  fnk.listenTo("PULSE", setPulse);
  fnk.listenTo("GEAR", setGearBoxRatio);
  fnk.listenTo("POS", getPosition);

  steppingDelay = long(1000000 / (stepsPerRev * (rpm / 60.0)));
}


/*
   TYPICAL: POS
   Return: current position (long)
*/
void getPosition(char *c) {
  char *token = fnk.getToken(c);

  long currentPosition = long((double(currentStep) / stepsPerRev) * 360.0);
  long goalPosition = long((double(stepToGo) / stepsPerRev) * 360.0);

  String answer = F("");
  answer += currentPosition;
  answer += F(" ");
  answer += goalPosition;
  fnk.respond(token, answer);
}

/*
   TYPICAL: PULSE 200 or PULSE [int]
*/
void setPulse(char *c) {
  char *token = fnk.getToken(c);
  char *val = fnk.getArgument(c);

  pulse = atoi(val);
  stepsPerRev = pulse * gearRatio;
  steppingDelay = long(1000000 / (stepsPerRev * (rpm / 60.0)));
}


/*
   TYPICAL: GEAR 48 or GEAR [int]
*/
void setGearBoxRatio(char *c) {
  char *token = fnk.getToken(c);
  char *val = fnk.getArgument(c);

  gearRatio = atoi(val);
  stepsPerRev = pulse * gearRatio;
  steppingDelay = long(1000000 / (stepsPerRev * (rpm / 60.0)));
}


/*
   TYPICAL: CROT 1 or CROT [-1(backward) or 0(stop) or 1(forward)]
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
   TYPICAL: RPM 60 or RPM [int]
*/
void setRPM(char *c) {
  char *token = fnk.getToken(c);
  char *val = fnk.getArgument(c);

  rpm = atoi(val);
  steppingDelay = long(1000000 / (stepsPerRev * (rpm / 60.0)));
}


/*
   TYPICAL: GOABS 90 or GOABS [int]
*/
void goAbsolute(char *c) {
  char *token = fnk.getToken(c);
  char *val = fnk.getArgument(c);

  long degree = atoi(val);
  long steps = long((stepsPerRev * degree) / 360.0);

  stepToGo = steps;

  isContinuous = false;
}


/*
   TYPICAL: ABS 90 or ABS [int] (saves current position as the new absolute value)
*/
void setAbsolute(char *c) {
  char *token = fnk.getToken(c);
  char *val = fnk.getArgument(c);

  long degree = atoi(val);
  long steps = long((stepsPerRev * degree) / 360.0);

  currentStep = steps;
  stepToGo = steps;
}


/*
   TYPICAL: GOREL 90 or GOREL [int]
*/
void goRelative(char *c) {
  char *token = fnk.getToken(c);
  char *val = fnk.getArgument(c);

  long degree = atoi(val);
  long steps = long((stepsPerRev * degree) / 360.0);
  stepToGo = currentStep + steps;

  isContinuous = false;
}


/*
   TYPICAL: EN [NO ATTRIBUTES]
*/
void enableEx(char *c) {
  char *token = fnk.getToken(c);

  isEnabled = true;
  currentStep = 0;
  stepToGo = 0;

  digitalWrite(ENABLE_PIN, HIGH);
}


/*
   TYPICAL: DIS [NO ATTRIBUTES]
*/
void disableEx(char *c) {
  char *token = fnk.getToken(c);

  isEnabled = false;
  currentStep = 0;
  stepToGo = 0;

  digitalWrite(ENABLE_PIN, LOW);
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

  if (abs(currentStep) >= 2000000000) {
    long nRotations = long(double(currentStep) / stepsPerRev); // Number of rotations rounded off to the closest integer
    currentStep = currentStep - (nRotations * stepsPerRev); // Correct for rotations above 360 degrees
  }
}

void stepping() {
  if (isEnabled) {
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
}
