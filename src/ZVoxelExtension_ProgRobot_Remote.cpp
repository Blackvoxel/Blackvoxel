/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2017 Laurent Thiebaut & Olivia Merle
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
 * ZVoxelExtension_ProgRobot_Remote.cpp
 *
 *  Created on: 22 mai 2017
 *      Author: laurent
 */

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

#  include "ZVoxelExtension_ProgRobot_Remote.h"


extern ZGame * Ge;

ZVoxelExtension_ProgRobot_Remote::~ZVoxelExtension_ProgRobot_Remote()
{

}

bool ZVoxelExtension_ProgRobot_Remote::Save(ZStream_SpecialRamStream * Stream)
{
  ULong * ExtensionSize;
  ULong   StartLen;

  ExtensionSize = Stream->GetPointer_ULong();
  Stream->Put(0u);       // The size of the extension (defered storage).
  StartLen = Stream->GetActualBufferLen();
  Stream->Put((UShort)2); // Extension Version;

  // Storage informations.

  Stream->Put(RobotSerialNumber);
  Stream->Put(BuildVoxelType);
  Stream->Put(HomeSet);
  Stream->Put(HomeLocation.x);
  Stream->Put(HomeLocation.y);
  Stream->Put(HomeLocation.z);
  Stream->Put(SchoolModeEnableEasyInventory);
  Stream->PutZone(&VoxelType,sizeof(VoxelType));
  Stream->PutZone(&VoxelQuantity,sizeof(VoxelQuantity));

  *ExtensionSize = Stream->GetActualBufferLen() - StartLen;

  return(true);
}

bool ZVoxelExtension_ProgRobot_Remote::Load(ZStream_SpecialRamStream * Stream)
{
    bool Ok;

    ULong  ExtensionSize;
    UShort ExtensionVersion;
    UByte  Temp_Byte;

    Ok = Stream->Get(ExtensionSize);
    Ok&= Stream->Get(ExtensionVersion);     if(!Ok) return(false);

    // Check for supported extension version. If unsupported new version, throw content and continue with a blank extension.

    if (ExtensionVersion<2) { ExtensionSize-=2; for(ZMemSize i=0;i<ExtensionSize;i++) Ok = Stream->Get(Temp_Byte); if (Ok) return(true); else return(false);}

    Ok &= Stream->Get(RobotSerialNumber);
    Ok &= Stream->Get(BuildVoxelType);
    Ok &= Stream->Get(HomeSet);
    Ok &= Stream->Get(HomeLocation.x);
    Ok &= Stream->Get(HomeLocation.y);
    Ok &= Stream->Get(HomeLocation.z);
    Ok &= Stream->Get(SchoolModeEnableEasyInventory);
    Ok &= Stream->GetZone(&VoxelType, sizeof(VoxelType));
    Ok &= Stream->GetZone(&VoxelQuantity, sizeof(VoxelQuantity));

    return(Ok);
}
