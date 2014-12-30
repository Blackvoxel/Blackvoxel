/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2014 Laurent Thiebaut & Olivia Merle
 *    Contributions by: d3x0r
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
 * ZMachine_Circuit_Parallel_Interface.h
 *
 *  Created on: 7 nov. 2014
 *      Author: laurent
 */

#ifndef Z_ZCHIP_PARALLEL_INTERFACE_H
#define Z_ZCHIP_PARALLEL_INTERFACE_H

//#ifndef Z_ZCHIP_PARALLEL_INTERFACE_H
//#  include "ZChip_Parallel_Interface.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZCIRCUIT_H
#  include "ZChip.h"
#endif

#ifndef Z_ZVCPU_CHIP_INTERFACE_H
#  include "ZVCPU_Chip_Interface.h"
#endif

#include <stdio.h>

#define ZCHIP_PI_CR_INTERRUPT_ENABLE 1
#define ZCHIP_PI_CR_INTERRUPT_TRIGGERED 2

class ZChip_Parallel_Interface : public ZChip
{
  protected:

    // Data Stored
    ULong Data;
    ULong Direction;
    ULong Interrupt_EnableMask;
    ULong Interrupt_Level;
    ULong Control_Register;

    // Effective chip input/output.
    ULong In;  // Set here value comming from outside.
    ULong Out; // Read here value
    ULong Out_Changed;

    ZVCPU_Chip_Interface * Cpu;
    ULong Cpu_Interrupt_Line;

  public:
    ZChip_Parallel_Interface()
    {
      Reset();
    }

    void  Reset()
    {
      Data      = 0;
      Direction = 0; // Read
      Interrupt_EnableMask = 0;
      Interrupt_Level = 0;
      Control_Register = 0;
      In = 0xFFFFFFFF;
      Out = 0;
      Cpu = 0;
      Cpu_Interrupt_Line = 1;
      Out_Changed = 0;
    }

  protected:
    // Register set
    inline void SetRegister_Data(ULong Value)
    {
      ULong New_Out;

      New_Out = Value | ~Direction;
      Data    = Value;
      Out_Changed = New_Out ^ Out;
      Out = New_Out;
      if (   (Control_Register & ZCHIP_PI_CR_INTERRUPT_ENABLE)
          && (Out_Changed & Interrupt_EnableMask & ~(New_Out ^ Interrupt_Level) )) TriggerInterrupt();
    }

    inline ULong GetRegister_Data()
    {
      return((In & ~Direction) | (Direction & Data));
    }

    inline void TriggerInterrupt()
    {
      Control_Register |= ZCHIP_PI_CR_INTERRUPT_TRIGGERED;
      Cpu->TriggerInterrupt(Cpu_Interrupt_Line);
    }

  public:

    inline void  SetRegister(ULong Register, ULong Value)
    {
      printf("ZChip_Parallel_Interface::SetRegister(b) ");PrintStatus();
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
      printf("ZChip_Parallel_Interface::SetRegister(a) ");PrintStatus();

    }

    inline ULong GetRegister(ULong Register)
    {
      //printf("ZChip_Parallel_Interface::GetRegister ");PrintStatus();
      switch(Register & 28)
      {
        case 0 << 2: return(GetRegister_Data());
        case 1 << 2: return(Direction);
        case 2 << 2: return(Interrupt_EnableMask);
        case 3 << 2: return(Interrupt_Level);
        case 4 << 2: return(Control_Register);
        default: return(0);
      }
    }

    // Set the data entering the chip.

    inline void SetChipInputPortData(ULong Value)
    {
      //printf("ZChip_Parallel_Interface::SetChipInputPortData(b) ");PrintStatus();
      if (     (Control_Register & ZCHIP_PI_CR_INTERRUPT_ENABLE)
             &&((In ^ Value) & (~(Value ^ Interrupt_Level)) & Interrupt_EnableMask ) )
      {
        TriggerInterrupt();
      }

      In = Value;
      //printf("ZChip_Parallel_Interface::SetChipInputPortData(a) ");PrintStatus();

    }

    inline ULong GetChipInputPortData()
    {
      return(In);
    }


    inline void SetCPUInterruptLineNumber(ULong InterruptLine)
    {
      Cpu_Interrupt_Line = InterruptLine;
    }

    inline void  DoChipManagement()
    {

    };

    void PrintStatus()
    {
      printf("PIA Data: %x Dir: %x In: %x Out: %x\n", this->Data, this->Direction, this->In, this->Out);
    }

    virtual bool Save(ZStream_SpecialRamStream * Stream);
    virtual bool Load(ZStream_SpecialRamStream * Stream);



    ~ZChip_Parallel_Interface(){}
};


#endif /* Z_ZCIRCUIT_PARALLEL_INTERFACE_H */
