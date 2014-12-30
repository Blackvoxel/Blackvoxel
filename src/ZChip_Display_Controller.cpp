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
 * ZChip_Display_Controller.cpp
 *
 *  Created on: 24 déc. 2014
 *      Author: laurent
 */

#include "ZChip_Display_Controller.h"

#ifndef Z_ZVMACHINE_T1_H
#  include "ZVMachine_T1.h"
#endif

#ifndef Z_ZACTIVEVOXELINTERFACE_H
#  include "ZActiveVoxelInterface.h"
#endif


#define ZCHIP_PI_SERVO_MOTORTHRESHOLD 5000

void ZChip_Display_Controller::Reset()
{
  DisplayMode = 0;
  VideoPointer = 0;
  Interrupts = 0;
}

void ZChip_Display_Controller::TriggerInterrupt()
{
  if (Interrupts & Z_ZCHIP_DISPLAY_CONTROLLER_INTERRUPT_ENABLE)
  {
    Interrupts |= Z_ZCHIP_DISPLAY_CONTROLLER_INTERRUPT_TRIGGERED;
    ((ZVMachine_T1 *)VMachine)->CPU.TriggerInterrupt(4);
  }
}

void ZChip_Display_Controller::SetRegister(ULong Register, ULong Value)
{
  // printf("ZChip_Parallel_Interface_Servo::SetRegister(b) ");PrintStatus();

  switch(Register & 28)
  {
    case 0 << 2: DisplayMode  = Value; break;
    case 1 << 2: VideoPointer = Value; break;
    case 2 << 2: Interrupts   = Value; break;
  }
  // printf("ZChip_Parallel_Interface_Servo::SetRegister(a) ");PrintStatus();
}

ULong ZChip_Display_Controller::GetRegister(ULong Register)
{
  switch(Register & 28)
  {
    case 0 << 2: return(DisplayMode);
    case 1 << 2: return(VideoPointer);
    case 2 << 2: return(Interrupts);
    default: return(0);
  }
}


bool ZChip_Display_Controller::Save(ZStream_SpecialRamStream * Stream)
{
  ULong * BlockSize;
  ULong   StartLen;

  BlockSize = Stream->GetPointer_ULong();
  Stream->Put(0u);        // The size of the block (defered storage).
  StartLen = Stream->GetActualBufferLen();
  Stream->Put((UShort)1); // Data Block Version;

  // Storage informations.

  Stream->Put(DisplayMode);
  Stream->Put(Interrupts);
  Stream->Put(Interrupts);

  // Compute Block Len and write it to start of the block.

  *BlockSize = Stream->GetActualBufferLen() - StartLen;

  return(true);
}

bool ZChip_Display_Controller::Load(ZStream_SpecialRamStream * Stream)
{
  bool Ok;

  ULong  BlockSize;
  UShort BlockVersion;
  UByte  Temp_Byte;

  Ok = Stream->Get(BlockSize);
  Ok&= Stream->Get(BlockVersion);     if(!Ok) return(false);

  // Check for supported block version. If unsupported new version, throw content and continue with a blank dataset.

  if (BlockVersion!=1) { BlockSize-=2; for(ZMemSize i=0;i<BlockSize;i++) Ok = Stream->Get(Temp_Byte); if (Ok) return(true); else return(false);}

  Ok &= Stream->Get(DisplayMode);
  Ok &= Stream->Get(VideoPointer);
  Ok &= Stream->Get(Interrupts);

  return(Ok);
}
