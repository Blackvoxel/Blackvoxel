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
 * ZVoxelType_ProgRobot_Asm.cpp
 *
 *  Created on: 6 nov. 2014
 *      Author: laurent
 */

#include "ZVoxelType_ProgRobot_Asm.h"
#include "ZGame.h"

#ifndef Z_ZVOXELEXTENSION_PROGROBOT_ASM_H
#  include "ZVoxelExtension_ProgRobot_Asm.h"
#endif


ZVoxelExtension * ZVoxelType_ProgRobot_Asm::CreateVoxelExtension(bool IsLoadingPhase)
{
  ZVoxelExtension_ProgRobot_Asm * NewVoxelExtension = 0;

  NewVoxelExtension = new ZVoxelExtension_ProgRobot_Asm;

  return (NewVoxelExtension);
}

void ZVoxelType_ProgRobot_Asm::DeleteVoxelExtension(ZMemSize VoxelExtension, bool IsUnloadingPhase)
{
  ZVoxelExtension_ProgRobot_Asm * Ext;

  Ext = (ZVoxelExtension_ProgRobot_Asm *)VoxelExtension;
  if (!Ext) return;

  delete Ext;
}

void  ZVoxelType_ProgRobot_Asm::UserAction_Activate(ZMemSize VoxelInfo, Long x, Long y, Long z)
{
//  if (GameEnv->GameWindow_Programmable->Is_Shown()) return;
//  GameEnv->GameWindow_Programmable->SetVoxelExtension((ZVoxelExtension *)VoxelInfo);
//  GameEnv->GameWindow_Programmable->Show();
}

ULong ZVoxelType_ProgRobot_Asm::Interface_PushBlock_Push( VoxelLocation * DestLocation, UShort VoxelType, ULong Count )
{
  ZVoxelExtension_ProgRobot_Asm * Ext_Storage;

  Ext_Storage = (ZVoxelExtension_ProgRobot_Asm *)DestLocation->Sector->OtherInfos[DestLocation->Offset];

  if (Ext_Storage)
  {
    return(Ext_Storage->StoreBlocks(VoxelType, Count));
  }

  return(0);
}


ULong ZVoxelType_ProgRobot_Asm::Interface_PushBlock_Pull( VoxelLocation * DestLocation,  UShort * VoxelType, ULong Count )
{
  ZVoxelExtension_ProgRobot_Asm * Ext_Storage;
  ULong SlotNum;
  Ext_Storage = (ZVoxelExtension_ProgRobot_Asm *)DestLocation->Sector->OtherInfos[DestLocation->Offset];

  if (Ext_Storage)
  {
    SlotNum = Ext_Storage->FindFirstUsedBlock();
    if (SlotNum== (ULong)(-1)) return(0);

    return(Ext_Storage->UnstoreBlocks(SlotNum,Count, VoxelType));
  }

  return(0);
}

ULong ZVoxelType_ProgRobot_Asm::Interface_PushBlock_PullTest( VoxelLocation * DestLocation,  UShort * VoxelType, ULong Count )
{
  ZVoxelExtension_ProgRobot_Asm * Ext_Storage;
  ULong SlotNum;
  Ext_Storage = (ZVoxelExtension_ProgRobot_Asm *)DestLocation->Sector->OtherInfos[DestLocation->Offset];

  if (Ext_Storage)
  {
    SlotNum = Ext_Storage->FindFirstUsedBlock();
    if (SlotNum== (ULong)(-1)) return(0);

    *VoxelType = Ext_Storage->VoxelType[SlotNum];
    return( (Ext_Storage->VoxelQuantity[SlotNum] > Count) ? Count : Ext_Storage->VoxelQuantity[SlotNum] );
  }

  return(0);
}
