/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2015 Laurent Thiebaut & Olivia Merle
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
 * ZOs_Specific_ViewDoc.cpp
 *
 *  Created on: 4 janv. 2015
 *      Author: laurent
 */

#include "ZOs_Specific_ViewDoc.h"

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif


#ifdef ZENV_OS_LINUX

#include <stdlib.h>
#include <unistd.h>

extern void SystemWithReturn(char * CommandLine);


bool ZViewDoc::ViewDocPage(ULong Number, bool Online)
{
  ZString URL;
  Online = true;
  if (Number)
  {
    if (Online) URL.Clear() << "xdg-open http://www.blackvoxel.com/view.php?node=" << Number;
    else        URL.Clear() << "xdg-open manual/page_" << Number << ".htm";

   // Development mode local test server
   #if DEVELOPPEMENT_ON == 1
    ZString LocalServer;
    if (LocalServer.LoadFromFile("LocalServer.txt"))
    {
      LocalServer.StripTrailling(0xa);
      URL.Clear() << "xdg-open " << LocalServer << Number;
    }
   #endif

  }
  else // Main manual page
  {
    if (Online) URL.Clear() << "xdg-open http://www.blackvoxel.com/view.php?node=1251" << Number;
    else        URL.Clear() << "xdg-open manual/index.htm";
  }
  SystemWithReturn(URL.String);

  return(true);
}

#endif

#ifdef ZENV_OS_WINDOWS

#include "windows.h"

bool ZViewDoc::ViewDocPage(ULong Number, bool Online)
{
  ZString URL;

  if (Number)
  {
    URL.Clear() << "http://www.blackvoxel.com/view.php?node=" << Number;
  }
  else // Main manual page
  {
    URL.Clear() << "http://www.blackvoxel.com/view.php?node=1251" << Number;
  }

  ShellExecute( 0,              // HWND hwnd,
                0,              // LPCTSTR lpOperation,
                URL.String,     // LPCTSTR lpFile,
                0,              // LPCTSTR lpParameters,
                0,              // LPCTSTR lpDirectory,
                SW_SHOWNORMAL); // INT nShowCmd

  return(true);
}

#endif
