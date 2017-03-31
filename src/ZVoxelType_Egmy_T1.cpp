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
 * ZVoxelType_Egmy_T1.cpp
 *
 *  Created on: 23 sept. 2013
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPE_EGMY_T1_H
#  include "ZVoxelType_Egmy_T1.h"
#endif

#ifndef Z_ZVOXELEXTENSION_SEQUENCER_H
#  include "ZVoxelExtension_Egmy_T1.h"
#endif

#ifndef Z_ZVOXELSECTOR_H
#  include "ZVoxelSector.h"
#endif


ZVoxelExtension * ZVoxelType_Egmy_T1::CreateVoxelExtension(bool IsLoadingPhase)
{
  ZVoxelExtension * NewVoxelExtension = 0;

  NewVoxelExtension = new ZVoxelExtension_Egmy_T1;

  return (NewVoxelExtension);
}

void ZVoxelType_Egmy_T1::GetBlockInformations(ZVoxelLocation * DestLocation, ZString & Infos)
{
  ZVoxelExtension_Egmy_T1 * Ext;

  Ext = (ZVoxelExtension_Egmy_T1 * )DestLocation->Sector->OtherInfos[DestLocation->Offset];

  Infos << "Loc :" << Ext->TargetLocation.x << "," << Ext->TargetLocation.y << "," << Ext->TargetLocation.z << Infos.NewLine();
  Infos << "TargetVoxelType :" << Ext->TargetVoxelType << Infos.NewLine();
  Infos << "EvasionMode :" << ((Ext->EvasionMode) ? "Y" : "N") << Infos.NewLine();
  Infos << "EvasionCount :" << (Ext->EvasionCount) << Infos.NewLine();
}
