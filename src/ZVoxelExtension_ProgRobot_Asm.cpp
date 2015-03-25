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

extern ZGame * Ge;

ZVoxelExtension_ProgRobot_Asm::~ZVoxelExtension_ProgRobot_Asm()
{

}

bool ZVoxelExtension_ProgRobot_Asm::CompileAndRunScript(ULong ContextInfo, ULong ScriptToRun, bool StepMode)
{
  ZString ProgramText, FileName, FileSpec, BinaryOut;

  ScriptNum = ScriptToRun;

  // Make filename for the program to load

  FileSpec = Ge->Path_UserScripts_Asm_1;
  FileName = ScriptToRun;
  FileName.Append_pchar(".asm");
  FileSpec.AddToPath(FileName);

  // Clear Status

  Status.Clear();

  // Load from the file.

  if (!ProgramText.LoadFromFile(FileSpec.String))
  {
    Status.CompilationResult = false;
    Status.Error = new ZMacroAssembler::ZError;
    Status.Error->CantLoadFile = true;
    Status.Error->FileName = FileSpec;
    return(false);
  }

  // Assemble program

  if (!MacroAssembler.Assemble(ProgramText, BinaryOut, Status )) return(false);

  // Clear Memory

  VirtualMachine.ClearMemory();

  // Load it into the virtual machine

  if (!VirtualMachine.BinaryLoad(&BinaryOut)) return(false);

  // Step mode

  VirtualMachine.SetStepModeOn(StepMode);

  // Run it.

  VirtualMachine.PowerOn();

  return(true);
}

void ZVoxelExtension_ProgRobot_Asm::StopProgram()
{
  VirtualMachine.StopProgram();
}

bool ZVoxelExtension_ProgRobot_Asm::IsRunningProgram()
{
  return(VirtualMachine.IsRunningProgram());
}

bool ZVoxelExtension_ProgRobot_Asm::Save(ZStream_SpecialRamStream * Stream)
{
  ULong * ExtensionSize;
  ULong   StartLen;

  ExtensionSize = Stream->GetPointer_ULong();
  Stream->Put(0u);       // The size of the extension (defered storage).
  StartLen = Stream->GetActualBufferLen();
  Stream->Put((UShort)1); // Extension Version;

  // Storage informations.

  Stream->Put(ScriptNum);
  Stream->Put(RobotLevel);
  Stream->Put(RobotSerialNumber);
  Stream->Put(ProgramText);
  Stream->PutZone(&VoxelType,sizeof(VoxelType));
  Stream->PutZone(&VoxelQuantity,sizeof(VoxelQuantity));

  VirtualMachine.Save(Stream);

  *ExtensionSize = Stream->GetActualBufferLen() - StartLen;

  return(true);
}

bool ZVoxelExtension_ProgRobot_Asm::Load(ZStream_SpecialRamStream * Stream)
{
    bool Ok;

    ULong  ExtensionSize;
    UShort ExtensionVersion;
    UByte  Temp_Byte;

    Ok = Stream->Get(ExtensionSize);
    Ok&= Stream->Get(ExtensionVersion);     if(!Ok) return(false);

    // Check for supported extension version. If unsupported new version, throw content and continue with a blank extension.

    if (ExtensionVersion!=1) { ExtensionSize-=2; for(ZMemSize i=0;i<ExtensionSize;i++) Ok = Stream->Get(Temp_Byte); if (Ok) return(true); else return(false);}

    Ok &= Stream->Get(ScriptNum);
    Ok &= Stream->Get(RobotLevel);
    Ok &= Stream->Get(RobotSerialNumber);
    Ok &= Stream->Get(ProgramText);
    Ok &= Stream->GetZone(&VoxelType, sizeof(VoxelType));
    Ok &= Stream->GetZone(&VoxelQuantity, sizeof(VoxelQuantity));
    Ok &= VirtualMachine.Load(Stream);

    return(Ok);
}



