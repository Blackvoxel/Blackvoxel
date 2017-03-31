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
 * ZVoxelExtension_MiningRobot_xr1.h
 *
 *  Created on: 21 févr. 2013
 *      Author: laurent
 */

#ifndef Z_ZVOXELEXTENSION_MININGROBOT_XR1_H
#define Z_ZVOXELEXTENSION_MININGROBOT_XR1_H

//#ifndef Z_ZVOXELEXTENSION_MININGROBOT_XR1_H
//#  include "ZVoxelExtension_MiningRobot_xr1.h"
//#endif

#ifndef Z_ZVOXELEXTENSION_H
#  include "ZVoxelExtension.h"
#endif

#ifndef Z_ZVOXELEXTENSION_STORAGE_H
#  include "ZVoxelExtension_Storage.h"
#endif

#ifndef Z_ZWORLD_H
#  include "ZWorld.h"
#endif

class ZVoxelExtension_MiningRobot_xr1 : public ZVoxelExtension
{
  public:
    enum {Storage_NumSlots = 80};

    ULong     State;
    ZVector3L StorageLocation;
    ZVector3L Mine_InitialCount;
    ZVector3L Mine_Count;
    ZVector3L Dir;
    Long      CycleSkip;
    Long      SkipCounter;
    ULong     StorePerCycle;
    Long      RobotLevel;

    UShort VoxelType[Storage_NumSlots];
    ULong  VoxelQuantity[Storage_NumSlots];

  public:

  void DebugOut();

  virtual ZVoxelExtension * GetNewCopy()
  {
    ZVoxelExtension_MiningRobot_xr1 * NewCopy;
    NewCopy = new ZVoxelExtension_MiningRobot_xr1(*this);
    return(NewCopy);
  }

  ZVoxelExtension_MiningRobot_xr1()
  {
    ULong i;

    ExtensionType = Extension_MiningRobot_xr1;
    StorageLocation = 0;
    Mine_Count = 0;
    Mine_InitialCount = 0;
    Dir = 0;
    CycleSkip = 0;
    SkipCounter = 0;
    RobotLevel = 1;
    for (i=0;i<Storage_NumSlots;i++) { VoxelType[i]=0;VoxelQuantity[i]=0; }
    State = 0;
  }

  ~ZVoxelExtension_MiningRobot_xr1()
  {

  }

  virtual ULong GetExtensionID()
  {
    return( MulticharConst('M','R','X','1') );
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

  void Robot_Init( ZVector3L * StorageLocation,
                   ULong       DirectionCode,
                   Long HoleWidth_x,
                   Long HoleWidth_z,
                   Long HoleDeep_y,
                   Long CycleSkip = 0,
                   ULong StorePerCycle = 1,
                   ULong RobotLevel = 1);

  void Robot_Move( ZVector3L * Pos, ZGame * GameEnv );

};


#endif /* Z_ZVOXELEXTENSION_MININGROBOT_XR1_H */
