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
 * ZVoxelType_Programmable.cpp
 *
 *  Created on: 28 déc. 2012
 *      Author: laurent
 */

#include "ZVoxelType_Programmable.h"
#include "ZGame.h"

#ifndef Z_ZVOXELEXTENSION_PROGRAMMABLE_H
#  include "ZVoxelExtension_Programmable.h"
#endif


ZVoxelExtension * ZVoxelType_Programmable::CreateVoxelExtension(bool IsLoadingPhase)
{
  ZVoxelExtension_Programmable * NewVoxelExtension = 0;

  NewVoxelExtension = new ZVoxelExtension_Programmable;

  if (!IsLoadingPhase)
  {
    NewVoxelExtension->SetSerial(GameEnv->Machine_Serial++);
    NewVoxelExtension->CompileAndRunScript(ZVoxelExtension_Programmable::CONTEXT_NEWVOXEL);
  }

  return (NewVoxelExtension);
}

void ZVoxelType_Programmable::DeleteVoxelExtension(ZMemSize VoxelExtension, bool IsUnloadingPhase)
{
  ZVoxelExtension_Programmable * Ext;

  Ext = (ZVoxelExtension_Programmable *)VoxelExtension;
  if (!Ext) return;

  if (Ext->IsRunningProgram())
  {
    do
    {
      Ext->StopProgram();
    } while (Ext->IsRunningProgram());
  }

  // If not unloading phase

  if (!IsUnloadingPhase)
  {
    Ext->Script_Engine.RunScript("Voxel_Unload", false, ZScripting_Squirrel3::RUNCONTEXT_DESTROY);
  }

  // If there is a window opened on this robot, then close it before it could displays bad memory locations (or crash).

  if (GameEnv->GameWindow_Programmable)
  {
    // Does we are displaying the extension data of the robot we are about to get out of memory ?
    if (GameEnv->GameWindow_Programmable->GetVoxelExtension() == Ext) GameEnv->GameWindow_Programmable->Hide();
  }

  // Ok, then we can destroy extension.

  delete Ext;
}

void  ZVoxelType_Programmable::UserAction_Activate(ZMemSize VoxelInfo, Long x, Long y, Long z)
{
  if (GameEnv->GameWindow_Programmable->Is_Shown()) return;
  GameEnv->GameWindow_Programmable->SetVoxelExtension((ZVoxelExtension *)VoxelInfo);
  GameEnv->GameWindow_Programmable->Show();
}

ULong ZVoxelType_Programmable::Interface_PushBlock_Push( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count )
{
  ZVoxelExtension_Programmable * Ext_Storage;

  Ext_Storage = (ZVoxelExtension_Programmable *)DestLocation->Sector->OtherInfos[DestLocation->Offset];

  if (Ext_Storage)
  {
    return(Ext_Storage->StoreBlocks(VoxelType, Count));
  }

  return(0);
}


ULong ZVoxelType_Programmable::Interface_PushBlock_Pull( ZVoxelLocation * DestLocation,  UShort * VoxelType, ULong Count )
{
  ZVoxelExtension_Programmable * Ext_Storage;
  ULong SlotNum;
  Ext_Storage = (ZVoxelExtension_Programmable *)DestLocation->Sector->OtherInfos[DestLocation->Offset];

  if (Ext_Storage)
  {
    SlotNum = Ext_Storage->FindFirstUsedBlock();
    if (SlotNum== (ULong)(-1)) return(0);

    return(Ext_Storage->UnstoreBlocks(SlotNum,Count, VoxelType));
  }

  return(0);
}

ULong ZVoxelType_Programmable::Interface_PushBlock_PullTest( ZVoxelLocation * DestLocation,  UShort * VoxelType, ULong Count )
{
  ZVoxelExtension_Programmable * Ext_Storage;
  ULong SlotNum;
  Ext_Storage = (ZVoxelExtension_Programmable *)DestLocation->Sector->OtherInfos[DestLocation->Offset];

  if (Ext_Storage)
  {
    SlotNum = Ext_Storage->FindFirstUsedBlock();
    if (SlotNum== (ULong)(-1)) return(0);

    *VoxelType = Ext_Storage->VoxelType[SlotNum];
    return( (Ext_Storage->VoxelQuantity[SlotNum] > Count) ? Count : Ext_Storage->VoxelQuantity[SlotNum] );
  }

  return(0);
}
