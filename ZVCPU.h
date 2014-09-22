#ifndef Z_ZVCPU_H
#define Z_ZVCPU_H

//#ifndef Z_ZVCPU_H
//#  include "zvcpu.h"
//#endif


#define Z_ZVCPU_EXPERIMENTAL_ASMCODE 0

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

unsigned char optable[]=
{
  1+128,0,128+2,0,0,0,
  1+128,1,2,0,0,0,
  13,1,
  255,255,
  255,0
};

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
  ULong ProgramCounter;
  ZStatusRegister StatusRegister;
  
  
  BlackCPU()
  {
    unsigned long i;

    for(i=0;i<16;i++) GeneralRegister[i].Reg_ULargest = 0;
    ProgramCounter = 0;
    StatusRegister.WholeRegister = 0;
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
    return(optable[ProgramCounter]);
  }
  
  inline UByte FetchOperand_8(ULong ProgramCounter)
  {
    return(optable[ProgramCounter]);
  }

  inline UShort FetchOperand_16(ULong ProgramCounter)
  {
    return( *(UShort *)(optable+ProgramCounter));
  }

  inline ULong FetchOperand_32(ULong ProgramCounter)
  {
    return(*(ULong *)(optable+ProgramCounter));
  }

  inline UELong FetchOperand_64(ULong ProgramCounter)
  {
    return(optable[ProgramCounter]);
  }

  inline UByte ReadMemory_8(ULong Address)
  {
    return(optable[Address]);
  }

  inline UShort ReadMemory_16(ULong Address)
  {
    return(optable[Address]);
  }

  inline ULong ReadMemory_32(ULong Address)
  {
    return(optable[Address]);
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
        case 0:         // break
        case 1|SZ_08:   // move.b #imm,reg
                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                        Op2 = FetchOperand_8(ProgramCounter); ProgramCounter+=2; // Immediate value
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                        GeneralRegister[Op1 & 0x0f].Reg_UByte = Op2;
                        Status_Test_ZN_8(Op2,Status);
#else
                        asm
                        (
                          "testb %b3,%b3;"
                          "movb  %b3,%b0;"
                          "lahf;"
                          "rcrl $1,%1;"
                          "rcrl $15,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $1,%%eax;"
                          "rcll $2,%1;"
                          : "=m" (GeneralRegister[Op1&0x0f]), "=q" (Status)
                          : "1" (Status), "q" (Op2)
                          : "%eax"
                        );
#endif
                        break;

        case 1|SZ_16:  // move.w #imm,reg
                        Op1 = FetchOperand_8(ProgramCounter++) & 0x0f; // Register
                        Op2 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Immediate value
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                        GeneralRegister[Op1 & 0x0f].Reg_UWord = Op2;
                        Status_Test_ZN_16(Op2,Status);
#else
                        asm
                        (
                          "testw %w3,%w3;"
                          "movw  %w3,%w0;"
                          "lahf;"
                          "rcrl $1,%1;"
                          "rcrl $15,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $1,%%eax;"
                          "rcll $2,%1;"
                          : "=m" (GeneralRegister[Op1]), "=q" (Status)
                          : "1" (Status), "q" (Op2)
                          : "%eax"
                        );
#endif
                        break;

        case 1|SZ_32:   // move.l #imm,reg
                        Op1 = FetchOperand_8(ProgramCounter++) & 0x0f; // Register
                        Op2 = FetchOperand_32(ProgramCounter); ProgramCounter+=4;  // Immediate value
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                        GeneralRegister[Op1 & 0x0f].Reg_ULong = Op2;
                        Status_Test_ZN_32(Op2,Status);
#else
                        asm
                        (
                          "testl %3,%3;"
                          "movl  %3,%0;"
                          "lahf;"
                          "rcrl $1,%1;"
                          "rcrl $15,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $1,%%eax;"
                          "rcll $2,%1;"
                          : "=m" (GeneralRegister[Op1]), "=q" (Status)
                          : "1" (Status), "q" (Op2)
                          : "%eax"
                        );
#endif
                   break;
        case 2|SZ_08:    // movex.sb #imm,reg
                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                        Op2 = FetchOperand_8(ProgramCounter++);  // Immediate value
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                        GeneralRegister[Op1 & 0x0f].Reg_SLargest = (Byte)Op2;
                        Status_Test_ZN_8(Op2,Status);
#else
                        asm
                        (
                          "testb %b3,%b3;"
                          "lahf;"
                          "rcrl $1,%1;"
                          "rcrl $15,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $1,%%eax;"
                          "rcll $2,%1;"
                          "movsbl %b3,%%eax;"
                          "movl %%eax,%0;"
                          : "=m" (GeneralRegister[Op1&0x0f]), "=q" (Status)
                          : "1" (Status), "q" (Op2)
                          : "%eax"
                        );
#endif
                        break;
        case 2|SZ_16:   // movex.sw #imm,reg
                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                        Op2 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Immediate value
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                        GeneralRegister[Op1 & 0x0f].Reg_SLargest = (Short)Op2;
                        Status_Test_ZN_16(Op2,Status);
#else
                        asm
                        (
                          "testw %w3,%w3;"
                          "lahf;"
                          "rcrl $1,%1;"
                          "rcrl $15,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $1,%%eax;"
                          "rcll $2,%1;"
                          "movswl %w3,%%eax;"
                          "movl   %%eax,%0;"
                          : "=m" (GeneralRegister[Op1&0x0f]), "=q" (Status)
                          : "1" (Status), "q" (Op2)
                          : "%eax"
                        );
#endif
                        break;

        case 2|SZ_32:   // movex.l #imm,reg
                        // does nothing. Reserved for future extension

        case 3|SZ_08:   // movex.ub #imm,reg
                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                        Op2 = FetchOperand_8(ProgramCounter++);  // Immediate value
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                        GeneralRegister[Op1 & 0x0f].Reg_ULargest = (UByte)Op2;
                        Status_Test_ZN_8(Op2,Status);
#else
                        asm
                        (
                          "testb %b3,%b3;"
                          "lahf;"
                          "rcrl $1,%1;"
                          "rcrl $15,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $1,%%eax;"
                          "rcll $2,%1;"
                          "movzbl %b3,%%eax;"
                          "movl   %%eax,%0;"
                          : "=m" (GeneralRegister[Op1&0x0f]), "=q" (Status)
                          : "1" (Status), "q" (Op2)
                          : "%eax"
                        );
#endif
                        break;

         case 3|SZ_16:  // movex.uw #imm,reg
                        Op1 = FetchOperand_8(ProgramCounter++); // Register
                        Op2 = FetchOperand_16(ProgramCounter); ProgramCounter+=2; // Immediate value
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                        GeneralRegister[Op1 & 0x0f].Reg_ULargest = (UShort)Op2;
                        Status_Test_ZN_16(Op2,Status);
#else
                        asm
                        (
                          "testw %w3,%w3;"
                          "lahf;"
                          "rcrl $1,%1;"
                          "rcrl $15,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $1,%%eax;"
                          "rcll $2,%1;"
                          "movzwl %w3,%%eax;"
                          "movl %%eax,%0;"
                          : "=m" (GeneralRegister[Op1&0x0f]), "=q" (Status)
                          : "1" (Status), "q" (Op2)
                          : "%eax"
                        );
#endif
                        break;

        case 4|SZ_08:   // move.b (reg),reg
                        Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                        Status_Test_ZN_8( GeneralRegister[Op1 & 0x0f].Reg_UByte = ReadMemory_8(GeneralRegister[(Op1&0xf0)>>4].Reg_ULargest)
                                         ,Status );
#else
                        asm
                        (
                          "movb %3,%%al;"
                          "movb %%al,%0;"
                          "testb %%al,%%al;"
                          "lahf;"
                          "rcrl $1,%1;"
                          "rcrl $15,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $1,%%eax;"
                          "rcll $2,%1;"
                          : "=m" (GeneralRegister[Op1&0x0f]), "=q" (Status)
                          : "1" (Status), "q" (ReadMemory_8(GeneralRegister[(Op1&0xf0)>>4].Reg_ULargest))
                          : "%eax"
                        );
#endif
                break;
        case 4|SZ_16:   // move.w (reg),reg
                        Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                        Status_Test_ZN_16( GeneralRegister[Op1 & 0x0f].Reg_UWord = ReadMemory_16(GeneralRegister[(Op1&0xf0)>>4].Reg_ULargest)
                                          ,Status );
#else
                        asm
                        (
                          "movw %w3,%%ax;"
                          "movw %%ax,%w0;"
                          "testw %%ax,%%ax;"
                          "lahf;"
                          "rcrl $1,%1;"
                          "rcrl $15,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $1,%%eax;"
                          "rcll $2,%1;"
                          : "=m" (GeneralRegister[Op1&0x0f]), "=q" (Status)
                          : "1" (Status), "q" (ReadMemory_16(GeneralRegister[(Op1&0xf0)>>4].Reg_ULargest))
                          : "%eax"
                        );
#endif
                        break;

        case 4|SZ_32:   // move.l (reg),reg
                        Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                        Status_Test_ZN_32( GeneralRegister[Op1 & 0x0f].Reg_ULong = ReadMemory_32(GeneralRegister[(Op1&0xf0)>>4].Reg_ULargest)
                                           ,Status );
#else
                        asm
                        (
                          "movl %3,%%eax;"
                          "movl %%eax,%0;"
                          "testl %%eax,%%eax;"
                          "lahf;"
                          "rcrl $1,%1;"
                          "rcrl $15,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $1,%%eax;"
                          "rcll $2,%1;"
                          : "=m" (GeneralRegister[Op1&0x0f]), "=q" (Status)
                          : "1" (Status), "q" (ReadMemory_32(GeneralRegister[(Op1&0xf0)>>4].Reg_ULargest))
                          : "%eax"
                        );
#endif
                        break;

        case 5|SZ_08:   // move.b reg,reg
                        Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                        Status_Test_ZN_8( GeneralRegister[Op1&0x0f].Reg_UByte = GeneralRegister[(Op1&0xf0)>>4].Reg_UByte
                                          ,Status );
#else
                        GeneralRegister[Op1&0x0f].Reg_UByte = GeneralRegister[(Op1&0xf0)>>4].Reg_UByte;
                        asm
                        (
                          "testb %b2,%b2;"
                          "lahf;"
                          "rcrl $1,%0;"
                          "rcrl $15,%%eax;"
                          "rcrl $1,%0;"
                          "rcrl $1,%%eax;"
                          "rcll $2,%0;"
                          : "=q" (Status)
                          : "0" (Status), "r" (GeneralRegister[(Op1&0xf0)>>4])
                          : "%eax"
                        );
#endif
                        break;

        case 5|SZ_16:   // move.w reg,reg
                        Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                        Status_Test_ZN_16( GeneralRegister[Op1&0x0f].Reg_UWord = GeneralRegister[(Op1&0xf0)>>4].Reg_UWord
                                          ,Status );
#else
                        GeneralRegister[Op1&0x0f].Reg_UWord = GeneralRegister[(Op1&0xf0)>>4].Reg_UWord;
                        asm
                        (
                          "testw %w2,%w2;"
                          "lahf;"
                          "rcrl $1,%0;"
                          "rcrl $15,%%eax;"
                          "rcrl $1,%0;"
                          "rcrl $1,%%eax;"
                          "rcll $2,%0;"
                          : "=q" (Status)
                          : "0" (Status), "r" (GeneralRegister[(Op1&0xf0)>>4])
                          : "%eax"
                        );
#endif
                        break;

        case 5|SZ_32:   // move.l reg,reg
                        Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                        Status_Test_ZN_32( GeneralRegister[Op1&0x0f].Reg_ULong = GeneralRegister[(Op1&0xf0)>>4].Reg_ULong
                                           ,Status );
#else
                        GeneralRegister[Op1&0x0f].Reg_ULong = GeneralRegister[(Op1&0xf0)>>4].Reg_ULong;
                        asm
                        (
                          "testl %2,%2;"
                          "lahf;"
                          "rcrl $1,%0;"
                          "rcrl $15,%%eax;"
                          "rcrl $1,%0;"
                          "rcrl $1,%%eax;"
                          "rcll $2,%0;"
                          : "=q" (Status)
                          : "0" (Status), "r" (GeneralRegister[(Op1&0xf0)>>4])
                          : "%eax"
                        );
#endif
                        break;

        case 6:         // pushregs R0-R14(reg)
                        {
                          Op1 = FetchOperand_16(ProgramCounter++); // Register
                          for(i=0;i<15;i++)
                          {
                            if (Op1&1) PushStack_32(GeneralRegister[i].Reg_ULargest);
                            Op1>>=1;
                          }
                        }
                        break;

        case 7:         // popregs R0-R15
                        Op1 = FetchOperand_16(ProgramCounter++); // Register
                        for(i=14;i<15;i--)
                        {
                          if (Op1&0x04000) GeneralRegister[i].Reg_ULargest = PopStack_32();
                          Op1<<=1;
                        }
                        break;

        case 8|SZ_08:   // add.b reg,reg (No carry)
                        Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                        {
                          register UByte D1,D2,D3;
                          D1 = GeneralRegister[(Op1&0xf0)>>4].Reg_UByte;
                          D2 = GeneralRegister[Op1&0xf].Reg_UByte;
                          D3 = D1+D2;
                          Status.Flags.CarryFlag = (D1&D2&0x80) != 0;
                          Status.Flags.OverflowFlag = ((Byte)((D1^D3) & (~(D1^D2))) < 0);
                          Status.Flags.ZeroFlag = (!D3);
                          Status.Flags.NegativeFlag = ((Byte)D3 < 0);
                        }
#else
                        asm
                        (
                          "addb %b2,%b0;"
                          "pushf;"
                          "popl %%eax;"
                          "rcrl $1,%1;"
                          "rcrl $7,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $1,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $4,%%eax;"
                          "rcrl $1,%1;"
                          "rcll $0xB,%%eax;"
                          "rcll $4,%1;"
                          : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                          : "r" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                          : "%eax"
                        );
#endif
                        break;

        case 8|SZ_16:   // add.b reg,reg (No carry)
                        Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                        {
                          register UShort D1,D2,D3;
                          D1 = GeneralRegister[(Op1&0xf0)>>4].Reg_UWord;
                          D2 = GeneralRegister[Op1&0xf].Reg_UWord;
                          D3 = D1+D2;
                          Status.Flags.CarryFlag = (D1&D2&0x80) != 0;
                          Status.Flags.OverflowFlag = ((Short)((D1^D3) & (~(D1^D2))) < 0);
                          Status.Flags.ZeroFlag = (!D3);
                          Status.Flags.NegativeFlag = ((Short)D3 < 0);
                        }
#else
                        asm
                        (
                          "addw %w2,%w0;"
                          "pushf;"
                          "popl %%eax;"
                          "rcrl $1,%1;"
                          "rcrl $7,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $1,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $4,%%eax;"
                          "rcrl $1,%1;"
                          "rcll $0xB,%%eax;"
                          "rcll $4,%1;"
                          : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                          : "r" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                          : "%eax"
                        );
#endif
                        break;

        case 8|SZ_32:   // add.b reg,reg (No carry)
                        Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                        {
                          register ULong D1,D2,D3;
                          D1 = GeneralRegister[(Op1&0xf0)>>4].Reg_ULong;
                          D2 = GeneralRegister[Op1&0xf].Reg_ULong;
                          D3 = D1+D2;
                          Status.Flags.CarryFlag = (D1&D2&0x80) != 0;
                          Status.Flags.OverflowFlag = ((Long)((D1^D3) & (~(D1^D2))) < 0);
                          Status.Flags.ZeroFlag = (!D3);
                          Status.Flags.NegativeFlag = ((Long)D3 < 0);
                        }
#else
                        asm
                        (
                          "addl %2,%0;"
                          "pushf;"
                          "popl %%eax;"
                          "rcrl $1,%1;"
                          "rcrl $7,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $1,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $4,%%eax;"
                          "rcrl $1,%1;"
                          "rcll $0xB,%%eax;"
                          "rcll $4,%1;"
                          : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                          : "r" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                          : "%eax"
                        );
#endif
                        break;

        case 9|SZ_08:   // sub.b reg,reg (No carry)
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
                        }
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
#else
                        asm
                        (
                          "subb %b2,%b0;"
                          "pushf;"
                          "popl %%eax;"
                          "rcrl $1,%1;"
                          "rcrl $7,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $1,%%eax;"
                          "rcrl $1,%1;"
                          "rcrl $4,%%eax;"
                          "rcrl $1,%1;"
                          "rcll $0xB,%%eax;"
                          "rcll $4,%1;"
                          : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                          : "r" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                          : "%eax"
                        );
