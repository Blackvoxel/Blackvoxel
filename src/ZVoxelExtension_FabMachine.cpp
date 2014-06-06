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
 * ZVoxelExtension_FabMachine.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: laurent
 */

#ifndef Z_VOXELEXTENSION_FABMACHINE_H
#  include "ZVoxelExtension_FabMachine.h"
#endif

bool ZVoxelExtension_FabMachine::Load(ZStream_SpecialRamStream * Stream)
{
  bool Ok;
  ULong  ExtensionSize;
  UShort ExtensionVersion;
  UByte  Temp_Byte;

  Ok = Stream->Get(ExtensionSize);
  Ok&= Stream->Get(ExtensionVersion);     if(!Ok) return(false);

  // Check for supported extension version. If unsupported new version, throw content and continue with a blank extension.

  if (ExtensionVersion!=3) { ExtensionSize-=2; for(ZMemSize i=0;i<ExtensionSize;i++) Ok = Stream->Get(Temp_Byte); if (Ok) return(true); else return(false);}

  Ok&=Stream->Get(LastInDirection);
  Ok&=Stream->Get(MachineState);
  Ok&=Stream->GetZone(&VoxelType, sizeof(VoxelType));
  Ok&=Stream->GetZone(&VoxelQuantity, sizeof(VoxelQuantity));
  Ok&=Stream->GetZone(&VoxelToOutput, sizeof(VoxelToOutput));
  Ok&=Stream->GetZone(&VoxelToOutputQuantities, sizeof(VoxelToOutputQuantities));
  return(Ok);
}

bool ZVoxelExtension_FabMachine::Save(ZStream_SpecialRamStream * Stream)
{
  ULong * ExtensionSize;
  ULong   StartLen;

  ExtensionSize = Stream->GetPointer_ULong();
  Stream->Put(0u);       // The size of the extension (defered storage).
  StartLen = Stream->GetActualBufferLen();
  Stream->Put((UShort)3); // Extension Version;

  // Storage informations.
  Stream->Put(LastInDirection);
  Stream->Put(MachineState);
  Stream->PutZone(&VoxelType,sizeof(VoxelType));
  Stream->PutZone(&VoxelQuantity,sizeof(VoxelQuantity));
  Stream->PutZone(&VoxelToOutput,sizeof(VoxelToOutput));
  Stream->PutZone(&VoxelToOutputQuantities,sizeof(VoxelToOutputQuantities));

  *ExtensionSize = Stream->GetActualBufferLen() - StartLen;

  return(true);
}
