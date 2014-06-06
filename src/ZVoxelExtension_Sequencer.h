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
 * ZVoxelExtension_Sequencer.h
 *
 *  Created on: 30 avr. 2013
 *      Author: laurent
 */

#ifndef Z_VOXELEXTENSION_SEQUENCER_H
#define Z_VOXELEXTENSION_SEQUENCER_H

//#ifndef Z_VOXELEXTENSION_SEQUENCER_H
//#  include "ZVoxelExtension_Sequencer.h"
//#endif

#ifndef Z_ZVOXELEXTENSION_H
#  include "ZVoxelExtension.h"
#endif

class ZVoxelExtension_Sequencer : public ZVoxelExtension
{
  public:

  enum {Storage_NumSlots = 100, Input_NumSlots=20, Sequence_NumSlots = 80 };
  enum {
        Start_InputSlots = 0, End_InputSlots = Start_InputSlots + Input_NumSlots,
        Start_SequenceSlots=20, End_SequenceSlots = Start_SequenceSlots + Sequence_NumSlots
       };

  UShort VoxelType[Storage_NumSlots];
  ULong  VoxelQuantity[Storage_NumSlots];

  ULong  SequencePosition;
  ULong  PositionRemain;
  ULong  Mode; // Input = 0; Output = 1

  ULong  OutputLocation;

  virtual ZVoxelExtension * GetNewCopy()
  {
    ZVoxelExtension_Sequencer * NewCopy;
    NewCopy = new ZVoxelExtension_Sequencer(*this);
    return(NewCopy);
  }

  ZVoxelExtension_Sequencer()
  {
    ULong i;
    ExtensionType = Extension_Sequencer;
    for (i=0;i<Storage_NumSlots;i++) {VoxelType[i] = 0; VoxelQuantity[i] = 0;}
    SequencePosition = 0;
    PositionRemain = 0;
    OutputLocation = 0;
    Mode = 0;
  }

  virtual ULong GetExtensionID()
  {
    return( MulticharConst('S','E','Q','U') );
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
    Stream->Put(Mode);
    Stream->Put(OutputLocation);
    Stream->Put(SequencePosition);
    Stream->Put(PositionRemain);
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

    if (ExtensionVersion!=1) { ExtensionSize-=2; for(ZMemSize i=0;i<ExtensionSize;i++) Ok = Stream->Get(Temp_Byte); if (Ok) return(true); else return(false);}

    Ok &= Stream->Get(Mode);
    Ok &= Stream->Get(OutputLocation);
    Ok &= Stream->Get(SequencePosition);
    Ok &= Stream->Get(PositionRemain);
    Stream->GetZone(&VoxelType, sizeof(VoxelType));
    Stream->GetZone(&VoxelQuantity, sizeof(VoxelQuantity));
    return(Ok);
  }

  virtual bool FindSlot(UShort VoxelType, ULong &Slot)
  {
    ULong i;

    for(i=Start_InputSlots; i<End_InputSlots; i++)
    {
      if (this->VoxelType[i] == VoxelType && VoxelQuantity[i]>0) { Slot = i; return(true); }
    }
    return(false);
  }

  virtual bool FindFreeSlot(ULong &Slot)
  {
    ULong i;

    for(i=Start_InputSlots;i<End_InputSlots;i++)
    {
      if (VoxelQuantity[i] == 0) {Slot = i; return(true);}
    }

    return(false);
  }

  virtual ULong FindFirstUsedBlock()
  {
    ULong i;

    for(i=Start_InputSlots;i<End_InputSlots;i++)
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


#endif /* ZVOXELEXTENSION_SEQUENCER_H_ */