#endif
                break;
        case 9|SZ_16: // sub.w reg,reg (No carry)
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
#else
                asm
                (
                  "subw %w2,%w0;"
                  "pushf;"
                  "popl %%eax;"
                  "rcrl $1,%1;"
                  "rcrl $7,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $4,%%eax;"
                  "rcrl $1,%1;"
                  "rcll $0xB,%%eax;"
                  "rcll $4,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "r" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax"
                );
#endif
                break;
        case 9|SZ_32: // sub.l reg,reg (No carry)
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
#else
                asm
                (
                  "subl %2,%0;"
                  "pushf;"
                  "popl %%eax;"
                  "rcrl $1,%1;"
                  "rcrl $7,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $4,%%eax;"
                  "rcrl $1,%1;"
                  "rcll $0xB,%%eax;"
                  "rcll $4,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "r" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax"
                );
#endif
                break;
        case 10|SZ_08: // and.b reg,reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                Status_Test_ZN_8( GeneralRegister[Op1 & 0x0f].Reg_UByte &= GeneralRegister[(Op1&0xf0)>>4].Reg_UByte
                                 ,Status );
#else
                asm
                (
                  "andb %b2,%b0;"
                  "lahf;"
                  "rcrl $1,%1;"
                  "rcrl $15,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcll $2,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "r" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax"
                );
