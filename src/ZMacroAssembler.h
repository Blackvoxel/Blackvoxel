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
 * ZMacroAssembler.h
 *
 *  Created on: 26 sept. 2014
 *      Author: laurent
 */

#ifndef Z_MACROASSEMBLER_H
#define Z_MACROASSEMBLER_H

//#ifndef Z_MACROASSEMBLER_H
//#  include "ZMacroAssembler.h"
//#endif

#ifndef Z_ZParser_H
#  include "z/ZParser.h"
#endif

#ifndef Z_ZVCPU_H
#  include "ZVCPU.h"
#endif

#ifndef Z_ZSIMPLEHASH_H
#  include "z/ZSimpleHash.h"
#endif

#ifndef Z_ZGENERICTABLE_H
#  include "z/ZGenericTable.h"
#endif

#ifndef Z_ZSTREAM_SPECIALRAMSTREAM_H
#  include "z/ZStream_SpecialRamStream.h"
#endif

class ZMacroAssembler
{


  class ZLabelStore
  {
    protected:

      struct ZEntry
      {
        ZString Label;
        ULong   Value;
      };

      ZSimpleHash<ZEntry> Hash;
      ZGenericTable<ZEntry, 8192> Table;

      ZEntry * FindEntry(char const * Name)
      {
        ZMemSize FoundHash;
        ZEntry * Entry;

        if ( !(Entry = Hash.FindItem(Name, FoundHash))) return(0);
        do
        {
          if (Entry->Label == Name) { return(Entry); }
        } while ((Entry = Hash.FindNext(FoundHash)));

        return(0);
      }

    public:

      ZLabelStore()
      {
        Hash.Init(14);
      }

      ZLabelStore(ULong SizePowerOfTwo)
      {
        Hash.Init(SizePowerOfTwo);
      }

      void Clear()
      {
        Hash.Clear();
        Table.Clear();
      }

      bool FindLabel(char const * Name, ULong & LabelValue )
      {
        ZEntry * Entry;

        if (!(Entry = FindEntry(Name))) return(false);

        LabelValue = Entry->Value;

        return(true);
      }

      bool StoreLabel(char const * Name, ULong Value)
      {
        ZEntry * Entry;
        ZMemSize Index;

        if ((Entry = FindEntry(Name))) return(false);

        if (!Table.CreateNew(Index)) return(false);
        Entry = Table.GetElement(Index);
        Entry->Label = Name;
        Entry->Value = Value;
        Hash.AddItem(Name,Entry);

        return(true);
      }

  };

  struct AssemblyData
  {
     ZLabelStore LabelStore;
     ZFastKeyWordSet KeyworSet;
     ZParser Parser;
     ZString * Text;
     ULong Address;
     ULong SectionSize;
     ZStream_SpecialRamStream Stream;
     ZMemSize Stream_SectionPos_Save;
     ULong *  Stream_SectionSize_Ptr;
  };

  enum { KW_META_ORG,
         KW_META_DATA,
         KW_OP_MOVE,
         KW_OP_MOVEX,
         KW_OP_PUSHREGS,
         KW_OP_POPREGS,
         KW_OP_ADD,
         KW_OP_SUB,
         KW_OP_AND,
         KW_OP_OR,
         KW_OP_XOR,
         KW_OP_ASL,
         KW_OP_ASR,
         KW_OP_LSL,
         KW_OP_LSR,
         KW_OP_ROL,
         KW_OP_ROR,
         KW_OP_UMUL,
         KW_OP_SMUL,
         KW_OP_CMP,
         KW_OP_JMP,
         KW_OP_JSR,
         KW_OP_BRA,
         KW_OP_BEQ,
         KW_OP_BNE,
         KW_OP_BCS,
         KW_OP_BCC,
         KW_OP_BMI,
         KW_OP_BPL,
         KW_OP_BVS,
         KW_OP_BVC,
         KW_OP_BHI_U,
         KW_OP_BLE_U,
         KW_OP_BHE_S,
         KW_OP_BLO_S,
         KW_OP_BHI_S,
         KW_OP_BLE_S,
         KW_OP_RTS,
         KW_OP_RTI,
         KW_OP_EXT_BW,
         KW_OP_EXT_BL,
         KW_OP_EXT_WL,

         KW_SZ_BYTE,
         KW_SZ_WORD,
         KW_SZ_LONG,
         KW_SZ_UBYTE,
         KW_SZ_SBYTE,
         KW_SZ_UWORD,
         KW_SZ_SWORD,

         KW_REG_R0,
         KW_REG_R1,
         KW_REG_R2,
         KW_REG_R3,
         KW_REG_R4,
         KW_REG_R5,
         KW_REG_R6,
         KW_REG_R7,
         KW_REG_R8,
         KW_REG_R9,
         KW_REG_R10,
         KW_REG_R11,
         KW_REG_R12,
         KW_REG_R13,
         KW_REG_R14,
         KW_REG_R15,


         KW_PNK_SHARP,
         KW_PNK_VIRG,
         KW_PNK_OPENPARENT,
         KW_PNK_CLOSEPARENT,
         KW_PNK_TIRET,
         KW_PNK_TWOPOINTS,
         KW_PNK_ADD,
         KW_PNK_SUB,
         KW_PNK_MUL,
         KW_PNK_DIV,
         KW_PNK_OR,
         KW_PNK_XOR,
         KW_PNK_AND,
         KW_PNK_NOT
       };
  enum { ERROR_EXPECTEDOPERANDSIZE, ERROR_EXPECTEDOPERAND, ERROR_EXPECTEDEXPRESSION, ERROR_EXPECTEDINTEGERVALUE, ERROR_EXPECTEDREGISTER, ERROR_EXPECTEDVIRGULE,
         ERROR_EXPECTEDOPENPARENT, ERROR_EXPECTEDCLOSEPARENT, ERROR_UNEPECTEDSTATMENT, ERROR_EXPECTEDSHARP, ERROR_EXPECTEDTWOPOINTS };


