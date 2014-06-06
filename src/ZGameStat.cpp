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
 * ZGameStat.cpp
 *
 *  Created on: 16 oct. 2012
 *      Author: laurent
 */

#include "ZGameStat.h"
#include <stdio.h>

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

void ZGameStat::DoLogRecord()
{
  #if COMPILEOPTION_FINETIMINGTRACKING == 1
    Log_Buffer[0][BufferOffset] = FrameTime;
    Log_Buffer[1][BufferOffset] = SectorRefresh_Count;
    Log_Buffer[2][BufferOffset] = SectorRefresh_TotalTime;
    Log_Buffer[3][BufferOffset] = SectorRefresh_MaxTime;
    Log_Buffer[4][BufferOffset] = SectorRefresh_MinTime;
    Log_Buffer[5][BufferOffset] = SectorRender_Count;
    Log_Buffer[6][BufferOffset] = SectorRender_TotalTime;
    Log_Buffer[7][BufferOffset] = SectorRefresh_Waiting;

  BufferOffset++;
  #endif
}

bool ZGameStat::SaveLogToFile(char * FileSpec)
{
  FILE * fl;
  ULong i;
  ZString As;

  fl = fopen(FileSpec, "wb");
  if ((fl))
  {
    for (i=0;i<BufferOffset;i++)
    {
      As = Log_Buffer[0][i];
      As << "," << Log_Buffer[1][i];
      As << "," << Log_Buffer[2][i];
      As << "," << Log_Buffer[3][i];
      As << "," << Log_Buffer[4][i];
      As << "," << Log_Buffer[5][i];
      As << "," << Log_Buffer[6][i];
      As << "," << Log_Buffer[7][i];
      As << "\n";
      fwrite ( As.String, As.Len, 1, fl );
    }
    fclose (fl);
  }
  return(true);
}
