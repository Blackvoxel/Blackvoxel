/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2017 Laurent Thiebaut & Olivia Merle
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
 * ZSpinLock.cpp
 *
 *  Created on: 30 déc. 2016
 *      Author: laurent
 */


#include "ZSpinLock.h"

#ifdef ZENV_OS_LINUX
#  include <sched.h>
#endif

#ifdef ZENV_OS_WINDOWS
#  include "Windows.h"
#endif

#ifdef ZENV_OS_OSX
#  include <sched.h>
#endif

void ZSpinLock::GoSleep()
{
#ifdef ZENV_OS_LINUX
  sched_yield();
#endif

#ifdef ZENV_OS_WINDOWS
  Sleep(0); // Give the remaining time quantum to the system.
#endif

#ifdef ZENV_OS_OSX
  sched_yield();
#endif
}



