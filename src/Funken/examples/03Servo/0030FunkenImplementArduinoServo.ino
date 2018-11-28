#ifndef SERVO
#define SERVO
#include <Servo.h>
#endif

Servo fnkservo01;
Servo fnkservo02;
Servo fnkservo03;

int servoStartAngle = 90;

int angleToGo = 0;

void FunkenSetupImplementArduinoServo() {
  fnk.listenTo("ATTACHSRV", funkenAttachServo);
  fnk.listenTo("DETACHSRV", funkenDetachServo);
  fnk.listenTo("SRV", funkenGoServo);
}

/*
   THE FOLLOWING IS NOT A BEST PRACTICE IMPLEMENTATION DUE TO THE FACT THAT OUR SERVOS WERE SHIT

   TYPICAL: SRV 1 90 or SRV [0 to 2] [0 to 180]
*/
void funkenGoServo(char *c) {
  char *token = fnk.getToken(c);
  char *servObjNum = fnk.getArgument(c);
  char *angle = fnk.getArgument(c);

  int intServObjNum = atoi(servObjNum);
  int servoangle = atoi(angle);
  
  writeServo(intServObjNum, servoangle);
}

/*
   TYPICAL: ATTACHSRV 1 9 90 or ATTACHSRV [0 to 2] [pin] [0 to 180]
*/
void funkenAttachServo(char *c) {
  char *token = fnk.getToken(c);
  char *num = fnk.getArgument(c);
  char *pin = fnk.getArgument(c);
  char *angle = fnk.getArgument(c);

  int servoNum = atoi(num);
  int servoPin = atoi(pin);
  int servoAngle = atoi(angle);
  
  switch(servoNum){
    case 2:
      if(!fnkservo02.attached()){
        fnkservo02.attach(servoPin);
        fnkservo02.write(servoStartAngle);  
      }
    break;

    case 3:
      if(!fnkservo03.attached()){
        fnkservo03.attach(servoPin);
        fnkservo03.write(servoStartAngle);
      }
    break;

    default:
      if(!fnkservo01.attached()){
        fnkservo01.attach(servoPin);
        fnkservo01.write(servoStartAngle);
      }
      
  }
}

/*
   TYPICAL: DETACHSRV 1 or DETACHSRV [0 to 2]
*/
void funkenDetachServo(char *c) {
  char *token = fnk.getToken(c);
  char *num = fnk.getArgument(c);
  
  int servoNum = atoi(num);
  
  switch(servoNum){
    case 2:
      if(fnkservo02.attached()) fnkservo02.detach();
    break;

    case 3:
      if(fnkservo03.attached()) fnkservo03.detach();
    break;

    default:
      if(fnkservo01.attached()) fnkservo01.detach();
  }
}


void writeServo(int servNum, int angle) {
  angleToGo = angle;

  switch(servNum){
    case 2:
      fnkservo02.write(angleToGo);  
    break;

    case 3:
       fnkservo03.write(angleToGo);  
    break;

    default:
       fnkservo01.write(angleToGo);  
  }
}
