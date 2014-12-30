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
 * ZTest_Parts.h
 *
 *  Created on: 11 jul. 2014
 *      Author: laurent
 */

#ifndef Z_ZVCPU_H
#define Z_ZVCPU_H

//#ifndef Z_ZVCPU_H
//#  include "ZVCPU.h"
//#endif

#include <stdio.h>

#define Z_ZVCPU_EXPERIMENTAL_ASMCODE 0

#ifndef Z_ZVCPU_CHIP_INTERFACE_H
#  include "ZVCPU_Chip_Interface.h"
#endif

#ifndef Z_ZSTREAM_SPECIALRAMSTREAM_H
#  include "z/ZStream_SpecialRamStream.h"
#endif

/*
unsigned char optable[]=
{
  1+128,0,128+2,0,0,0,
  1+128,1,2,0,0,0,
  13,1,
  255,255,
  255,0
};
*/

#define FLAG_CARRY 1
#define FLAG_ZERO  64
#define FLAG_NEGATIVE 128
#define FLAG_OVERFLOW 2048
#define SZ_08 0
#define SZ_16 64
#define SZ_32 128
#define SZ_64 192

struct StatusRegister_x86
{
  bool CarryFlag:1;
  bool reserved_1:1;
  bool ParityFlag:1;
  bool reserved_2:1;
  bool AdjustFlag:1;
  bool reserved_3:1;
  bool ZeroFlag:1;
  bool SignFlag:1;
  bool TrapFlag:1;
  bool InterruptEnableFlag:1;
  bool DirectionFlag:1;
  bool OverflowFlag:1;
  int  IOPL:2;
  int  Other:16;
};

struct StatusRegister_BCPU
{
  bool  ZeroFlag:1;
  bool  NegativeFlag:1;
  bool  OverflowFlag:1;
  bool  CarryFlag:1;
  bool  pad:20;
  UByte InterruptLevel:4;
};



template <typename ZVMachine>
class BlackCPU : public ZVCPU_Chip_Interface
{
  public:
  union ZGeneralRegister
  {
    UByte  Reg_UByte;
    Byte   Reg_SByte;
    UShort Reg_UWord;
    Short  Reg_SWord;
    ULong  Reg_ULong;
    Long   Reg_SLong;
    ULong  Reg_ULargest;
    Long   Reg_SLargest;
  };
  
  union ZStatusRegister
  {
    StatusRegister_BCPU Flags;
    ULong WholeRegister;
  };

  ZGeneralRegister GeneralRegister[16];
  ULong            ProgramCounter;
  ZStatusRegister  StatusRegister;
  UShort           Interrupt_Pending;
  ZVMachine      * VMachine;

  // Step Mode - When set, ignore run unless StepCount 'account' is reloaded.

  bool StepMode;
  ULong StepCount;

  // External execution control

  bool IsRunning;// Running flag. Is true when the CPU is executing an execution slice.
  bool BreakRun; // Force virtual machine to stop execution in a reasonnable delay.

  BlackCPU()
  {
    unsigned long i;

    for(i=0;i<15;i++) GeneralRegister[i].Reg_ULargest = 0;
    GeneralRegister[15].Reg_ULargest = 65536; // Stack pointer start on end of the ram.
    ProgramCounter = 0;
    StatusRegister.WholeRegister = 0;
    Interrupt_Pending = 0;
    VMachine = 0;
    ElapsedCycles = 0;
    FrameCycles = 0;
    StepMode = false;
    StepCount = 0;
    IsRunning = false;
    BreakRun  = false;
  }

  virtual ~BlackCPU()
  {


  }

  void StartCPU()
  {
    ULong i;

    BreakRun = false;
    ProgramCounter = 0;
    ElapsedCycles = 0;
    FrameCycles = 0;
    for (i=0;i<16;i++) GeneralRegister[i].Reg_ULargest = 0;
    StatusRegister.WholeRegister = 0;
  }

  virtual void StopProgram()
  {
    BreakRun = true;
  }

  virtual bool IsRunningProgram()
  {
    return(IsRunning);
  }

  void TriggerInterrupt(ULong InterruptLevel)
  {
    Interrupt_Pending |= 1 << (InterruptLevel);
  }


  void SetVMachine(ZVMachine * VMachine)
  {
    this->VMachine = VMachine;
  }

  inline void SetStepModeOn(bool StepMode = true)
  {
    this->StepMode = StepMode;
    StepCount = 0;
  }

  inline void EnableRunningStepCount(ULong StepCount)
  {
    this->StepCount = StepCount;
  }

  virtual ULong GetCPURegisterCount(ULong InfoType)
  {
    switch(InfoType)
    {
      case 0: return(16);
      case 1: return(5);
      case 2: return(0);
      default: return(0);
    }
  }

  virtual bool  GetCPURegister_Alpha(ULong InfoType, ULong RegisterNum, ZString & RegisterName, ZString & RegisterValue)
  {
    RegisterName.Clear();
    RegisterValue.Clear();
    switch(InfoType)
    {
      case 0: if (RegisterNum>15) return(false);
              RegisterName<<"R"<<RegisterNum;
              RegisterValue = GeneralRegister[RegisterNum].Reg_ULong;
              return(true);

      case 1: if (RegisterNum>5) return(false);
              switch(RegisterNum)
              {
                case 0: RegisterName << "Z";
                        RegisterValue << (ULong)StatusRegister.Flags.ZeroFlag;
                        return(true);
                case 1: RegisterName << "N";
                        RegisterValue << (ULong)StatusRegister.Flags.NegativeFlag;
                        return(true);
                case 2: RegisterName << "V";
                        RegisterValue << (ULong)StatusRegister.Flags.OverflowFlag;
                        return(true);
                case 3: RegisterName << "C";
                        RegisterValue << (ULong)StatusRegister.Flags.CarryFlag;
                        return(true);
                case 4: RegisterName << "I";
                        RegisterValue << (ULong)StatusRegister.Flags.InterruptLevel;
                        return(true);
                default:return(false);
              }
              return(false);

      default:return(false);
    }

  }


  inline void Status_Test_ZN_8(UByte Data, ZStatusRegister &Status)
  {
    Status.Flags.ZeroFlag = (!Data);
    Status.Flags.NegativeFlag = (((Byte)Data) < 0);
  }

  inline void Status_Test_ZN_16(UShort Data, ZStatusRegister &Status)
  {
    Status.Flags.ZeroFlag = (!Data);
    Status.Flags.NegativeFlag = (((Short)Data) < 0);
  }

  inline void Status_Test_ZN_32(ULong Data, ZStatusRegister &Status)
  {
    Status.Flags.ZeroFlag = (!Data);
    Status.Flags.NegativeFlag = (((Long)Data) < 0);
  }

  inline void Status_Test_ZNVC(UByte Data, ZStatusRegister &Status)
  {
    Status.Flags.ZeroFlag = (!Data);
    Status.Flags.NegativeFlag = (((Byte)Data) < 0);
  }

  inline UByte FetchOpcode(ULong ProgramCounter)
  {
    return( VMachine->FetchOpcode(ProgramCounter) );
  }
  
  inline UByte FetchOperand_8(ULong ProgramCounter)
  {
    return( VMachine->FetchOperand_8(ProgramCounter) );
  }

  inline UShort FetchOperand_16(ULong ProgramCounter)
  {
    return( VMachine->FetchOperand_16(ProgramCounter));
  }

  inline ULong FetchOperand_32(ULong ProgramCounter)
  {
    return( VMachine->FetchOperand_32(ProgramCounter));
  }

  inline UByte ReadMemory_8(ULong Address)
  {
    return( VMachine->ReadMemory_8(Address));
  }

  inline UShort ReadMemory_16(ULong Address)
  {
    return( VMachine->ReadMemory_16(Address));
  }

  inline ULong ReadMemory_32(ULong Address)
  {
    return( VMachine->ReadMemory_32(Address));
  }

  inline void WriteMemory_8(ULong Address, UByte Data)
  {
    VMachine->WriteMemory_8(Address, Data);
  }

  inline void WriteMemory_16(ULong Address, UShort Data)
  {
    VMachine->WriteMemory_16(Address, Data);
  }

  inline void WriteMemory_32(ULong Address, ULong Data)
  {
    VMachine->WriteMemory_32(Address, Data);
  }

  inline void PushStack_32(ULong Data)
  {
    GeneralRegister[15].Reg_ULargest-=4;
    VMachine->PushStack_32(GeneralRegister[15].Reg_ULargest, Data);
  }

  inline ULong PopStack_32()
  {
    ULong RetVal;
    RetVal = VMachine->PopStack_32(GeneralRegister[15].Reg_ULargest);
    GeneralRegister[15].Reg_ULargest+=4;
    return(RetVal);
  }

  void PrintStatus(ZStatusRegister StatusFlag)
  {
	StatusRegister_BCPU * s = (StatusRegister_BCPU *)&StatusFlag;
    printf("Zero Flag  : %s\n", s->ZeroFlag     ? "1" : "0");
    printf("Sign Flag  : %s\n", s->NegativeFlag ? "1" : "0");
    printf("Over Flag  : %s\n", s->OverflowFlag ? "1" : "0");
    printf("Carry Flag : %s\n", s->CarryFlag    ? "1" : "0");
  }

  /*
  asm volatile
  (
    "testb %b2,%b2;"
    "pushf;"
    "popl %1;"
    "andl 192,w1;" // FLAG_ZERO | FLAG_NEGATIVE
    "movb %b2,%b0;"
    : "=m" (GeneralRegister[Op1]), "=r" (Status)
    : "r" (Op2)
    :
  );
  */

  enum {

        // Byte forms (or unique opcode).

        OPCODE_NOOP = 0,
        OPCODE_BREAK,
        OPCODE_MOVE_IMM_B,
        OPCODE_MOVE_IND_REG_B,
        OPCODE_MOVE_REG_IND_B,
        OPCODE_MOVE_REG_REG_B,
        OPCODE_MOVEX_IMM_SB,
        OPCODE_MOVEX_IMM_UB,
        OPCODE_PUSHREGS,
        OPCODE_POPREGS,
        OPCODE_INC,
        OPCODE_ADD_B,
        OPCODE_SUB_B,
        OPCODE_AND_B,
        OPCODE_OR_B,
        OPCODE_XOR_B,
        OPCODE_ASL_B,
        OPCODE_ASR_B,
        OPCODE_LSL_B,
        OPCODE_LSR_B,
        OPCODE_ROL_B,
        OPCODE_ROR_B,
        OPCODE_UMUL_B,
        OPCODE_SMUL_B,
        OPCODE_CMP_B,
        OPCODE_JMP_IND,
        OPCODE_JSR_IND,
        OPCODE_BRANCH_BRA,
        OPCODE_BRANCH_BCC,
        OPCODE_BRANCH_BVC,
        OPCODE_BRANCH_BLO_S,
        OPCODE_RTS,
        OPCODE_RTI,
        OPCODE_EXT_BW,



        // Word forms (or sometime opcode variations).

