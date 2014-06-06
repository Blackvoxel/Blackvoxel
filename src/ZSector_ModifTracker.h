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
 * ZSector_ModifTracker.h
 *
 *  Created on: 19 sept. 2012
 *      Author: laurent
 */

#ifndef Z_ZSECTOR_MODIFTRACKER_H
#define Z_ZSECTOR_MODIFTRACKER_H

//#ifndef Z_ZSECTOR_MODIFTRACKER_H
//#  include "ZSector_ModifTracker.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

class ZSectorModifTracker
{
  protected:

    ULong * Storage;
    ULong ActualCycleNum;
    ULong LastUpdateCycleNum;
    ZMemSize StorageSize_Num_Ulongs;
    ZMemSize BitSize;

  public:
    inline void Init(ZMemSize SizeInBits)
    {
      BitSize = SizeInBits;
      StorageSize_Num_Ulongs = (BitSize + 1) >> 5;
      Storage = new ULong[StorageSize_Num_Ulongs];
      if (sizeof(ULong)!=4) throw; // ULong Needs to be 32 bits
      ActualCycleNum = (ULong)(-1);
      LastUpdateCycleNum = 0;
    }

    ZSectorModifTracker()
    {
      BitSize = 0;
      StorageSize_Num_Ulongs = 0;
      Storage = 0;
      if (sizeof(ULong)!=4) throw; // ULong Needs to be 32 bits
      ActualCycleNum = (ULong)(-1);
      LastUpdateCycleNum = 0;
    }

    ZSectorModifTracker(ZMemSize SizeInBits)
    {
      Init(SizeInBits);
    }

    ~ZSectorModifTracker()
    {
      if ((Storage)) { delete Storage; Storage = 0; }
      ActualCycleNum = (ULong)(-1);
      LastUpdateCycleNum = 0;
    }

    inline void SetActualCycleNum(ULong ActualCycleNum)
    {
      this->ActualCycleNum = ActualCycleNum;
    }

    inline void Clear() // Warning, before using this funcion, you must SetActualCycleNum(...)
    {
      ULong i;
      for(i=0;i<StorageSize_Num_Ulongs;i++) Storage[i]=0;
      LastUpdateCycleNum = ActualCycleNum;
    }

    inline void Set(ULong Index)
    {

      UByte Remain;
      ULong Offset;

      if (LastUpdateCycleNum != ActualCycleNum) { Clear(); }

      Remain = Index;
      Offset = Index >> 5;
      Remain &= 0x1f;
      Storage[Offset] |= 1<<Remain;
    }

    inline bool Get(ULong Index)
    {
      register UByte Remain;
      register ULong Offset;

      if (LastUpdateCycleNum != ActualCycleNum) return(false);

      Remain = Index;
      Offset = Index >> 5;
      Remain &= 0x1f;
      return( Storage[Offset] & (1<<Remain) );
    }

};

#endif /* Z_ZSECTOR_MODIFTRACKER_H */
