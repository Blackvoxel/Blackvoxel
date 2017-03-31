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
 * ZVoxelType_Storage.cpp
 *
 *  Created on: 30 sept. 2011
 *      Author: laurent
 */

#include "ZVoxelType_Storage.h"
#include "ZGame.h"

#ifndef Z_ZVOXELEXTENSION_STORAGE_H
#  include "ZVoxelExtension_Storage.h"
#endif


ZVoxelExtension * ZVoxelType_Storage::CreateVoxelExtension(bool IsLoadingPhase)
{
  ZVoxelExtension * NewVoxelExtension = 0;

  NewVoxelExtension = new ZVoxelExtension_Storage;

  return (NewVoxelExtension);
}

void  ZVoxelType_Storage::UserAction_Activate(ZMemSize VoxelInfo, Long x, Long y, Long z)
{
  if (GameEnv->GameWindow_Storage->Is_Shown()) return;
  GameEnv->GameWindow_Storage->SetVoxelExtension((ZVoxelExtension *)VoxelInfo);
  GameEnv->GameWindow_Storage->Show();
}

ULong ZVoxelType_Storage::Interface_PushBlock_Push( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count )
{
  ZVoxelExtension_Storage * Ext_Storage;

  Ext_Storage = (ZVoxelExtension_Storage *)DestLocation->Sector->OtherInfos[DestLocation->Offset];

  if (Ext_Storage)
  {
    return(Ext_Storage->StoreBlocks(VoxelType, Count));
  }

  return(0);
}


ULong ZVoxelType_Storage::Interface_PushBlock_Pull( ZVoxelLocation * DestLocation,  UShort * VoxelType, ULong Count )
{
  ZVoxelExtension_Storage * Ext_Storage;
  ULong SlotNum;
  Ext_Storage = (ZVoxelExtension_Storage *)DestLocation->Sector->OtherInfos[DestLocation->Offset];

  if (Ext_Storage)
  {
    SlotNum = Ext_Storage->FindFirstUsedBlock();
    if (SlotNum== (ULong)(-1)) return(0);

    return(Ext_Storage->UnstoreBlocks(SlotNum,Count, VoxelType));
  }

  return(0);
}

ULong ZVoxelType_Storage::Interface_PushBlock_PullTest( ZVoxelLocation * DestLocation,  UShort * VoxelType, ULong Count )
{
  ZVoxelExtension_Storage * Ext_Storage;
  ULong SlotNum;
  Ext_Storage = (ZVoxelExtension_Storage *)DestLocation->Sector->OtherInfos[DestLocation->Offset];

  if (Ext_Storage)
  {
    SlotNum = Ext_Storage->FindFirstUsedBlock();
    if (SlotNum == (ULong)(-1)) return(0);

    *VoxelType = Ext_Storage->VoxelType[SlotNum];
    return( (Ext_Storage->VoxelQuantity[SlotNum] > Count) ? Count : Ext_Storage->VoxelQuantity[SlotNum] );
  }

  return(0);
}

bool ZVoxelType_Storage::UserAction_TryToDestroy(ZVoxelLocation * DestLocation, ZString * Reason)
{
  ZVoxelExtension_Storage * Ext_Storage;
  Ext_Storage = (ZVoxelExtension_Storage *)DestLocation->Sector->OtherInfos[DestLocation->Offset];

  if (Ext_Storage)
  {
    if (Ext_Storage->IsInventoryEmpty()) return(true);

    *Reason = "NOT EMPTY";
  }

  return(false);
}

