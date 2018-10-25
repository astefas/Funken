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

#include "SeriaLine.h"

//max 64 byte input buffer on an arduino uno
#define COMMAND_SIZE 32

class CommandList : public SeriaLine {
	public:
    CommandList();
    void listenTo(const char *com, void(*callback)(char *));
    void process(char *buff);
    void unhandled(void (*function)(char *));
    void verbosed(void (*function)(char *));
    void hark();
    struct CB {
      char com[COMMAND_SIZE];
      void (*callback)(char *);
    };
    CB *callbacks;
    byte numOfCallbacks;
    char *p;
    char* getToken(char *c);
    char* getArgument(char *c);
		char* getRemaining();
		byte verbose;

  protected:
		void cleanup();
    void (*unhandlerCB)(char *);
    void (*verboseCB)(char *);
    char lastInputBuffer[BUFFER_SIZE];
    char *prev;
		char delPattern[3] = {'_', '<', '*'};
};
