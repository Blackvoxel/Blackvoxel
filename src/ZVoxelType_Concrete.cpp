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
 * ZVoxelType_Concrete.cpp
 *
 *  Created on: 16 mars 2014
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPE_CONCRETE_H
#  include "ZVoxelType_Concrete.h"
#endif

#ifndef Z_ZVOXELSECTOR_H
#  include "ZVoxelSector.h"
#endif

#ifndef Z_VOXELEXTENSIONTYPE_VOXELFLUID_H
#  include "ZVoxelExtensionType_VoxelFluid.h"
#endif

ZVoxelExtension * ZVoxelType_Concrete::CreateVoxelExtension(bool IsLoadingPhase)
{
  float Pressure = 0.0;
  ZMemSize Ms = *((ULong *)&Pressure);

  return( (ZVoxelExtension *)Ms);
}

void  ZVoxelType_Concrete::GetBlockInformations(ZVoxelLocation * DestLocation, ZString & Infos)
{
  ZVoxelExtensionType_VoxelFluid * VoxelInfo;
  UShort   Pressure;

  VoxelInfo = (ZVoxelExtensionType_VoxelFluid *)&DestLocation->Sector->OtherInfos[DestLocation->Offset];

  Pressure = VoxelInfo->Pressure;

  Infos << "Pressure : " << Pressure;

  return;
}