#endif
                break;
        case 10|SZ_16: // and.w reg,reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                Status_Test_ZN_16( GeneralRegister[Op1 & 0x0f].Reg_UWord &= GeneralRegister[(Op1&0xf0)>>4].Reg_UWord
                                   ,Status );
#else
                asm
                (
                  "andw %w2,%w0;"
                  "lahf;"
                  "rcrl $1,%1;"
                  "rcrl $15,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcll $2,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "r" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax"
                );
#endif
                break;
        case 10|SZ_32: // and.l reg,reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                Status_Test_ZN_32( GeneralRegister[Op1 & 0x0f].Reg_ULong &= GeneralRegister[(Op1&0xf0)>>4].Reg_ULong
                                  ,Status );
#else
                asm
                (
                  "andl %2,%0;"
                  "lahf;"
                  "rcrl $1,%1;"
                  "rcrl $15,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcll $2,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "r" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax"
                );
#endif
                break;
        case 11|SZ_08: // or.b  reg,reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                Status_Test_ZN_8( GeneralRegister[Op1 & 0x0f].Reg_UByte |= GeneralRegister[(Op1&0xf0)>>4].Reg_UByte
                                  ,Status );
#else
                asm
                (
                  "orb %b2,%b0;"
                  "lahf;"
                  "rcrl $1,%1;"
                  "rcrl $15,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcll $2,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "r" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax"
                );
