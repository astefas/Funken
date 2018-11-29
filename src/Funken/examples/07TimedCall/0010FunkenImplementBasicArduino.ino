void FunkenSetupImplementBasicArduino() {
  fnk.listenTo("PM", funkenPinMode);
  fnk.listenTo("DW", funkenDigitalWrite);
  fnk.listenTo("DR", funkenDigitalRead);
  fnk.listenTo("AW", funkenAnalogWrite);
  fnk.listenTo("AR", funkenAnalogRead);
}

/*
   TYPICAL: PM 13 1 or PM [pin] [1 or 0]
*/
void funkenPinMode(char *c) {
  char *token = fnk.getToken(c);
  char *pin = fnk.getArgument(c);
  char *state = fnk.getArgument(c);

  int pinint = atoi(pin);
  int stateint = atoi(state);

  pinMode(pinint, stateint);
}

/*
   TYPICAL: DR 13 or DR [pin]
*/
void funkenDigitalRead(char *c) {
  char *token = fnk.getToken(c);
  char *pin = fnk.getArgument(c);

  int pinint = atoi(pin);

  byte debug = 0;

  String answer = F("");
  answer += pin;
  answer += F(" ");
  answer += digitalRead(pinint);
  fnk.respond(token, answer);
}

/*
   TYPICAL: DW 9 1 or DW [pin] [0 or 1]
*/
void funkenDigitalWrite(char *c) {
  char *token = fnk.getToken(c);
  char *pin = fnk.getArgument(c);
  char *state = fnk.getArgument(c);

  int pinint = atoi(pin);
  int stateint = atoi(state);

  digitalWrite(pinint, stateint);
}


/*
   TYPICAL: AW 9 100 or AW [pin] [0 or 255]
*/
void funkenAnalogWrite(char *c) {
  char *token = fnk.getToken(c);
  char *pin = fnk.getArgument(c);
  char *value = fnk.getArgument(c);

  int pinint = atoi(pin);
  int valueint = atoi(value);

  analogWrite(pinint, valueint);
}


/*
   TYPICAL: AR A0 or AR [A0 to A6]
*/
void funkenAnalogRead(char *c) {
  char *token = fnk.getToken(c);
  char *pin = fnk.getArgument(c);

  int pinint = atoi(pin);

  byte debug = 0;

  String answer = "";
  answer += pin;
  answer += String(" ");
  answer += analogRead(pinint);

  fnk.respond(token, answer);
}
