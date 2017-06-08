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
 * ZChip_Parallel_Interface_Servo.cpp
 *
 *  Created on: 15 déc. 2014
 *      Author: laurent
 */

#include "ZChip_Parallel_Interface_Servo.h"

#ifndef Z_ZVMACHINE_T1_H
#  include "ZVMachine_T1.h"
#endif

#ifndef Z_ZACTIVEVOXELINTERFACE_H
#  include "ZActiveVoxelInterface.h"
#endif

#ifndef Z_ZVOXELSECTOR_H
#  include "ZVoxelSector.h"
#endif

// Bit 0 X axis servo on
// Bit 1 X axis servo direction
// Bit 2 Y axis servo on
// Bit 4 Y axis servo direction
// Bit 5 Z axis servo on
// bit 6 Z axis servo direction

#define ZCHIP_PI_SERVO_XPOWER 1
#define ZCHIP_PI_SERVO_XDIRECTION 2
#define ZCHIP_PI_SERVO_YPOWER 4
#define ZCHIP_PI_SERVO_YDIRECTION 8
#define ZCHIP_PI_SERVO_ZPOWER 16
#define ZCHIP_PI_SERVO_ZDIRECTION 32
#define ZCHIP_PI_SERVO_X_RETURN 64
#define ZCHIP_PI_SERVO_Y_RETURN 128
#define ZCHIP_PI_SERVO_Z_RETURN 256
#define ZCHIP_PI_SERVO_FLASH    512
#define ZCHIP_PI_SERVO_FRAMEBIT 1024

#define ZCHIP_PI_SERVO_MOTORTHRESHOLD 10000

void ZChip_Parallel_Interface_Servo::DoChipManagement()
{
  ZVMachine_T1 * Vm;
  ZMemSize CycleCounter, CycleQuantum ;
  ZVector3L Mv;
  bool      Moved;

  Vm = (ZVMachine_T1 *)VMachine;

  if (  (Data & (ZCHIP_PI_SERVO_XPOWER | ZCHIP_PI_SERVO_YPOWER | ZCHIP_PI_SERVO_ZPOWER)) ^ (ZCHIP_PI_SERVO_XPOWER | ZCHIP_PI_SERVO_YPOWER | ZCHIP_PI_SERVO_ZPOWER) )
  {


   //printf("ZChip_Parallel_Interface_Servo::DoChipManagement(b) ");PrintStatus();



    CycleCounter   = Vm->CPU.GetElapsedCycles();
    CycleQuantum   = CycleCounter - Vm->Servo_LastMoveCycleCount;
    Vm->Servo_LastMoveCycleCount = CycleCounter;


    Vm = (ZVMachine_T1 *)VMachine;

    Moved = false;
    Mv.x = Mv.y = Mv.z = 0;

    if (!(Out & ZCHIP_PI_SERVO_XPOWER))
    {
      if (Out & ZCHIP_PI_SERVO_XDIRECTION)
      {
        Vm->Servo_MovePos.x += CycleQuantum;
        if (Vm->Servo_MovePos.x > ZCHIP_PI_SERVO_MOTORTHRESHOLD) { Mv.x = 1; Moved = true; Vm->Servo_MovePos.x = 0; }
      }
      else
      {
        Vm->Servo_MovePos.x -= CycleQuantum;
        if (Vm->Servo_MovePos.x <-ZCHIP_PI_SERVO_MOTORTHRESHOLD) { Mv.x = -1; Moved = true; Vm->Servo_MovePos.x = 0;  }
      }
    }

    if (!(Out & ZCHIP_PI_SERVO_YPOWER))
    {
      if (Out & ZCHIP_PI_SERVO_YDIRECTION)
      {
        Vm->Servo_MovePos.y += CycleQuantum;
        if (Vm->Servo_MovePos.y > ZCHIP_PI_SERVO_MOTORTHRESHOLD) { Mv.y = 1; Moved = true; Vm->Servo_MovePos.y = 0; }
      }
      else
      {
        Vm->Servo_MovePos.y -= CycleQuantum;
        if (Vm->Servo_MovePos.y <-ZCHIP_PI_SERVO_MOTORTHRESHOLD) { Mv.y = -1; Moved = true; Vm->Servo_MovePos.y = 0; }
      }
    }

    if (!(Out & ZCHIP_PI_SERVO_ZPOWER))
    {
      if (Out & ZCHIP_PI_SERVO_ZDIRECTION)
      {
        Vm->Servo_MovePos.z += CycleQuantum;
        if (Vm->Servo_MovePos.z > ZCHIP_PI_SERVO_MOTORTHRESHOLD) { Mv.z = 1; Moved = true; Vm->Servo_MovePos.z = 0; }
      }
      else
      {
        Vm->Servo_MovePos.z -= CycleQuantum;
        if (Vm->Servo_MovePos.z <-ZCHIP_PI_SERVO_MOTORTHRESHOLD) { Mv.z = -1; Moved = true; Vm->Servo_MovePos.z = 0; }
      }
    }

    if (Moved)
    {
      UShort VoxelType;
      ZVoxelLocation Loc;
      Vm->HasMoved = true;
      if (Vm->VoxelInterface->GetVoxelExt_Rel(Mv.x, Mv.y, Mv.z, VoxelType, &Loc ))
      {
        if (Vm->VoxelInterface->VoxelTypeManager->VoxelTable[VoxelType]->Is_PlayerCanPassThrough)
        {
          Vm->VoxelInterface->MoveThis( Mv.x, Mv.y, Mv.z , ZActiveVoxelInterface::CHANGE_CRITICAL);
          Vm->VoxelInterface->Coords += Mv;
          Loc.Sector->Flag_HighPriorityRefresh = true;
        }
      }
    }
  }

  SetChipInputPortData(  (  In & ~(ZCHIP_PI_SERVO_X_RETURN | ZCHIP_PI_SERVO_Y_RETURN | ZCHIP_PI_SERVO_Z_RETURN))
                           | (( Vm->VoxelInterface->Coords.x & 1) ? ZCHIP_PI_SERVO_X_RETURN : 0 )
                           | (( Vm->VoxelInterface->Coords.y & 1) ? ZCHIP_PI_SERVO_Y_RETURN : 0 )
                           | (( Vm->VoxelInterface->Coords.z & 1) ? ZCHIP_PI_SERVO_Z_RETURN : 0 ) );
  //printf("ZChip_Parallel_Interface_Servo::DoChipManagement(a) ");PrintStatus();

  if (Control_Register & ZCHIP_PI_CR_INTERRUPT_TRIGGERED) TriggerInterrupt();

}