        OPCODE_MOVE_IMM_W     = OPCODE_MOVE_IMM_B         + 64,
        OPCODE_MOVE_IND_REG_W = OPCODE_MOVE_IND_REG_B     + 64,
        OPCODE_MOVE_REG_IND_W = OPCODE_MOVE_REG_IND_B     + 64,
        OPCODE_MOVE_REG_REG_W = OPCODE_MOVE_REG_REG_B     + 64,
        OPCODE_MOVEX_IMM_SW   = OPCODE_MOVEX_IMM_SB       + 64,
        OPCODE_MOVEX_IMM_UW   = OPCODE_MOVEX_IMM_UB       + 64,
        // OPCODE_PUSHREGS
        // OPCODE_POPREGS
        OPCODE_DEC            = OPCODE_INC                + 64,
        OPCODE_ADD_W          = OPCODE_ADD_B              + 64,
        OPCODE_SUB_W          = OPCODE_SUB_B              + 64,
        OPCODE_AND_W          = OPCODE_AND_B              + 64,
        OPCODE_OR_W           = OPCODE_OR_B               + 64,
        OPCODE_XOR_W          = OPCODE_XOR_B              + 64,
        OPCODE_ASL_W          = OPCODE_ASL_B              + 64,
        OPCODE_ASR_W          = OPCODE_ASR_B              + 64,
        OPCODE_LSL_W          = OPCODE_LSL_B              + 64,
        OPCODE_LSR_W          = OPCODE_LSR_B              + 64,
        OPCODE_ROL_W          = OPCODE_ROL_B              + 64,
        OPCODE_ROR_W          = OPCODE_ROR_B              + 64,
        OPCODE_UMUL_W         = OPCODE_UMUL_B             + 64,
        OPCODE_SMUL_W         = OPCODE_SMUL_B             + 64,
        OPCODE_CMP_W          = OPCODE_CMP_B              + 64,
        //OPCODE_JMP_IND
        OPCODE_BRANCH_BEQ     = OPCODE_BRANCH_BRA         + 64,
        OPCODE_BRANCH_BMI     = OPCODE_BRANCH_BCC         + 64,
        OPCODE_BRANCH_BHI_U   = OPCODE_BRANCH_BVC         + 64,
        OPCODE_BRANCH_BHI_S   = OPCODE_BRANCH_BLO_S       + 64,
        //OPCODE_JSR_IND
        //OPCODE_RTS
        //OPCODE_RTI
        OPCODE_EXT_BL         = OPCODE_EXT_BW             + 64,

        // Long forms (or sometime opcode variations).

        OPCODE_MOVE_IMM_L     = OPCODE_MOVE_IMM_B         + 128,
        OPCODE_MOVE_IND_REG_L = OPCODE_MOVE_IND_REG_B     + 128,
        OPCODE_MOVE_REG_IND_L = OPCODE_MOVE_REG_IND_B     + 128,
        OPCODE_MOVE_REG_REG_L = OPCODE_MOVE_REG_REG_B     + 128,
        OPCODE_MOVEX_IMM_SN   = OPCODE_MOVEX_IMM_SB       + 128,
        OPCODE_MOVEX_IMM_UN   = OPCODE_MOVEX_IMM_UB       + 128,
        // OPCODE_PUSHREGS
        // OPCODE_POPREGS
        OPCODE_ADD_L          = OPCODE_ADD_B              + 128,
        OPCODE_SUB_L          = OPCODE_SUB_B              + 128,
        OPCODE_AND_L          = OPCODE_AND_B              + 128,
        OPCODE_OR_L           = OPCODE_OR_B               + 128,
        OPCODE_XOR_L          = OPCODE_XOR_B              + 128,
        OPCODE_ASL_L          = OPCODE_ASL_B              + 128,
        OPCODE_ASR_L          = OPCODE_ASR_B              + 128,
        OPCODE_LSL_L          = OPCODE_LSL_B              + 128,
        OPCODE_LSR_L          = OPCODE_LSR_B              + 128,
        OPCODE_ROL_L          = OPCODE_ROL_B              + 128,
        OPCODE_ROR_L          = OPCODE_ROR_B              + 128,
        OPCODE_UMUL_L         = OPCODE_UMUL_B             + 128,
        OPCODE_SMUL_L         = OPCODE_SMUL_B             + 128,
        OPCODE_CMP_L          = OPCODE_CMP_B              + 128,
        //OPCODE_JMP_IND
        OPCODE_BRANCH_BNE     = OPCODE_BRANCH_BRA         + 128,
        OPCODE_BRANCH_BPL     = OPCODE_BRANCH_BCC         + 128,
        OPCODE_BRANCH_BLE_U   = OPCODE_BRANCH_BVC         + 128,
        OPCODE_BRANCH_BLE_S   = OPCODE_BRANCH_BLO_S       + 128,
        //OPCODE_JSR_IND
        //OPCODE_RTS
        //OPCODE_RTI
        OPCODE_EXT_WL         = OPCODE_EXT_BW             + 128,

        // 2xLong form (or sometime opcode variation)
        OPCODE_BRANCH_BCS     = OPCODE_BRANCH_BRA         + 192,
        OPCODE_BRANCH_BVS     = OPCODE_BRANCH_BCC         + 192,
        OPCODE_BRANCH_BHE_S   = OPCODE_BRANCH_BVC         + 192,
        OPCODE_BRANCH_BXX     = OPCODE_BRANCH_BLO_S       + 192,

        // End opcode
        OPCODE_END = 255
  };

  void ShowStatus()
  {
    ULong i;

    printf("[%8x] ", ProgramCounter);
    for (i=0;i<15;i++) printf("%8x ", GeneralRegister[i].Reg_ULong);
    printf("\n");

  }

