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
 * ZVoxelExtension_ProgRobot_Remote.h
 *
 *  Created on: 22 mai 2017
 *      Author: laurent
 */

#ifndef Z_ZVOXELEXTENSION_PROGROBOT_REMOTE_H
#define Z_ZVOXELEXTENSION_PROGROBOT_REMOTE_H

//#ifndef Z_ZVOXELEXTENSION_PROGROBOT_REMOTE_H
//#  include "ZVoxelExtension_ProgRobot_Remote.h"
//#endif

#ifndef Z_ZVOXELEXTENSION_H
#  include "ZVoxelExtension.h"
#endif

#ifndef Z_ZVMACHINE_T1_H
#  include "ZVMachine_T1.h"
#endif

#ifndef Z_MACROASSEMBLER_H
#  include "ZMacroAssembler.h"
#endif

class ZActiveVoxelInterface;

class ZVoxelExtension_ProgRobot_Remote : public ZVoxelExtension
{
  public:
    enum {Storage_NumSlots = 20, ImageTable_Size = 64};

    UShort    VoxelType[Storage_NumSlots];
    ULong     VoxelQuantity[Storage_NumSlots];
    ULong     RobotSerialNumber;
    UShort    BuildVoxelType;
    ZVector3L HomeLocation;
    bool      HomeSet;
    bool      SchoolModeEnableEasyInventory;

    virtual ZVoxelExtension * GetNewCopy()
    {
      return(0);
    }

    ZVoxelExtension_ProgRobot_Remote()
    {
      ULong i;

      ExtensionType = Extension_WebRobot;
      for (i=0;i<Storage_NumSlots;i++) {VoxelType[i] = 0; VoxelQuantity[i] = 0;}
      RobotSerialNumber = 0;
      BuildVoxelType = 1;
      HomeLocation = 0;
      HomeSet = false;
      SchoolModeEnableEasyInventory = true;
    }

    virtual ~ZVoxelExtension_ProgRobot_Remote();

    void Cleanup()
    {

    }

    virtual ULong GetExtensionID()
    {
      return( MulticharConst('P','W','E','B') );
    }

    virtual bool Save(ZStream_SpecialRamStream * Stream);
    virtual bool Load(ZStream_SpecialRamStream * Stream);


    inline void SetActiveVoxelInterface(ZActiveVoxelInterface * VoxelInterface)
    {

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

    virtual void SetSerial(ULong SerialNumber) {this->RobotSerialNumber = SerialNumber; }


};


#endif /* Z_ZVOXELEXTENSION_PROGROBOT_REMOTE_H */
