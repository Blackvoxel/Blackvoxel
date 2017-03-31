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
 * ZVoxelType_PlaneZ1.cpp
 *
 *  Created on: 17 nov. 2012
 *      Author: laurent
 */


#include "ZVoxelType_PlaneZ1.h"
#include "ZGame.h"

#ifndef Z_VOXELEXTENSION_PLANEZ1_H
#  include "ZVoxelExtension_PlaneZ1.h"
#endif




ZVoxelExtension * ZVoxelType_PlaneZ1::CreateVoxelExtension(bool IsLoadingPhase)
{
  ZVoxelExtension * NewVoxelExtension = 0;

  NewVoxelExtension = new ZVoxelExtension_Storage;

  return (NewVoxelExtension);
}

void  ZVoxelType_PlaneZ1::UserAction_Activate(ZMemSize VoxelInfo, Long x, Long y, Long z)
{
  ZActor * Actor;

  Actor = GameEnv->PhysicEngine->GetSelectedActor();
  Actor->Start_Riding(x,y,z);
  // GameEnv->GameWindow_Storage->SetVoxelExtension((ZVoxelExtension *)VoxelInfo);
  // GameEnv->GameWindow_Storage->Show();
}

ULong ZVoxelType_PlaneZ1::Interface_PushBlock_Push( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count )
{
  ZVoxelExtension_Storage * Ext_Storage;

  Ext_Storage = (ZVoxelExtension_Storage *)DestLocation->Sector->OtherInfos[DestLocation->Offset];

  if (Ext_Storage)
  {
    return(Ext_Storage->StoreBlocks(VoxelType, Count));
  }

  return(0);
}
