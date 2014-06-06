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
 * ZVoxelExtension_PlaneZ1.h
 *
 *  Created on: 17 nov. 2012
 *      Author: laurent
 */

#ifndef Z_VOXELEXTENSION_PLANEZ1_H
#define Z_VOXELEXTENSION_PLANEZ1_H

// #ifndef Z_VOXELEXTENSION_PLANEZ1_H
// #  include "ZVoxelExtension_PlaneZ1.h"
// #endif

#ifndef Z_ZVOXELEXTENSION_H
#  include "ZVoxelExtension.h"
#endif

class ZVoxelExtension_PlaneZ1 : public ZVoxelExtension
{
  public:
  enum {Storage_NumSlots = 5};

  UShort VoxelType[Storage_NumSlots];
  ULong  VoxelQuantity[Storage_NumSlots];

  public:

  ZVoxelExtension_PlaneZ1()
  {
    ULong i;
    ExtensionType = Extension_PlaneZ1;
    for (i=0;i<Storage_NumSlots;i++) {VoxelType[i] = 0; VoxelQuantity[i] = 0;}
  }

  virtual ZVoxelExtension * GetNewCopy()
  {
    ZVoxelExtension_PlaneZ1 * NewCopy;
    NewCopy = new ZVoxelExtension_PlaneZ1(*this);
    return(NewCopy);
  }

  virtual ULong GetExtensionID()
  {
    return( MulticharConst('P','L','Z','1') );
  }

  virtual bool Save(ZStream_SpecialRamStream * Stream)
  {
    ULong * ExtensionSize;
    ULong   StartLen;

    ExtensionSize = Stream->GetPointer_ULong();
    Stream->Put(0u);       // The size of the extension (defered storage).
    StartLen = Stream->GetActualBufferLen();
    Stream->Put((UShort)1); // Extension Version;

    // Storage informations.
    Stream->PutZone(&VoxelType,sizeof(VoxelType));
    Stream->PutZone(&VoxelQuantity,sizeof(VoxelQuantity));

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

    if (ExtensionVersion!=1) { ExtensionSize-=2; for(ZMemSize i;i<ExtensionSize;i++) Ok = Stream->Get(Temp_Byte); if (Ok) return(true); else return(false);}

    Stream->GetZone(&VoxelType, sizeof(VoxelType));
    Stream->GetZone(&VoxelQuantity, sizeof(VoxelQuantity));
    return(Ok);
  }


  virtual bool FindSlot(UShort VoxelType, ULong &Slot)
  {
    ULong i;

    for(i=0;i<Storage_NumSlots;i++)
    {
      if (this->VoxelType[i] == VoxelType && VoxelQuantity[i]>0) { Slot = i; return(true); }
    }
    return(false);
  }

  virtual bool FindFreeSlot(ULong &Slot)
  {
    ULong i;

    for(i=0;i<Storage_NumSlots;i++)
    {
      if (VoxelQuantity[i] == 0) {Slot = i; return(true);}
    }

    return(false);
  }

  virtual ULong StoreBlocks(UShort VoxelType, ULong VoxelQuantity)
  {
    ULong Slot;

    if (FindSlot(VoxelType,Slot))
    {
      this->VoxelQuantity[Slot] += VoxelQuantity;
      return(VoxelQuantity);
    }
    else if (FindFreeSlot(Slot))
    {
      this->VoxelType[Slot]     = VoxelType;
      this->VoxelQuantity[Slot] = VoxelQuantity;
      return(true);
    }

    return(false);
  }
};


#endif /* ZVOXELEXTENSION_PLANEZ1_H_ */
