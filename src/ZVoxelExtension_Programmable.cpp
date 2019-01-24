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
 * ZvoxelExtension_Programmable.cpp
 *
 *  Created on: 8 janv. 2013
 *      Author: laurent
 */

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

#ifndef Z_ZVOXELEXTENSION_PROGRAMMABLE_H
#  include "ZVoxelExtension_Programmable.h"
#endif

#ifndef Z_ZGENERICCANVA2_H
#  include "ZGenericCanva_2.h"
#endif

extern ZGame * Ge;

ZVoxelExtension_Programmable::~ZVoxelExtension_Programmable()
{
  ULong i;

  for (i=0;i<ImageTable_Size;i++) if (ImageTable[i]) delete ImageTable[i];
}

void ZVoxelExtension_Programmable::CompileAndRunScript(ULong ContextInfo, ULong ScriptToRun)
{
  ZString FileName;

  if (ScriptToRun == (ULong)-1) ScriptToRun = ScriptNum;



  // If program continue running then stop it.
  if (Script_Engine.IsRunningProgram())
  {
    do { Script_Engine.StopProgram(); } while(Script_Engine.IsRunningProgram());
  }

  Script_Engine.SetExtension(this);
  if (Script_Engine.IsCompiledOk())
  {
    IsAllowedToRun = true;
    switch(ContextInfo)
    {
      case CONTEXT_LOADING:       break;
      case CONTEXT_NEWVOXEL:      break;
      case CONTEXT_PROGRAMCHANGE: Script_Engine.RunScript("Voxel_Unload",false, ZScripting_Squirrel3::RUNCONTEXT_PROGRAMCHANGE ); break;
    }
  }

  ScriptNum = ScriptToRun;
  FileName << ScriptNum << ".nut";
  Script_Engine.Init();
  Script_Engine.SetExtension(this);
  Script_Engine.SetScriptFileSpec(Ge->Path_UserScripts.Path("Squirrel").Path(FileName).String );
  Script_Engine.LoadAndCompileScript();

  switch(ContextInfo)
  {
    case CONTEXT_LOADING:       Script_Engine.RunScript("Voxel_Load",false, ZScripting_Squirrel3::RUNCONTEXT_LOADING); break;
    case CONTEXT_NEWVOXEL:      Script_Engine.RunScript("Voxel_Load",false, ZScripting_Squirrel3::RUNCONTEXT_CREATE); break;
    case CONTEXT_PROGRAMCHANGE: Script_Engine.RunScript("Voxel_Load",false, ZScripting_Squirrel3::RUNCONTEXT_PROGRAMCHANGE); break;
  }

}

void ZVoxelExtension_Programmable::StopProgram()
{
  Script_Engine.StopProgram();
  IsAllowedToRun = false;
}

bool ZVoxelExtension_Programmable::IsRunningProgram()
{
  return(Script_Engine.IsRunningProgram());
}

bool ZVoxelExtension_Programmable::Save(ZStream_SpecialRamStream * Stream)
{
  ULong * ExtensionSize;
  ULong   StartLen;

  ExtensionSize = Stream->GetPointer_ULong();
  Stream->Put(0u);       // The size of the extension (defered storage).
  StartLen = Stream->GetActualBufferLen();
  Stream->Put((UShort)4); // Extension Version;

  // Storage informations.
  Stream->Put(ScriptNum);
  //Stream->Put(Script_Engine.ScriptCompiledOK);
  Stream->Put(IsAllowedToRun);
  Stream->Put(RobotSerialNumber);
  Stream->Put(Overclock);
  Stream->PutZone(&VoxelType,sizeof(VoxelType));
  Stream->PutZone(&VoxelQuantity,sizeof(VoxelQuantity));

  *ExtensionSize = Stream->GetActualBufferLen() - StartLen;

  Script_Engine.RunScript("Voxel_Unload",false, ZScripting_Squirrel3::RUNCONTEXT_LOADING );

  return(true);
}

bool ZVoxelExtension_Programmable::Load(ZStream_SpecialRamStream * Stream)
{
  bool Ok;
  ULong  ExtensionSize;
  UShort ExtensionVersion;
  UByte  Temp_Byte;
  bool   Compiled;

  Ok = Stream->Get(ExtensionSize);
  Ok&= Stream->Get(ExtensionVersion);     if(!Ok) return(false);

  // Check for supported extension version. If unsupported new version, throw content and continue with a blank extension.

  if (ExtensionVersion>4) { ExtensionSize-=2; for(ZMemSize i=0;i<ExtensionSize;i++) Ok = Stream->Get(Temp_Byte); if (Ok) return(true); else return(false);}

  Ok&=Stream->Get(ScriptNum);
  if (ExtensionVersion<4) Ok&=Stream->Get(Compiled);
  if (ExtensionVersion>3) Ok&=Stream->Get(IsAllowedToRun);
  if (ExtensionVersion>2) Ok&=Stream->Get(RobotSerialNumber);
  else                    RobotSerialNumber = Ge->GameInfo.RobotNextSerial++;
  if (ExtensionVersion>3) Ok&=Stream->Get(Overclock);
  else                    Overclock = 0;

  Ok&=Stream->GetZone(&VoxelType, sizeof(VoxelType));
  Ok&=Stream->GetZone(&VoxelQuantity, sizeof(VoxelQuantity));

  // Init the Script Engine

  CompileAndRunScript(CONTEXT_LOADING);

  return(Ok);
}