  void RegisterKeywords(ZKeyWordSet * Ks)
  {
    Ks->AddKeyWord(".org",       KW_META_ORG   );
    Ks->AddKeyWord(".data",      KW_META_DATA  );
    Ks->AddKeyWord("move",       KW_OP_MOVE    );
    Ks->AddKeyWord("movex",      KW_OP_MOVEX   );
    Ks->AddKeyWord("pushregs",   KW_OP_PUSHREGS);
    Ks->AddKeyWord("popregs",    KW_OP_POPREGS );
    Ks->AddKeyWord("add",        KW_OP_ADD     );
    Ks->AddKeyWord("sub",        KW_OP_SUB     );
    Ks->AddKeyWord("and",        KW_OP_AND     );
    Ks->AddKeyWord("or",         KW_OP_OR      );
    Ks->AddKeyWord("xor",        KW_OP_XOR     );
    Ks->AddKeyWord("asl",        KW_OP_ASL     );
    Ks->AddKeyWord("asr",        KW_OP_ASR     );
    Ks->AddKeyWord("lsl",        KW_OP_LSL     );
    Ks->AddKeyWord("lsr",        KW_OP_LSR     );
    Ks->AddKeyWord("rol",        KW_OP_ROL     );
    Ks->AddKeyWord("ror",        KW_OP_ROR     );
    Ks->AddKeyWord("umul",       KW_OP_UMUL    );
    Ks->AddKeyWord("smul",       KW_OP_SMUL    );
    Ks->AddKeyWord("cmp",        KW_OP_CMP     );
    Ks->AddKeyWord("jmp",        KW_OP_JMP     );
    Ks->AddKeyWord("bra",        KW_OP_BRA     );
    Ks->AddKeyWord("beq",        KW_OP_BEQ     );
    Ks->AddKeyWord("bne",        KW_OP_BNE     );
    Ks->AddKeyWord("bcs",        KW_OP_BCS     );
    Ks->AddKeyWord("bcc",        KW_OP_BCC     );
    Ks->AddKeyWord("bmi",        KW_OP_BMI     );
    Ks->AddKeyWord("bpl",        KW_OP_BPL     );
    Ks->AddKeyWord("bvs",        KW_OP_BVS     );
    Ks->AddKeyWord("bvc",        KW_OP_BVC     );
    Ks->AddKeyWord("bhi.u",      KW_OP_BHI_U   );
    Ks->AddKeyWord("ble.u",      KW_OP_BLE_U   );
    Ks->AddKeyWord("bhe.s",      KW_OP_BHE_S   );
    Ks->AddKeyWord("blo.s",      KW_OP_BLO_S   );
    Ks->AddKeyWord("bhi.s",      KW_OP_BHI_S   );
    Ks->AddKeyWord("ble.s",      KW_OP_BLE_S   );
    Ks->AddKeyWord("rts",        KW_OP_RTS     );
    Ks->AddKeyWord("rti",        KW_OP_RTI     );
    Ks->AddKeyWord("ext.bw",     KW_OP_EXT_BW  );
    Ks->AddKeyWord("ext.bl",     KW_OP_EXT_BL  );
    Ks->AddKeyWord("ext.wl",     KW_OP_EXT_WL  );

    Ks->AddKeyWord(".b",         KW_SZ_BYTE    );
    Ks->AddKeyWord(".w",         KW_SZ_WORD    );
    Ks->AddKeyWord(".l",         KW_SZ_LONG    );
    Ks->AddKeyWord(".bu",        KW_SZ_UBYTE   );
    Ks->AddKeyWord(".bs",        KW_SZ_SBYTE   );
    Ks->AddKeyWord(".wu",        KW_SZ_UWORD   );
    Ks->AddKeyWord(".ws",        KW_SZ_SWORD   );
    Ks->AddKeyWord("#",          KW_PNK_SHARP  );
    Ks->AddKeyWord(",",          KW_PNK_VIRG   );
    Ks->AddKeyWord("(",          KW_PNK_OPENPARENT);
    Ks->AddKeyWord(")",          KW_PNK_CLOSEPARENT);
    Ks->AddKeyWord("-",          KW_PNK_TIRET  );
    Ks->AddKeyWord(":",          KW_PNK_TWOPOINTS );
    Ks->AddKeyWord("+",          KW_PNK_ADD );
    Ks->AddKeyWord("-",          KW_PNK_SUB );
    Ks->AddKeyWord("*",          KW_PNK_MUL );
    Ks->AddKeyWord("/",          KW_PNK_DIV );

    Ks->AddKeyWord("|",          KW_PNK_OR );
    Ks->AddKeyWord("^",          KW_PNK_XOR );
    Ks->AddKeyWord("&",          KW_PNK_AND );
    Ks->AddKeyWord("!",          KW_PNK_NOT );

    Ks->AddKeyWord("r0",         KW_REG_R0     );
    Ks->AddKeyWord("r1",         KW_REG_R1     );
    Ks->AddKeyWord("r2",         KW_REG_R2     );
    Ks->AddKeyWord("r3",         KW_REG_R3     );
    Ks->AddKeyWord("r4",         KW_REG_R4     );
    Ks->AddKeyWord("r5",         KW_REG_R5     );
    Ks->AddKeyWord("r6",         KW_REG_R6     );
    Ks->AddKeyWord("r7",         KW_REG_R7     );
    Ks->AddKeyWord("r8",         KW_REG_R8     );
    Ks->AddKeyWord("r9",         KW_REG_R9     );
    Ks->AddKeyWord("r10",        KW_REG_R10    );
    Ks->AddKeyWord("r11",        KW_REG_R11    );
    Ks->AddKeyWord("r12",        KW_REG_R12    );
    Ks->AddKeyWord("r13",        KW_REG_R13    );
    Ks->AddKeyWord("r14",        KW_REG_R14    );
    Ks->AddKeyWord("r15",        KW_REG_R15    );


    // Ks->AddKeyWord("")
  }

