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
 * ZVoxelExtension_WirelessTransmitter.h
 *
 *  Created on: 29 déc. 2016
 *      Author: laurent
 */

#ifndef Z_ZVOXELEXTENSION_WIRELESSTRANSMITTER_H
#define Z_ZVOXELEXTENSION_WIRELESSTRANSMITTER_H

//#ifndef Z_ZVOXELEXTENSION_WIRELESSTRANSMITTER_H
//#  include "ZVoxelExtension_WirelessTransmitter.h"
//#endif

#ifndef Z_ZVOXELEXTENSION_H
#  include "ZVoxelExtension.h"
#endif

#ifndef Z_ZGENERICTABLE_H
#  include "z/ZGenericTable.h"
#endif

#ifndef Z_ZSPINLOCK_H
#  include "z/ZSpinLock.h"
#endif

class ZVoxelExtension_WirelessTransmitter : public ZVoxelExtension
{
  public:

    class DestPoint
    {
      public:
        ZVector3L Location;

        bool operator == (DestPoint const & Other)
        {
          return(Location == Other.Location);
        }
    };

  protected:

    ZGenericTable<DestPoint, 128 > DestPointTable;
    ZSpinLock Lock;
  public:
    ULong                          NextUsed;


  virtual ZVoxelExtension * GetNewCopy()
  {
    ZVoxelExtension_WirelessTransmitter * NewCopy;
    NewCopy = new ZVoxelExtension_WirelessTransmitter(*this);
    return(NewCopy);
  }

  ZVoxelExtension_WirelessTransmitter()
  {
    NextUsed = 0;
  }

  virtual ULong GetExtensionID()
  {
    return( MulticharConst('W','T','R','N') );
  }

  bool AddConsummer(DestPoint * Consummer)
  {
    ZMemSize Index;
    DestPoint * Point;
    Bool Result;

    Lock.GetLock();

    Result = false;
    if (DestPointTable.CreateNew(Index))
    {
      Point = DestPointTable.GetElement(Index);
      *Point = *Consummer;
      Result = true;
    }

    Lock.ReleaseLock();

    return(Result);
  }

  bool IsConsummerRegistered(DestPoint * Consummer)
  {
    ZMemSize Index;
    return(DestPointTable.FindElement(Consummer, &Index));
  }

  bool RemoveConsummer(ZVector3L * Consummer)
  {
    DestPoint SearchElement;
    SearchElement.Location = *Consummer;
    ZMemSize Index;
    bool Result;

    Lock.GetLock();

    Result = false;
    if (DestPointTable.FindElement(&SearchElement,&Index))
    {
      DestPointTable.RemoveFromTable(Index);
      Result = true;
    }

    Lock.ReleaseLock();

    return(Result);
  }

  ZMemSize GetConsummerCount()
  {
    ZMemSize Result;

    Lock.GetLock();

    Result = DestPointTable.GetCount();

    Lock.ReleaseLock();
    return(Result);
  }

  bool GetConsummerInfo(ZMemSize Index, DestPoint & OutConsummer)
  {
    bool Result;

    Lock.GetLock();

    Result = false;
    if (Index < DestPointTable.GetCount())
    {
      OutConsummer = *DestPointTable.GetElement(Index);
      Result = true;
    }

    Lock.ReleaseLock();
    return(Result);
  }

  virtual bool Save(ZStream_SpecialRamStream * Stream)
  {
    ULong * ExtensionSize;
    ULong   StartLen;

    ExtensionSize = Stream->GetPointer_ULong();
    Stream->Put(0u);       // The size of the extension (defered storage).
    StartLen = Stream->GetActualBufferLen();
    Stream->Put((UShort)1); // Extension Version;

    // Lock storage

    Lock.GetLock();


    // Save informations

    ZMemSize Count = DestPointTable.GetCount();
    ZMemSize i;
    DestPoint * Pnt;

    Stream->Put( (ULong)NextUsed);
    Stream->Put( (ULong)Count);

    for (i=0;i<Count;i++)
    {
      Pnt = DestPointTable.GetElement(i);
      if (Pnt)
      {
        Stream->Put(Pnt->Location.x);
        Stream->Put(Pnt->Location.y);
        Stream->Put(Pnt->Location.z);
      }
    }

    // Unlock Storage.

    Lock.ReleaseLock();

    *ExtensionSize = Stream->GetActualBufferLen() - StartLen;

    return(true);
  }

  virtual bool Load(ZStream_SpecialRamStream * Stream)
  {
    bool Ok;
    ULong  ExtensionSize;
    UShort ExtensionVersion;
    UByte  Temp_Byte;

    Ok = Stream->Get(ExtensionSize);
    Ok&= Stream->Get(ExtensionVersion);     if(!Ok) return(false);

    // Check for supported extension version. If unsupported new version, throw content and continue with a blank extension.

    if (ExtensionVersion!=1) { ExtensionSize-=2; for(ZMemSize i=0;i<ExtensionSize;i++) Ok = Stream->Get(Temp_Byte); if (Ok) return(true); else return(false);}

    ULong Count,i;
    DestPoint * Pnt;

    Lock.GetLock();

    Ok &= Stream->Get(NextUsed);
    Ok &= Stream->Get(Count);

    for (i=0;i<Count;i++)
    {
      Pnt = DestPointTable.CreateNewPnt();
      if (Pnt)
      {
        Ok &= Stream->Get(Pnt->Location.x);
        Ok &= Stream->Get(Pnt->Location.y);
        Ok &= Stream->Get(Pnt->Location.z);
      }
    }

    Lock.ReleaseLock();

    return(Ok);
  }

};



#endif /* Z_ZVOXELEXTENSION_WIRELESSTRANSMITTER_H */
