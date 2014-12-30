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
 * ZVMachine.h
 *
 *  Created on: 15 déc. 2014
 *      Author: laurent
 */

#ifndef Z_ZVMACHINE_H
#define Z_ZVMACHINE_H

// #ifndef Z_ZVMACHINE_H
// #  include "ZVMachine.h"
// #endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

class ZGame;
class ZString;
class ZStream_SpecialRamStream;

class ZVMachine
{
  public:
    virtual ~ZVMachine() {}

    // Control


    virtual void PowerOn()=0;
    virtual void RunQuantum()=0;
    virtual void StopProgram()=0;
    virtual bool IsRunningProgram()=0;

    // Debug Interface.

    virtual ZMemSize GetProgramCounter()=0;
    virtual bool     GetDisassembly(ZMemSize & ProgramCounter, ZString & Opcode, ZString & Operand) = 0;
    virtual bool     GetPreviousInstructionAddress(ZMemSize &ProgramCounter) = 0;
    virtual void     SetStepModeOn(bool StepMode = true) = 0;
    virtual void     EnableRunningStepCount(ULong StepCount) = 0;

    // Status display interface

    virtual ULong GetCPURegisterCount(ULong InfoType)=0;
    virtual bool  GetCPURegister_Alpha(ULong InfoType, ULong RegisterNum, ZString & RegisterName, ZString & RegisterValue)=0;

    // Visual Display Interface

    virtual void GetVRAMInfo(ULong & DisplayMode, UByte * &RamBase, ZMemSize &RamMask,  ZMemSize & VideoMemoryOffset) {};
    virtual void TriggerDisplayInterrupt() {};

    // Persistence.

    virtual bool Save(ZStream_SpecialRamStream * Stream)=0;
    virtual bool Load(ZStream_SpecialRamStream * Stream)=0;

};

#endif /* Z_ZVMACHINE_H */
