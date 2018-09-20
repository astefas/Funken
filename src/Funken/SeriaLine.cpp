/*
 * SeriaLine
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

#include "SeriaLine.h"

/*
  Constructor of class CommandList
  
  @desc The constructor of SeriaLine that searches incoming bytes for delimiters

  @def nl(newline) delimiter by default set to \n
  @def cr(carriage return) delimiter by default set to \r
*/
SeriaLine::SeriaLine():nl('\n'),cr('\r') {
  clear();
}

/*
  public
  SeriaLine:begin(long baudrate, byte v)

  @desc a method that has to be called in order to start listening for serial input

  @arg long baudrate The baudrate for serial communitation: be aware of certain baudrate incompatibilities with certain chipsets. the CH340 that is part of some NANO setups has a max baudrate of 576000

  @arg byte v A value either 0 or 1 that sets the verbose variable. A 1 will make this library talk back to you.

  @return VOID
*/
void SeriaLine::begin(long baudrate, byte v){
  Serial.begin(baudrate);
	while (!Serial) {;}
	verbose = v;

  /*
    reset
  */
  clear();
}

/*
  public
  SeriaLine::hark()

  @desc in order to listen for new incoming bytes you have to call this method in a through out your main loop
  this could be inside loop() or via a certain timer if you want to have more control over timing

  @return boolan TRUE or FALSE indicating a newly found line. This line is interpreted via the delimiters defined in the constructor
*/
boolean SeriaLine::hark() {
  boolean foundALine = false;

  if(Serial.available() > 0) {
    char available = Serial.read();

    byte XORNewLine = available ^ nl;
    byte XORCarriageReturn  = available ^ cr;

    if(XORNewLine != 0 && XORCarriageReturn != 0) {
      inputBuffer[inputBuffIndex] = available;
      inputBuffIndex += 1;

      if(inputBuffIndex >= BUFFER_SIZE - 1) clear();
    } else if(XORNewLine == 0) {
      inputBuffer[inputBuffIndex] = '\0';
      foundALine = true;
    }
  }

  return foundALine;
}

/*
  protected
  SeriaLine::clear()

  @desc a method resetting the index of read bytes to 0 and therefore prepares for new input and interpretation

  @return VOID
*/
void SeriaLine::clear() {
	inputBuffIndex = 0;
}
