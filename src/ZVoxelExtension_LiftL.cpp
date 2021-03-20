/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2021 Laurent Thiebaut & Olivia Merle
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
 * ZVoxelExtension_LiftL.cpp
 *
 *  Created on: 21 févr. 2021
 *      Author: laurent
 */

#ifndef Z_ZVOXELEXTENSION_LIFTL_H
#  include "ZVoxelExtension_LiftL.h"
#endif

ZVoxelExtension_LiftL::ZVoxelExtension_LiftL()
{
  ULong i;
  ExtensionType = Extension_LiftL;
  for (i=0;i<Storage_NumSlots;i++) {VoxelType[i] = 0; VoxelQuantity[i] = 0;}
}

ZVoxelExtension * ZVoxelExtension_LiftL::GetNewCopy()
{
  ZVoxelExtension_LiftL * NewCopy;
  NewCopy = new ZVoxelExtension_LiftL(*this);
  return(NewCopy);
}

bool ZVoxelExtension_LiftL::Save(ZStream_SpecialRamStream * Stream)
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

bool ZVoxelExtension_LiftL::Load(ZStream_SpecialRamStream * Stream)
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

bool ZVoxelExtension_LiftL::FindSlot(UShort VoxelType, ULong &Slot)
{
  ULong i;

  for(i=0;i<Storage_NumSlots;i++)
  {
    if (this->VoxelType[i] == VoxelType && VoxelQuantity[i]>0) { Slot = i; return(true); }
  }
  return(false);
}

bool ZVoxelExtension_LiftL::FindFreeSlot(ULong &Slot)
{
  ULong i;

  for(i=0;i<Storage_NumSlots;i++)
  {
    if (VoxelQuantity[i] == 0) {Slot = i; return(true);}
  }

  return(false);
}

ULong ZVoxelExtension_LiftL::StoreBlocks(UShort VoxelType, ULong VoxelQuantity)
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
