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
 * ZVoxelType_MiningRobot_xr1.cpp
 *
 *  Created on: 21 févr. 2013
 *      Author: laurent
 */

#include "ZGame.h"

#ifndef Z_ZVOXELTYPE_MININGROBOT_XR1_H
#  include "ZVoxelType_MiningRobot_xr1.h"
#endif

#ifndef Z_ZVOXELEXTENSION_MININGROBOT_XR1_H
#  include "ZVoxelExtension_MiningRobot_xr1.h"
#endif


ZVoxelExtension * ZVoxelType_MiningRobot_xr1::CreateVoxelExtension(bool IsLoadingPhase)
{
  ZVoxelExtension * NewVoxelExtension = 0;

  NewVoxelExtension = new ZVoxelExtension_MiningRobot_xr1;
  // printf("Create Mining Robot Extension : %lx\n", NewVoxelExtension);

  return (NewVoxelExtension);
}

void ZVoxelType_MiningRobot_xr1::GetBlockInformations(ZVoxelLocation * DestLocation, ZString & Infos)
{
  ZVoxelExtension_MiningRobot_xr1 * Ext;
  ULong i;

  Ext = (ZVoxelExtension_MiningRobot_xr1 * )DestLocation->Sector->OtherInfos[DestLocation->Offset];

  Infos << "Storage Location : " << Ext->StorageLocation.x << "," << Ext->StorageLocation.y << "," << Ext->StorageLocation.z << Infos.NewLine();
  Infos << "Direction : " << Ext->Dir.x << "," << Ext->Dir.z << Infos.NewLine();

  Infos  << "State : " << Ext->State << " ";
  switch (Ext->State)
  {
    case 1: Infos << "Going Down"; break;
    case 2: Infos << "Going x-Axis"; break;
    case 3: Infos << "Going z-Axis"; break;
    case 4: Infos << "Returning to storage"; break;
    case 5: Infos << "Unloading to storage"; break;
    case 6: Infos << "Searching new storage"; break;
  }
  Infos << Infos.NewLine();

  Infos << "Inventory :" << Infos.NewLine();
  for (i=0;i<ZVoxelExtension_MiningRobot_xr1::Storage_NumSlots; i++)
  {
    if (Ext->VoxelQuantity[i] > 0)
    {
      Infos << ">" << Ext->VoxelType[i] << ":" << Ext->VoxelQuantity[i] << this->GameEnv->VoxelTypeManager.VoxelTable[Ext->VoxelType[i]]->VoxelTypeName << Infos.NewLine();
    }
  }
  Infos << Infos.NewLine();
}

ULong ZVoxelType_MiningRobot_xr1::Interface_PushBlock_Pull( ZVoxelLocation * DestLocation,  UShort * VoxelType, ULong Count )
{
  ZVoxelExtension_MiningRobot_xr1 * Ext_Storage;
  ULong SlotNum;
  Ext_Storage = (ZVoxelExtension_MiningRobot_xr1 *)DestLocation->Sector->OtherInfos[DestLocation->Offset];

  if (Ext_Storage)
  {
    SlotNum = Ext_Storage->FindFirstUsedBlock();
    if (SlotNum== (ULong)(-1)) return(0);

    return(Ext_Storage->UnstoreBlocks(SlotNum,Count, VoxelType));
  }

  return(0);
}

ULong ZVoxelType_MiningRobot_xr1::Interface_PushBlock_PullTest( ZVoxelLocation * DestLocation,  UShort * VoxelType, ULong Count )
{
  ZVoxelExtension_MiningRobot_xr1 * Ext_Storage;
  ULong SlotNum;
  Ext_Storage = (ZVoxelExtension_MiningRobot_xr1 *)DestLocation->Sector->OtherInfos[DestLocation->Offset];

  if (Ext_Storage)
  {
    SlotNum = Ext_Storage->FindFirstUsedBlock();
    if (SlotNum == (ULong)(-1)) return(0);

    *VoxelType = Ext_Storage->VoxelType[SlotNum];
    return( (Ext_Storage->VoxelQuantity[SlotNum] > Count) ? Count : Ext_Storage->VoxelQuantity[SlotNum] );
  }

  return(0);
}