#endif
                break;
        case 11|SZ_16: // or.w  reg,reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                Status_Test_ZN_16( GeneralRegister[Op1 & 0x0f].Reg_UWord |= GeneralRegister[(Op1&0xf0)>>4].Reg_UWord
                                   ,Status );
#else
                asm
                (
                  "orw %w2,%w0;"
                  "lahf;"
                  "rcrl $1,%1;"
                  "rcrl $15,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcll $2,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "r" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax"
                );
#endif
                break;
        case 11|SZ_32: // or.l  reg,reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                Status_Test_ZN_32( GeneralRegister[Op1 & 0x0f].Reg_ULong |= GeneralRegister[(Op1&0xf0)>>4].Reg_ULong
                                   ,Status );
#else
                asm
                (
                  "orl %2,%0;"
                  "lahf;"
                  "rcrl $1,%1;"
                  "rcrl $15,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcll $2,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "r" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax"
                );
#endif
                break;
        case 12|SZ_08: // xor.b reg.reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                Status_Test_ZN_8( GeneralRegister[Op1 & 0x0f].Reg_UByte ^= GeneralRegister[(Op1&0xf0)>>4].Reg_UByte
                                  ,Status );
#else
                asm
                (
                  "xorb %b2,%b0;"
                  "lahf;"
                  "rcrl $1,%1;"
                  "rcrl $15,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcll $2,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "r" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax"
                );
