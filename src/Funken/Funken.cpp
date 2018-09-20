/*
 * FUNKEN
 *
 * Copyright (C) 2017 Alexander Stefas, for DDU, TU-Darmstadt
 * Funken: Serial protocol toolkit (GPL) initiated by Alexander Stefas
 *
 * Parts of Funken have been developed by Alexander Stefas
 * as part of research on democratic machines at:
 * DDU Digital Design Unit - Prof. Oliver Tessmann
 * Technische Universität Darmstadt
 *
 * This file is part of Funken.
 *
 * Copyright (c) 2017, Alexander Stefas <alexander.stefas@stefas.de>
 * Funken is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 3 of the License,
 * or (at your option) any later version.
 *
 * Funken is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wasp; If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0 <https://www.gnu.org/licenses/gpl.html>
 */

#include "Funken.h"

/*
  Constructor of class CommandList
  public
  Funken_h

  extending

  public
  CommandList

  @desc The constructor of Funken

  @def id identification for up to 256 devices. a ormal funken setup is one device
*/
Funken::Funken():id(0) {
	/*
		reset
	*/
	clear();
}

/*
  public
  Funken::begin(long baudrate, byte v, byte ID)

  @desc kind of overloaded method from SeriaLine but with an additional ID as an argument. Calling inherited method begin from CommandList, inherited from SeriaLine

	@arg long baudrate The baudrate for serial communitation: be aware of certain baudrate incompatibilities with certain chipsets. the CH340 that is part of some NANO setups has a max baudrate of 576000

  @arg byte v A value either 0 or 1 that sets the verbose variable. A 1 will make this library talk back to you.

	@arg byte ID  an identifier in the range of 0 to 255

  @return void
*/
void Funken::begin(long baudrate, byte v, byte ID){
	CommandList::begin(baudrate, v);
	id = ID;
}

/*
  public
  Funken::process(char *buff)

  @desc kind of overloaded method for inherited method from CommandList. Calls CommandList.process after a String has been fromatted like a char array and processed as if it was coming over Serialport.

  @arg STring str a standard ardiuno string to be treated like a charr array

  @return void
*/
void Funken::process(String str){
		//making char arrays the Arduino way and avoiding to include string.h
	  str.toCharArray(stringInputBuffer, BUFFER_SIZE);

		/*
			This is what is needed to treat the String inpout as a standard string from string.h
		*/
		stringInputBuffer[str.length()] = '\0';

		CommandList::process(stringInputBuffer);
}

/*
  public
  Funken::isId(byte ID)

  @desc helper method to check for e.g. an incoming char array in a multiarduino / multifunken setup. checks for equality of a id with the internal id. basically more readable than the standard comparisson of x == x

  @arg byte ID the id to check against the internal id

  @return boolean
*/
boolean Funken::isID(byte ID) {
	return (id==ID);
}

/*
  protected
  Funken::resStart(char *c, byte id)

  @desc internal helper method for making Funken responses (e.g. sensordata) more structured. it is possible to alter this in order to completely change the way funken structurizes its responses. writes directly to the serialport.

  @arg char *c the token as an identifier for the message

	@arg byte id the id of the funken instance

  @return void
*/
void Funken::resStart(char *c, byte id){
	Serial.print("<F:");
	Serial.print(id);
	Serial.print(":");
	Serial.print(String(c));
	Serial.print(":");
}

/*
  protected
	Funken::resEnd()

  @desc internal helper method for making Funken responses (e.g. sensordata) more structured. it is possible to alter this in order to completely change the way funken structurizes its responses. writes directly to the serialport.

  @return void
*/
void Funken::resEnd() {
	//COULD BE MORE "COMPLEX"
	Serial.println(">");
}

/*
  protected
	Funken::respond(char *c, byte value)

  @desc overloaded method for writing different responses with different datatypes. writes directly to the serialport

	@arg char c* the original token responsible for selecting the CallBack
	@arg byte value the data as a byte

  @return void
*/
void Funken::respond(char *c, byte value) {
	resStart(c, id);
	Serial.print(value);
	resEnd();
}

/*
  protected
	Funken::respond(char *c, byte value)

  @desc overloaded method for writing different responses with different datatypes. writes directly to the serialport

	@arg char c* the original token responsible for selecting the CallBack
	@arg int value the data as an integer

  @return void
*/
void Funken::respond(char *c, int value) {
	resStart(c, id);
	Serial.print(value);
	resEnd();
}

/*
  protected
	Funken::respond(char *c, byte value)

  @desc overloaded method for writing different responses with different datatypes. writes directly to the serialport

	@arg char c* the original token responsible for selecting the CallBack
	@arg double value the data as a double

  @return void
*/
void Funken::respond(char *c, double value){
	resStart(c, id);
	Serial.print(value);
	resEnd();
}

/*
  protected
	Funken::respond(char *c, byte value)

  @desc overloaded method for writing different responses with different datatypes. writes directly to the serialport

	@arg char c* the original token responsible for selecting the CallBack
	@arg long value the data as a long

  @return void
*/
void Funken::respond(char *c, long value){
	resStart(c, id);
	Serial.print(value);
	resEnd();
}

/*
  protected
	Funken::respond(char *c, byte value)

  @desc overloaded method for writing different responses with different datatypes. writes directly to the serialport

	@arg char c* the original token responsible for selecting the CallBack
	@arg float value the data as a float

  @return void
*/
void Funken::respond(char *c, float value){
	resStart(c, id);
	Serial.print(value);
	resEnd();
}

/*
  protected
	Funken::respond(char *c, byte value)

  @desc overloaded method for writing different responses with different datatypes. writes directly to the serialport

	@arg char c* the original token responsible for selecting the CallBack
	@arg String value the data as a String

  @return void
*/
void Funken::respond(char *c, String value){
	resStart(c, id);
	Serial.print(value);
	resEnd();
}
