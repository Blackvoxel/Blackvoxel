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
 * ZOs_Specific_EditCode.cpp
 *
 *  Created on: 24 févr. 2015
 *      Author: laurent
 */

#include "ZOs_Specific_EditCode.h"

#ifdef ZENV_OS_LINUX

#include <stdlib.h>
#include <unistd.h>

void SystemWithReturn(char * CommandLine)
{
  pid_t ChildPID;

  switch (ChildPID = fork())
  {
    case -1: // Fail
             break;
    case 0:  // Child
      execl("/bin/sh", "sh", "-c", CommandLine, NULL);
      exit(0);
      break;

    default: // Parent
             return;
  }
}

bool ZEditCode::EditCode(ZString * Tool, ZString * File)
{
  ZString DefaultTool, CommandLine;

  if ((!Tool) || (!Tool->Len))
  {
    DefaultTool = "gedit";
    Tool = &DefaultTool;
  }

  CommandLine << *Tool << " " << *File;

  SystemWithReturn(CommandLine.String);

  return(true);
}

#endif

#ifdef ZENV_OS_WINDOWS

#include "windows.h"

bool ZEditCode::EditCode(ZString * Tool, ZString * File)
{
  ZString DefaultTool, CommandLine, Parameters;

  if ((!Tool) || (!Tool->Len))
  {
    DefaultTool = "notepad.exe";
    Tool = &DefaultTool;
  }

  ShellExecute( 0,              // HWND hwnd,
                0,              // LPCTSTR lpOperation,
                Tool->String,   // LPCTSTR lpFile,
                File->String,   // LPCTSTR lpParameters,
                0,              // LPCTSTR lpDirectory,
                SW_SHOWNORMAL); // INT nShowCmd

  return(true);
}

#endif

#ifdef ZENV_OS_OSX

#include <stdlib.h>
#include <unistd.h>

void SystemWithReturn(char * CommandLine)
{
  pid_t ChildPID;

  switch (ChildPID = fork())
  {
    case -1: // Fail
             break;
    case 0:  // Child
      execl("/bin/sh", "sh", "-c", CommandLine, NULL);
      exit(0);
      break;

    default: // Parent
             return;
  }
}

bool ZEditCode::EditCode(ZString * Tool, ZString * File)
{
  ZString DefaultTool, CommandLine;

  if ((!Tool) || (!Tool->Len))
  {
    DefaultTool = "open -e";
    Tool = &DefaultTool;
  }

  CommandLine << *Tool << " " << *File;

  SystemWithReturn(CommandLine.String);

  return(true);
}

#endif
