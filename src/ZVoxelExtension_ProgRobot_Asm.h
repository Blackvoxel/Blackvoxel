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
 * ZVoxelExtension_ProgRobot_Asm.h
 *
 *  Created on: 6 nov. 2014
 *      Author: laurent
 */

#ifndef Z_ZVOXELEXTENSION_PROGROBOT_ASM_H
#define Z_ZVOXELEXTENSION_PROGROBOT_ASM_H

//#ifndef Z_ZVOXELEXTENSION_PROGROBOT_ASM_H
//#  include "ZVoxelExtension_ProgRobot_Asm.h"
//#endif

#ifndef Z_ZVOXELEXTENSION_H
#  include "ZVoxelExtension.h"
#endif

class ZVoxelExtension_ProgRobot_Asm : public ZVoxelExtension
{
  public:
    enum {Storage_NumSlots = 20, ImageTable_Size = 64};

    UShort  VoxelType[Storage_NumSlots];
    ULong   VoxelQuantity[Storage_NumSlots];
    ULong   RobotLevel;
    ULong   RobotSerialNumber;
    ULong   ScriptNum;
    ZString ProgramText;

    bool    IsAllowedToRun;

    // Squirrel script engine


    virtual ZVoxelExtension * GetNewCopy()
    {
      return(0);
    }

    ZVoxelExtension_ProgRobot_Asm()
    {
      ULong i;
      ExtensionType = Extension_Programmable;
      for (i=0;i<Storage_NumSlots;i++) {VoxelType[i] = 0; VoxelQuantity[i] = 0;}
      ScriptNum = 0;
      RobotLevel = 3;
      RobotSerialNumber = 0;
      IsAllowedToRun = true;
    }

    virtual ~ZVoxelExtension_ProgRobot_Asm();

    void Cleanup()
    {

    }



    virtual ULong GetExtensionID()
    {
      return( MulticharConst('P','A','S','M') );
    }

    virtual bool Save(ZStream_SpecialRamStream * Stream);
    virtual bool Load(ZStream_SpecialRamStream * Stream);


    virtual void SetVoxelPosition(ZVector3L * VoxelPosition)
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

    virtual void SetScriptNum(ULong ScriptNum) {this->ScriptNum = ScriptNum; }
    virtual void SetSerial(ULong SerialNumber) {this->RobotSerialNumber = SerialNumber; }
    virtual void CompileAndRunScript(ULong Context, ULong ScriptToRun = -1);
    virtual bool IsCompiledOk() { return(true); }

    virtual void StopProgram();
    virtual bool IsRunningProgram();
};

#endif /* Z_ZVOXELEXTENSION_PROGROBOT_ASM_H */