  void ThrowError(AssemblyData * Asd, ZParser_Token * Token, ULong MessageNum)
  {
     ZString As;

     switch (MessageNum)
     {
       case ERROR_EXPECTEDOPERANDSIZE: As = "Expected operand size : '.b', '.w' or '.l')."; break;
       case ERROR_EXPECTEDOPERAND:     As = "Valid operand expected : '.b', '.w' or '.l')."; break;
       case ERROR_EXPECTEDEXPRESSION:  As = "Expression is expected."; break;
       case ERROR_EXPECTEDINTEGERVALUE:As = "Integer value expected."; break;
       case ERROR_EXPECTEDREGISTER:    As = "Expected register"; break;
       case ERROR_EXPECTEDVIRGULE:     As = "',' separator expected."; break;
       case ERROR_EXPECTEDOPENPARENT:  As = "'(' expected."; break;
       case ERROR_EXPECTEDCLOSEPARENT: As = "')' expected."; break;
       case ERROR_UNEPECTEDSTATMENT:   As = "Unexpected statment."; break;
       case ERROR_EXPECTEDSHARP:       As = "'#' Sharp sign expected."; break;
       case ERROR_EXPECTEDTWOPOINTS:   As = "':' Two Points sign expected."; break;
       default: As = "Unkown Error."; break;
     }
  }

  void BinaryOut_Byte(UByte Code, AssemblyData * Asd)
  {
    Asd->Stream.Put(Code);
    Asd->Address++;
    Asd->SectionSize++;
  }
  void BinaryOut_Word(UShort Code, AssemblyData * Asd)
  {
    Asd->Stream.Put(Code);
    Asd->Address++;
    Asd->SectionSize++;
  }
  void BinaryOut_Long(ULong Code, AssemblyData * Asd)
  {
    Asd->Stream.Put(Code);
    Asd->Address++;
    Asd->SectionSize++;
  }


  bool inline AssembleSection_OperandSize(AssemblyData * Asd, ZParser_Token & Token)
  {
    if (!Asd->Parser.GetToken(Token))                 {ThrowError(Asd, &Token, ERROR_EXPECTEDOPERANDSIZE); return(false);}
    if (Token.TokenType != ZParser_Token::TK_KEYWORD) {ThrowError(Asd, &Token,ERROR_EXPECTEDOPERANDSIZE); return(false);}
    switch(Token.KeyWordId)
    {
      case KW_SZ_BYTE:
      case KW_SZ_WORD:
      case KW_SZ_LONG:
      case KW_SZ_UBYTE:
      case KW_SZ_UWORD:
      case KW_SZ_SBYTE:
      case KW_SZ_SWORD:
                       return(true);
      default:         ThrowError(Asd, &Token,ERROR_EXPECTEDOPERANDSIZE); return(false);
    }
  }

  bool inline GetIdentifierValue(AssemblyData * Asd, ZString & IdentifierName, ULong & Value)
  {
    if (!Asd->LabelStore.FindLabel(IdentifierName.String, Value)) Value = 0;

    return(true);
  }


  bool inline GetExpression_ConstAndIdentifier(AssemblyData * Asd, ULong & ExpressionValue)
  {
     ZParser_Token Token;
     ULong Value;

     Asd->Parser.GetToken(Token);

     switch(Token.TokenType)
     {
       case ZParser_Token::TK_IDENTIFIER: GetIdentifierValue(Asd, Token.AlphaValue, Value);
                                          break;

       case ZParser_Token::TK_CONST:      if (Token.ConstType!= ZParser_Token::CT_INTEGER)  { ThrowError(Asd, &Token, ERROR_EXPECTEDINTEGERVALUE); return(false);}
                                          ExpressionValue = Token.ConstValue.ValULong;
                                          break;
       case ZParser_Token::TK_KEYWORD:    if (Token.KeyWordId != KW_PNK_OPENPARENT)         { ThrowError(Asd, &Token, KW_PNK_OPENPARENT); return(false); }
                                          GetExpression(Asd, ExpressionValue);
                                          if (!ExpectToken(Asd, KW_PNK_OPENPARENT, KW_PNK_CLOSEPARENT)) return(false);
                                          break;


       default: ThrowError(Asd, &Token, ERROR_EXPECTEDEXPRESSION); return(false); break;
     }
     return(true);
   }

  bool inline GetExpression_MultDiv(AssemblyData * Asd, ULong & ExpressionValue)
  {
    ZParser_Token Token;
    ZParser_State Pos;
    ULong Value;

    if (!GetExpression_ConstAndIdentifier(Asd, ExpressionValue)) return(false);

    while(true)
    {
      Asd->Parser.SavePos(Pos);
      Asd->Parser.GetToken(Token);
      if (Token.TokenType != ZParser_Token::TK_KEYWORD) { Asd->Parser.RestorePos(Pos); return(true); }
      switch(Token.KeyWordId)
      {
        case KW_PNK_ADD: if (!GetExpression_ConstAndIdentifier(Asd, Value)) return(false);
                         ExpressionValue += Value;
                         break;
        case KW_PNK_SUB: if (!GetExpression_ConstAndIdentifier(Asd, Value)) return(false);
                         ExpressionValue -= Value;
                         break;
        default:         Asd->Parser.RestorePos(Pos); return(true);
      }
    }

  }

