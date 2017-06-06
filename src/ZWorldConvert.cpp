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
 * ZWorldConvert.cpp
 *
 *  Created on: 21 juin 2011
 *      Author: laurent
 */

#include <dirent.h>
#include <stdio.h>
#include "ZWorldConvert.h"
#include "z/ZString.h"

void ZWorldConverter::SectorConvert(ZVoxelSector * Sector)
{
  ULong i, SectorSize;

  SectorSize = ZVOXELBLOCSIZE_X * ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_Z;

  for(i=0;i<SectorSize;i++)
  {
    Sector->Data[i] = ConversionTable[Sector->Data[i]];
  }
}


#ifdef ZENV_OS_LINUX
bool ZWorldConverter::WorldConvert(char const * Path)
{

  struct dirent * Entry;

  ZVoxelSector Sector;

  DIR * Directory;
  ZString FileSpec, FileName;

  if (!(Directory = opendir(Path))) { return(false); }
  while (( Entry = readdir(Directory) ))
  {
    FileName = Entry->d_name;
    FileSpec = Path; FileSpec << "/" << FileName;
    // printf("%s %x\n",Entry->d_name, Entry->d_type);
    if (Entry->d_type == DT_DIR)
    {


    }
    else if (Entry->d_type == DT_REG)
    {
      if (FileName.Lefts(6)=="Sector")
      {
        printf("Loading Sector %s\n", FileSpec.String);
        Sector.Load(0,FileSpec.String);
        SectorConvert(&Sector);
        Sector.Save(0,FileSpec.String);
      }
    }
  }

  closedir(Directory);
  return(true);
}
#endif
#ifdef ZENV_OS_WINDOWS

// Not yet implemented on windows. These fucking "standards headers" don't have same function subsets accross platforms...
bool ZWorldConverter::WorldConvert(char const * Path)
{
  return(false);
}
#endif
#ifdef ZENV_OS_OSX
bool ZWorldConverter::WorldConvert(char const * Path)
{

  struct dirent * Entry;

  ZVoxelSector Sector;

  DIR * Directory;
  ZString FileSpec, FileName;

  if (!(Directory = opendir(Path))) { return(false); }
  while (( Entry = readdir(Directory) ))
  {
    FileName = Entry->d_name;
    FileSpec = Path; FileSpec << "/" << FileName;
    // printf("%s %x\n",Entry->d_name, Entry->d_type);
    if (Entry->d_type == DT_DIR)
    {


    }
    else if (Entry->d_type == DT_REG)
    {
      if (FileName.Lefts(6)=="Sector")
      {
        printf("Loading Sector %s\n", FileSpec.String);
        Sector.Load(0,FileSpec.String);
        SectorConvert(&Sector);
        Sector.Save(0,FileSpec.String);
      }
    }
  }

  closedir(Directory);
  return(true);
}
#endif
