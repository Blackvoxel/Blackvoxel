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
 * ZVoxelExtension_ProgRobot_Asm.cpp
 *
 *  Created on: 6 nov. 2014
 *      Author: laurent
 */

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

#ifndef Z_ZVOXELEXTENSION_PROGROBOT_ASM_H
#  include "ZVoxelExtension_ProgRobot_Asm.h"
#endif


ZVoxelExtension_ProgRobot_Asm::~ZVoxelExtension_ProgRobot_Asm()
{

}

void ZVoxelExtension_ProgRobot_Asm::CompileAndRunScript(ULong ContextInfo, ULong ScriptToRun)
{


}

void ZVoxelExtension_ProgRobot_Asm::StopProgram()
{

}

bool ZVoxelExtension_ProgRobot_Asm::IsRunningProgram()
{
  return(false);
}

bool ZVoxelExtension_ProgRobot_Asm::Save(ZStream_SpecialRamStream * Stream)
{

  return(true);
}

bool ZVoxelExtension_ProgRobot_Asm::Load(ZStream_SpecialRamStream * Stream)
{
  bool Ok;
  ULong  ExtensionSize;
  UShort ExtensionVersion;
  UByte  Temp_Byte;
  bool   Compiled;

  Ok = Stream->Get(ExtensionSize);
  Ok&= Stream->Get(ExtensionVersion);     if(!Ok) return(false);

  // Check for supported extension version. If unsupported new version, throw content and continue with a blank extension.

  if (ExtensionVersion>3) { ExtensionSize-=2; for(ZMemSize i=0;i<ExtensionSize;i++) Ok = Stream->Get(Temp_Byte); if (Ok) return(true); else return(false);}

  Ok&=Stream->Get(ScriptNum);
  Ok&=Stream->Get(Compiled);


  Ok&=Stream->GetZone(&VoxelType, sizeof(VoxelType));
  Ok&=Stream->GetZone(&VoxelQuantity, sizeof(VoxelQuantity));

  // Init the Script Engine

  return(Ok);
}