  bool inline GetExpression_AddSub(AssemblyData * Asd, ULong & ExpressionValue)
  {
    ZParser_Token Token;
    ZParser_State Pos;
    ULong Value;

    if (!GetExpression_MultDiv(Asd, ExpressionValue)) return(false);

    while(true)
    {
      Asd->Parser.SavePos(Pos);
      Asd->Parser.GetToken(Token);
      if (Token.TokenType != ZParser_Token::TK_KEYWORD) { Asd->Parser.RestorePos(Pos); return(true); }
      switch(Token.KeyWordId)
      {
        case KW_PNK_ADD: if (!GetExpression_MultDiv(Asd, Value)) return(false);
                         ExpressionValue += Value;
                         break;
        case KW_PNK_SUB: if (!GetExpression_MultDiv(Asd, Value)) return(false);
                         ExpressionValue -= Value;
                         break;
        default:         Asd->Parser.RestorePos(Pos); return(true);
      }
    }

  }
  bool inline GetExpression_OrXorAnd(AssemblyData * Asd, ULong & ExpressionValue)
  {
    ZParser_Token Token;
    ZParser_State Pos;
    ULong Value;

    if (!GetExpression_AddSub(Asd, ExpressionValue)) return(false);

    while(true)
    {
      Asd->Parser.SavePos(Pos);
      Asd->Parser.GetToken(Token);
      if (Token.TokenType != ZParser_Token::TK_KEYWORD) { Asd->Parser.RestorePos(Pos); return(true); }
      switch(Token.KeyWordId)
      {
        case KW_PNK_OR:  if (!GetExpression_AddSub(Asd, Value)) return(false);
                         ExpressionValue |= Value;
                         break;
        case KW_PNK_XOR: if (!GetExpression_AddSub(Asd, Value)) return(false);
                         ExpressionValue ^= Value;
                         break;
        case KW_PNK_AND: if (!GetExpression_AddSub(Asd, Value)) return(false);
                         ExpressionValue &= Value;
                         break;

        default:         Asd->Parser.RestorePos(Pos); return(true);
      }
    }

  }



  bool inline GetExpression(AssemblyData * Asd, ULong & ExpressionValue)
  {
    return(GetExpression_OrXorAnd(Asd, ExpressionValue));
  }

  bool inline ExpectToken(AssemblyData * Asd, ULong Opcode, ULong ErrorCase)
  {
    ZParser_Token Token;

    // Fetch Token

    Asd->Parser.GetToken(Token);

    // Ensure token is what's expected

    if ( Token.TokenType != ZParser_Token::TK_KEYWORD || Token.KeyWordId != Opcode )
    {
      ThrowError(Asd, &Token, ErrorCase);
      return(false);
    }

    return(true);
  }

  bool inline ExpectRegisterOnToken(AssemblyData * Asd, ZParser_Token & Token_Register, ULong * Register)
  {
    // Ensure token is a register number.

    Asd->Parser.GetToken(Token_Register);
    if (   Token_Register.TokenType != ZParser_Token::TK_KEYWORD
        || Token_Register.KeyWordId < KW_REG_R0
        || Token_Register.KeyWordId > KW_REG_R15 )
    {
      ThrowError(Asd, &Token_Register, ERROR_EXPECTEDREGISTER);
      return(false);
    }

    // Convert token to register number

    *Register = Token_Register.KeyWordId - KW_REG_R0;

    return(true);
  }

  bool inline ExpectRegister(AssemblyData * Asd, ULong * Register)
  {
    ZParser_Token Token_Register;

    // Fetch Token

    Asd->Parser.GetToken(Token_Register);

    // Parse Token and return result

    return( ExpectRegisterOnToken(Asd, Token_Register, Register) );

  }



