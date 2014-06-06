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
 * ZGameStat.h
 *
 *  Created on: 16 oct. 2012
 *      Author: laurent
 */

#ifndef Z_ZGAMESTAT_H
#define Z_ZGAMESTAT_H

// #ifndef Z_ZGAMESTAT_H
// #  include "ZGameStat.h"
// #endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

class ZGame;

class ZGameStat
{
    ZGame * GameEnv;
    ULong * Log_Buffer[64];
    ZMemSize BufferOffset;
  public:
    ULong FrameTime; // Frame time in milliseconds

    ULong SectorRefresh_Count;
    ULong SectorRefresh_TotalTime;
    ULong SectorRefresh_MaxTime;
    ULong SectorRefresh_MinTime;
    ULong SectorRefresh_Waiting;

    ULong SectorRender_Count;
    ULong SectorRender_TotalTime;

    ZGameStat()
    {
      ULong i;

      FrameTime = 0;
      SectorRefresh_Count = 0;
      SectorRefresh_TotalTime = 0;
      SectorRefresh_MaxTime = 0;
      SectorRefresh_MinTime = 0;
      SectorRender_Count = 0;
      SectorRender_TotalTime = 0;
      SectorRefresh_Waiting = 0;
      GameEnv = 0;
      BufferOffset = 0;

      for (i=0;i<64;i++) Log_Buffer[i] = 0;
      #if COMPILEOPTION_FINETIMINGTRACKING == 1
        for (i=0;i<8;i++) {Log_Buffer[i] = new ULong[1000000]; if (!Log_Buffer[i]) throw(0);}
      #endif
    }

    ~ZGameStat()
    {
      ULong i;

      for ( i=0; i<64; i++ ) if (Log_Buffer[i]) {delete Log_Buffer[i]; Log_Buffer[i] = 0; }
    }

    bool Start()
    {
      return(true);
    }

    bool End()
    {
      #if COMPILEOPTION_FINETIMINGTRACKING == 1
      SaveLogToFile((char *) "/home/laurent/workspace/blackvoxel/log_framerate.txt");
      #endif
      return(true);
    }

    void DoLogRecord();

    bool SaveLogToFile(char * FileSpec);

};

#endif /* Z_ZGAMESTAT_H */
