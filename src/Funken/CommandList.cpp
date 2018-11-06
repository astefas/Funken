/*
 * CommandList
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

#include "CommandList.h"

/*
  Constructor of class CommandList
  public
  CommandList

  extending

  public
  SeriaLine

  @desc The constructor of CommandList that adds the CallBack functionlaity to SeriaLine

  @def numOfCallbacks the initial number of callbacks. Of course 0
  @def prev set to NULL because no messages have been processed
  œdef verbose set to 0 because we normally don't wnat to fill up buffers with unneccessary data
*/
CommandList::CommandList():numOfCallbacks(0),prev(NULL),verbose(0) {

  clear();
}

/*
  public
  CommandList::unhandled(void (*function)(char *))

  @desc this method is called when a a serial message has been identified but could not be associated with a callback.
  reasons for this could be typos or simply not registered commands

  @arg void (*function)(char *) a method that takes a char array as an input without any return type

  @return VOID
*/
void CommandList::unhandled(void (*function)(char *)) {
  unhandlerCB = function;
}

/*
  public
  CommandList::verbosed(void (*function)(char *))

  @desc this method is called everytime a command has been identified and could be routed to a callback function. use this if you have to check whether a command was eecuted

  @arg void (*function)(char *) a method that takes a char array as an input without any return type

  @return VOID
*/
void CommandList::verbosed(void (*function)(char *)) {
  verboseCB = function;
}

/*
  public
  CommandList::listenTo(const char *com, void(*callback)(char *))

  @desc this method is called everytime a command has been identified and could be routed to a callback function. use this if you have to check whether a command was executed. This method uses dynamic mempory allocation and therefore should be used with caution. Doing this at the very beginning of runtime should not result in any problems.

  @arg void (*function)(char *) a method that takes a char array as an input without any return type. it gets a charr arrax

  @return VOID
*/
void CommandList::listenTo(const char *com, void(*callback)(char *)){
	callbacks = (CB *) realloc(callbacks, (numOfCallbacks + 1) * sizeof(CB));
	strncpy(callbacks[numOfCallbacks].com, com, COMMAND_SIZE);
	callbacks[numOfCallbacks].callback = callback;
  numOfCallbacks++;
}

/*
  public
  CommandList::getToken(char *c)

  @desc This method is used inside a registered callback. It needs to get called before any calls to getARgument as the charr arrays are tokenized from an char array buffer. It is basically the first set of chars until the

  @arg char *c The char arry that was given as an argument to the callback function.

  @return char array
*/
char* CommandList::getToken(char *c){
  return strtok_r(c, " ", &p);
}

/*
  public
  CommandList::getToken(char *c)

  @desc This method is used inside a registered callback. It needs to get called before any calls to getARgument as the char arrays are tokenized from an char array buffer

  @arg char *c The char arry that was given as an argument to the callback function

  @return char array
*/
char* CommandList::getArgument(char *c){
  return strtok_r(NULL, " ", &p);
}

/*
  public
  CommandList::getRemaining()

  @desc This method returns any not tokenized parts of the internal char array buffer. could be useful if you want to programatically check if there needs to be any action on data delivered to a callback. Could also be used to route parts of messages to e.g. I2C communication

  @return char array
*/
char* CommandList::getRemaining(){
  return p;
}

/*
  public
  CommandList::hark()

  @desc in order to listen for new incoming bytes you have to call this method in a through out your main loop
  this could be inside loop() or via a certain timer if you want to have more control over timing. This is done via calling the inherited method from SeriaLine. Was the search for the message delimiting ascii character succesful the internal buffer is processed in the process method

  @return void
*/
void CommandList::hark() {
  boolean foundALine = SeriaLine::hark();
  if(foundALine) process(inputBuffer);
}

/*
  public
  CommandList::process(char *buff)

  @desc this method searches for the first token in the char buffer and compares this token with reistered char buffers associated with callbak methods

  @arg char *buff normally the internal buffer that is searched. could be a generic buffer that was generated in a method or a buffer that has already been processed and gets rerouted again.

  @return void
*/
void CommandList::process(char *buff){
	strncpy(lastInputBuffer, buff, BUFFER_SIZE);
	char *token = strtok_r(lastInputBuffer, " ", &prev);

	boolean isCB = false;

	for(int i=0; i < numOfCallbacks; i++){
		if(strcmp(token, callbacks[i].com) == 0){
      if(verbose) verboseCB(buff);

      (*callbacks[i].callback)(buff);
      isCB = true;

			break;
		}
	}

	if(!isCB) if(unhandlerCB) unhandlerCB(buff);

  /*
   reset
  */
	clear();
}
