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
 * ZHighPerfTimer.h
 *
 *  Created on: 19 févr. 2011
 *      Author: laurent
 */

#ifndef Z_ZHIGHPERFTIMER_H
#define Z_ZHIGHPERFTIMER_H

//#ifndef Z_ZHIGHPERFTIMER_H
//#  include "ZHighPerfTimer.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

class ZHighPerfTimer
{
    UELong StartTime;
    UELong EndTime;
    UELong IntermediateTime;
    UELong Result;

  public:

    ZHighPerfTimer()               { StartTime = EndTime = Result = IntermediateTime = 0; }

    static UELong GetActualTime(); // Return time counter in microseconds

    void Start() { StartTime = IntermediateTime = GetActualTime(); }

    ULong GetIntermediateResult()
    {
      return((ULong)(GetActualTime() - StartTime));
    }

    ULong GetIntermediateSlice()
    {
      UELong ActualTime;
      ULong  Result;

      ActualTime = GetActualTime();
      Result = (ULong)(ActualTime - IntermediateTime);
      IntermediateTime = ActualTime;

      return(Result);
    }

    void End()
    {
      EndTime = GetActualTime();
      Result = EndTime - StartTime;
      if (StartTime > EndTime) Result = 1; // Workaround for timechange causing problems.
    }

    ULong GetResult()
    {
      return((ULong)( Result));
    }

    UELong GetLongResult()
    {
      return(Result);
    }

};

#endif
