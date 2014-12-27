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
 * ZFastBit_Array.h
 *
 *  Created on: 15 sept. 2012
 *      Author: laurent
 */

#ifndef Z_ZFASTBIT_ARRAY_H
#define Z_ZFASTBIT_ARRAY_H

//#ifndef Z_ZFASTBIT_ARRAY_H
//#  include "ZFastBit_Array.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

class ZFastBit_Array
{
  public:
    ULong   * Storage;
    ZMemSize  StorageSize;

    ZFastBit_Array()
    {
      if (sizeof(ULong)!=4) throw; // ULong Needs to be 32 bits
      Storage = 0;
      StorageSize = 0;
    }

    bool InitSize(ZMemSize Size)
    {
      if (sizeof(ULong)!=4) throw; // ULong Needs to be 32 bits
      StorageSize = (Size >> 5) + ((Size & 0x1F) ? 1 : 0);
      if (Storage) delete [] Storage;
      Storage = new ULong[StorageSize]; if (!Storage) return(false);
      return(true);
    }

    ZFastBit_Array(ZMemSize Size)
    {
      Storage = 0;
      InitSize(Size);
    }

    ~ZFastBit_Array()
    {
      if ((Storage)) { delete [] Storage; Storage = 0; }
    }


    inline void Clear()
    {
      ULong i;
      for(i=0;i<StorageSize;i++) Storage[i]=0;
    }

    inline void Set(ZMemSize Index, bool Data)
    {
      register UByte Remain;
      register ZMemSize Offset;
      Remain = Index;
      Offset = Index >> 5;
      Remain &= 0x1f;
#if COMPILEOPTION_BOUNDCHECKINGSLOW==1
      if (Offset >= StorageSize) throw;
#endif
      if (Data) Storage[Offset] |= 1<<Remain;
      else      Storage[Offset] &= ~(1<<Remain);
    }

    inline bool Get(ZMemSize Index)
    {
      register UByte Remain;
      register ZMemSize Offset;
      Remain = Index;
      Offset = Index >> 5;
#if COMPILEOPTION_BOUNDCHECKINGSLOW==1
      if (Offset >= StorageSize) throw;
#endif
      Remain &= 0x1f;
      return( Storage[Offset] & (1<<Remain) );
    }
};

// A specialized version.

class ZFastBit_Array_64k
{
  public:
    ULong * Storage;

    ZFastBit_Array_64k()
    {
      Storage = new ULong[2048];
      if (sizeof(ULong)!=4) throw; // ULong Needs to be 32 bits
    }

    ~ZFastBit_Array_64k()
    {
      if ((Storage)) { delete [] Storage; Storage = 0; }
    }


    inline void Clear()
    {
      ULong i;
      for(i=0;i<2048;i++) Storage[i]=0;
    }

    inline void Set(UShort Index, bool Data)
    {
      UByte Remain;
      ULong Offset;
      Remain = Index;
      Offset = Index >> 5;
      Remain &= 0x1f;
      if (Data) Storage[Offset] |= 1<<Remain;
      else      Storage[Offset] &= ~(1<<Remain);
    }

    inline bool Get(UShort Index)
    {
      register UByte Remain;
      register ULong Offset;
      Remain = Index;
      Offset = Index >> 5;
      Remain &= 0x1f;
      return( Storage[Offset] & (1<<Remain) );
    }
};


#endif /* Z_ZFASTBIT_ARRAY_H */