  bool inline AssembleSection_Token_Move(AssemblyData * Asd)
  {
    ZParser_Token Token_Size, Token_Arg, Token_Register,Token_Register2, Token_Separator;

    ULong ImmediateValue;
    ULong RegisterNumber, RegisterNumber2;

    if (!AssembleSection_OperandSize(Asd,Token_Size)) return(false);

    Asd->Parser.GetToken(Token_Arg); if (Token_Arg.TokenType != ZParser_Token::TK_KEYWORD) {ThrowError(Asd, &Token_Arg, ERROR_EXPECTEDOPERAND); return(false);}
    switch(Token_Arg.KeyWordId)
    {
      case KW_PNK_SHARP: if (!GetExpression(Asd, ImmediateValue)) return(false);
                         if (!ExpectToken(Asd, KW_PNK_VIRG, ERROR_EXPECTEDVIRGULE)) return(false);
                         if (!ExpectRegister(Asd, &RegisterNumber)) return(false);

                         switch(Token_Size.KeyWordId)
                         {
                           case KW_SZ_BYTE: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_IMM_B, Asd );
                                            BinaryOut_Byte(RegisterNumber, Asd);
                                            BinaryOut_Byte(ImmediateValue, Asd);
                                            break;
                           case KW_SZ_WORD: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_IMM_W, Asd );
                                            BinaryOut_Byte(RegisterNumber, Asd);
                                            BinaryOut_Word(ImmediateValue, Asd);
                                            break;
                           case KW_SZ_LONG: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_IMM_L, Asd );
                                            BinaryOut_Byte(RegisterNumber, Asd);
                                            BinaryOut_Long(ImmediateValue, Asd);
                                            break;
                           default:         ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);
                         }
                         break;

       case KW_REG_R0:
       case KW_REG_R1:
       case KW_REG_R2:
       case KW_REG_R3:
       case KW_REG_R4:
       case KW_REG_R5:
       case KW_REG_R6:
       case KW_REG_R7:
       case KW_REG_R8:
       case KW_REG_R9:
       case KW_REG_R10:
       case KW_REG_R11:
       case KW_REG_R12:
       case KW_REG_R13:
       case KW_REG_R14:
       case KW_REG_R15:  RegisterNumber = Token_Arg.KeyWordId - KW_REG_R0;
                         if (!ExpectToken(Asd, KW_PNK_VIRG, ERROR_EXPECTEDVIRGULE)) return(false);
                         Asd->Parser.GetToken(Token_Register2); if (Token_Register2.TokenType != ZParser_Token::TK_KEYWORD) {ThrowError(Asd, &Token_Register2, ERROR_UNEPECTEDSTATMENT); return(false);}
                         if (Token_Register2.KeyWordId == KW_PNK_OPENPARENT)
                         {
                           if (!ExpectRegister(Asd, &RegisterNumber2)) return(false);
                           if (!ExpectToken(Asd, KW_PNK_CLOSEPARENT, ERROR_EXPECTEDCLOSEPARENT)) return(false);

                           switch(Token_Size.KeyWordId)
                           {
                             case KW_SZ_BYTE: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_IND_B, Asd);
                                              BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                              break;
                             case KW_SZ_WORD: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_IND_W, Asd);
                                              BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                              break;
                             case KW_SZ_LONG: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_IND_L, Asd);
                                              BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                              break;
                             default:         ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);
                           }
                         }
                         else if (Token_Register2.KeyWordId >= KW_REG_R0 && Token_Register2.KeyWordId <= KW_REG_R15)
                         {
                           RegisterNumber2 = Token_Register2.KeyWordId - KW_REG_R0;
                           switch(Token_Size.KeyWordId)
                           {
                             case KW_SZ_BYTE: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_REG_B, Asd);
                                              BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                              break;
                             case KW_SZ_WORD: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_REG_B, Asd);
                                              BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                              break;
                             case KW_SZ_LONG: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_REG_B, Asd);
                                              BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                              break;
                             default:         ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);
                           }
                         }
                         else {ThrowError(Asd, &Token_Register, ERROR_UNEPECTEDSTATMENT); return(false);}
                         break;

      case KW_PNK_OPENPARENT:
                         if (Token_Register.KeyWordId < KW_REG_R0 || Token_Register.KeyWordId > KW_REG_R15) {ThrowError(Asd, &Token_Register, ERROR_EXPECTEDREGISTER); return(false); }
                         RegisterNumber = Token_Register.KeyWordId - KW_REG_R0;
                         if (!ExpectToken(Asd, KW_PNK_CLOSEPARENT, ERROR_EXPECTEDCLOSEPARENT)) return(false);
                         if (!ExpectToken(Asd, KW_PNK_VIRG, ERROR_EXPECTEDVIRGULE)) return(false);
                         if (!ExpectRegister(Asd, &RegisterNumber2)) return(false);

                         switch(Token_Size.KeyWordId)
                         {
                           case KW_SZ_BYTE: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_IND_B, Asd);
                                            BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                            break;
                           case KW_SZ_WORD: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_IND_W, Asd);
                                            BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                            break;
                           case KW_SZ_LONG: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_IND_L, Asd);
                                            BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                            break;
                           default:         ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);
                         }
                         break;

      default: ThrowError(Asd, &Token_Arg, ERROR_EXPECTEDOPERAND); return(false);

    }

    return(true);
  }

  bool inline AssembleSection_Token_Movex(AssemblyData * Asd)
  {
    ZParser_Token Token_Size, Token_Separator, Token_Register;
    ULong ImmediateValue, Register;

    // .x size

    if (!AssembleSection_OperandSize(Asd,Token_Size)) return(false);

    // # sharp sign.

    if (!ExpectToken(Asd, KW_PNK_SHARP, ERROR_EXPECTEDSHARP)) return(false);

    // Immediate constant expression.

    if (!GetExpression(Asd, ImmediateValue)) return(false);

    // ',' separator

    if (!ExpectToken(Asd, KW_PNK_VIRG, ERROR_EXPECTEDVIRGULE)) return(false);

    // Destination register.

    if (!ExpectRegister(Asd, &Register)) return(false);

    // Code output

    switch(Token_Size.KeyWordId)
    {
      case KW_SZ_BYTE:
      case KW_SZ_UBYTE: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVEX_IMM_UB, Asd);
                        BinaryOut_Byte( Register, Asd);
                        BinaryOut_Byte(ImmediateValue, Asd);
                        break;
      case KW_SZ_SBYTE: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVEX_IMM_SB, Asd);
                        BinaryOut_Byte( Register, Asd);
                        BinaryOut_Byte(ImmediateValue, Asd);
                        break;
      case KW_SZ_WORD:
      case KW_SZ_UWORD: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVEX_IMM_UW, Asd);
                        BinaryOut_Byte( Register, Asd);
                        BinaryOut_Byte(ImmediateValue, Asd);
                        break;
      case KW_SZ_SWORD: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVEX_IMM_SW, Asd);
                        BinaryOut_Byte( Register, Asd);
                        BinaryOut_Byte(ImmediateValue, Asd);
                        break;
      default:          ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);

    }
    return(true);
  }

  bool inline AssembleSection_Token_PushPopRegs(AssemblyData * Asd, ULong Opcode)
  {
    ZParser_Token Token_Separator, Token_Register, Token;
    ULong  RegisterMask, RegisterNum, RegisterNum2,Tmp,i;
    ZParser_State Pos;


    RegisterMask = 0;
    do
    {
      if (!ExpectRegister( Asd, &RegisterNum )) return(false);

      Asd->Parser.SavePos(Pos);
      Asd->Parser.GetToken(Token);
      if ( Token.TokenType == ZParser_Token::TK_KEYWORD && ( Token.KeyWordId == KW_PNK_TIRET )  )
      {
        if (!ExpectRegister( Asd, &RegisterNum2 )) return(false);
        if (RegisterNum>RegisterNum2) {Tmp = RegisterNum; RegisterNum = RegisterNum2; RegisterNum2 = Tmp; }
        for (i=RegisterNum; i<=RegisterNum2;i++) RegisterMask |= 1 << i;
        Asd->Parser.SavePos(Pos);
        Asd->Parser.GetToken(Token);
      }

    } while ( Token_Register.TokenType != ZParser_Token::TK_KEYWORD || Token_Register.KeyWordId < KW_PNK_VIRG);

    Asd->Parser.RestorePos(Pos);

    BinaryOut_Byte(Opcode, Asd);
    BinaryOut_Word(RegisterMask, Asd);

    return(true);
  }

  bool inline AssembleSection_Token_OneParameterAritmetic(AssemblyData * Asd, ULong Opcode, bool Size=true)
  {
    ZParser_Token Token_Size, Token_Register;
    ULong DestinationRegister;

    // .x size

    if (Size)
    {
      if (!AssembleSection_OperandSize(Asd,Token_Size)) return(false);
    }
    else Token_Size.KeyWordId = KW_SZ_BYTE;

    // Destination register.

    if (!ExpectRegister( Asd, &DestinationRegister )) return(false);

    // Code output

    switch(Token_Size.KeyWordId)
    {
      case KW_SZ_BYTE:  BinaryOut_Byte(Opcode, Asd);
                        BinaryOut_Byte(DestinationRegister , Asd);
                        break;
      case KW_SZ_WORD:  BinaryOut_Byte(Opcode+64, Asd);
                        BinaryOut_Byte(DestinationRegister , Asd);
                        break;
      case KW_SZ_LONG:  BinaryOut_Byte(Opcode+128, Asd);
                        BinaryOut_Byte(DestinationRegister , Asd);
                        break;
      default:          ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);
    }

    return(true);
  }

  bool inline AssembleSection_Token_TwoParameterAritmetic(AssemblyData * Asd, ULong Opcode, bool Size=true)
  {
    ZParser_Token Token_Size, Token_Separator, Token_Register;
    ULong SourceRegister, DestinationRegister;

    // .x size

    if (Size)
    {
      if (!AssembleSection_OperandSize(Asd,Token_Size)) return(false);
    }
    else Token_Size.KeyWordId = KW_SZ_BYTE;

    // Source register.

    Asd->Parser.GetToken(Token_Register);
    if (   Token_Register.TokenType != ZParser_Token::TK_KEYWORD
        || Token_Register.KeyWordId < KW_REG_R0
        || Token_Register.KeyWordId > KW_REG_R15                ) { ThrowError(Asd, &Token_Register, ERROR_EXPECTEDREGISTER); return(false); }
    SourceRegister = Token_Register.KeyWordId - KW_REG_R0;

    // ',' separator

    Asd->Parser.GetToken(Token_Separator); if (Token_Separator.TokenType != ZParser_Token::TK_KEYWORD || Token_Separator.KeyWordId != KW_PNK_VIRG) {ThrowError(Asd, &Token_Separator, ERROR_EXPECTEDVIRGULE); return(false);}

    // Destination register.

    Asd->Parser.GetToken(Token_Register);
    if (   Token_Register.TokenType != ZParser_Token::TK_KEYWORD
        || Token_Register.KeyWordId < KW_REG_R0
        || Token_Register.KeyWordId > KW_REG_R15                ) { ThrowError(Asd, &Token_Register, ERROR_EXPECTEDREGISTER); return(false); }
    DestinationRegister = Token_Register.KeyWordId - KW_REG_R0;

    // Code output

    switch(Token_Size.KeyWordId)
    {
      case KW_SZ_BYTE:  BinaryOut_Byte(Opcode, Asd);
                        BinaryOut_Byte(SourceRegister & (DestinationRegister << 4) , Asd);
                        break;
      case KW_SZ_WORD:  BinaryOut_Byte(Opcode+64, Asd);
                        BinaryOut_Byte(SourceRegister & (DestinationRegister << 4) , Asd);
                        break;
      case KW_SZ_LONG:  BinaryOut_Byte(Opcode+128, Asd);
                        BinaryOut_Byte(SourceRegister & (DestinationRegister << 4) , Asd);
                        break;
      default:          ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);
    }

    return(true);
  }




  bool inline AssembleSection_Token_IndirectJump(AssemblyData * Asd, ULong Opcode)
  {
    ZParser_Token Token_Size, Token_Separator, Token_Register;
    ULong Register;

    // Open parenthesis

    if (!ExpectToken(Asd, KW_PNK_OPENPARENT, ERROR_EXPECTEDOPENPARENT)) return(false);

    // Source register.

    if (!ExpectRegister(Asd, &Register)) return(false);

    // Close parenthesis

    if (!ExpectToken(Asd, KW_PNK_CLOSEPARENT, ERROR_EXPECTEDCLOSEPARENT)) return(false);

    // Code output

    BinaryOut_Byte(Opcode, Asd);
    BinaryOut_Byte(Register , Asd);

    return(true);
  }

  bool inline AssembleSection_Token_ConditionalBranch(AssemblyData * Asd, ULong Opcode)
  {
    ULong ImmediateValue;

    // Immediate constant expression.

    if (!GetExpression(Asd, ImmediateValue)) return(false);

    // Code output

    BinaryOut_Byte(Opcode, Asd);
    BinaryOut_Word( ImmediateValue, Asd);

    return(true);
  }

  bool inline AssembleSection_Token_SingleInstruction(AssemblyData * Asd, ULong Opcode)
  {
    // Code output

    BinaryOut_Byte(Opcode, Asd);

    return(true);
  }

  bool inline AssembleSection_Token_Org(AssemblyData * Asd)
  {
    ULong Value;

    if (!GetExpression(Asd, Value)) return(false);

    CloseSection(Asd);
    OpenNewSection(Asd, MulticharConst('C','O','D','E') , Value);

    return(true);
  }

  bool inline AssembleSection_Token_Data(AssemblyData * Asd)
  {
    ZParser_Token Token_Size, Token_Separator;
    ULong Value;

    // .x size

    if (!AssembleSection_OperandSize(Asd,Token_Size)) return(false);

    // Open Parent

    if (!ExpectToken(Asd, KW_PNK_OPENPARENT, ERROR_EXPECTEDOPENPARENT)) return(false);

    // Data

    do
    {
      if (!GetExpression(Asd, Value)) return(false);

      Asd->Parser.GetToken(Token_Separator);

      switch(Token_Size.KeyWordId)
      {
        case KW_SZ_BYTE: BinaryOut_Byte(Value, Asd );    break;
        case KW_SZ_WORD: BinaryOut_Word(Value, Asd );    break;
        case KW_SZ_LONG: BinaryOut_Long(Value, Asd );    break;
        default:         ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);
      }

      if ( Token_Separator.TokenType != ZParser_Token::TK_KEYWORD ) { ThrowError(Asd, &Token_Separator, ERROR_EXPECTEDCLOSEPARENT); return(false); }
    } while(Token_Separator.KeyWordId == KW_PNK_VIRG);

    if (Token_Separator.KeyWordId != KW_PNK_CLOSEPARENT) { ThrowError(Asd, &Token_Separator, ERROR_EXPECTEDCLOSEPARENT); return(false); }

    return(true);
  }

  bool inline AssembleSection_Token(AssemblyData * Asd, ZParser_Token &Token)
  {
    switch(Token.KeyWordId)
    {
      case KW_META_ORG:      AssembleSection_Token_Org(Asd);                                              break;
      case KW_META_DATA:     AssembleSection_Token_Data(Asd);                                             break;
      case KW_OP_MOVE:       AssembleSection_Token_Move(Asd);                                             break;
      case KW_OP_MOVEX:      AssembleSection_Token_Movex(Asd);                                            break;
      case KW_OP_PUSHREGS:   AssembleSection_Token_PushPopRegs(Asd, BlackCPU<int>::OPCODE_PUSHREGS);           break;
      case KW_OP_POPREGS:    AssembleSection_Token_PushPopRegs(Asd, BlackCPU<int>::OPCODE_POPREGS);            break;
      case KW_OP_ADD:        AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_ADD_B );   break;
      case KW_OP_SUB:        AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_SUB_B );   break;
      case KW_OP_AND:        AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_AND_B );   break;
      case KW_OP_OR:         AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_OR_B  );   break;
      case KW_OP_XOR:        AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_XOR_B );   break;
      case KW_OP_ASL:        AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_ASL_B );   break;
      case KW_OP_ASR:        AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_ASR_B );   break;
      case KW_OP_LSL:        AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_LSL_B );   break;
      case KW_OP_LSR:        AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_LSR_B );   break;
      case KW_OP_ROL:        AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_ROL_B );   break;
      case KW_OP_ROR:        AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_ROR_B );   break;
      case KW_OP_UMUL:       AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_UMUL_B );  break;
      case KW_OP_SMUL:       AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_SMUL_B );  break;
      case KW_OP_CMP:        AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_CMP_B );   break;
      case KW_OP_JMP:        AssembleSection_Token_IndirectJump(Asd, BlackCPU<int>::OPCODE_JMP_IND );          break;
      case KW_OP_JSR:        AssembleSection_Token_IndirectJump(Asd, BlackCPU<int>::OPCODE_JSR_IND );          break;

      case KW_OP_BRA:        AssembleSection_Token_ConditionalBranch(Asd, BlackCPU<int>::OPCODE_BRANCH_BRA ); break;
      case KW_OP_BEQ:        AssembleSection_Token_ConditionalBranch(Asd, BlackCPU<int>::OPCODE_BRANCH_BEQ ); break;
      case KW_OP_BNE:        AssembleSection_Token_ConditionalBranch(Asd, BlackCPU<int>::OPCODE_BRANCH_BNE ); break;
      case KW_OP_BCS:        AssembleSection_Token_ConditionalBranch(Asd, BlackCPU<int>::OPCODE_BRANCH_BCS ); break;
      case KW_OP_BCC:        AssembleSection_Token_ConditionalBranch(Asd, BlackCPU<int>::OPCODE_BRANCH_BCC ); break;
      case KW_OP_BMI:        AssembleSection_Token_ConditionalBranch(Asd, BlackCPU<int>::OPCODE_BRANCH_BMI ); break;
      case KW_OP_BPL:        AssembleSection_Token_ConditionalBranch(Asd, BlackCPU<int>::OPCODE_BRANCH_BPL ); break;
      case KW_OP_BVS:        AssembleSection_Token_ConditionalBranch(Asd, BlackCPU<int>::OPCODE_BRANCH_BVS ); break;
      case KW_OP_BVC:        AssembleSection_Token_ConditionalBranch(Asd, BlackCPU<int>::OPCODE_BRANCH_BVC ); break;
      case KW_OP_BHI_U:      AssembleSection_Token_ConditionalBranch(Asd, BlackCPU<int>::OPCODE_BRANCH_BHI_U);break;
      case KW_OP_BLE_U:      AssembleSection_Token_ConditionalBranch(Asd, BlackCPU<int>::OPCODE_BRANCH_BLE_U);break;
      case KW_OP_BHE_S:      AssembleSection_Token_ConditionalBranch(Asd, BlackCPU<int>::OPCODE_BRANCH_BHE_S);break;
      case KW_OP_BLO_S:      AssembleSection_Token_ConditionalBranch(Asd, BlackCPU<int>::OPCODE_BRANCH_BLO_S);break;
      case KW_OP_BHI_S:      AssembleSection_Token_ConditionalBranch(Asd, BlackCPU<int>::OPCODE_BRANCH_BHI_S);break;
      case KW_OP_BLE_S:      AssembleSection_Token_ConditionalBranch(Asd, BlackCPU<int>::OPCODE_BRANCH_BLE_S);break;

      case KW_OP_RTS:        AssembleSection_Token_SingleInstruction(Asd, BlackCPU<int>::OPCODE_RTS);         break;
      case KW_OP_RTI:        AssembleSection_Token_SingleInstruction(Asd, BlackCPU<int>::OPCODE_RTI);         break;
      case KW_OP_EXT_BW:     AssembleSection_Token_OneParameterAritmetic(Asd, BlackCPU<int>::OPCODE_EXT_BW);  break;
      case KW_OP_EXT_BL:     AssembleSection_Token_OneParameterAritmetic(Asd, BlackCPU<int>::OPCODE_EXT_BL);  break;
      case KW_OP_EXT_WL:     AssembleSection_Token_OneParameterAritmetic(Asd, BlackCPU<int>::OPCODE_EXT_WL);  break;

      default: break;
    }
    return(true);
  }

  bool inline AssembleSection_Label(AssemblyData * Asd, ZParser_Token &Token)
  {
    ZParser_Token Token_Separator;

    // Separator :

    Asd->Parser.GetToken(Token_Separator);
    if ( Token_Separator.TokenType != ZParser_Token::TK_KEYWORD || Token_Separator.KeyWordId != KW_PNK_TWOPOINTS ) {ThrowError(Asd, &Token_Separator, ERROR_EXPECTEDTWOPOINTS); return(false); }

    Asd->LabelStore.StoreLabel(Token.AlphaValue.String, Asd->Address);

    return(true);
  }

  bool OpenNewSection(AssemblyData * Asd, ULong SectionType, ULong Origin_Address)
  {
    Asd->Address = Origin_Address;
    Asd->SectionSize = 0;
    Asd->Stream_SectionPos_Save = Asd->Stream.GetPos();
    Asd->Stream.Put( SectionType ); // MulticharConst('C','O','D','E')
    Asd->Stream.Put( Origin_Address );
    Asd->Stream_SectionSize_Ptr = Asd->Stream.GetPointer_ULong();
    Asd->Stream.Put( 0 );

    return(true);
  }

  bool CloseSection(AssemblyData * Asd)
  {

    // If section size is 0, delete it.

    if (Asd->SectionSize == 0)
    {
      Asd->Stream.SetPos(Asd->Stream_SectionPos_Save);
    }
    else
    {
      *Asd->Stream_SectionSize_Ptr = Asd->SectionSize;
    }

    return(true);
  }

  bool AssembleSection(AssemblyData * Asd)
  {
    ZParser_Token Token;

    while( Asd->Parser.GetToken(Token) )
    {
      switch(Token.TokenType)
      {
        case ZParser_Token::TK_KEYWORD: AssembleSection_Token(Asd, Token); break;

        case ZParser_Token::TK_IDENTIFIER: AssembleSection_Label(Asd, Token); break;
        default: break;

      }

    }
    return(true);
  }

  public:

  bool Assembly_Pass(AssemblyData * Asd)
  {
    bool Result;
    ULong * TotalSize;

    Asd->Parser.Init(Asd->Text->String, Asd->Text->Len, &Asd->KeyworSet);
    Asd->Address = 0;
    Asd->Stream.OpenWrite();
    Asd->Stream.Put(MulticharConst('Z','B','I','N'));
    TotalSize = Asd->Stream.GetPointer_ULong();
    Asd->Stream.Put((ULong)0);
      OpenNewSection(Asd, MulticharConst('C','O','D','E') ,0);
        Result = AssembleSection(Asd);
      CloseSection(Asd);
    *TotalSize = Asd->Stream.GetActualBufferLen() - 8;
    return(Result);
  }

  bool Assemble(ZString & Text,ZString & Output)
  {
    AssemblyData AsData, * Asd;
    ZStream_SpecialRamStream Stream;

    // End test

    Asd = &AsData;

    Asd->LabelStore.Clear();
    Asd->Text = &Text;
    RegisterKeywords(&Asd->KeyworSet);

    // Pass 1 : Fill the label table.

    if (!Assembly_Pass(Asd)) return(false);

    // Pass 2 : Final pass with right label values.

    if (!Assembly_Pass(Asd)) return(false);

    // Output

    Asd->Stream.BufferToString(Output);

    return(true);
  }


};


#endif /* Z_MACROASSEMBLER_H */
