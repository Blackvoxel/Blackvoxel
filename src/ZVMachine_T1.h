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
 * ZVMachine_T1.h
 *
 *  Created on: 7 nov. 2014
 *      Author: laurent
 */

#ifndef Z_ZVMACHINE_T1_H
#define Z_ZVMACHINE_T1_H

//#ifndef Z_ZVMACHINE_T1_H
//#  include "ZVMachine_T1.h"
//#endif

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

#ifndef Z_ZFASTBIT_ARRAY_H
#  include "z/ZFastBit_Array.h"
#endif

#ifndef Z_ZCHIP_PARALLEL_INTERFACE_SERVO_H
#  include "ZChip_Parallel_Interface_Servo.h"
#endif

#ifndef Z_ZCHIP_DISPLAY_CONTROLLER_H
#  include "ZChip_Display_Controller.h"
#endif

#ifndef Z_ZVCPU_H
#  include "ZVCPU.h"
#endif

#ifndef Z_ZVMACHINE_H
#  include "ZVMachine.h"
#endif


// 1M Ram.

#define ZVMACHINE_MEMSIZE 65536

class ZActiveVoxelInterface;

class ZVMachine_T1 : public ZVMachine
{
  public:

   ZActiveVoxelInterface * VoxelInterface;
   UByte Memory[ZVMACHINE_MEMSIZE];
   ZChip_Parallel_Interface_Servo PIA_1;
   ZChip_Display_Controller       Display_1;
   BlackCPU<ZVMachine_T1>CPU;
   ZFastBit_Array DisassemblyTags;

   ZVector3L Servo_MovePos;
   ZMemSize  Servo_LastMoveCycleCount;

   bool Running;
   bool HasMoved;

   ZVMachine_T1()
   {
     ZMemSize i;

     CPU.SetVMachine(this); Running = false;
     PIA_1.SetVMachine(this);
     PIA_1.SetCPUInterruptLineNumber(3);
     PIA_1.SetCPU(&CPU);
     HasMoved = false;
     VoxelInterface = 0;
     DisassemblyTags.InitSize(ZVMACHINE_MEMSIZE);
     DisassemblyTags.Clear();
     Servo_MovePos = 0;
     Servo_LastMoveCycleCount = 0;

     // Erase memory

     for(i=0;i<ZVMACHINE_MEMSIZE;i++) Memory[i]=0;
   }

   ~ZVMachine_T1()
   {
   }


   void Envcall()
   {
     PIA_1.DoChipManagement();
   }

   void FrameCall()
   {
     PIA_1.DoChipManagement_FrameCycle();
   }

   virtual void ClearMemory()
   {
     ZMemSize i;

     for (i=0;i<ZVMACHINE_MEMSIZE;i++) Memory[i]=0;
     DisassemblyTags.Clear();

   }

   virtual void PowerOn()
   {

     DisassemblyTags.Clear();

     // Reset All chips

     PIA_1.Reset();
     Display_1.Reset();
     CPU.StartCPU();
     Servo_MovePos = 0;
     Servo_LastMoveCycleCount = 0;

     // Allow machine to run

     Running = true;

   }

   virtual void RunQuantum()
   {
     if (Running)
     {
       HasMoved = false;
       PIA_1.SetChipInputPortData( (PIA_1.GetChipInputPortData() & (~1024)) | ((CPU.GetFrameCycles() & 1) << 10));
       CPU.Execute(5000);
     }
   }

   virtual void StopProgram()
   {
     CPU.StopProgram();
   }

   virtual bool IsRunningProgram()
   {
     return(CPU.IsRunningProgram());
   }

   inline void SetVoxelInterface(ZActiveVoxelInterface * VoxelInterface)
   {
     this->VoxelInterface = VoxelInterface;
   }

   // CPU Memory interfaces

   inline UByte FetchOpcode(ULong Address)
   {
     return(Memory[ Address & (ZVMACHINE_MEMSIZE-1) ]);
   }

   inline UByte FetchOperand_8(ULong Address)
   {
     return(Memory[ Address & (ZVMACHINE_MEMSIZE-1) ]);
   }

   inline UShort FetchOperand_16(ULong Address)
   {
     ZMemSize a;

     a = (ZMemSize)& Memory;
     a += (Address & (ZVMACHINE_MEMSIZE-1));

     return (* (UShort *)a);
     // return(*(UShort *)(&Memory + (Address & (ZVMACHINE_MEMSIZE-1)) ));
   }

   inline ULong FetchOperand_32(ULong Address)
   {
     ZMemSize a;

     a = (ZMemSize)& Memory;
     a += (Address & (ZVMACHINE_MEMSIZE-1));

     return (* (ULong *)a);
   }

   inline UByte ReadMemory_8(ULong Address)
   {
     ZMemSize a;


     if (!(Address >> 31))
     {
       a = (ZMemSize)& Memory;
       a += (Address & (ZVMACHINE_MEMSIZE-1));
       return(*(UByte *)a);
     }
     else return(ReadMemory_32(Address));
   }