#endif
                break;
        case 12|SZ_16: // xor.w reg.reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                Status_Test_ZN_16( GeneralRegister[Op1 & 0x0f].Reg_UWord ^= GeneralRegister[(Op1&0xf0)>>4].Reg_UWord
                                  ,Status );
#else
                asm
                (
                  "xorw %w2,%w0;"
                  "lahf;"
                  "rcrl $1,%1;"
                  "rcrl $15,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcll $2,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "r" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax"
                );
#endif
                break;
        case 12|SZ_32: // xor.l reg.reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                Status_Test_ZN_16( GeneralRegister[Op1 & 0x0f].Reg_ULong ^= GeneralRegister[(Op1&0xf0)>>4].Reg_ULong
                                  ,Status );
#else
                asm
                (
                  "xorl %2,%0;"
                  "lahf;"
                  "rcrl $1,%1;"
                  "rcrl $15,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcll $2,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "r" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax"
                );
#endif
                break;
        case 13|SZ_08: // asl.b reg,reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SByte < 0;
                Status_Test_ZN_8( GeneralRegister[(Op1&0xf0)].Reg_SByte = (GeneralRegister[(Op1&0xf0)].Reg_SByte << GeneralRegister[Op1&0xf].Reg_UByte) || (GeneralRegister[(Op1&0xf0)].Reg_SByte & 0x80)
                                  ,Status );
#else
                asm
                (
                  "movb %b2,%%cl;"
                  "salb %%cl,%b0;"
                  "lahf;"
                  "rcrl $1,%1;"
                  "rcrl $15,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcrl $2,%1;"
                  "rcll $7,%%eax;"
                  "rcll $4,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "q" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax","%ecx"
                );
#endif
                break;
        case 13|SZ_16: // asl.w reg,reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SWord < 0;
                Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)].Reg_SWord = (GeneralRegister[(Op1&0xf0)].Reg_SWord << GeneralRegister[Op1&0xf].Reg_UByte) || (GeneralRegister[(Op1&0xf0)].Reg_SWord & 0x8000)
                                   ,Status );
#else
                asm
                (
                  "movb %b2,%%cl;"
                  "salw %%cl,%w0;"
                  "lahf;"
                  "rcrl $1,%1;"
                  "rcrl $15,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcrl $2,%1;"
                  "rcll $7,%%eax;"
                  "rcll $4,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "q" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax","%ecx"
                );
#endif
                break;
        case 13|SZ_32: // asl.l reg,reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SLong < 0;
                Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)].Reg_SLong = (GeneralRegister[(Op1&0xf0)].Reg_SLong << GeneralRegister[Op1&0xf].Reg_UByte) || (GeneralRegister[(Op1&0xf0)].Reg_SLong & 0x80000000UL)
                                  ,Status );
#else
                asm
                (
                  "movb %b2,%%cl;"
                  "sall %%cl,%0;"
                  "lahf;"
                  "rcrl $1,%1;"
                  "rcrl $15,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcrl $2,%1;"
                  "rcll $7,%%eax;"
                  "rcll $4,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "q" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax","%ecx"
                );
#endif
                break;
        case 14|SZ_08: // asr.b reg,reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0

#else
                asm
                (
                  "movb %b2,%%cl;"
                  "sarb %%cl,%b0;"
                  "lahf;"
                  "rcrl $1,%1;"
                  "rcrl $15,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcrl $2,%1;"
                  "rcll $7,%%eax;"
                  "rcll $4,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "q" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax","%ecx"
                );
#endif
                break;
        case 14|SZ_16: // asr.w reg,reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SWord < 0;
                Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)].Reg_SWord = (GeneralRegister[(Op1&0xf0)].Reg_SWord >> GeneralRegister[Op1&0xf].Reg_UByte) || (GeneralRegister[(Op1&0xf0)].Reg_SWord & 0x8000)
                                   ,Status );
#else
                asm
                (
                  "movb %b2,%%cl;"
                  "sarb %%cl,%w0;"
                  "lahf;"
                  "rcrl $1,%1;"
                  "rcrl $15,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcrl $2,%1;"
                  "rcll $7,%%eax;"
                  "rcll $4,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "q" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax","%ecx"
                );
#endif
                break;
        case 14|SZ_32: // asr.l reg,reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SLong < 0;
                Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)].Reg_SLong = (GeneralRegister[(Op1&0xf0)].Reg_SLong >> GeneralRegister[Op1&0xf].Reg_UByte) || (GeneralRegister[(Op1&0xf0)].Reg_SLong & 0x80000000UL)
                                   ,Status );
