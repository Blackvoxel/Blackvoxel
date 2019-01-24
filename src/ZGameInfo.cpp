/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2018 Laurent Thiebaut & Olivia Merle
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
 * ZGameInfo.cpp
 *
 *  Created on: 25 sept. 2018
 *      Author: laurent
 */

#include <stdio.h>

#ifndef Z_ZGAMEINFO_H
#  include "ZGameInfo.h"
#endif

#ifndef Z_ZSTREAMS_FILE_H
#  include "z/ZStream_File.h"
#endif

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

ZGameInfo::ZGameInfo()
{

}

bool ZGameInfo::Save(char * FileSpec)
{
  ZStream_File Stream;

  Stream.SetFileName(FileSpec);

  if (!Stream.OpenWrite()) return(false);

  Stream<<(ULong)0xB14C08E1;               // Magic Number
  Stream<<(ULong)COMPILEOPTION_VERSIONNUM; // Game Version
  Stream<<(ULong)3;                        // Format version
  Stream<<GameType;                        // Type of the Game
  Stream<<RobotNextSerial;                 // Next serial number for machines, robot, etc.
  Stream.Close();

  return(!Stream.IsError());
}

bool ZGameInfo::Load(char * FileSpec)
{
  ZStream_File Stream;

  Stream.SetFileName(FileSpec);

  if (!Stream.OpenRead()) return(false);

  ULong MagicCookie, Fv;

  Stream>>MagicCookie;          if (MagicCookie!=0xB14C08E1) { Stream.Close(); return(false); }
  Stream>>Previous_GameVersion;
  Stream>>Fv; // Format Version

  // If format is future and unsupported, stop here

  if (Fv>3) { Stream.Close(); return(false); }

  // Type of game

  if (Fv>=3) Stream>>GameType;
  else       GameType = 0;


  if (Fv>=2) Stream>>RobotNextSerial; // Next serial number for machines, robot, etc.
  else       RobotNextSerial = 0;

  Stream.Close();

  return(true);
}

ULong ZGameInfo::GetGameType(ULong UniverseNum)
{
  ZString FileName;
  ZGameInfo Gi;

  if (COMPILEOPTION_USEHOMEDIRSTORAGE) FileName = ZStream_File::Get_Directory_UserData();
  FileName.AddToPath(COMPILEOPTION_SAVEFOLDERNAME);
  FileName.AddToPath("Universes");
  FileName.AddToPath(UniverseNum).AddToPath("WorldInfo.dat");

  if (!Gi.Load(FileName.String)) return(-1);

  return(Gi.GameType);
}

bool ZGameInfo::GetWorldCustomDescription(ULong UniverseNum, ZString &OutDescription)
{
  ZString FileName;
  ZStream_File Stream;

  if (COMPILEOPTION_USEHOMEDIRSTORAGE) FileName = ZStream_File::Get_Directory_UserData();
  FileName.AddToPath(COMPILEOPTION_SAVEFOLDERNAME);
  FileName.AddToPath("Universes");
  FileName.AddToPath(UniverseNum).AddToPath("Description.txt");

  Stream.SetFileName(FileName.String);
  if (!Stream.GetFileContent(OutDescription)) return(false);

  return(true);
}


