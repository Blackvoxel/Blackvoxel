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
 * ZSpinlock.h
 *
 *  Created on: 30 déc. 2016
 *      Author: laurent
 */

#ifndef Z_ZSPINLOCK_H
#define Z_ZSPINLOCK_H

//#ifndef Z_ZSPINLOCK_H
//#  include "SpinLock.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

class ZSpinLock
{
  volatile ZMemSize Lock;

  void GoSleep();
  public:

  ZSpinLock()
  {
    Lock = 0;
  }

  void GetLock()
  {
    register ZMemSize i;

    while(true)
    {
      for(i=0;i<1000;i++)
      {
         if (__sync_bool_compare_and_swap(&Lock, 0, 1))
         {
           return;
         }
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386) || defined(_M_IX86)
         //__builtin_ia32_pause();
         __asm volatile ("pause" ::: "memory");
#endif
         // todo : Find an Arm equivalent of x86 pause instruction if exists.
      }
      GoSleep();
    }
  }

  void ReleaseLock()
  {
    __sync_bool_compare_and_swap(&Lock, 1, 0);
  }

};




#endif /* Z_ZSPINLOCK_H */