   inline UShort ReadMemory_16(ULong Address)
   {
     ZMemSize a;
     if (!(Address >> 31))
     {
       a = (ZMemSize)& Memory;
       a += (Address & (ZVMACHINE_MEMSIZE-1));
       return(*(UShort *)a);
     }
     else return(ReadMemory_32(Address));
   }



   inline void WriteMemory_8(ULong Address, UByte Data)
   {
     ZMemSize a;

     a = (ZMemSize)& Memory;
     a += (Address & (ZVMACHINE_MEMSIZE-1));

     * (UByte *)a = Data;
   }

   inline void WriteMemory_16(ULong Address, UShort Data)
   {
     ZMemSize a;

     a = (ZMemSize)& Memory;
     a += (Address & (ZVMACHINE_MEMSIZE-1));

     * (UByte *)a = Data;
   }

   inline ULong ReadMemory_32(ULong Address)
   {
     if (!(Address >> 31))
     {
       register ZMemSize a;
       a = (ZMemSize)& Memory;
       a += (Address & (ZVMACHINE_MEMSIZE-1));
       return(*(ULong *)a);
     }
     else
     {
       switch(Address >> 27)
       {
         case 0x10: return(PIA_1.GetRegister(Address));
         default:   return(0);
       }
     }
   }

   inline void WriteMemory_32(ULong Address, ULong Data)
   {
     if (!(Address >> 31))
     {
       register ZMemSize a;
       a = (ZMemSize)& Memory;
       a += (Address & (ZVMACHINE_MEMSIZE-1));
       *(ULong *)a = Data;
     }
     else
     {
       // 0xxxxxx
       // 00000000 00000000 00000000 00000000
       switch(Address >> 25)
       {
         case 0x40: PIA_1.SetRegister(Address, Data);     break;
         case 0x41: Display_1.SetRegister(Address, Data); break;
         default:   break;
       }
     }
   }

   inline void PushStack_32(ULong Address, ULong Data)
   {
     ZMemSize a;

     a = (ZMemSize)& Memory;
     a += (Address & (ZVMACHINE_MEMSIZE-1));

     * (ULong *)a = Data;
   }

   inline ULong PopStack_32(ULong Address)
   {
     ZMemSize a;

     a = (ZMemSize)& Memory;
     a += (Address & (ZVMACHINE_MEMSIZE-1));

     return (* (ULong *)a);
   }

   bool BinaryLoad(ZString * BinaryFile);


   virtual ZMemSize GetProgramCounter() { return(CPU.ProgramCounter); }
   virtual bool GetDisassembly(ZMemSize & ProgramCounter, ZString & Opcode, ZString & Operand)
   {
     ULong Pc;
     Bool RetVal;

     // Disassemble current location.

     Pc = (ULong)ProgramCounter;
     RetVal = CPU.Disassemble(Pc, Opcode, Operand);

     // Update the disassembly tag array to recall this instruction location.

     if (ProgramCounter < ZVMACHINE_MEMSIZE)
     {
       if (RetVal) DisassemblyTags.Set(ProgramCounter, true);
       else        DisassemblyTags.Set(ProgramCounter, false);
     }

     // Update Program counter to next instruction.

     ProgramCounter = Pc;

     return(RetVal);
   };

   virtual bool GetPreviousInstructionAddress(ZMemSize &ProgramCounter)
   {
     ZMemSize Pc, i;

     Pc = ProgramCounter;

     // Search for previous instruction in the tag array.

     for (i=0;i<6;i++)
     {
       --Pc;
       if (Pc >= ZVMACHINE_MEMSIZE) return(false);
       if (DisassemblyTags.Get(Pc)) { ProgramCounter = Pc; return(true); }
     }

     return(false);
   }

    virtual void  SetStepModeOn(bool StepMode = true)
    {
      CPU.SetStepModeOn(StepMode);
    }

    virtual void  EnableRunningStepCount(ULong StepCount)
    {
      CPU.EnableRunningStepCount(StepCount);
    }

    virtual ULong GetCPURegisterCount(ULong InfoType)
    {
      return(CPU.GetCPURegisterCount(InfoType));
    }

    virtual bool  GetCPURegister_Alpha(ULong InfoType, ULong RegisterNum, ZString & RegisterName, ZString & RegisterValue)
    {
      return(CPU.GetCPURegister_Alpha(InfoType, RegisterNum, RegisterName, RegisterValue));
    }

    virtual void GetVRAMInfo(ULong & DisplayMode, UByte * &RamBase, ZMemSize &RamMask,  ZMemSize & VideoMemoryOffset)
    {
      DisplayMode = Display_1.GetDisplayMode();
      RamBase = Memory;
      RamMask = ZVMACHINE_MEMSIZE - 1;
      VideoMemoryOffset = Display_1.GetVideoPointer();
    }
    virtual void TriggerDisplayInterrupt()
    {
      Display_1.TriggerInterrupt();
    }

    virtual bool Save(ZStream_SpecialRamStream * Stream);
    virtual bool Load(ZStream_SpecialRamStream * Stream);

  protected:

    bool ChipSave(ZStream_SpecialRamStream * Stream);

};

#endif /* Z_ZVMACHINE_T1_H */