void ZChip_Parallel_Interface_Servo::Reset()
{
  ZVMachine_T1 * Vm;

  ZChip_Parallel_Interface::Reset();
  if ((Vm = (ZVMachine_T1 *)VMachine))
  {
    if (Vm->VoxelInterface->GetVoxelType_Main() == 237 ) Vm->VoxelInterface->Location.Sector->Data[Vm->VoxelInterface->Location.Offset] = 236;
  }
}


void ZChip_Parallel_Interface_Servo::SetRegister_Data(ULong Value)
{
  ZVMachine_T1 * Vm;

  ZChip_Parallel_Interface::SetRegister_Data(Value);

  if (Out_Changed & ZCHIP_PI_SERVO_FLASH)
  {
    ZVoxelLocation * Location;
    Vm = (ZVMachine_T1 *)VMachine;
    Location = &Vm->VoxelInterface->Location;
    Location->Sector->Data[Location->Offset] = (Out & ZCHIP_PI_SERVO_FLASH) ? 236 : 237;
    Location->Sector->Flag_IsModified |= ZVoxelSector::CHANGE_IMPORTANT;
    Location->Sector->Flag_Render_Dirty = true;
    Location->Sector->Flag_HighPriorityRefresh = true;
  }
}

void ZChip_Parallel_Interface_Servo::SetRegister(ULong Register, ULong Value)
{
  // printf("ZChip_Parallel_Interface_Servo::SetRegister(b) ");PrintStatus();

  switch(Register & 28)
  {
    case 0 << 2: SetRegister_Data(Value);      break;
    case 1 << 2: Direction = Value;            break;
    case 2 << 2: Interrupt_EnableMask = Value; break;
    case 3 << 2: Interrupt_Level = Value;      break;
    case 4 << 2: Control_Register = Value;     break;
    case 5 << 2: break;
    case 6 << 2: break;
    case 7 << 2: break;
  }
  // printf("ZChip_Parallel_Interface_Servo::SetRegister(a) ");PrintStatus();
}
