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

#ifndef Funken_h
#define Funken_h

#include "CommandList.h"

class Funken : public CommandList {
	public:
    Funken();
    void begin(long baudrate, byte v, byte ID);
    void process(String str);
    void respond(char *c, byte value);
    void respond(char *c, int value);
    void respond(char *c, double value);
    void respond(char *c, long value);
    void respond(char *c, float value);
    void respond(char *c, String value);
		boolean isID(byte id);
    byte id;

  protected:
    void resStart(char *c, byte id);
    void resEnd();
    char stringInputBuffer[BUFFER_SIZE];
};

#endif //Funken_h
