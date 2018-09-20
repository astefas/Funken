![Funken logo](https://github.com/astefas/Funken/blob/master/material/FunkenLogo.png?raw=true)
# Examples
Serial protocol toolkit for interactive prototyping (GPL) initiated by Alexander Stefas, astefas(at)googlemail.com

Funken is a [Arduino](https://www.arduino.cc) [library](https://www.arduino.cc/en/Reference/Libraries) that enables [callbacks](https://en.wikipedia.org/wiki/Callback_(computer_programming)) on an arduino. It is part of a workflow that simplifies communication between your Arduino and any host, that is able to speak to it via serial messages. The current version of Funken is doing this via a serial connection on a [serialport](https://en.wikipedia.org/wiki/Serial_port). It is designed to enable people to quickly define a serial [protocol](https://en.wikipedia.org/wiki/Communication_protocol) for their application and its physical computing parts without patronizing them in the use of hardware or the way they have to speak to these parts.

0. [How to use and understand this repository](https://github.com/astefas/Funken#usecases-for-funken)
1. [How to use Funken](https://github.com/astefas/Funken#how-to-use-funken)
   - [Calling a method without arguments via CLI](https://github.com/astefas/Funken#calling-a-method-without-arguments-via-cli)
   - [Calling a method with arguments via CLI](https://github.com/astefas/Funken#calling-a-method-with-arguments-via-cli)
   - [Calling a method from every software layer that can talk to a serialport](https://github.com/astefas/Funken/blob/master/README.md#calling-a-method-from-every-software-layer-that-can-talk-to-a-serialport)
   - [Mimicking Arduino's API on the serial port](https://github.com/astefas/Funken/blob/master/README.md#mimicking-arduinos-api-on-the-serial-port)
   - [Getting basic responses from Funken](https://github.com/astefas/Funken/blob/master/README.md#getting-basic-responses-from-funken)
   - [Changing the respond-protocol](https://github.com/astefas/Funken#changing-the-respond-protocol)

2. [Quickstart](https://github.com/astefas/Funken/blob/master/README.md#quickstart)

3. [Serialhosts for Funken](https://github.com/astefas/Funken/blob/master/README.md#serialhosts-for-funken)

4. [License](https://github.com/astefas/Funken/blob/master/README.md#license)

5. [References](https://github.com/astefas/Funken/blob/master/README.md#references)

## How to use and understand this repository
I am building tools. Tools that in the best case enable people to do something they were not able to do before. My main target is not to save somebody time and therefore money. I want to educate and enable people. I will provide examples and try to explain as good as I can. I help you in simplifying your technology. But I won't give you in-depth copy and paste examples of how to do a certain task. It is more likely that I explain to you the basics that are necessary to understand my workflow and how you can use it too. I will not teach you Arduino unless you want to. But in some cases a basic arduino knowledge might be necessary. Feel free to be informed [here:Getting Started](https://www.arduino.cc/en/Guide/HomePage) and [here:Tutorials](https://www.arduino.cc/en/Tutorial/HomePage). Please understand that this is a software repository and therefore does not cover electronics. If I do so in the future, this will be outside of GitHhub on a seperate webpage with tutorials or here in some example section but most likely without explaining the electronics.

## How to use Funken
Funken helps define [tokens](https://en.wikipedia.org/wiki/Token) that are mapped to specific parts of code in an [arduino sketch](https://www.arduino.cc/en/Tutorial/Sketch). You can compare this to [event based approaches](https://en.wikipedia.org/wiki/Event-driven_programming) or [callbacks](https://en.wikipedia.org/wiki/Callback_(computer_programming)) in e.g. [javascript](https://en.wikipedia.org/wiki/JavaScript). You define an event. When that event is triggered a [method](https://en.wikipedia.org/wiki/Method_(computer_programming)) is called. That [method](https://en.wikipedia.org/wiki/Method_(computer_programming)) might be called with an [object](https://en.wikipedia.org/wiki/Object_(computer_science)) as [argument](https://en.wikipedia.org/wiki/Parameter_(computer_programming)) representing in-detail information about that event and therefore enables a specific action to that event. Callbacks are mostly working the same way but without the loose coupling of an event based architecture.

Funken tries to be both. Simple string messages over the serialport trigger a method. Within that method additional information about the event are provided and could be used as [variables](https://en.wikipedia.org/wiki/Variable_(computer_science)). It is compatible with any arduino compatible hardware that is able to talk to a serial port and has at least 64 bytes of incoming serial buffer. It does not define a [protocol](https://en.wikipedia.org/wiki/Communication_protocol) like [firmata](https://github.com/firmata/arduino) which is heavily bound to the microcontroller that is used in your project. It helps define your own [protocol](https://en.wikipedia.org/wiki/Communication_protocol) which might not be as fast as a [firmata](https://github.com/firmata/arduino) based application but human readable and easy in implementation.

### Calling a method without arguments via CLI(Command Line Interface)
Let's assume you have a method called ```startAnimation``` and you want this to be triggered from the [arduino IDE](https://www.arduino.cc/en/Main/Software)'s own [commandline](https://de.wikipedia.org/wiki/Kommandozeile). No additional information needs to be given to that method as arguments.
```c++
/*
  Code inside an arduino .ino file
*/

void startAnimation(){
  //DO YOUR MAGIC
}
```

```c++
/*
  Code inside an arduino .ino file utilizing Funken
*/
#include <Funken.h>

Funken fnk;

void setup() {
  fnk.begin(57600,0, 0);
  fnk.listenTo("STARTANIMATION", startAnimation);
}

void loop(){
   /*
      this actually lets funken check for incoming bytes on the serial port
   */
   fnk.hark();
}

void startAnimation(char *c) {
  //DO YOUR MAGIG
  /*
  	//e.g. a set of digital commands like the following
	//stupid code on purpose ;-)
	//feels like using macros.
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
	
	digitalWrite(3, HIGH);
	digitalWrite(4, HIGH);
	digitalWrite(5, LOW);
	digitalWrite(6, HIGH);
	digitalWrite(7, LOW);
	
  */
}
```
Everytime you type ```CLI:>STARTANIMATION\nl``` inside a [console](https://en.wikipedia.org/wiki/System_console) and terminate that line with a [new line character](https://en.wikipedia.org/wiki/Newline), that information is routed to arduino's internal serial buffer from where Funken takes it and searches for registered strings. Could a string be found as a regsitered string associated with a method, the corresponding callback is called. The argument **c** that has been added to the startAnimation method can hold additional information that could be used to parametrize that method.
The way you call the above set of instructions - which could be anything - feels like playing back macros. These macros could be parametrized as explained later on. This was extremely useful in teaching when I wanted to provide a certain functionality to a student which uses any application as a tool and not for learning physical computing.
```
CLI:>STARTANIMATION\nl //arduino console configured to send a NEW LINE CHARACTER at [ENTER]
```
### Calling a method with arguments via CLI
Let's assume that you want to trigger your ```startAnimation``` actions in a loop for x times.
```c++
/*
  Code inside an arduino .ino file utilizing Funken
*/
#include <Funken.h>

Funken fnk;

void setup() {
  fnk.begin(57600,0, 0);
  fnk.listenTo("STARTANIMATION", startAnimation);
}

void loop(){
   /*
      this actually lets funken check for incoming bytes on the serial port
   */
   fnk.hark();
}

void startAnimation(char *c) {
  /*
    this is the command -> startAnimation
  */
  char *token = fnk.getToken(c);
  /*
    the first argument
  */
  char *firstArgument = fnk.getArgument(c);

  /*
    cast your argument to whatever type you need. in our case integer / int
  */
  int times = atoi(firstArgument);

  for(int i = 0; i < times; i++){
    //DO YOUR MAGIC
    //delay a bit
  }
}
```
```
CLI:>STARTANIMATION 5\nl //arduino console configured to send a NEW LINE CHARACTER at [ENTER]
```
To execute your callback 5 times is not a specific functionality of funken. You defined the logic around it yourself when you filled the startAnimation function with instructions and therefore also unwillingly defined a tiny [protocol](https://en.wikipedia.org/wiki/Communication_protocol). Funken helps you do that via making everything after the command  - the first string - that is seperated via a [SPACE] an argument. You want to give your RGB LED that is responsible for your animation a certain color?
```
CLI:>STARTANIMATION 5 255 0 128\nl //arduino console configured to send a NEW LINE CHARACTER at [ENTER]
```
You of course have to alter your code to something like this. It is **not** mandatory to actually use all arguments or even get them parsed from the incoming char array, which basically is a copy of the internal serial buffer. You migt want to skip the colors. If you want both to work you have to check against the existence of an argument and alter the code accordingly.
```c++
void startAnimation(char *c) {
  /*
    this is the command -> startAnimation
  */
  char *token = fnk.getToken(c);

  char *red = fnk.getArgument(c);
  char *green = fnk.getArgument(c);
  char *blue = fnk.getArgument(c);

  /*
    the first argument
  */
  char *firstArgument = fnk.getArgument(c);

  char *red = fnk.getArgument(c);
  char *green = fnk.getArgument(c);
  char *blue = fnk.getArgument(c);


  /*
    cast your argument to whatever type you need. in our case integer / int
  */
  int times = atoi(firstArgument);
  int redValue = atoi(red);
  int greenValue = atoi(green);
  int blueValue = atoi(blue);


  for(int i = 0; i < times; i++){
    //DO YOUR MAGIC AND USE THE INTEGER VALUES TO MAKE SOME COLOR
  }
}
```
### Calling a method from every software layer that can talk to a serialport
Because Funken uses strings for communictaion and [SPACE]s to bring that iformation in an order, it is easy to write an interface for Funken in a language or software setup that needs to talk to an arduino. There will be implementations for:
* [Rhino](https://www.rhino3d.com/) / [Grasshopper3D](https://www.grasshopper3d.com/)
* [NodeJS](https://nodejs.org/en/)
* [Processing](https://processing.org/)
* [Python](https://www.python.org/)

These are the ones that were developed because they were needed and share with you. Feel free to write your own implementations and share them with us. We will continue doing so in the future. Check back for [serialhosts available for Funken](https://github.com/astefas/Funken/blob/master/README.md#serialhosts-for-funken).

### Mimicking Arduino's API on the serial port
As you have seen in the explanantion above it is easy to call custom methods from the comandline. It is also fairly easy to get arguments passed to that callback. Therefore we can set digital pin modes and also write pins digitally HIGH or LOW. In theory we can mimick every Arduino method from its API.
```c++
/*
  Code inside an arduino .ino file utilizing Funken
*/
#include <Funken.h>

Funken fnk;

void setup() {
  fnk.begin(57600,0, 0);
  fnk.listenTo("PM", setPinMode);
  fnk.listenTo("DW", digitallyWritePin);
}

void loop(){
   /*
      this actually lets funken check for incoming bytes in the serial port
   */
   fnk.hark();
}

void setPinMode(char *c) {
  char *token = fnk.getToken(c); //->PM
  char *pinArgument = fnk.getArgument(c); //-> e.g. -> 13
  char *pinValue = fnk.getArgument(c) // -> e.g. -> 1

  int pin = atoi(pinArgument);
  int value = atoi(pinValue);

  //CALL ARDUINOS OWN PINMODE METHOD
  pinMode(pin, value);
}

void digitallyWritePin(char *c) {
  char *token = fnk.getToken(c); //->DW
  char *pinArgument = fnk.getArgument(c); //-> e.g. -> 13
  char *pinValue = fnk.getArgument(c) // -> e.g. -> 1

  int pin = atoi(pinArgument);
  int value = atoi(pinValue);

  //CALL ARDUINOS OWN DIGITALWRITE METHOD
  digitalWrite(pin, value);
}
```
This works as values like ```HIGH``` and ```LOW```, ```INPUT``` and ```OUTPUT``` are just [arduino constants](https://www.arduino.cc/reference/en/language/variables/constants/constants/) to a numeric value. In most cases values representing a boolen expression. Therefore casting them to an integer is fine. You also can see that the methods are identical except the call to the arduino method at the end. There are basic Arduino examples in the examples section. We made these examples so that when installing the funken library they are available in the [Arduino IDE](https://www.arduino.cc/en/Main/Software) as cheat sheets / best prcatices.

### Getting basic responses from Funken
Up until now all the examples were unidirectional. We were triggering actions on an arduino. This is sufficient for most actor based examples. We turn on LEDs or inform a servo motor to go to a certain rotation. For the examples that need to talk back to whoever is working with it, there is one function in funken, that does exactly this. Lets assume we want to have the meassurememnts of a basic analog sensor mirrored back to a console or host. There are countless ways of doing this. I am providing here the most basic example. Reading the input on an analog pin.
```c++
/*
  Code inside an arduino .ino file utilizing Funken
*/
#include <Funken.h>

Funken fnk;

void setup() {
  fnk.begin(57600,0, 0);
  fnk.listenTo("AR", analogReadPin);
}

void loop(){
   /*
      this actually lets funken check for incoming bytes in the serial port
   */
   fnk.hark();
}

void funkenAnalogRead(char *c) {
  char *token = fnk.getToken(c); //->AR
  char *pin = fnk.getArgument(c) // -> e.g. -> A0

  int pinint = atoi(pin);

  String answer = "";
  answer += pin;
  answer += String(" ");
  answer += analogRead(pinint);
  
  fnk.respond(token, answer);
}
```
On the console or CLI you would ask for that value like following:
```
CLI:>AR A0\nl //arduino console configured to send a NEW LINE CHARACTER at [ENTER]
```
Other than the previous examples this will mirror back on the console as following:
```
CLI:>AR A0\nl //arduino console configured to send a NEW LINE CHARACTER at [ENTER]
CLI:><F:0:AR:A0 505>
```
The answer that is coming from our funken enabled device now is structured in a simple way. The reason for this is for example that a lot of arduino libraries are giving innformation to the serial port without asking for it. So we have to structure our specific information in a way that is most unlikely used by another library. At least that is what I usually do. Every answer is packed in brackets. This way they are easily identifiable as a single piece of information. Besides a new line character that is send by the static Serial class when we close the brackets via ```Serial.println(">");```. This is not absolutely neccessary but helps for host implementations in a lot of programming languages. There are language implementations for the Serial Port which have methods that specifically read until a linebreak or newline. Also referred to as EOL - End Of Line.
```
CLI:><..>
```
The information inside the brackets are seperated by a ```:```. Easily parseable. First is an identifier. ```F``` for Funken. Second comes an ID that you could give your Funken device. You do this when you write this in your setup function.
```fnk.begin(57600,0, 0);```. This is handy if you have a multi device setup. Third parameter is the token you asked for ```AR``` and last is the actual reading with pin and value ```A0 505```.

### Changing the respond-protocol
All of my examples use the kind of formatted answer you can see above. It is an convention and open enough for me to do whatever I wanted to do. If you are capeable of doing so, you could change the format easily inside [Funken.cpp](https://github.com/astefas/Funken/blob/master/src/Funken/Funken.cpp). The parts resposnsible for it are the following ones. Please be aware that there are a bunch of [overloaded](https://en.wikipedia.org/wiki/Function_overloading) respond functions doing the same. The really interesting part is in the functions ```resStart``` and ```resEnd```. And also be aware that other software around Funken most likely uses this formatting.

```c++
//THERE ARE OVERLOADED FUNCTIONS FOR A LOT OF DATATYPES.
void Funken::respond(char *c, String value){
	resStart(c, id);
	Serial.print(value);
	resEnd();
}

void Funken::resStart(char *c, byte id){
	Serial.print("<F:");
	Serial.print(id);
	Serial.print(":");
	Serial.print(String(c));
	Serial.print(":");
}

void Funken::resEnd() {
	//COULD BE MORE "COMPLEX"
	Serial.println(">");
}
```

## Quickstart
Download the [/Funken](https://github.com/astefas/Funken/tree/master/src/Funken) folder from this repositories [/src](https://github.com/astefas/Funken/tree/master/src) folder and put it in your Arduino's IDE library folder. Please find detailed information on how-to install libraries for Arduino [here](https://www.arduino.cc/en/Guide/Libraries). Scroll down and read the part about "Manual installation". Well, if you are there already go and read the whole article.

For the release of Funken we provide basic examples in the [/examples](https://github.com/astefas/Funken/tree/master/src/Funken/examples) folder of the [/Funken](https://github.com/astefas/Funken/tree/master/src/Funken) folder. When you installed the Funken library correct, you can access these examples easily from within your Arduino IDE.
![example_examples](https://github.com/astefas/Funken/blob/master/material/examples.png)

## Serialhosts for Funken
* [GhFunken](https://github.com/ar0551/GhFunken) (Implementation for Funken Host in Rhino/Grasshopper3D) by [Andrea Rossi](https://github.com/ar0551)
* [PyFunken](https://github.com/ar0551/PyFunken) (Implementation for Funken Host in Python) by [Andrea Rossi](https://github.com/ar0551)
* FunkenJS (Implementation for Funken Host in NodeJS) by [Alexander Stefas](https://github.com/astefas)

## License
Funken: Serial Protocol Toolkit Arduinolibrary (GPL) initiated by Alexander Stefas

Copyright (c) 2018, Alexander Stefas

Funken is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License version 3.0 as published by the Free Software Foundation.

Funken is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Wasp; If not, see http://www.gnu.org/licenses/.

@license GPL-3.0 https://www.gnu.org/licenses/gpl.html

Significant parts of Funken have been developed by Alexander Stefas as part of research on democratizing tools at [DDU Digital Design Unit - Prof. Oliver Tessmann - Technische Universität Darmstadt](http://www.dg.architektur.tu-darmstadt.de/dg/startseite_3/index.de.jsp).

## References
[Stefas, A, Rossi, A and Tessmann, O. 2018. Funken: Serial Protocol Toolkit for Interactive Prototyping, In Proceedings of ECAADE 2018, Lodz. Poland](http://papers.cumincad.org/data/works/att/ecaade2018_388.pdf)
