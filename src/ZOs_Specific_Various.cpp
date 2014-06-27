/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2014 Laurent Thiebaut & Olivia Merle
 *
 * Blackvoxel is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Blackvoxel is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
 * ZOs_Specific_Various.cpp
 *
 *  Created on: 24 juin 2014
 *      Author: laurent
 */


#ifndef Z_ZOS_SPECIFIC_VARIOUS_H
#  include "ZOs_Specific_Various.h"
#endif

#ifdef ZENV_OS_WINDOWS

#include "windows.h"
#include <stdio.h>

// This function fix a problem with SDL which redirect terminal output to files in Windows version.
// Should be coupled with the -mwindows flag on mingw g++ linker.

  void Windows_DisplayConsole()
  {
    // Allocate a console for displaying texts.

    AllocConsole();

    // Redirect the standard terminal output to terminal.

    freopen( "CON", "w", stdout );
    freopen( "CON", "w", stderr );

  }


#endif
