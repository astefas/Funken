// import of Funken
#include <Funken.h>

#define ledPin 13

// instantiation of Funken
Funken fnk;

/*
 * THE SETUP
 */
void setup() {
  pinMode(ledPin, OUTPUT);

  fnk.begin(57600,1,1);
x
  fnk.listenTo("BLINK", funkenBlink);

  fnk.verbosed(funkenVerboseOutput);
  fnk.unhandled(funkenUnhandledSerialInput);
}

/*
 * THE LOOP
 */
void loop() {
    fnk.hark();
}

void funkenBlink(char *c) {
  char *token = fnk.getToken(c);

  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
  digitalWrite(ledPin, HIGH);
}

/*
 * TYPICAL: SOMETHING NOT IN THE COMANDLIST. YOU CALL e.g. "blink" instead of "BLINK"
 */
void funkenUnhandledSerialInput(char *c) {
  fnk.respond(c, String("!H"));
}

/*
 * WE MIGHT NEED A MORE HANDSHAKE KIND OF BEHAVIOUR. SO THIS IS THE MESSAGE THAT MIRROS BEACK COMMANDS AND SHOWS THAT THEY ERE EXECUTED
 */
void funkenVerboseOutput(char *c) {
  fnk.respond(c, String("H"));
}
