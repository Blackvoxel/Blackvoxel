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
 * ZHighPerfTimer.cpp
 *
 *  Created on: 19 févr. 2011
 *      Author: laurent
 */

#include "ZHighPerfTimer.h"
#include <sys/time.h>

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif


#if COMPILEOPTION_WINXPTIMECOMPUTING==1

#include "windows.h"

UELong ZHighPerfTimer::GetActualTime()
{
  LARGE_INTEGER tick, freq, Res;
    Res.QuadPart = 1000000;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&tick);
    return ((tick.QuadPart * Res.QuadPart) / freq.QuadPart);
}

#else

UELong ZHighPerfTimer::GetActualTime()
{
  timeval t;

  UELong time;

  gettimeofday(&t,0);

  time = ((UELong)t.tv_sec * 1000000ULL) + (UELong)t.tv_usec;

  return(time);
}

#endif
