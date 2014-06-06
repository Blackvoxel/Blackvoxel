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
 * ZWorld_Stat.cpp
 *
 *  Created on: 11 févr. 2013
 *      Author: laurent
 */

#include <string.h>

#ifndef Z_ZWORLD_STAT_H
#  include "ZWorld_Stat.h"
#endif


void ZWorld_Stat::Get_Stat_DistanceToMineForVoxelType(UShort VoxelType)
{
  ZVoxelSector * Sector;
  UShort Voxel;
  ULong * StatTable;
  ULong Distance, RemainVoxels,i, StatZoneSize;
  double EntryCount;

  StatZoneSize = 131072;
  StatTable = new ULong[StatZoneSize];
  for (i=0;i<StatZoneSize;i++) StatTable[i]=0;
  Sector = World->SectorList;
  RemainVoxels = ZVOXELBLOCSIZE_X*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_Z;
  Distance = 0;
  EntryCount = 0;
  i=0;

  while (true)
  {
    Voxel = Sector->Data[i];
    if (Voxel == VoxelType)
    {
      if (Distance >= StatZoneSize) Distance = StatZoneSize-1;
      StatTable[Distance]++;
      Distance = 0;
      EntryCount += 1.0;
    }

    Distance++; i++; RemainVoxels--;
    if (!RemainVoxels)
    {
      bool Repeat = true; ;
      do
      {
        Sector = Sector->GlobalList_Next;
        if (!Sector) break;
        for (i=0;i<ZVOXELBLOCSIZE_X*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_Z;i++) {if (Sector->Data[i]!=0) {Repeat = false; break;} }
      } while (Repeat);
      if (!Sector) break;

      RemainVoxels = ZVOXELBLOCSIZE_X*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_Z;
      i = 0;
    }
  }

  // Find the first bound;

  Long Start, End,j;
  for (Start=0 ; Start<(Long)StatZoneSize ; Start++) { if (StatTable[Start]!=0) break; }
  for (End =StatZoneSize - 1 ; End < 0 ; End--) { if (StatTable[End]!=0) break; }

  printf(">Stats for VoxelType : %d\n",(ULong)VoxelType);
  for (j = Start; j<=End; j++) if (StatTable[j]>0) printf("[%d]=%d (%lf %%)\n", j, StatTable[j], 100.0 / EntryCount * ((double)StatTable[j]) );

  delete StatTable;
}