  void Execute(ULong CycleCount)
  {
    ULong CycleLimit;
    register UByte Opcode;
    register int Op1,Op2;
    register ZStatusRegister Status;
    int i;
    ULong ChipManagementCycleGranularity = 50, ChipManagementCount;
    
    // Set the running flag.

    IsRunning = true;

    // Load status register.

    Status = StatusRegister;

    // Compute cycle count to run

    if (StepMode)
    {
      CycleLimit = ElapsedCycles + StepCount;
      ChipManagementCycleGranularity = 1;
      StepCount = 0;
    }
    else
    {
      CycleLimit = ElapsedCycles + CycleCount;
    }

    while( (ElapsedCycles < CycleLimit) && (!BreakRun) )
    {
      // Environment call.
      VMachine->Envcall();

      ChipManagementCount = ElapsedCycles + ChipManagementCycleGranularity;
      while(ElapsedCycles < ChipManagementCount)
      {
        // Interrupt Management

        if (Interrupt_Pending)
        {
          register int NumInt;
          NumInt = 32 - __builtin_clz(Interrupt_Pending);
          if (NumInt > StatusRegister.Flags.InterruptLevel)
          {
            PushStack_32(ProgramCounter);
            PushStack_32(StatusRegister.WholeRegister);
            ProgramCounter = ReadMemory_32((NumInt<<2)+1024);
          }
        }

        // Fetch Instruction

        //Disassemble(ProgramCounter);
        Opcode = FetchOpcode(ProgramCounter++);
        //PrintStatus(Status);
        //printf("-- Opcode : %d --\n",Opcode);

        switch(Opcode)
        {
          case OPCODE_BREAK:            this->StepCount = 0;
                                        this->StepMode = true;
                                        ChipManagementCount = CycleLimit = ElapsedCycles; // Break actual quantum
                                        break;

  // ---------------- MOVE

          case OPCODE_MOVE_IMM_B:       // move.b #imm,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Op2 = FetchOperand_8(ProgramCounter); ProgramCounter+=2; // Immediate value
                                        GeneralRegister[Op1 & 0x0f].Reg_UByte = Op2;
                                        Status_Test_ZN_8(Op2,Status);
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_MOVE_IMM_W:       // move.w #imm,reg
                                        Op1 = FetchOperand_8(ProgramCounter++) & 0x0f; // Register
                                        Op2 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Immediate value
                                        GeneralRegister[Op1 & 0x0f].Reg_UWord = Op2;
                                        Status_Test_ZN_16(Op2,Status);
                                        ElapsedCycles += 10;
                                        break;

          case OPCODE_MOVE_IMM_L:       // move.l #imm,reg
                                        Op1 = FetchOperand_8(ProgramCounter++) & 0x0f; // Register
                                        Op2 = FetchOperand_32(ProgramCounter); ProgramCounter+=4;  // Immediate value
                                        GeneralRegister[Op1 & 0x0f].Reg_ULong = Op2;
                                        Status_Test_ZN_32(Op2,Status);
                                        ElapsedCycles += 14;
                                        break;

          case OPCODE_MOVE_IND_REG_B:   // move.b (reg),reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status_Test_ZN_8( GeneralRegister[Op1 & 0x0f].Reg_UByte = ReadMemory_8(GeneralRegister[(Op1&0xf0)>>4].Reg_ULargest)
                                                          ,Status );
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_MOVE_IND_REG_W:   // move.w (reg),reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status_Test_ZN_16( GeneralRegister[Op1 & 0x0f].Reg_UWord = ReadMemory_16(GeneralRegister[(Op1&0xf0)>>4].Reg_ULargest)
                                                           ,Status );
                                        ElapsedCycles += 10;
                                        break;

          case OPCODE_MOVE_IND_REG_L:   // move.l (reg),reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status_Test_ZN_32( GeneralRegister[Op1 & 0x0f].Reg_ULong = ReadMemory_32(GeneralRegister[(Op1&0xf0)>>4].Reg_ULargest)
                                                           ,Status );
                                        ElapsedCycles += 14;
                                        break;

          case OPCODE_MOVE_REG_IND_B:   // move.b reg,(reg)
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        WriteMemory_8(GeneralRegister[Op1 & 0x0f].Reg_ULargest, GeneralRegister[(Op1&0xf0)>>4].Reg_UByte);
                                        Status_Test_ZN_8( GeneralRegister[(Op1&0xf0)>>4].Reg_UByte ,Status );
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_MOVE_REG_IND_W:   // move.w reg,(reg)
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        WriteMemory_16(GeneralRegister[Op1 & 0x0f].Reg_ULargest, GeneralRegister[(Op1&0xf0)>>4].Reg_UWord);
                                        Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)>>4].Reg_UWord ,Status );
                                        ElapsedCycles += 10;
                                        break;

          case OPCODE_MOVE_REG_IND_L:   // move.b reg,(reg)
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        WriteMemory_32(GeneralRegister[Op1 & 0x0f].Reg_ULargest, GeneralRegister[(Op1&0xf0)>>4].Reg_ULong);
                                        Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)>>4].Reg_ULargest ,Status );
                                        ElapsedCycles += 14;
                                        break;

          case OPCODE_MOVE_REG_REG_B:   // move.b reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status_Test_ZN_8( GeneralRegister[Op1&0x0f].Reg_UByte = GeneralRegister[(Op1&0xf0)>>4].Reg_UByte
                                                          ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_MOVE_REG_REG_W:   // move.w reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status_Test_ZN_16( GeneralRegister[Op1&0x0f].Reg_UWord = GeneralRegister[(Op1&0xf0)>>4].Reg_UWord
                                                           ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_MOVE_REG_REG_L:   // move.l reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status_Test_ZN_32( GeneralRegister[Op1&0x0f].Reg_ULong = GeneralRegister[(Op1&0xf0)>>4].Reg_ULong
                                                           ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_MOVEX_IMM_SN:     // movex.un #imm,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        GeneralRegister[Op1 & 0x0f].Reg_ULargest = (Op1 & 0x80) ? 0xfffffff0 : 0x00000000 ;
                                        GeneralRegister[Op1 & 0x0f].Reg_ULargest |= (Op1 >> 4);
                                        Status_Test_ZN_8(GeneralRegister[Op1 & 0x0f].Reg_UByte,Status);
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_MOVEX_IMM_SB:     // movex.sb #imm,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Op2 = FetchOperand_8(ProgramCounter++);  // Immediate value
                                        GeneralRegister[Op1 & 0x0f].Reg_SLargest = (Byte)Op2;
                                        Status_Test_ZN_8(Op2,Status);
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_MOVEX_IMM_SW:     // movex.sw #imm,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Op2 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Immediate value
                                        GeneralRegister[Op1 & 0x0f].Reg_SLargest = (Short)Op2;
                                        Status_Test_ZN_16(Op2,Status);
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_MOVEX_IMM_UN:     // movex.un #imm,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        GeneralRegister[Op1 & 0x0f].Reg_ULargest = (Op1 >> 4);
                                        Status_Test_ZN_8(Op1 >> 4,Status);
                                        ElapsedCycles += 6;
                                        break;


          case OPCODE_MOVEX_IMM_UB:     // movex.ub #imm,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Op2 = FetchOperand_8(ProgramCounter++);  // Immediate value
                                        GeneralRegister[Op1 & 0x0f].Reg_ULargest = (UByte)Op2;
                                        Status_Test_ZN_8(Op2,Status);
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_MOVEX_IMM_UW:     // movex.uw #imm,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Op2 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Immediate value
                                        GeneralRegister[Op1 & 0x0f].Reg_ULargest = (UShort)Op2;
                                        Status_Test_ZN_16(Op2,Status);
                                        ElapsedCycles += 10;
                                        break;





          case OPCODE_PUSHREGS:         // pushregs R0-R14(reg)
                                        {
                                          Op1 = FetchOperand_16(ProgramCounter++); // Register
                                          ElapsedCycles += 8;
                                          for(i=0;i<15;i++)
                                          {
                                            if (Op1&1) {PushStack_32(GeneralRegister[i].Reg_ULargest); ElapsedCycles += 4;}
                                            Op1>>=1;
                                          }
                                        }
                                        break;

          case OPCODE_POPREGS:          // popregs R0-R15
                                        Op1 = FetchOperand_16(ProgramCounter++); // Register
                                        ElapsedCycles += 8;
                                        for(i=14;i<15;i--)
                                        {
                                          if (Op1&0x04000) {GeneralRegister[i].Reg_ULargest = PopStack_32(); ElapsedCycles += 4; }
                                          Op1<<=1;
                                        }
                                        break;

          case OPCODE_INC:              Op1 = FetchOperand_8(ProgramCounter++); // Register + Value
                                        {
                                          register Long Data, OldData;
                                          OldData = GeneralRegister[Op1&0xf].Reg_ULargest;
                                          Data = GeneralRegister[Op1&0xf].Reg_ULargest+= Op1 >> 4;
                                          Status.Flags.ZeroFlag     = !Data;
                                          Status.Flags.OverflowFlag =  ((Long)OldData)<0 && (((Long)(Data ^ OldData)) < 0) ;
                                          Status.Flags.NegativeFlag =  ((Long)Data) < 0;
                                          ElapsedCycles += 6;
                                        }

                                        break;
          case OPCODE_DEC:              Op1 = FetchOperand_8(ProgramCounter++); // Register + Value
                                        {
                                          register Long Data, OldData;
                                          OldData = GeneralRegister[Op1&0xf].Reg_ULargest;
                                          Data = GeneralRegister[Op1&0xf].Reg_ULargest-= Op1 >> 4;
                                          Status.Flags.ZeroFlag     = !Data;
                                          Status.Flags.OverflowFlag =  ((Long)OldData)>=0 && ((Long)(Data ^ OldData)) < 0;
                                          Status.Flags.NegativeFlag =  ((Long)Data) < 0;
                                          ElapsedCycles += 6;
                                        }
                                        break;

          case OPCODE_ADD_B:            // add.b reg,reg (No carry)
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          register UByte D1,D2,D3;
                                          D1 = GeneralRegister[(Op1&0xf0)>>4].Reg_UByte;
                                          D2 = GeneralRegister[Op1&0xf].Reg_UByte;
                                          D3 = D1+D2;
                                          GeneralRegister[Op1&0xf].Reg_UByte = D3;
                                          Status.Flags.CarryFlag = (D1&D2&0x80) != 0;
                                          Status.Flags.OverflowFlag = ((Byte)((D1^D3) & (~(D1^D2))) < 0);
                                          Status.Flags.ZeroFlag = (!D3);
                                          Status.Flags.NegativeFlag = ((Byte)D3 < 0);
                                          ElapsedCycles += 6;
                                        }
                                        break;

          case OPCODE_ADD_W:            // add.w reg,reg (No carry)
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          register UShort D1,D2,D3;
                                          D1 = GeneralRegister[(Op1&0xf0)>>4].Reg_UWord;
                                          D2 = GeneralRegister[Op1&0xf].Reg_UWord;
                                          D3 = D1+D2;
                                          GeneralRegister[Op1&0xf].Reg_UWord = D3;
                                          Status.Flags.CarryFlag = (D1&D2&0x80) != 0;
                                          Status.Flags.OverflowFlag = ((Short)((D1^D3) & (~(D1^D2))) < 0);
                                          Status.Flags.ZeroFlag = (!D3);
                                          Status.Flags.NegativeFlag = ((Short)D3 < 0);
                                          ElapsedCycles += 6;
                                        }
                                        break;

          case OPCODE_ADD_L:            // add.l reg,reg (No carry)
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          register ULong D1,D2,D3;
                                          D1 = GeneralRegister[(Op1&0xf0)>>4].Reg_ULong;
                                          D2 = GeneralRegister[Op1&0xf].Reg_ULong;
                                          D3 = D1+D2;
                                          GeneralRegister[Op1&0xf].Reg_ULong = D3;
                                          Status.Flags.CarryFlag = (D1&D2&0x80) != 0;
                                          Status.Flags.OverflowFlag = ((Long)((D1^D3) & (~(D1^D2))) < 0);
                                          Status.Flags.ZeroFlag = (!D3);
                                          Status.Flags.NegativeFlag = ((Long)D3 < 0);
                                          ElapsedCycles += 6;
                                        }
                                        break;

          case OPCODE_SUB_B:            // sub.b reg,reg (No carry)
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          register UByte D1,D2,D3;
                                          D1 = GeneralRegister[(Op1&0xf0)>>4].Reg_UByte;
                                          D2 = GeneralRegister[Op1&0xf].Reg_UByte;
                                          D3 = D1-D2;
                                          Status.Flags.CarryFlag = (D1&D2&0x80) != 0;
                                          Status.Flags.OverflowFlag = ((Byte)((D1^D3) & (~(D1^D2))) < 0);
                                          Status.Flags.ZeroFlag = (!D3);
                                          Status.Flags.NegativeFlag = ((Byte)D3 < 0);
                                          ElapsedCycles += 6;
                                        }
                                        break;

          case OPCODE_SUB_W:            // sub.w reg,reg (No carry)
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          register UByte D1,D2,D3;
                                          D1 = GeneralRegister[(Op1&0xf0)>>4].Reg_UWord;
                                          D2 = GeneralRegister[Op1&0xf].Reg_UWord;
                                          D3 = D1-D2;
                                          Status.Flags.CarryFlag = (D1&D2&0x80) != 0;
                                          Status.Flags.OverflowFlag = ((Byte)((D1^D3) & (~(D1^D2))) < 0);
                                          Status.Flags.ZeroFlag = (!D3);
                                          Status.Flags.NegativeFlag = ((Byte)D3 < 0);
                                          ElapsedCycles += 6;
                                        }
                                        break;

          case OPCODE_SUB_L:            // sub.l reg,reg (No carry)
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          register UByte D1,D2,D3;
                                          D1 = GeneralRegister[(Op1&0xf0)>>4].Reg_ULong;
                                          D2 = GeneralRegister[Op1&0xf].Reg_ULong;
                                          D3 = D1-D2;
                                          Status.Flags.CarryFlag = (D1&D2&0x80) != 0;
                                          Status.Flags.OverflowFlag = ((Byte)((D1^D3) & (~(D1^D2))) < 0);
                                          Status.Flags.ZeroFlag = (!D3);
                                          Status.Flags.NegativeFlag = ((Byte)D3 < 0);
                                          ElapsedCycles += 6;
                                        }
                                        break;

          case OPCODE_AND_B:            // and.b reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status_Test_ZN_8( GeneralRegister[Op1 & 0x0f].Reg_UByte &= GeneralRegister[(Op1&0xf0)>>4].Reg_UByte
                                                          ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_AND_W:            // and.w reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status_Test_ZN_16( GeneralRegister[Op1 & 0x0f].Reg_UWord &= GeneralRegister[(Op1&0xf0)>>4].Reg_UWord
                                                          ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_AND_L:            // and.l reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status_Test_ZN_32( GeneralRegister[Op1 & 0x0f].Reg_ULong &= GeneralRegister[(Op1&0xf0)>>4].Reg_ULong
                                                           ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_OR_B:             // or.b  reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status_Test_ZN_8( GeneralRegister[Op1 & 0x0f].Reg_UByte |= GeneralRegister[(Op1&0xf0)>>4].Reg_UByte
                                                          ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_OR_W:             // or.w  reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status_Test_ZN_16( GeneralRegister[Op1 & 0x0f].Reg_UWord |= GeneralRegister[(Op1&0xf0)>>4].Reg_UWord
                                                           ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_OR_L:             // or.l  reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status_Test_ZN_32( GeneralRegister[Op1 & 0x0f].Reg_ULong |= GeneralRegister[(Op1&0xf0)>>4].Reg_ULong
                                                           ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_XOR_B:            // xor.b reg.reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status_Test_ZN_8( GeneralRegister[Op1 & 0x0f].Reg_UByte ^= GeneralRegister[(Op1&0xf0)>>4].Reg_UByte
                                                          ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_XOR_W:            // xor.w reg.reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status_Test_ZN_16( GeneralRegister[Op1 & 0x0f].Reg_UWord ^= GeneralRegister[(Op1&0xf0)>>4].Reg_UWord
                                                           ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_XOR_L:            // xor.l reg.reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status_Test_ZN_16( GeneralRegister[Op1 & 0x0f].Reg_ULong ^= GeneralRegister[(Op1&0xf0)>>4].Reg_ULong
                                                           ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_ASL_B:            // asl.b reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SByte < 0;
                                        Status_Test_ZN_8( GeneralRegister[(Op1&0xf0)].Reg_SByte = (GeneralRegister[(Op1&0xf0)].Reg_SByte << GeneralRegister[Op1&0xf].Reg_UByte) || (GeneralRegister[(Op1&0xf0)].Reg_SByte & 0x80)
                                                          ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_ASL_W:            // asl.w reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SWord < 0;
                                        Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)].Reg_SWord = (GeneralRegister[(Op1&0xf0)].Reg_SWord << GeneralRegister[Op1&0xf].Reg_UByte) || (GeneralRegister[(Op1&0xf0)].Reg_SWord & 0x8000)
                                                           ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_ASL_L:            // asl.l reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SLong < 0;
                                        Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)].Reg_SLong = (GeneralRegister[(Op1&0xf0)].Reg_SLong << GeneralRegister[Op1&0xf].Reg_UByte) || (GeneralRegister[(Op1&0xf0)].Reg_SLong & 0x80000000UL)
                                                           ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_ASR_B:            // asr.b reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        ElapsedCycles += 6;
                                        // Todo
                                        break;

          case OPCODE_ASR_W:            // asr.w reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SWord < 0;
                                        Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)].Reg_SWord = (GeneralRegister[(Op1&0xf0)].Reg_SWord >> GeneralRegister[Op1&0xf].Reg_UByte) || (GeneralRegister[(Op1&0xf0)].Reg_SWord & 0x8000)
                                                           ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_ASR_L:            // asr.l reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SLong < 0;
                                        Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)].Reg_SLong = (GeneralRegister[(Op1&0xf0)].Reg_SLong >> GeneralRegister[Op1&0xf].Reg_UByte) || (GeneralRegister[(Op1&0xf0)].Reg_SLong & 0x80000000UL)
                                                           ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_LSL_B:            // lsl.b reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SByte < 0;
                                        Status_Test_ZN_8( GeneralRegister[(Op1&0xf0)].Reg_SByte <<= GeneralRegister[Op1&0xf].Reg_UByte
                                                          ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_LSL_W:            // lsl.w reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SWord < 0;
                                        Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)].Reg_SWord <<= GeneralRegister[Op1&0xf].Reg_UByte
                                                           ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_LSL_L:            // lsl.l reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SLong < 0;
                                        Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)].Reg_SLong <<= GeneralRegister[Op1&0xf].Reg_UByte
                                                           ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_LSR_B:            // lsr.b reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SByte < 0;
                                        Status_Test_ZN_8( GeneralRegister[(Op1&0xf0)].Reg_SByte >>= GeneralRegister[Op1&0xf].Reg_UByte
                                                          ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_LSR_W:            // lsr.w reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SWord < 0;
                                        Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)].Reg_SWord >>= GeneralRegister[Op1&0xf].Reg_UByte
                                                           ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_LSR_L:            // lsr.l reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SLong < 0;
                                        Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)].Reg_SLong >>= GeneralRegister[Op1&0xf].Reg_UByte
                                                           ,Status );
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_ROL_B:            // rol.b reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          register bool Carry;
                                          Carry = Status.Flags.CarryFlag;
                                          Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SByte < 0;
                                          Status_Test_ZN_8( GeneralRegister[(Op1&0xf0)].Reg_UByte = GeneralRegister[(Op1&0xf0)].Reg_UByte >> GeneralRegister[Op1&0xf].Reg_UByte | (Carry ? 1:0)
                                                            ,Status );
                                          ElapsedCycles += 6;
                                        }
                                        break;

          case OPCODE_ROL_W:            // rol.w reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          register bool Carry;
                                          Carry = Status.Flags.CarryFlag;
                                          Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SWord < 0;
                                          Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)].Reg_UWord = GeneralRegister[(Op1&0xf0)].Reg_UByte >> GeneralRegister[Op1&0xf].Reg_UByte | (Carry ? 1:0)
                                                             ,Status );
                                          ElapsedCycles += 6;
                                        }
                                        break;

          case OPCODE_ROL_L:            // rol.l reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          register bool Carry;
                                          Carry = Status.Flags.CarryFlag;
                                          Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SLong < 0;
                                          Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)].Reg_ULong = GeneralRegister[(Op1&0xf0)].Reg_ULong >> GeneralRegister[Op1&0xf].Reg_UByte | (Carry ? 1:0)
                                                             ,Status );
                                          ElapsedCycles += 6;
                                        }
                                        break;

          case OPCODE_ROR_B:            // ror.b reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          register bool Carry;
                                          Carry = Status.Flags.CarryFlag;
                                          Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SByte | 1;
                                          Status_Test_ZN_8( GeneralRegister[(Op1&0xf0)].Reg_UByte = GeneralRegister[(Op1&0xf0)].Reg_UByte >> GeneralRegister[Op1&0xf].Reg_UByte | (Carry ? 1:0)
                                                            ,Status );
                                          ElapsedCycles += 6;
                                        }
                                        break;

          case OPCODE_ROR_W:            // ror.b reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          register bool Carry;
                                          Carry = Status.Flags.CarryFlag;
                                          Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SWord | 1;
                                          Status_Test_ZN_8( GeneralRegister[(Op1&0xf0)].Reg_UWord = GeneralRegister[(Op1&0xf0)].Reg_UWord >> GeneralRegister[Op1&0xf].Reg_UByte | (Carry ? 1:0)
                                                            ,Status );
                                          ElapsedCycles += 6;
                                        }
                                        break;

          case OPCODE_ROR_L:            // ror.b reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          register bool Carry;
                                          Carry = Status.Flags.CarryFlag;
                                          Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SLong | 1;
                                          Status_Test_ZN_8( GeneralRegister[(Op1&0xf0)].Reg_ULong = GeneralRegister[(Op1&0xf0)].Reg_ULong >> GeneralRegister[Op1&0xf].Reg_UByte | (Carry ? 1:0)
                                                            ,Status );
                                          ElapsedCycles += 6;
                                        }
                                        break;

          case OPCODE_UMUL_B:           // umul.b reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        GeneralRegister[(Op1&0xf0)].Reg_UWord = (GeneralRegister[(Op1&0xf0)].Reg_UByte) * (GeneralRegister[Op1&0xf].Reg_UByte);
                                        Status.Flags.CarryFlag = Status.Flags.NegativeFlag = GeneralRegister[(Op1&0xf0)].Reg_UWord > 255;
                                        Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)].Reg_UWord ,Status );
                                        ElapsedCycles += 150;
                                        break;

          case OPCODE_UMUL_W:           // umul.w reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        GeneralRegister[(Op1&0xf0)].Reg_ULong = (GeneralRegister[(Op1&0xf0)].Reg_UWord) * (GeneralRegister[Op1&0xf].Reg_UWord);
                                        Status.Flags.CarryFlag = Status.Flags.NegativeFlag = GeneralRegister[(Op1&0xf0)].Reg_UWord > 65535;
                                        Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)].Reg_UWord ,Status );
                                        ElapsedCycles += 150;
                                        break;

          case OPCODE_UMUL_L:           // umul.l reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          UELong Tmp;
                                          Tmp = GeneralRegister[(Op1&0xf0)].Reg_ULong * GeneralRegister[Op1&0xf].Reg_ULong;
                                          GeneralRegister[(Op1&0xf0)].Reg_ULong = (ULong)Tmp;
                                          GeneralRegister[((Op1+0x10)&0xf0)].Reg_ULong = (ULong)(Tmp>>16);
                                          Status.Flags.CarryFlag = Status.Flags.NegativeFlag = GeneralRegister[(Op1&0xf0)].Reg_ULong != 0;
                                          Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)].Reg_ULong ,Status );
                                          ElapsedCycles += 150;
                                        }
                                        break;

          case OPCODE_SMUL_B:           // smul.b reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        GeneralRegister[(Op1&0xf0)].Reg_SWord = (GeneralRegister[(Op1&0xf0)].Reg_SByte) * (GeneralRegister[Op1&0xf].Reg_SByte);
                                        Status.Flags.CarryFlag = Status.Flags.NegativeFlag = GeneralRegister[(Op1&0xf0)].Reg_SWord > 255;
                                        Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)].Reg_UWord ,Status );
                                        ElapsedCycles += 150;
                                        break;

          case OPCODE_SMUL_W:           // smul.w reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        GeneralRegister[(Op1&0xf0)].Reg_SLong = (GeneralRegister[(Op1&0xf0)].Reg_SWord) * (GeneralRegister[Op1&0xf].Reg_SWord);
                                        Status.Flags.CarryFlag = Status.Flags.NegativeFlag = GeneralRegister[(Op1&0xf0)].Reg_UWord > 65535;
                                        Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)].Reg_UWord ,Status );
                                        ElapsedCycles += 150;
                                        break;

          case OPCODE_SMUL_L:           // smul.l reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          ELong Tmp;
                                          Tmp = GeneralRegister[(Op1&0xf0)].Reg_SLong * GeneralRegister[Op1&0xf].Reg_SLong;
                                          GeneralRegister[(Op1&0xf0)].Reg_ULong = (ULong)Tmp;
                                          GeneralRegister[((Op1+0x10)&0xf0)].Reg_ULong = (ULong)(Tmp>>16);
                                          Status.Flags.CarryFlag = Status.Flags.NegativeFlag = GeneralRegister[(Op1&0xf0)].Reg_SLong != 0;
                                          Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)].Reg_ULong ,Status );
                                          ElapsedCycles += 150;
                                        }
                                        break;

          case OPCODE_CMP_B:            // cmp.b reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          register UByte D1,D2,D3;
                                          D1 = GeneralRegister[(Op1&0xf0)>>4].Reg_UByte;
                                          D2 = GeneralRegister[Op1&0xf].Reg_UByte;
                                          D3 = D1-D2;
                                          Status.Flags.CarryFlag = (D1&D2&0x80) != 0;
                                          Status.Flags.OverflowFlag = ((Byte)((D1^D3) & (~(D1^D2))) < 0);
                                          Status.Flags.ZeroFlag = (!D3);
                                          Status.Flags.NegativeFlag = ((Byte)D3 < 0);
                                          ElapsedCycles += 6;
                                        }
                                        break;

          case OPCODE_CMP_W:            // cmp.w reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          register UShort D1,D2,D3;
                                          D1 = GeneralRegister[(Op1&0xf0)>>4].Reg_UWord;
                                          D2 = GeneralRegister[Op1&0xf].Reg_UWord;
                                          D3 = D1-D2;
                                          Status.Flags.CarryFlag = (D1&D2&0x80) != 0;
                                          Status.Flags.OverflowFlag = ((Byte)((D1^D3) & (~(D1^D2))) < 0);
                                          Status.Flags.ZeroFlag = (!D3);
                                          Status.Flags.NegativeFlag = ((Byte)D3 < 0);
                                          ElapsedCycles += 6;
                                        }
                                        break;

          case OPCODE_CMP_L:            // cmp.l reg,reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        {
                                          register ULong D1,D2,D3;
                                          D1 = GeneralRegister[(Op1&0xf0)>>4].Reg_ULong;
                                          D2 = GeneralRegister[Op1&0xf].Reg_ULong;
                                          D3 = D1-D2;
                                          Status.Flags.CarryFlag = (D1&D2&0x80) != 0;
                                          Status.Flags.OverflowFlag = ((Byte)((D1^D3) & (~(D1^D2))) < 0);
                                          Status.Flags.ZeroFlag = (!D3);
                                          Status.Flags.NegativeFlag = ((Byte)D3 < 0);
                                          ElapsedCycles += 6;
                                        }
                                        break;

          case OPCODE_JMP_IND:          // jmp (reg)
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        ProgramCounter = GeneralRegister[Op1&0xf].Reg_ULargest;
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_BRANCH_BRA:       // bra #imm
                                        Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
                                        ProgramCounter += Op1;
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_BRANCH_BEQ :      // beq #imm
                                        Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
                                        if (Status.Flags.ZeroFlag) ProgramCounter += Op1;
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_BRANCH_BNE :      // bne #imm
                                        Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
                                        if (!Status.Flags.ZeroFlag) ProgramCounter += Op1;
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_BRANCH_BCS :      // bcs #imm bhe.u #imm
                                        Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
                                        if (Status.Flags.CarryFlag) ProgramCounter += Op1;
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_BRANCH_BCC :      // bcc #imm blo.u #imm
                                        Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
                                        if (!Status.Flags.CarryFlag) ProgramCounter += Op1;
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_BRANCH_BMI :      // bmi #imm
                                        Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
                                        if (Status.Flags.NegativeFlag) ProgramCounter += Op1;
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_BRANCH_BPL :      // bpl #imm
                                        Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
                                        if (!Status.Flags.NegativeFlag) ProgramCounter += Op1;
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_BRANCH_BVS :      // bvs #imm
                                        Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
                                        if (Status.Flags.OverflowFlag) ProgramCounter += Op1;
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_BRANCH_BVC :      // bvc #imm
                                        Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
                                        if (!Status.Flags.OverflowFlag) ProgramCounter += Op1;
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_BRANCH_BHI_U :    // bhi.u #imm
                                        Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
                                        if (Status.Flags.CarryFlag && (!Status.Flags.ZeroFlag)) ProgramCounter += Op1;
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_BRANCH_BLE_U :    // ble.u #imm
                                        Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
                                        if (Status.Flags.CarryFlag && (!Status.Flags.ZeroFlag)) ProgramCounter += Op1;
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_BRANCH_BHE_S :    // bhe.s #imm
                                        Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
                                        if (Status.Flags.NegativeFlag == Status.Flags.OverflowFlag) ProgramCounter += Op1;
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_BRANCH_BLO_S :    // blo.s #imm
                                        Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
                                        if (Status.Flags.NegativeFlag != Status.Flags.OverflowFlag) ProgramCounter += Op1;
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_BRANCH_BHI_S :    // bhi.s #imm
                                        Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
                                        if ((!Status.Flags.ZeroFlag) && (Status.Flags.NegativeFlag == Status.Flags.OverflowFlag)) ProgramCounter += Op1;
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_BRANCH_BLE_S :    // ble.s #imm
                                        Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
                                        if ((Status.Flags.ZeroFlag) && (Status.Flags.NegativeFlag != Status.Flags.OverflowFlag)) ProgramCounter += Op1;
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_BRANCH_BXX :      // bra #imm
                                        Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
                                        ProgramCounter += Op1;
                                        ElapsedCycles += 8;
                                        break;

          case OPCODE_JSR_IND:          // jsr (reg)
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        PushStack_32(ProgramCounter);
                                        ProgramCounter = GeneralRegister[Op1&0xf].Reg_ULargest;
                                        ElapsedCycles += 14;
                                        break;

          case OPCODE_RTS:              // rts
                                        ProgramCounter = PopStack_32();
                                        ElapsedCycles += 12;
                                        break;

          case OPCODE_RTI:              // rti
                                        Status.WholeRegister = PopStack_32();
                                        ProgramCounter = PopStack_32();
                                        ElapsedCycles += 16;
                                        break;

          //case 30:// int reg
          case OPCODE_EXT_BW:           // ext.bw reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        GeneralRegister[Op1&0xf].Reg_SWord = (Short)GeneralRegister[Op1&0xf].Reg_SByte;
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_EXT_BL:           // ext.bl reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        GeneralRegister[Op1&0xf].Reg_SLong = (Short)GeneralRegister[Op1&0xf].Reg_SByte;
                                        ElapsedCycles += 6;
                                        break;

          case OPCODE_EXT_WL:           // ext.wl reg
                                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                                        GeneralRegister[Op1&0xf].Reg_SLong = (Short)GeneralRegister[Op1&0xf].Reg_SWord;
                                        ElapsedCycles += 6;
                                        break;

          case 255:                     ElapsedCycles +=10;
                                        break;
        }

        // End of quick loop
      }
      StatusRegister = Status;
    }

    VMachine->FrameCall();

    FrameCycles++;
    IsRunning = false;
  }




  bool Disassemble(ULong & ProgramCounter, ZString & Opc, ZString & Ope )
  {
    UByte Opcode;
    ULong Op1, Op2;
    ZString RS,RD;

    Ope.Set_DisplayBase(16);

    Opcode = FetchOpcode(ProgramCounter++);

    switch(Opcode)
    {
      case OPCODE_NOOP:             Opc = "[00]";
                                    Ope = "";
                                    return(false);

      case OPCODE_BREAK:            Opc = "[brk]";
                                    Ope = "(BreakPoint)";
                                    break;

      case OPCODE_MOVE_IMM_B:       // move.b #imm,reg
                                    Opc = "move.b";
                                    Ope = "#";
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    Op2 = FetchOperand_8(ProgramCounter); ProgramCounter+=2; // Immediate value
                                    RD = Op1 & 0x0f;
                                    Ope << Op2 << ",r" << RD;
                                    break;

      case OPCODE_MOVE_IMM_W:       // move.w #imm,reg
                                    Opc = "move.w";
                                    Ope = "#";
                                    Op1 = FetchOperand_8(ProgramCounter++) & 0x0f; // Register
                                    Op2 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Immediate value
                                    RD = Op1 & 0x0f;
                                    Ope << Op2 << ",r" << RD;
                                    break;

      case OPCODE_MOVE_IMM_L:       // move.l #imm,reg
                                    Opc = "move.l";
                                    Ope = "#";
                                    Op1 = FetchOperand_8(ProgramCounter++) & 0x0f; // Register
                                    Op2 = FetchOperand_32(ProgramCounter); ProgramCounter+=4;  // Immediate value
                                    RD = Op1 & 0x0f;
                                    Ope << Op2 << ",r" << RD;
                                    break;

      case OPCODE_MOVE_IND_REG_B:   // move.b (reg),reg
                                    Opc = "move.b";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1 & 0x0f;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "(r" << RS << "),r" << RD;
                                    break;

      case OPCODE_MOVE_IND_REG_W:   // move.w (reg),reg
                                    Opc = "move.w";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1 & 0x0f;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "(r" << RS << "),r" << RD;
                                    break;

      case OPCODE_MOVE_IND_REG_L:   // move.l (reg),reg
                                    Opc = "move.l";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1 & 0x0f;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "(r" << RS << "),r" << RD;
                                    break;

      case OPCODE_MOVE_REG_IND_B:   // move.b reg,(reg)
                                    Opc = "move.b";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1 & 0x0f;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",(r" << RD << ")";
                                    break;

      case OPCODE_MOVE_REG_IND_W:   // move.w reg,(reg)
                                    Opc = "move.w";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1 & 0x0f;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",(r" << RD << ")";
                                    break;

      case OPCODE_MOVE_REG_IND_L:   // move.l reg,(reg)
                                    Opc = "move.l";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1 & 0x0f;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",(r" << RD << ")";
                                    break;

      case OPCODE_MOVE_REG_REG_B:   // move.b reg,reg
                                    Opc = "move.b";
                                    Ope = "#";
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1 & 0x0f;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_MOVE_REG_REG_W:   // move.w reg,reg
                                    Opc = "move.w";
                                    Ope = "#";
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1 & 0x0f;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_MOVE_REG_REG_L:   // move.l reg,reg
                                    Opc = "move.l";
                                    Ope = "#";
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1 & 0x0f;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_MOVEX_IMM_SN:     // movex.un #imm,reg
                                    Opc = "movex.sn";
                                    Ope = "#";
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1 & 0x0f;
                                    Ope << (ULong)(((Op1 & 0x80) ? 0xfffffff0 : 0x00000000) | (Op1 >> 4))  << RS << ",r" << RD;
                                    break;

      case OPCODE_MOVEX_IMM_SB:     // movex.sb #imm,reg
                                    Opc = "movex.sb";
                                    Ope = "#";
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    Op2 = FetchOperand_8(ProgramCounter++); // Immediate value
                                    RD = Op1 & 0x0f;
                                    Ope << (Long)Op2 << RS << ",r" << RD;
                                    break;

      case OPCODE_MOVEX_IMM_SW:     // movex.sw #imm,reg
                                    Opc = "movex.sw";
                                    Ope = "#";
                                    Op1 = FetchOperand_8(ProgramCounter++);  // Register
                                    Op2 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Immediate value
                                    RD = Op1 & 0x0f;
                                    Ope << (Long)Op2 << RS << ",r" << RD;
                                    break;

      case OPCODE_MOVEX_IMM_UN:     // movex.un #imm,reg
                                    Opc = "movex.n";
                                    Ope = "#";
                                    Op1 = FetchOperand_8(ProgramCounter++);  // Register
                                    RD = Op1 & 0x0f;
                                    Ope << (ULong)((Op1 & 0xf0)>>4) << ",r" << RD;
                                    break;

      case OPCODE_MOVEX_IMM_UB:     // movex.ub #imm,reg
                                    Opc = "movex.b";
                                    Ope = "#";
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    Op2 = FetchOperand_8(ProgramCounter++); // Immediate value
                                    RD = Op1 & 0x0f;
                                    Ope << (ULong)Op2 << RS << ",r" << RD;
                                    break;

      case OPCODE_MOVEX_IMM_UW:     // movex.uw #imm,reg
                                    Opc = "movex.w";
                                    Ope = "#";
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    Op2 = FetchOperand_16(ProgramCounter++); // Immediate value
                                    RD = Op1 & 0x0f;
                                    Ope << (ULong)Op2 << RS << ",r" << RD;
                                    break;

      case OPCODE_PUSHREGS:         // pushregs R0-R14(reg)
                                    {
                                      ULong i;
                                      bool yet;

                                      Opc = "pushregs";
                                      Ope.Clear();
                                      Op1 = FetchOperand_16(ProgramCounter++); // Register

                                      for(i=0, yet=false ; i<15 ; i++)
                                      {
                                        if (Op1&1)
                                        {
                                          if (yet) Ope <<"/";
                                          Ope<<"r"<<i;
                                        }
                                        Op1>>=1;
                                      }
                                    }
                                    break;

      case OPCODE_POPREGS:          // popregs R0-R15
                                    {
                                      ULong i;
                                      bool yet;

                                      Opc = "popregs";
                                      Ope.Clear();
                                      Op1 = FetchOperand_16(ProgramCounter++); // Register

                                      for(i=0, yet=false ; i<15 ; i++)
                                      {
                                        if (Op1&1)
                                        {
                                          if (yet) Ope <<"/";
                                          Ope<<"r"<<i;
                                        }
                                        Op1>>=1;
                                      }
                                    }
                                    break;

      case OPCODE_INC:              // inc reg
                                    Opc = "inc";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register + Value
                                    RD = Op1 & 0xf;
                                    Ope << "r" << RD;
                                    break;

      case OPCODE_DEC:              // dec reg
                                    Opc = "dec";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register + Value
                                    RD = Op1 & 0xf;
                                    Ope << "r" << RD;
                                    break;

      case OPCODE_ADD_B:            // add.b reg,reg (No carry)
                                    Opc = "add.b";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_ADD_W:            // add.w reg,reg (No carry)
                                    Opc = "add.w";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_ADD_L:            // add.l reg,reg (No carry)
                                    Opc = "add.l";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_SUB_B:            // sub.b reg,reg (No carry)
                                    Opc = "sub.b";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_SUB_W:            // sub.w reg,reg (No carry)
                                    Opc = "sub.w";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;


      case OPCODE_SUB_L:            // sub.l reg,reg (No carry)
                                    Opc = "sub.l";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_AND_B:            // and.b reg,reg
                                    Opc = "and.b";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_AND_W:            // and.w reg,reg
                                    Opc = "and.w";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_AND_L:            // and.l reg,reg
                                    Opc = "and.l";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_OR_B:             // or.b  reg,reg
                                    Opc = "or.b";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_OR_W:             // or.w  reg,reg
                                    Opc = "or.w";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_OR_L:             // or.l  reg,reg
                                    Opc = "or.l";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_XOR_B:            // xor.b reg.reg
                                    Opc = "xor.b";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_XOR_W:            // xor.w reg.reg
                                    Opc = "xor.w";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_XOR_L:            // xor.l reg.reg
                                    Opc = "xor.l";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_ASL_B:            // asl.b reg,reg
                                    Opc = "asl.b";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_ASL_W:            // asl.w reg,reg
                                    Opc = "asl.w";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_ASL_L:            // asl.l reg,reg
                                    Opc = "asl.l";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_ASR_B:            // asr.b reg,reg
                                    Opc = "asr.b";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << RS << "," << RD;
                                    break;

      case OPCODE_ASR_W:            // asr.w reg,reg
                                    Opc = "asr.w";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_ASR_L:            // asr.l reg,reg
                                    Opc = "asr.l";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_LSL_B:            // lsl.b reg,reg
                                    Opc = "lsl.b";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_LSL_W:            // lsl.w reg,reg
                                    Opc = "lsl.w";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_LSL_L:            // lsl.l reg,reg
                                    Opc = "lsl.l";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_LSR_B:            // lsr.b reg,reg
                                    Opc = "lsr.b";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_LSR_W:            // lsr.w reg,reg
                                    Opc = "lsr.w";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_LSR_L:            // lsr.l reg,reg
                                    Opc = "lsr.l";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_ROL_B:            // rol.b reg,reg
                                    Opc = "rol.b";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_ROL_W:            // rol.w reg,reg
                                    Opc = "rol.w";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_ROL_L:            // rol.l reg,reg
                                    Opc = "rol.l";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_ROR_B:            // ror.b reg,reg
                                    Opc = "ror.b";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_ROR_W:            // ror.b reg,reg
                                    Opc = "ror.w";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_ROR_L:            // ror.b reg,reg
                                    Opc = "ror.l";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_UMUL_B:           // umul.b reg,reg
                                    Opc = "umul.b";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_UMUL_W:           // umul.w reg,reg
                                    Opc = "umul.w";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_UMUL_L:           // umul.l reg,reg
                                    Opc = "umul.l";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_SMUL_B:           // smul.b reg,reg
                                    Opc = "smul.b";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_SMUL_W:           // smul.w reg,reg
                                    Opc = "smul.w";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_SMUL_L:           // smul.l reg,reg
                                    Opc = "smul.l";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_CMP_B:            // cmp.b reg,reg
                                    Opc = "cmp.b";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_CMP_W:            // cmp.w reg,reg
                                    Opc = "cmp.w";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_CMP_L:            // cmp.l reg,reg
                                    Opc = "cmp.l";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    RS = (Op1&0xf0)>>4;
                                    Ope << "r" << RS << ",r" << RD;
                                    break;

      case OPCODE_JMP_IND:          // jmp (reg)
                                    Opc = "jmp";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    Ope << "(" << RD << ")";
                                    break;

      case OPCODE_BRANCH_BRA:       // bra #imm
                                    Opc = "bra";
                                    Ope.Clear();
                                    Op1 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Displacement
                                    Ope << "0x" << ProgramCounter + (Short)Op1;
                                    break;

      case OPCODE_BRANCH_BEQ :      // beq #imm
                                    Opc = "beq";
                                    Ope.Clear();
                                    Op1 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Displacement
                                    Ope << "0x" << ProgramCounter + (Short)Op1;
                                    break;

      case OPCODE_BRANCH_BNE :      // bne #imm
                                    Opc = "bne";
                                    Ope.Clear();
                                    Op1 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Displacement
                                    Ope << "0x" << ProgramCounter + (Short)Op1;
                                    break;

      case OPCODE_BRANCH_BCS :      // bcs #imm bhe.u #imm
                                    Opc = "bcs";
                                    Ope.Clear();
                                    Op1 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Displacement
                                    Ope << "0x" << ProgramCounter + (Short)Op1;
                                    break;

      case OPCODE_BRANCH_BCC :      // bcc #imm blo.u #imm
                                    Opc = "bcc";
                                    Ope.Clear();
                                    Op1 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Displacement
                                    Ope << "0x" << ProgramCounter + (Short)Op1;
                                    break;

      case OPCODE_BRANCH_BMI :      // bmi #imm
                                    Opc = "bmi";
                                    Ope.Clear();
                                    Op1 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Displacement
                                    Ope << "0x" << ProgramCounter + (Short)Op1;
                                    break;

      case OPCODE_BRANCH_BPL :      // bpl #imm
                                    Opc = "bpl";
                                    Ope.Clear();
                                    Op1 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Displacement
                                    Ope << "0x" << ProgramCounter + (Short)Op1;
                                    break;

      case OPCODE_BRANCH_BVS :      // bvs #imm
                                    Opc = "bvs";
                                    Ope.Clear();
                                    Op1 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Displacement
                                    Ope << "0x" << ProgramCounter + (Short)Op1;
                                    break;

      case OPCODE_BRANCH_BVC :      // bvc #imm
                                    Opc = "bvc";
                                    Ope.Clear();
                                    Op1 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Displacement
                                    Ope << "0x" << ProgramCounter + (Short)Op1;
                                    break;

      case OPCODE_BRANCH_BHI_U :    // bhi.u #imm
                                    Opc = "bhi.u";
                                    Ope.Clear();
                                    Op1 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Displacement
                                    Ope << "0x" << ProgramCounter + (Short)Op1;
                                    break;

      case OPCODE_BRANCH_BLE_U :    // ble.u #imm
                                    Opc = "ble.u";
                                    Ope.Clear();
                                    Op1 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Displacement
                                    Ope << "0x" << ProgramCounter + (Short)Op1;
                                    break;

      case OPCODE_BRANCH_BHE_S :    // bhe.s #imm
                                    Opc = "bhe.s";
                                    Ope.Clear();
                                    Op1 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Displacement
                                    Ope << "0x" << ProgramCounter + (Short)Op1;
                                    break;

      case OPCODE_BRANCH_BLO_S :    // blo.s #imm
                                    Opc = "blo.s";
                                    Ope.Clear();
                                    Op1 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Displacement
                                    Ope << "0x" << ProgramCounter + (Short)Op1;
                                    break;

      case OPCODE_BRANCH_BHI_S :    // bhi.s #imm
                                    Opc = "bhi.s";
                                    Ope.Clear();
                                    Op1 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Displacement
                                    Ope << "0x" << ProgramCounter + (Short)Op1;
                                    break;

      case OPCODE_BRANCH_BLE_S :    // ble.s #imm
                                    Opc = "ble.s";
                                    Ope.Clear();
                                    Op1 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Displacement
                                    Ope << "0x" << ProgramCounter + (Short)Op1;
                                    break;

      case OPCODE_BRANCH_BXX :      // bra #imm
                                    Opc = "bxx";
                                    Ope.Clear();
                                    Op1 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Displacement
                                    Ope << "0x" << ProgramCounter + (Short)Op1;
                                    break;

      case OPCODE_JSR_IND:          // jsr (reg)
                                    Opc = "jsr";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    Ope << "(" << RD << ")";
                                    break;

      case OPCODE_RTS:              // rts
                                    Opc = "rts";
                                    Ope.Clear();
                                    break;

      case OPCODE_RTI:              // rti
                                    Opc = "rti";
                                    Ope.Clear();
                                    break;
      //case 30:// int reg
      case OPCODE_EXT_BW:           // ext.bw reg
                                    Opc = "ext.bw";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    Ope << "r" << RD;
                                    break;

      case OPCODE_EXT_BL:           // ext.bl reg
                                    Opc = "ext.bl";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    Ope << "r" << RD;
                                    break;

      case OPCODE_EXT_WL:           // ext.wl reg
                                    Opc = "ext.wl";
                                    Ope.Clear();
                                    Op1 = FetchOperand_8(ProgramCounter++); // Register
                                    RD = Op1&0xf;
                                    Ope << "r" << RD;
                                    break;

      default:                      Opc.Clear();
                                    Opc << "[" << (ULong)Opcode << "]";
                                    Ope.Clear();
                                    Ope.Append_char(Opcode);
                                    return(false);
    }

    // printf("%x: %s %s\n",OldPC,  Opc.String, Ope.String);

    return(true);
  }

  protected:

  ZString RegisterOpcode(ULong FormatInfo,
                         ULong Cycles,
                         ULong InstructionBytes,
                         ULong Opcode,
                         char const * OpcodeLabel,
                         char const * Flags,
                         char const * OpcodeName,
                         char const * Size,
                         char const * Operand,
                         char const * Encoding,
                         char const * Description)
  {
    ZString Wrk, Wrk2, Out;

    switch(FormatInfo)
    {
      case 0: Wrk.Set_DisplayBase(16);
              Wrk.Set_DisplayTrailingZero();
              Wrk = Opcode;
              Out << "|"<<Wrk.Rights(2)<<"|";
              Wrk2 = Cycles&(~1024);
              if (Cycles & 1024)    Wrk2 << "+4r";
              Wrk2.PadOnLeftUptoLen(' ',4);
              Out << Wrk2;
              Out.PadUptoLen(' ',8);
              Out << "|" << Flags << "|";
              Out << OpcodeName << Size << " " << Operand;
              Out.PadUptoLen(' ', 46);
              Out << "|" << Description;
              Out.PadUptoLen(' ', 150);
              Out << "|\n";
              break;
    }

    return(Out);
  }

  public:

  ZString OutputOpcodeDatabase(ULong Fi)
  {
    ZString Out, Str;

    switch(Fi)
    {
      case 0: Str.SetLen(151); Str.Fill('-'); Str << "\n";
              Out << Str;
              Out << "|Op|Time|CVNZ| Instruction                    | Description                                                                                           |\n";
              Out << Str;
              break;
    }
    //                                                                                CVNZ
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_MOVE_IMM_B,     "OPCODE_MOVE_IMM_B",     "  XX", "move",    ".b",   "#_8BitsImmediateValue, rx", "0000DDDD IIIIIIII",          "Move  8 bit immediate value to register" );
    Out << RegisterOpcode(Fi, 10, 4, OPCODE_MOVE_IMM_W,     "OPCODE_MOVE_IMM_W",     "  XX", "move",    ".w",   "#16BitsImmediateValue, rx", "0000DDDD IIIIIIII IIIIIIII", "Move 16 bit immediate value to register" );
    Out << RegisterOpcode(Fi, 14, 6, OPCODE_MOVE_IMM_L,     "OPCODE_MOVE_IMM_L",     "  XX", "move",    ".l",   "#32BitsImmediateValue, rx", "0000DDDD IIIIIIII IIIIIIII IIIIIIII IIIIIIII", "Move 32 bits immediate value to register" );
    Out << RegisterOpcode(Fi,  8, 2, OPCODE_MOVE_IND_REG_B, "OPCODE_MOVE_IND_REG_B", "  XX", "move",    ".b",   "(rx),rx" , "SSSSDDDD", "Move 8 bits memory data pointed by source register to destination register" );
    Out << RegisterOpcode(Fi, 10, 2, OPCODE_MOVE_IND_REG_W, "OPCODE_MOVE_IND_REG_W", "  XX", "move",    ".w",   "(rx),rx" , "SSSSDDDD", "Move 16 bits memory data pointed by source register to destination register" );
    Out << RegisterOpcode(Fi, 14, 2, OPCODE_MOVE_IND_REG_L, "OPCODE_MOVE_IND_REG_L", "  XX", "move",    ".l",   "(rx),rx" , "SSSSDDDD", "Move 32 bits memory data pointed by source register to destination register" );
    Out << RegisterOpcode(Fi,  8, 2, OPCODE_MOVE_REG_IND_B, "OPCODE_MOVE_REG_IND_B", "  XX", "move",    ".b",   "rx,(rx)" , "SSSSDDDD", "Move  8 bits from source register to memory pointed by destination register" );
    Out << RegisterOpcode(Fi, 10, 2, OPCODE_MOVE_REG_IND_W, "OPCODE_MOVE_REG_IND_W", "  XX", "move",    ".w",   "rx,(rx)" , "SSSSDDDD", "Move 16 bits from source register to memory pointed by destination register" );
    Out << RegisterOpcode(Fi, 14, 2, OPCODE_MOVE_REG_IND_L, "OPCODE_MOVE_REG_IND_L", "  XX", "move",    ".l",   "rx,(rx)" , "SSSSDDDD", "Move 32 bits from source register to memory pointed by destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_MOVE_REG_REG_B, "OPCODE_MOVE_REG_REG_B", "  XX", "move",    ".b",   "rx,rx"   , "SSSSDDDD", "Move  8 bits from source register to destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_MOVE_REG_REG_W, "OPCODE_MOVE_REG_REG_W", "  XX", "move",    ".w",   "rx,rx"   , "SSSSDDDD", "Move 16 bits from source register to destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_MOVE_REG_REG_L, "OPCODE_MOVE_REG_REG_L", "  XX", "move",    ".l",   "rx,rx"   , "SSSSDDDD", "Move 32 bits from source register to destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_MOVEX_IMM_SN,   "OPCODE_MOVEX_IMM_SN",   "  XX", "movex",   ".sn",  "#_4BitsSignedValue,rx" , "IIIIDDDD", "Move 4 bits immediate signed value expanded to 32 bits destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_MOVEX_IMM_UN,   "OPCODE_MOVEX_IMM_UN",   "  XX", "movex",   ".un",  "#_4BitsSignedValue,rx" , "IIIIDDDD", "Move 4 bits immediate unsigned value expanded to 32 bits destination register" );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_MOVEX_IMM_SB,   "OPCODE_MOVEX_IMM_SB",   "  XX", "movex",   ".sb",  "#_8BitsSignedValue,rx" , "0000DDDD IIIIIIII", "Move  8 bits immediate signed value expanded to 32 bits destination register" );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_MOVEX_IMM_UB,   "OPCODE_MOVEX_IMM_UB",   "  XX", "movex",   ".b",   "#8BitsUnsignedValue,rx", "0000DDDD IIIIIIII", "Move  8 bits immediate unsigned value expanded to 32 bits destination register" );
    Out << RegisterOpcode(Fi,  8, 4, OPCODE_MOVEX_IMM_SW,   "OPCODE_MOVEX_IMM_SW",   "  XX", "movex",   ".sw",  "#16BitsSignedValue,rx" , "0000DDDD IIIIIIII IIIIIIII", "Move 16 bits immediate signed value expanded to 32 bits destination register" );
    Out << RegisterOpcode(Fi, 10, 4, OPCODE_MOVEX_IMM_UW,   "OPCODE_MOVEX_IMM_UW",   "  XX", "movex",   ".w",   "#16BitsUnsignedValue,rx","0000DDDD IIIIIIII IIIIIIII", "Move 16 bits immediate unsigned value expanded to 32 bits destination register" );
    Out << RegisterOpcode(Fi,1032,3, OPCODE_PUSHREGS,       "OPCODE_PUSHREGS",       "    ", "pushregs","",     "rx-rx/rx...",            "MMMMMMMM MMMMMMMM", "Push selected registers (by bitmask) to stack" );
    Out << RegisterOpcode(Fi,1032,3, OPCODE_POPREGS,        "OPCODE_POPREGS",        "    ", "popregs", "",     "rx-rx/rx...",            "MMMMMMMM MMMMMMMM", "Pop selected registers (by bitmask) to stack" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_INC,            "OPCODE_INC",            " XXX", "inc",     ".l",   "#4BitValue,rx",          "IIIIDDDD", "Increment selected register with the immediate 4 bit value" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_DEC,            "OPCODE_DEC",            " XXX", "dec",     ".l",   "#4BitSignedValue,rx",    "IIIIDDDD", "Decrement selected register with the immediate 4 bit value" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_ADD_B,          "OPCODE_ADD_B",          "XXXX", "add",     ".b",   "rx,rx",                  "SSSSDDDD", "8  bit Addition from the source register to the destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_ADD_W,          "OPCODE_ADD_W",          "XXXX", "add",     ".w",   "rx,rx",                  "SSSSDDDD", "16 bit Addition from the source register to the destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_ADD_L,          "OPCODE_ADD_L",          "XXXX", "add",     ".l",   "rx,rx",                  "SSSSDDDD", "32 bit Addition from the source register to the destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_SUB_B,          "OPCODE_SUB_B",          "XXXX", "sub",     ".b",   "rx,rx",                  "SSSSDDDD", "8  bit substraction from the source register to the destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_SUB_W,          "OPCODE_SUB_W",          "XXXX", "sub",     ".w",   "rx,rx",                  "SSSSDDDD", "16 bit substraction from the source register to the destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_SUB_L,          "OPCODE_SUB_L",          "XXXX", "sub",     ".l",   "rx,rx",                  "SSSSDDDD", "32 bit substraction from the source register to the destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_AND_B,          "OPCODE_AND_B",          "  XX", "and",     ".b",   "rx,rx",                  "SSSSDDDD", "8  bit boolean AND operation from the source register to the destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_AND_W,          "OPCODE_AND_W",          "  XX", "and",     ".w",   "rx,rx",                  "SSSSDDDD", "16  bit boolean AND operation from the source register to the destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_AND_L,          "OPCODE_AND_L",          "  XX", "and",     ".l",   "rx,rx",                  "SSSSDDDD", "32  bit boolean AND operation from the source register to the destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_OR_B,           "OPCODE_OR_B",           "  XX", "or",      ".b",   "rx,rx",                  "SSSSDDDD", "8   bit boolean OR operation from the source register to the destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_OR_W,           "OPCODE_OR_W",           "  XX", "or",      ".w",   "rx,rx",                  "SSSSDDDD", "16  bit boolean OR operation from the source register to the destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_OR_L,           "OPCODE_OR_L",           "  XX", "or",      ".l",   "rx,rx",                  "SSSSDDDD", "32  bit boolean OR operation from the source register to the destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_XOR_B,          "OPCODE_XOR_B",          "  XX", "xor",     ".b",   "rx,rx",                  "SSSSDDDD", "8   bit boolean XOR operation from the source register to the destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_XOR_W,          "OPCODE_XOR_W",          "  XX", "xor",     ".w",   "rx,rx",                  "SSSSDDDD", "16  bit boolean XOR operation from the source register to the destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_XOR_L,          "OPCODE_XOR_L",          "  XX", "xor",     ".l",   "rx,rx",                  "SSSSDDDD", "32  bit boolean XOR operation from the source register to the destination register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_ASL_B,          "OPCODE_ASL_B",          "X XX", "asl",     ".b",   "rx,rx",                  "SSSSDDDD", "8   bit arithmetic shift left of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_ASL_W,          "OPCODE_ASL_W",          "X XX", "asl",     ".w",   "rx,rx",                  "SSSSDDDD", "16  bit arithmetic shift left of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_ASL_L,          "OPCODE_ASL_L",          "X XX", "asl",     ".l",   "rx,rx",                  "SSSSDDDD", "32   bit arithmetic shift left of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_ASR_B,          "OPCODE_ASR_B",          "X XX", "asr",     ".b",   "rx,rx",                  "SSSSDDDD", "8   bit arithmetic shift right of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_ASR_W,          "OPCODE_ASR_W",          "X XX", "asr",     ".w",   "rx,rx",                  "SSSSDDDD", "16  bit arithmetic shift right of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_ASR_L,          "OPCODE_ASR_L",          "X XX", "asr",     ".l",   "rx,rx",                  "SSSSDDDD", "32  bit arithmetic shift right of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_LSL_B,          "OPCODE_LSL_B",          "X XX", "lsl",     ".b",   "rx,rx",                  "SSSSDDDD", "8   bit logical shift left of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_LSL_W,          "OPCODE_LSL_W",          "X XX", "lsl",     ".w",   "rx,rx",                  "SSSSDDDD", "16  bit logical shift left of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_LSL_L,          "OPCODE_LSL_L",          "X XX", "lsl",     ".l",   "rx,rx",                  "SSSSDDDD", "32  bit logical shift left of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_LSR_B,          "OPCODE_LSR_B",          "X XX", "lsr",     ".b",   "rx,rx",                  "SSSSDDDD", "8   bit logical shift right of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_LSR_W,          "OPCODE_LSR_W",          "X XX", "lsr",     ".w",   "rx,rx",                  "SSSSDDDD", "16  bit logical shift right of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_LSR_L,          "OPCODE_LSR_L",          "X XX", "lsr",     ".l",   "rx,rx",                  "SSSSDDDD", "32  bit logical shift right of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_ROL_B,          "OPCODE_ROL_B",          "X XX", "rol",     ".b",   "rx,rx",                  "SSSSDDDD", "8   bit left rotation with carry of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_ROL_W,          "OPCODE_ROL_W",          "X XX", "rol",     ".w",   "rx,rx",                  "SSSSDDDD", "16  bit left rotation with carry of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_ROL_L,          "OPCODE_ROL_L",          "X XX", "rol",     ".l",   "rx,rx",                  "SSSSDDDD", "32  bit left rotation with carry of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_ROR_B,          "OPCODE_ROR_B",          "X XX", "ror",     ".b",   "rx,rx",                  "SSSSDDDD", "8   bit right rotation with carry of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_ROR_W,          "OPCODE_ROR_W",          "X XX", "ror",     ".w",   "rx,rx",                  "SSSSDDDD", "16  bit right rotation with carry of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_ROR_L,          "OPCODE_ROR_L",          "X XX", "ror",     ".l",   "rx,rx",                  "SSSSDDDD", "32  bit right rotation with carry of the destination register with shift count from the source register" );
    Out << RegisterOpcode(Fi,150, 2, OPCODE_UMUL_B,         "OPCODE_UMUL_B",         "  XX", "umul",    ".b",   "rx,rx",                  "SSSSDDDD", "8   bit unsigned multiplication of the destination register with the source register" );
    Out << RegisterOpcode(Fi,150, 2, OPCODE_UMUL_W,         "OPCODE_UMUL_W",         "  XX", "umul",    ".w",   "rx,rx",                  "SSSSDDDD", "16  bit unsigned multiplication of the destination register with the source register" );
    Out << RegisterOpcode(Fi,150, 2, OPCODE_UMUL_L,         "OPCODE_UMUL_L",         "  XX", "umul",    ".l",   "rx,rx",                  "SSSSDDDD", "32  bit unsigned multiplication of the destination register with the source register" );
    Out << RegisterOpcode(Fi,150, 2, OPCODE_SMUL_B,         "OPCODE_SMUL_B",         "  XX", "smul",    ".b",   "rx,rx",                  "SSSSDDDD", "8   bit signed multiplication of the destination register with the source register" );
    Out << RegisterOpcode(Fi,150, 2, OPCODE_SMUL_W,         "OPCODE_SMUL_W",         "  XX", "smul",    ".w",   "rx,rx",                  "SSSSDDDD", "16  bit signed multiplication of the destination register with the source register" );
    Out << RegisterOpcode(Fi,150, 2, OPCODE_SMUL_L,         "OPCODE_SMUL_L",         "  XX", "smul",    ".l",   "rx,rx",                  "SSSSDDDD", "32  bit signed multiplication of the destination register with the source register" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_CMP_B,          "OPCODE_CMP_B",          "XXXX", "cmp",     ".b",   "rx,rx",                  "SSSSDDDD", "8   bit compare of source and destination registers" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_CMP_W,          "OPCODE_CMP_W",          "XXXX", "cmp",     ".w",   "rx,rx",                  "SSSSDDDD", "16  bit compare of source and destination registers" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_CMP_L,          "OPCODE_CMP_L",          "XXXX", "cmp",     ".l",   "rx,rx",                  "SSSSDDDD", "32  bit compare of source and destination registers" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_JMP_IND,        "OPCODE_JMP_IND",        "    ", "jmp",     "",     "(rx)" ,                  "0000DDDD", "Jump to the location designed by the destination register" );
    Out << RegisterOpcode(Fi, 14, 2, OPCODE_JSR_IND,        "OPCODE_JSR_IND",        "    ", "jsr",     "",     "(rx)" ,                  "0000DDDD", "Jump to the subroutine location designed by the destination register. Return address is on the stack." );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_BRANCH_BRA,     "OPCODE_BRANCH_BRA",     "    ", "bra",     "",     "#16BitsSignedValue",     "IIIIIIII IIIIIIII", "Jump to a location designed by relative immediate signed value" );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_BRANCH_BEQ,     "OPCODE_BRANCH_BEQ",     "    ", "beq",     "",     "#16BitsSignedValue",     "IIIIIIII IIIIIIII", "Conditionnal jump if values are equal" );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_BRANCH_BNE,     "OPCODE_BRANCH_BNE",     "    ", "bne",     "",     "#16BitsSignedValue",     "IIIIIIII IIIIIIII", "Conditionnal jump if values are not equal" );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_BRANCH_BCC,     "OPCODE_BRANCH_BCC",     "    ", "bcc",     "",     "#16BitsSignedValue",     "IIIIIIII IIIIIIII", "Conditionnal jump if carry is clear" );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_BRANCH_BCS,     "OPCODE_BRANCH_BCS",     "    ", "bcs",     "",     "#16BitsSignedValue",     "IIIIIIII IIIIIIII", "Conditionnal jump if carry is set" );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_BRANCH_BVC,     "OPCODE_BRANCH_BVC",     "    ", "bvc",     "",     "#16BitsSignedValue",     "IIIIIIII IIIIIIII", "Conditionnal jump if no overflow" );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_BRANCH_BVS,     "OPCODE_BRANCH_BVS",     "    ", "bvs",     "",     "#16BitsSignedValue",     "IIIIIIII IIIIIIII", "Conditionnal jump if overflow" );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_BRANCH_BMI,     "OPCODE_BRANCH_BMI",     "    ", "bmi",     "",     "#16BitsSignedValue",     "IIIIIIII IIIIIIII", "Conditionnal jump if negative" );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_BRANCH_BPL,     "OPCODE_BRANCH_BPL",     "    ", "bpl",     "",     "#16BitsSignedValue",     "IIIIIIII IIIIIIII", "Conditionnal jump if positive" );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_BRANCH_BLO_S,   "OPCODE_BRANCH_BLO_S",   "    ", "blo",     ".s",   "#16BitsSignedValue",     "IIIIIIII IIIIIIII", "Conditionnal jump if lower (signed)" );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_BRANCH_BHI_U,   "OPCODE_BRANCH_BHI_U",   "    ", "bhi",     ".u",   "#16BitsSignedValue",     "IIIIIIII IIIIIIII", "Conditionnal jump if higher" );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_BRANCH_BHI_S,   "OPCODE_BRANCH_BHI_S",   "    ", "bhi",     ".s",   "#16BitsSignedValue",     "IIIIIIII IIIIIIII", "Conditionnal jump if higher(signed)" );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_BRANCH_BLE_U,   "OPCODE_BRANCH_BLE_U",   "    ", "ble",     ".u",   "#16BitsSignedValue",     "IIIIIIII IIIIIIII", "Conditionnal jump if lower or equal" );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_BRANCH_BLE_S,   "OPCODE_BRANCH_BLE_S",   "    ", "ble",     ".s",   "#16BitsSignedValue",     "IIIIIIII IIIIIIII", "Conditionnal jump if lower or equal(signed)" );
    Out << RegisterOpcode(Fi,  8, 3, OPCODE_BRANCH_BHE_S,   "OPCODE_BRANCH_BHE_S",   "    ", "bhe",     ".s",   "#16BitsSignedValue",     "IIIIIIII IIIIIIII", "Conditionnal jump if higher or same(signed)" );
    Out << RegisterOpcode(Fi, 12, 1, OPCODE_RTS,            "OPCODE_RTS",            "    ", "rts",     "",     "",                       "",                  "Return from subroutine" );
    Out << RegisterOpcode(Fi, 16, 1, OPCODE_RTI,            "OPCODE_RTI",            "XXXX", "rti",     "",     "",                       "",                  "Return from interrupt" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_EXT_BW,         "OPCODE_EXT_BW",         "  XX", "ext",     ".bw",  "rx",                     "0000DDDD",          "Expand first  8 bits (signed) of the destination register to 16 bits" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_EXT_BL,         "OPCODE_EXT_BL",         "  XX", "ext",     ".bl",  "rx",                     "0000DDDD",          "Expand first  8 bits (signed) of the destination register to 32 bits" );
    Out << RegisterOpcode(Fi,  6, 2, OPCODE_EXT_WL,         "OPCODE_EXT_WL",         "  XX", "ext",     ".wl",  "rx",                     "0000DDDD",          "Expand first 16 bits (signed) of the destination register to 32 bits" );

    switch(Fi)
    {
      case 0: Out << Str;
              break;
    }
    return(Out);
  }

    virtual bool Save(ZStream_SpecialRamStream * Stream)
    {
      ULong * BlockSize;
      ULong   StartLen,i;

      BlockSize = Stream->GetPointer_ULong();
      Stream->Put(0u);        // The size of the block (defered storage).
      StartLen = Stream->GetActualBufferLen();
      Stream->Put((UShort)1); // Data Block Version;

      // Storage informations.

      Stream->Put(ProgramCounter);
      Stream->Put(StatusRegister.WholeRegister);
      Stream->Put(Interrupt_Pending);
      Stream->Put(StepMode);
      Stream->Put(StepCount);
      for(i=0;i<16;i++) Stream->Put(GeneralRegister[i].Reg_ULargest);

      // Compute Block Len and write it to start of the block.

      *BlockSize = Stream->GetActualBufferLen() - StartLen;

      return(true);
    }

    virtual bool Load(ZStream_SpecialRamStream * Stream)
    {
      bool Ok;
      ULong i;

      ULong  BlockSize;
      UShort BlockVersion;
      UByte  Temp_Byte;

      Ok = Stream->Get(BlockSize);
      Ok&= Stream->Get(BlockVersion);     if(!Ok) return(false);

      // Check for supported block version. If unsupported new version, throw content and continue with a blank dataset.

      if (BlockVersion!=1) { BlockSize-=2; for(ZMemSize i=0;i<BlockSize;i++) Ok = Stream->Get(Temp_Byte); if (Ok) return(true); else return(false);}

      // Load CPU informations

      Ok &= Stream->Get(ProgramCounter);
      Ok &= Stream->Get(StatusRegister.WholeRegister);
      Ok &= Stream->Get(Interrupt_Pending);
      Ok &= Stream->Get(StepMode);
      Ok &= Stream->Get(StepCount);
      for(i=0;i<16;i++) Ok &= Stream->Get(GeneralRegister[i].Reg_ULargest);

      return(Ok);
    }

};


#endif