#else
                asm
                (
                  "movb %b2,%%cl;"
                  "sarl %%cl,%0;"
                  "lahf;"
                  "rcrl $1,%1;"
                  "rcrl $15,%%eax;"
                  "rcrl $1,%1;"
                  "rcrl $1,%%eax;"
                  "rcrl $2,%1;"
                  "rcll $7,%%eax;"
                  "rcll $4,%1;"
                  : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
                  : "q" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
                  : "%eax","%ecx"
                );
#endif
                break;
        case 15|SZ_08:// lsl.b reg,reg
          Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
          Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SByte < 0;
          Status_Test_ZN_8( GeneralRegister[(Op1&0xf0)].Reg_SByte <<= GeneralRegister[Op1&0xf].Reg_UByte
                            ,Status );
#else
          asm
          (
            "movb %b2,%%cl;"
            "shlb %%cl,%b0;"
            "lahf;"
            "rcrl $1,%1;"
            "rcrl $15,%%eax;"
            "rcrl $1,%1;"
            "rcrl $1,%%eax;"
            "rcrl $2,%1;"
            "rcll $7,%%eax;"
            "rcll $4,%1;"
            : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
            : "q" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
            : "%eax","%ecx"
          );
#endif
          break;

        case 15|SZ_16:// lsl.w reg,reg
          Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
          Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SWord < 0;
          Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)].Reg_SWord <<= GeneralRegister[Op1&0xf].Reg_UByte
                             ,Status );
#else
          asm
          (
            "movb %b2,%%cl;"
            "shlw %%cl,%w0;"
            "lahf;"
            "rcrl $1,%1;"
            "rcrl $15,%%eax;"
            "rcrl $1,%1;"
            "rcrl $1,%%eax;"
            "rcrl $2,%1;"
            "rcll $7,%%eax;"
            "rcll $4,%1;"
            : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
            : "q" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
            : "%eax","%ecx"
          );
#endif
          break;
        case 15|SZ_32:// lsl.l reg,reg
          Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
          Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SLong < 0;
          Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)].Reg_SLong <<= GeneralRegister[Op1&0xf].Reg_UByte
                             ,Status );
#else
          asm
          (
            "movb %b2,%%cl;"
            "shll %%cl,%0;"
            "lahf;"
            "rcrl $1,%1;"
            "rcrl $15,%%eax;"
            "rcrl $1,%1;"
            "rcrl $1,%%eax;"
            "rcrl $2,%1;"
            "rcll $7,%%eax;"
            "rcll $4,%1;"
            : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
            : "q" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
            : "%eax","%ecx"
          );
#endif
          break;
        case 16|SZ_08:// lsr.b reg,reg
          Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
          Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SByte < 0;
          Status_Test_ZN_8( GeneralRegister[(Op1&0xf0)].Reg_SByte >>= GeneralRegister[Op1&0xf].Reg_UByte
                            ,Status );
#else
          asm
          (
            "movb %b2,%%cl;"
            "shrb %%cl,%b0;"
            "lahf;"
            "rcrl $1,%1;"
            "rcrl $15,%%eax;"
            "rcrl $1,%1;"
            "rcrl $1,%%eax;"
            "rcrl $2,%1;"
            "rcll $7,%%eax;"
            "rcll $4,%1;"
            : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
            : "q" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
            : "%eax","%ecx"
          );
#endif
          break;
        case 16|SZ_16:// lsr.w reg,reg
          Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
          Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SWord < 0;
          Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)].Reg_SWord >>= GeneralRegister[Op1&0xf].Reg_UByte
                             ,Status );
#else
          asm
          (
            "movb %b2,%%cl;"
            "shrw %%cl,%w0;"
            "lahf;"
            "rcrl $1,%1;"
            "rcrl $15,%%eax;"
            "rcrl $1,%1;"
            "rcrl $1,%%eax;"
            "rcrl $2,%1;"
            "rcll $7,%%eax;"
            "rcll $4,%1;"
            : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
            : "q" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
            : "%eax","%ecx"
          );
#endif
          break;
        case 16|SZ_32:// lsr.l reg,reg
          Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
          Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SLong < 0;
          Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)].Reg_SLong >>= GeneralRegister[Op1&0xf].Reg_UByte
                             ,Status );
#else
          asm
          (
            "movb %b2,%%cl;"
            "shrl %%cl,%0;"
            "lahf;"
            "rcrl $1,%1;"
            "rcrl $15,%%eax;"
            "rcrl $1,%1;"
            "rcrl $1,%%eax;"
            "rcrl $2,%1;"
            "rcll $7,%%eax;"
            "rcll $4,%1;"
            : "=m" (GeneralRegister[Op1&0xf])     , "=q" (Status)
            : "q" (GeneralRegister[(Op1&0xf0)>>4]), "1" (Status)
            : "%eax","%ecx"
          );
#endif
          break;
        case 17|SZ_08:// rol.b reg,reg
          Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
          {
            register bool Carry;
            Carry = Status.Flags.CarryFlag;
            Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SByte < 0;
            Status_Test_ZN_8( GeneralRegister[(Op1&0xf0)].Reg_UByte = GeneralRegister[(Op1&0xf0)].Reg_UByte >> GeneralRegister[Op1&0xf].Reg_UByte | (Carry ? 1:0)
                               ,Status );
          }
