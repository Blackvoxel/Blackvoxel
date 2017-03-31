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
 * ZVoxelType_MotherMachine.cpp
 *
 *  Created on: 22 déc. 2012
 *      Author: laurent
 */

#include "ZVoxelType_FabMachine.h"

#ifndef Z_VOXELEXTENSION_MOTHERMACHINE_H
#  include "ZVoxelExtension_FabMachine.h"
#endif

#ifndef Z_ZVOXELSECTOR_H
#  include "ZVoxelSector.h"
#endif

#ifndef Z_ZVOXELTYPEMANAGER_H
#  include "ZVoxelTypeManager.h"
#endif

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

ZVoxelExtension * ZVoxelType_FabMachine::CreateVoxelExtension(bool IsLoadingPhase)
{
  ZVoxelExtension * NewVoxelExtension = 0;

  NewVoxelExtension = new ZVoxelExtension_FabMachine;

  return (NewVoxelExtension);
}

void ZVoxelType_FabMachine::GetBlockInformations(ZVoxelLocation * DestLocation, ZString & Infos)
{
  ZVoxelExtension_FabMachine * Ext;

  ULong i, Quantity;
  UShort VoxelType;

  Ext = (ZVoxelExtension_FabMachine * )DestLocation->Sector->OtherInfos[DestLocation->Offset];

  for (i=0;i< ZVoxelExtension_FabMachine::Storage_NumSlots;i++ )
  {
    VoxelType = Ext->VoxelType[i];
    Quantity  = Ext->VoxelQuantity[i];

    if (Quantity && VoxelType)
    {
      Infos<<Quantity<<" "<<VoxelTypeManager->VoxelTable[VoxelType]->VoxelTypeName<<"\n";
    }
  }
}

void ZVoxelType_FabMachine::GetScanInformations(ZVoxelCoords * VoxelCoords, UShort VoxelType, ZMemSize VoxelInfo, ZString & Inf)
{
  ULong i;
  UShort Voxel;
  ZVoxelType * VoxelT;
  ZVoxelExtension_FabMachine * Ext = (ZVoxelExtension_FabMachine *) VoxelInfo;

  Inf.Append_pchar("\nMACHINE CONTENT :\n\n");
  for (i=0;i<ZVoxelExtension_FabMachine::Storage_NumSlots;i++)
  {
    if (Ext->VoxelQuantity[i]>0 && Ext->VoxelType[i]!=0)
    {
      Voxel = Ext->VoxelType[i];
      VoxelT = GameEnv->VoxelTypeManager.GetVoxelType(Voxel);
      Inf.Append_pchar(VoxelT->VoxelTypeName.String);
      Inf.Append_pchar(" : ");
      Inf.Append_ULong(Ext->VoxelQuantity[i]);
      Inf.Append_pchar("\n");
    }

  }
}

