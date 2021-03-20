/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2021 Laurent Thiebaut & Olivia Merle
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
 * ZVoxel.cpp
 *
 *  Created on: 7 févr. 2021
 *      Author: laurent
 */


#ifndef Z_ZVOXEL_H
#  include "ZVoxel.h"
#endif

#ifndef Z_ZVOXELTYPEMANAGER_H
#  include "ZVoxelTypeManager.h"
#endif

#ifndef Z_ZSTREAM_SPECIALRAMSTREAM_H
#  include "z/ZStream_SpecialRamStream.h"
#endif


ZVoxelTypeManager * ZVoxel::VoxelTypeManager=0;

ZVoxel::~ZVoxel()
{
  if (VoxelType==0) return;

  if (VoxelTypeManager->VoxelTable[VoxelType]->Is_HasAllocatedMemoryExtension) VoxelTypeManager->VoxelTable[VoxelType]->DeleteVoxelExtension(OtherInfos,false);
}

bool ZVoxel::Save(ZStream_SpecialRamStream * Stream, bool EmbedVersion)
{
  bool Ok;

  // Version is embeded on demand.
  if (EmbedVersion) Stream->Put32B(0x01); // Version

  Stream->Put(VoxelType);
  // Stream->Put(Temperature); // Not Yet
  if (VoxelTypeManager->VoxelTable[VoxelType]->Is_HasAllocatedMemoryExtension)
  {
    ZVoxelExtension * Ext = (ZVoxelExtension *)OtherInfos;
    Ok = Ext->Save(Stream); if (!Ok) return(false);
  }
  else
  {
    Stream->Put32B(OtherInfos);
  }

  return(true);
}

bool ZVoxel::Load(ZStream_SpecialRamStream * Stream, ULong & FormatVersion)
{
  bool Ok = true;

  if (FormatVersion == 0) {if (! Stream->Get32B(FormatVersion)) return(false);}

  if (FormatVersion != 1) return(false);


  Ok = Stream->Get(VoxelType); if (!Ok) return(false);

  if (VoxelTypeManager->VoxelTable[VoxelType]->Is_HasAllocatedMemoryExtension)
  {
    ZVoxelExtension * Ext;
    Ext = VoxelTypeManager->VoxelTable[VoxelType]->CreateVoxelExtension(false);
    OtherInfos = (ZMemSize)Ext;
    Ok = Ext->Load(Stream); if (!Ok) return(false);
  }
  else
  {
    ULong Infos;
    Ok = Stream->Get32B(Infos); if (!Ok) return(false);
    OtherInfos = Infos;
  }

  return(true);
}

void ZVoxel::Clear()
{
  if (VoxelType)
  {
    if (VoxelTypeManager->VoxelTable[VoxelType]->Is_HasAllocatedMemoryExtension)
    {
      VoxelTypeManager->VoxelTable[VoxelType]->DeleteVoxelExtension(OtherInfos, false);
    }
  }
  OtherInfos = 0;
  Temperature = 0;
}