#else
#endif
          break;

        case 17|SZ_16:// rol.w reg,reg
          Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
          {
            register bool Carry;
            Carry = Status.Flags.CarryFlag;
            Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SWord < 0;
            Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)].Reg_UWord = GeneralRegister[(Op1&0xf0)].Reg_UByte >> GeneralRegister[Op1&0xf].Reg_UByte | (Carry ? 1:0)
                               ,Status );
          }
#else
#endif
          break;
        case 17|SZ_32:// rol.l reg,reg
          Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
          {
            register bool Carry;
            Carry = Status.Flags.CarryFlag;
            Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SLong < 0;
            Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)].Reg_ULong = GeneralRegister[(Op1&0xf0)].Reg_ULong >> GeneralRegister[Op1&0xf].Reg_UByte | (Carry ? 1:0)
                               ,Status );
          }
#else
#endif
          break;


        case 18|SZ_08:// ror.b reg,reg
          Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
          {
            register bool Carry;
            Carry = Status.Flags.CarryFlag;
            Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SByte | 1;
            Status_Test_ZN_8( GeneralRegister[(Op1&0xf0)].Reg_UByte = GeneralRegister[(Op1&0xf0)].Reg_UByte >> GeneralRegister[Op1&0xf].Reg_UByte | (Carry ? 1:0)
                               ,Status );
          }
#else
#endif
          break;
        case 18|SZ_16:// ror.b reg,reg
          Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
          {
            register bool Carry;
            Carry = Status.Flags.CarryFlag;
            Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SWord | 1;
            Status_Test_ZN_8( GeneralRegister[(Op1&0xf0)].Reg_UWord = GeneralRegister[(Op1&0xf0)].Reg_UWord >> GeneralRegister[Op1&0xf].Reg_UByte | (Carry ? 1:0)
                               ,Status );
          }
#else
#endif
          break;
        case 18|SZ_32:// ror.b reg,reg
          Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
          {
            register bool Carry;
            Carry = Status.Flags.CarryFlag;
            Status.Flags.CarryFlag = GeneralRegister[(Op1&0xf0)].Reg_SLong | 1;
            Status_Test_ZN_8( GeneralRegister[(Op1&0xf0)].Reg_ULong = GeneralRegister[(Op1&0xf0)].Reg_ULong >> GeneralRegister[Op1&0xf].Reg_UByte | (Carry ? 1:0)
                               ,Status );
          }
#else
#endif
          break;
        case 19|SZ_08:// umul.b reg,reg
          Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
            GeneralRegister[(Op1&0xf0)].Reg_UWord = (GeneralRegister[(Op1&0xf0)].Reg_UByte) * (GeneralRegister[Op1&0xf].Reg_UByte);
            Status.Flags.CarryFlag = Status.Flags.NegativeFlag = GeneralRegister[(Op1&0xf0)].Reg_UWord > 255;
            Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)].Reg_UWord ,Status );
#else
#endif
          break;
        case 19|SZ_16:// umul.w reg,reg
            Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
            GeneralRegister[(Op1&0xf0)].Reg_ULong = (GeneralRegister[(Op1&0xf0)].Reg_UWord) * (GeneralRegister[Op1&0xf].Reg_UWord);
            Status.Flags.CarryFlag = Status.Flags.NegativeFlag = GeneralRegister[(Op1&0xf0)].Reg_UWord > 65535;
            Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)].Reg_UWord ,Status );
#else
#endif
          break;
        case 19|SZ_32:// umul.l reg,reg
                 Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                 {
                   UELong Tmp;
                   Tmp = GeneralRegister[(Op1&0xf0)].Reg_ULong * GeneralRegister[Op1&0xf].Reg_ULong;
                   GeneralRegister[(Op1&0xf0)].Reg_ULong = (ULong)Tmp;
                   GeneralRegister[((Op1+0x10)&0xf0)].Reg_ULong = (ULong)(Tmp>>16);
                   Status.Flags.CarryFlag = Status.Flags.NegativeFlag = GeneralRegister[(Op1&0xf0)].Reg_ULong != 0;
                   Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)].Reg_ULong ,Status );
                 }
#else
#endif
          break;


        case 20|SZ_08:// smul.b reg,reg
          Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
            GeneralRegister[(Op1&0xf0)].Reg_SWord = (GeneralRegister[(Op1&0xf0)].Reg_SByte) * (GeneralRegister[Op1&0xf].Reg_SByte);
            Status.Flags.CarryFlag = Status.Flags.NegativeFlag = GeneralRegister[(Op1&0xf0)].Reg_SWord > 255;
            Status_Test_ZN_16( GeneralRegister[(Op1&0xf0)].Reg_UWord ,Status );
#else
#endif
          break;
        case 20|SZ_16:// smul.w reg,reg
            Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
            GeneralRegister[(Op1&0xf0)].Reg_SLong = (GeneralRegister[(Op1&0xf0)].Reg_SWord) * (GeneralRegister[Op1&0xf].Reg_SWord);
            Status.Flags.CarryFlag = Status.Flags.NegativeFlag = GeneralRegister[(Op1&0xf0)].Reg_UWord > 65535;
            Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)].Reg_UWord ,Status );
