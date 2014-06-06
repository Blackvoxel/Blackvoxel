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
 * ZVoxelExtension_MotherMachine.h
 *
 *  Created on: 22 déc. 2012
 *      Author: laurent
 */

#ifndef Z_VOXELEXTENSION_FABMACHINE_H
#define Z_VOXELEXTENSION_FABMACHINE_H

//#ifndef Z_VOXELEXTENSION_FABMACHINE_H
//#  include "ZVoxelExtension_FabMachine.h"
//#endif

#ifndef Z_ZVOXELEXTENSION_H
#  include "ZVoxelExtension.h"
#endif

class ZVoxelExtension_FabMachine : public ZVoxelExtension
{
  public:
  enum {Storage_NumSlots = 64, Output_NumSlots = 16 };


  UShort VoxelType[Storage_NumSlots];
  ULong  VoxelQuantity[Storage_NumSlots];
  ULong  LastInDirection;
  ULong  MachineState;
  UShort VoxelToOutput[Output_NumSlots];
  UShort VoxelToOutputQuantities[Output_NumSlots];

  public:

  virtual ZVoxelExtension * GetNewCopy()
  {
    ZVoxelExtension_FabMachine * NewCopy;
    NewCopy = new ZVoxelExtension_FabMachine(*this);
    return(NewCopy);
  }

  ZVoxelExtension_FabMachine()
  {
    ULong i;
    ExtensionType = Extension_TransformationMachine;
    LastInDirection = 0;
    MachineState = 0;
    for (i=0;i<Storage_NumSlots;i++) {VoxelType[i] = 0; VoxelQuantity[i] = 0;}
    for (i=0;i<Output_NumSlots ;i++) {VoxelToOutput[i] = 0; VoxelToOutputQuantities[i]=0; }
  }

  virtual ULong GetExtensionID()
  {
    return( MulticharConst('F','M','C','H') );
  }

  virtual bool Save(ZStream_SpecialRamStream * Stream);
  virtual bool Load(ZStream_SpecialRamStream * Stream);



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

  virtual ULong FindFirstUsedBlock()
  {
    ULong i;

    for(i=0;i<Storage_NumSlots;i++)
    {
      if (this->VoxelType[i] !=0 && VoxelQuantity[i]>0) { return(i); }
    }
    return(-1);

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

  virtual ULong UnstoreBlocks(ULong SlotNum, ULong VoxelQuantity, UShort * VoxelType)
  {
    ULong Quantity;

    if (SlotNum>=Storage_NumSlots) return(0);
    if (this->VoxelType[SlotNum]==0 || this->VoxelQuantity[SlotNum]<1 ) return(0);
    *VoxelType = this->VoxelType[SlotNum];
    if (this->VoxelQuantity[SlotNum] > VoxelQuantity) { this->VoxelQuantity[SlotNum]-=VoxelQuantity; return(VoxelQuantity);}
    Quantity = this->VoxelQuantity[SlotNum];
    this->VoxelQuantity[SlotNum]=0;
    this->VoxelType[SlotNum]=0;
    return(Quantity);
  }


};


#endif /* Z_VOXELEXTENSION_FABMACHINE_H */
