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

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
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
  bool ZeroFlag:1;
  bool NegativeFlag:1;
  bool OverflowFlag:1;
  bool CarryFlag:1;
};

template <typename ZVMachine>
class BlackCPU
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
  ZVMachine      * VMachine;
  
  BlackCPU()
  {
    unsigned long i;

    for(i=0;i<16;i++) GeneralRegister[i].Reg_ULargest = 0;
    ProgramCounter = 0;
    StatusRegister.WholeRegister = 0;

    VMachine = 0;
  }

  void SetVMachine(ZVMachine * VMachine)
  {
    this->VMachine = VMachine;
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
    //return( *(UShort *)(optable+ProgramCounter));
    return(0);
  }

  inline ULong FetchOperand_32(ULong ProgramCounter)
  {
    //return(*(ULong *)(optable+ProgramCounter));
    return(0);
  }


  inline UByte ReadMemory_8(ULong Address)
  {
    //return(optable[Address]);
    return(0);
  }

  inline UShort ReadMemory_16(ULong Address)
  {
    //return(optable[Address]);
    return(0);
  }

  inline ULong ReadMemory_32(ULong Address)
  {
    //return(optable[Address]);
    return(0);
  }

  inline void WriteMemory_8(ULong Address, UByte Data)
  {
    return;
  }

  inline void WriteMemory_16(ULong Address, UShort Data)
  {
    return;
  }

  inline void WriteMemory_32(ULong Address, ULong Data)
  {
    return;
  }

  inline void PushStack_32(ULong Data)
  {
    return;
  }

  inline ULong PopStack_32()
  {
    return(0);
  }

  void PrintStatus(ZStatusRegister StatusFlag)
  {
	StatusRegister_BCPU * s = (StatusRegister_BCPU *)&StatusFlag;
    printf("Zero Flag  : %s\n", s->ZeroFlag ? "1" : "0");
    printf("Sign Flag  : %s\n", s->NegativeFlag ? "1" : "0");
    printf("Over Flag  : %s\n", s->OverflowFlag ? "1" : "0");
    printf("Carry Flag : %s\n", s->CarryFlag ? "1" : "0");
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

        OPCODE_BREAK = 0,
        OPCODE_MOVE_IMM_B,
        OPCODE_MOVE_IND_REG_B,
        OPCODE_MOVE_REG_IND_B,
        OPCODE_MOVE_REG_REG_B,
        OPCODE_MOVEX_IMM_SB,
        OPCODE_MOVEX_IMM_UB,
        OPCODE_PUSHREGS,
        OPCODE_POPREGS,
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
        // OPCODE_MOVEX_IMM_SL   = OPCODE_MOVEX_IMM_SB    + 128,
        // OPCODE_MOVEX_IMM_UL   = OPCODE_MOVEX_IMM_UB    + 128,
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

  void Execute(ULong CycleCount)
  {
    ULong ElapsedCycles;
    register UByte Opcode;
    register int Op1,Op2;
    register ZStatusRegister Status;
    int i;
    
    Status = StatusRegister;
    
    ElapsedCycles =  0;
    
    while(ElapsedCycles < CycleCount)
    {
      Opcode = FetchOpcode(ProgramCounter++);
      PrintStatus(Status);
      printf("-- Opcode : %d --\n",Opcode);
      switch(Opcode)
      {
        case OPCODE_BREAK:         // break



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
                                      WriteMemory_8(GeneralRegister[Op1 & 0x0f].Reg_ULargest, GeneralRegister[(Op1&0xf0)>>4].Reg_UWord);
                                      Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)>>4].Reg_UWord ,Status );
                                      ElapsedCycles += 10;
                                      break;

        case OPCODE_MOVE_REG_IND_L:   // move.b reg,(reg)
                                      Op1 = FetchOperand_8(ProgramCounter++); // Register
                                      WriteMemory_8(GeneralRegister[Op1 & 0x0f].Reg_ULargest, GeneralRegister[(Op1&0xf0)>>4].Reg_ULong);
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


        case OPCODE_MOVEX_IMM_UB:     // movex.ub #imm,reg
                                      Op1 = FetchOperand_8(ProgramCounter++); // Register
                                      Op2 = FetchOperand_8(ProgramCounter++);  // Immediate value
                                      GeneralRegister[Op1 & 0x0f].Reg_ULargest = (UByte)Op2;
                                      Status_Test_ZN_8(Op2,Status);
                                      ElapsedCycles += 8;
                                      break;

         case OPCODE_MOVEX_IMM_UW:    // movex.uw #imm,reg
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
                                      ElapsedCycles += 6;
                                      // Todo
                                      break;

        case OPCODE_SUB_L:            // sub.l reg,reg (No carry)
                                      Op1 = FetchOperand_8(ProgramCounter++); // Register
                                      ElapsedCycles += 6;
                                      // Todo
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

        case OPCODE_JMP_IND:          // jmp (reg)
                                      Op1 = FetchOperand_8(ProgramCounter++); // Register
                                      ProgramCounter = GeneralRegister[Op1&0xf].Reg_ULargest;
                                      ElapsedCycles += 6;
                                      break;

        case OPCODE_BRANCH_BRA:       // bra #imm
                                      Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
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

        case 255: return;
      }
    }

    // Environment call.



    VMachine->Envcall();

  }
};


#endif