#else
#endif
          break;
        case 20|SZ_32:// smul.l reg,reg
                 Op1 = FetchOperand_8(ProgramCounter++); // Register
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
                 {
                   ELong Tmp;
                   Tmp = GeneralRegister[(Op1&0xf0)].Reg_SLong * GeneralRegister[Op1&0xf].Reg_SLong;
                   GeneralRegister[(Op1&0xf0)].Reg_ULong = (ULong)Tmp;
                   GeneralRegister[((Op1+0x10)&0xf0)].Reg_ULong = (ULong)(Tmp>>16);
                   Status.Flags.CarryFlag = Status.Flags.NegativeFlag = GeneralRegister[(Op1&0xf0)].Reg_SLong != 0;
                   Status_Test_ZN_32( GeneralRegister[(Op1&0xf0)].Reg_ULong ,Status );
                 }
#else
#endif
          break;

        case 21|SZ_08:// cmp.b reg,reg
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
          }
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
#else
#endif
          break;
        case 21|SZ_16:// cmp.w reg,reg
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
          }
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
#else
#endif
          break;
        case 21|SZ_32:// cmp.l reg,reg
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
          }
#if Z_ZVCPU_EXPERIMENTAL_ASMCODE==0
#else
#endif
          break;
        case 22:// jmp (reg)
          Op1 = FetchOperand_8(ProgramCounter++); // Register
          ProgramCounter = GeneralRegister[Op1&0xf].Reg_ULargest;
          break;
        case 23:// bra #imm
          Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
          break;
        case 23|64 :// beq #imm
          Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
          if (Status.Flags.ZeroFlag) ProgramCounter += Op1;
          break;
        case 23|128 :// bne #imm
          Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
          if (!Status.Flags.ZeroFlag) ProgramCounter += Op1;
          break;
        case 23|192 : // bcs #imm bhe.u #imm
          Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
          if (Status.Flags.CarryFlag) ProgramCounter += Op1;
          break;
        case 24|000 : // bcc #imm blo.u #imm
          Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
          if (!Status.Flags.CarryFlag) ProgramCounter += Op1;
          break;
        case 24|064 : // bmi #imm
          Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
          if (Status.Flags.NegativeFlag) ProgramCounter += Op1;
          break;
        case 24|128 : // bpl #imm
          Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
          if (!Status.Flags.NegativeFlag) ProgramCounter += Op1;
          break;
        case 24|192 : // bvs #imm
          Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
          if (Status.Flags.OverflowFlag) ProgramCounter += Op1;
          break;
        case 25|000 : // bvc #imm
          Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
          if (!Status.Flags.OverflowFlag) ProgramCounter += Op1;
          break;
        case 25|64 : // bhi.u #imm
          Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
          if (Status.Flags.CarryFlag && (!Status.Flags.ZeroFlag)) ProgramCounter += Op1;
          break;
        case 25|128 : // ble.u #imm
          Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
          if (Status.Flags.CarryFlag && (!Status.Flags.ZeroFlag)) ProgramCounter += Op1;
          break;
        case 25|192 : // bhe.s #imm
          Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
          if (Status.Flags.NegativeFlag == Status.Flags.OverflowFlag) ProgramCounter += Op1;
          break;
        case 26|000 : // blo.s #imm
          Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
          if (Status.Flags.NegativeFlag != Status.Flags.OverflowFlag) ProgramCounter += Op1;
          break;
        case 26|064 : // bhi.s #imm
          Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
          if ((!Status.Flags.ZeroFlag) && (Status.Flags.NegativeFlag == Status.Flags.OverflowFlag)) ProgramCounter += Op1;
          break;
        case 26|128 : // ble.s #imm
          Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
          if ((Status.Flags.ZeroFlag) && (Status.Flags.NegativeFlag != Status.Flags.OverflowFlag)) ProgramCounter += Op1;
          break;
        case 26|192 : // bra #imm
          Op1 = (int)((Short)FetchOperand_16(ProgramCounter));  ProgramCounter+=2; // Register
          ProgramCounter += Op1;
          break;


        case 27:// jsr (reg)
          Op1 = FetchOperand_8(ProgramCounter++); // Register
          PushStack_32(ProgramCounter);
          ProgramCounter = GeneralRegister[Op1&0xf].Reg_ULargest;
          break;
        case 28:// rts
          ProgramCounter = PopStack_32();
          break;
        case 29:// rti
          Status.WholeRegister = PopStack_32();
          ProgramCounter = PopStack_32();
          break;
        case 30:// int reg
        case 31|000:// ext.bw reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
                GeneralRegister[Op1&0xf].Reg_SWord = (Short)GeneralRegister[Op1&0xf].Reg_SByte;

                break;
        case 31|064:// ext.bl reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
                GeneralRegister[Op1&0xf].Reg_SLong = (Short)GeneralRegister[Op1&0xf].Reg_SByte;
                break;
        case 31|128:// ext.wl reg
                Op1 = FetchOperand_8(ProgramCounter++); // Register
                GeneralRegister[Op1&0xf].Reg_SLong = (Short)GeneralRegister[Op1&0xf].Reg_SWord;
                break;
        case 255: return;
      }
    }
  }
};


#endif
