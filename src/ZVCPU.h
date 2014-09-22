#ifndef Z_ZVCPU_H
#define Z_ZVCPU_H

//#ifndef Z_ZVCPU_H
//#  include "zvcpu.h"
//#endif


#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

unsigned char optable[]=
{
  1,0,
  255,0
};

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


class BlackCPU
{
  public:
  
  ULong GeneralRegister[16];
  ULong ProgramCounter;
  UShort StatusRegister;
  
  
  BlackCPU()
  {
    unsigned long i;

    for(i=0;i<16;i++) GeneralRegister[i] = 0;
    ProgramCounter = 0;
    StatusRegister = 0;
  }

  inline UByte FetchOpcode(ULong ProgramCounter)
  {
    return(optable[ProgramCounter]);
  }
  
  inline UByte FetchOperand_b(ULong ProgramCounter)
  {
    return(optable[ProgramCounter]);
  }

  inline UByte FetchData_b(ULong ProgramCounter)
  {
    return(optable[ProgramCounter]);
  }

  void PrintStatus(ULong StatusFlag)
  {
    StatusRegister_x86 * s = (StatusRegister_x86 *)&StatusFlag;
    printf("Carry Flag : %s\n", s->CarryFlag ? "1" : "0");
    printf("Zero Flag  : %s\n", s->ZeroFlag ? "1" : "0");
    printf("Sign Flag  : %s\n", s->SignFlag ? "1" : "0");
    printf("Over Flag  : %s\n", s->OverflowFlag ? "1" : "0"); 
  }


  void Execute(ULong CycleCount)
  {
    ULong ElapsedCycles;
    register UByte Opcode;
    register int Op1;
    register int Op2;
    register ULong Status;
    
    Status = StatusRegister;
    
    ElapsedCycles =  0;
    
    while(ElapsedCycles < CycleCount)
    {
      Opcode = FetchOpcode(ProgramCounter++);
      
      switch(Opcode)
      {
        case 1: // move.x #imm,reg
                   Op1 = FetchOperand_b(ProgramCounter++) & 0x0f; // Register
                   Op2 = FetchOperand_b(ProgramCounter++);        // Immediate value
                   
                   asm volatile 
                   ( 
                     "test %2,%2;"
                     "pushf;"
                     "popl %1;"
                     "movb %b2,%b0;"
                     : "=m" (GeneralRegister[Op1]), "=r" (Status)
                     : "r" (Op2)
                     :
                   );
                   PrintStatus(Status);


                   
        case 2: // movef.x #8imm,reg
        case 3: // move.x (reg),reg
        case 4: // move.x reg,reg
        case 5: // moveregs.x (reg)
        case 6: // add.x reg,reg  ??? carry ?
        case 7: // sub.x reg,reg  ??? carry ?
        case 8: // and.x reg,reg
        case 9: // or.x  reg,reg
        case 10: // xor.x reg.reg
        case 11: // asl.x reg,reg
        case 12: // asr.x reg,reg
        case 13:// lsl.x reg,reg
        case 14:// lsr.x reg,reg
        case 15:// rol.x reg,reg
        case 16:// ror.x reg,reg
        case 17:// umul.x reg,reg
        case 18:// smul.x reg,reg
        case 19:// cmp.x reg,reg
        case 20:// jmp (reg)
        case 21:// bxx #imm
        case 22:// jsr (reg)
        case 23:// rts
        case 24:// rti
        case 25:// int reg
        case 26:// ext.x reg
        case 255: return;
      }
    }
  }
};


#endif
