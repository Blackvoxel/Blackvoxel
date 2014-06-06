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
 * ZLog.cpp
 *
 *  Created on: 20 sept. 2013
 *      Author: laurent
 */

#include <stdio.h>

#ifndef Z_ZLOG_H
#  include "ZLog.h"
#endif

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

#ifndef Z_ZSTREAMS_FILE_H
#  include "z/ZStream_File.h"
#endif

void ZLog::Log(ULong MessageID, TInfoType InfoType, const char * String)
{
  ZString Str = const_cast<char *>(String);
  Log(MessageID, InfoType, Str);
}

void ZLog::Log(ULong MessageID, ZLog::TInfoType InfoType, ZString & String)
{
  LogString << String;
  ZString As;

  // If logfile is not opened then open it.
  if (Stream==0)
  {
    ZStream_File * FileStream;
    FileStream = new ZStream_File;
    FileStream->SetFileName(OutputFilespec.String);
    if (!FileStream->OpenWrite()) delete FileStream;
    else Stream = FileStream;
  }

  // If logfile creation failled, then retry to do in the home directory.
  if (Stream==0)
  {
    ZString Filespec;
    ZStream_File * FileStream;

    FileStream = new ZStream_File;
    Filespec = FileStream->Get_Directory_UserData();
    Filespec.AddToPath("Blackvoxel_Log.txt");
    FileStream->SetFileName(Filespec.String);
    if (!FileStream->OpenWrite()) delete FileStream;
    else Stream = FileStream;
  }


  if (Stream!=0)
  {
    As.Clear() << "[" << Section << "." << MessageID << "]";
    if (COMPILEOPTION_VERBOSELOGS || InfoType == FAIL || InfoType == IMINFO)
    {
      As << " " << String.String ;
    }
    (*Stream) << As.String << String.NewLine();
    Stream->Flush();
    if (InfoType == FAIL) puts(As.String);
  }

}
