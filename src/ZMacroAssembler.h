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


class ZMacroAssembler
{
  struct AssemblyData
  {
     ZFastKeyWordSet KeyworSet;
     ZParser Parser;
     ZString * Text;
     ULong Address;
  };

  enum { KW_META_ORG,
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
         KW_OP_BRA,

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
         KW_PNK_CLOSEPARENT
       };
  enum { ERROR_EXPECTEDOPERANDSIZE, ERROR_EXPECTEDOPERAND, ERROR_EXPECTEDEXPRESSION, ERROR_EXPECTEDINTEGERVALUE, ERROR_EXPECTEDREGISTER, ERROR_EXPECTEDVIRGULE, ERROR_EXPECTEDCLOSEPARENT, ERROR_UNEPECTEDSTATMENT,
         ERROR_EXPECTEDSHARP};


  void RegisterKeywords(ZKeyWordSet * Ks)
  {
    Ks->AddKeyWord(".org",       KW_META_ORG   );
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
       case ERROR_EXPECTEDCLOSEPARENT: As = "')' expected."; break;
       case ERROR_UNEPECTEDSTATMENT:   As = "Unexpected statment."; break;
       case ERROR_EXPECTEDSHARP:       As = "'#' Sharp sign expected."; break;
       default: As = "Unkown Error."; break;
     }
  }

  void BinaryOut_Byte(UByte Code, AssemblyData * Asd)
  {

  }
  void BinaryOut_Word(UShort Code, AssemblyData * Asd)
  {

  }
  void BinaryOut_Long(ULong Code, AssemblyData * Asd)
  {

  }


  bool inline AssembleSection_OperandSize(AssemblyData * Asd, ZParser_Token & Token)
  {
    if (!Asd->Parser.GetToken(Token))                 {ThrowError(Asd, &Token, ERROR_EXPECTEDOPERANDSIZE); return(false);}
    if (Token.TokenType != ZParser_Token::TK_KEYWORD) {ThrowError(Asd, &Token,ERROR_EXPECTEDOPERANDSIZE); return(false);}
    switch(Token.KeyWordId)
    {
      case KW_SZ_BYTE:
      case KW_SZ_WORD:
      case KW_SZ_LONG: return(true);
      default:         ThrowError(Asd, &Token,ERROR_EXPECTEDOPERANDSIZE); return(false);
    }
  }

  bool inline GetIdentifierValue(ZString & IdentifierName, ULong & Value)
  {
    Value = 0;
    return(true);
  }

  bool inline GetExpression(AssemblyData * Asd, ULong & ExpressionValue)
  {
    ZParser_Token Token;
    ULong Value;

    Asd->Parser.GetToken(Token);

    switch(Token.TokenType)
    {
      case ZParser_Token::TK_IDENTIFIER: GetIdentifierValue(Token.AlphaValue, Value); break;

      case ZParser_Token::TK_CONST:      if (Token.ConstType!= ZParser_Token::CT_INTEGER) { ThrowError(Asd, &Token, ERROR_EXPECTEDINTEGERVALUE); return(false); }
                                         ExpressionValue = Token.ConstValue.ValULong;
                                         break;

      default: ThrowError(Asd, &Token, ERROR_EXPECTEDEXPRESSION); return(false); break;
    }
    return(true);
  }

  bool inline AssembleSection_Token_Move(AssemblyData * Asd)
  {
    ZParser_Token Token_Size, Token_Arg, Token_Register,Token_Register2, Token_Separator;

    ULong ImmediateValue;
    UByte RegisterNumber, RegisterNumber2;

    if (!AssembleSection_OperandSize(Asd,Token_Size)) return(false);

    Asd->Parser.GetToken(Token_Arg); if (Token_Arg.TokenType != ZParser_Token::TK_KEYWORD) {ThrowError(Asd, &Token_Arg, ERROR_EXPECTEDOPERAND); return(false);}
    switch(Token_Arg.KeyWordId)
    {
      case KW_PNK_SHARP: if (!GetExpression(Asd, ImmediateValue)) return(false);
                         Asd->Parser.GetToken(Token_Separator); if (Token_Separator.TokenType != ZParser_Token::TK_KEYWORD || Token_Separator.KeyWordId != KW_PNK_VIRG) {ThrowError(Asd, &Token_Register, ERROR_EXPECTEDVIRGULE); return(false);}
                         Asd->Parser.GetToken(Token_Register); if (Token_Register.TokenType != ZParser_Token::TK_KEYWORD) {ThrowError(Asd, &Token_Register, ERROR_EXPECTEDREGISTER); return(false);}
                         if (Token_Register.KeyWordId < KW_REG_R0 || Token_Register.KeyWordId > KW_REG_R15) {ThrowError(Asd, &Token_Register, ERROR_EXPECTEDREGISTER); return(false); }
                         RegisterNumber = Token_Register.KeyWordId - KW_REG_R0;
                         switch(Token_Size.KeyWordId)
                         {
                           case KW_SZ_BYTE: BinaryOut_Byte(BlackCPU::OPCODE_MOVE_IMM_B, Asd );
                                            BinaryOut_Byte(RegisterNumber, Asd);
                                            BinaryOut_Byte(ImmediateValue, Asd);
                                            break;
                           case KW_SZ_WORD: BinaryOut_Byte(BlackCPU::OPCODE_MOVE_IMM_W, Asd );
                                            BinaryOut_Byte(RegisterNumber, Asd);
                                            BinaryOut_Word(ImmediateValue, Asd);
                                            break;
                           case KW_SZ_LONG: BinaryOut_Byte(BlackCPU::OPCODE_MOVE_IMM_L, Asd );
                                            BinaryOut_Byte(RegisterNumber, Asd);
                                            BinaryOut_Long(ImmediateValue, Asd);
                                            break;
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
                         Asd->Parser.GetToken(Token_Separator); if (Token_Separator.TokenType != ZParser_Token::TK_KEYWORD || Token_Separator.KeyWordId != KW_PNK_VIRG) {ThrowError(Asd, &Token_Separator, ERROR_EXPECTEDVIRGULE); return(false);}
                         Asd->Parser.GetToken(Token_Register2); if (Token_Register2.TokenType != ZParser_Token::TK_KEYWORD) {ThrowError(Asd, &Token_Register2, ERROR_UNEPECTEDSTATMENT); return(false);}
                         if (Token_Register2.KeyWordId == KW_PNK_OPENPARENT)
                         {
                           Asd->Parser.GetToken(Token_Register2); if (Token_Register2.TokenType != ZParser_Token::TK_KEYWORD) {ThrowError(Asd, &Token_Register2, ERROR_EXPECTEDREGISTER); return(false);}
                           if (Token_Register2.KeyWordId < KW_REG_R0 || Token_Register2.KeyWordId > KW_REG_R15) {ThrowError(Asd, &Token_Register2, ERROR_EXPECTEDREGISTER); return(false); }
                           RegisterNumber2 = Token_Register2.KeyWordId - KW_REG_R0;
                           Asd->Parser.GetToken(Token_Separator); if (Token_Separator.TokenType != ZParser_Token::TK_KEYWORD || Token_Separator.KeyWordId != KW_PNK_CLOSEPARENT) {ThrowError(Asd, &Token_Separator, ERROR_EXPECTEDCLOSEPARENT); return(false);}
                           switch(Token_Size.KeyWordId)
                           {
                             case KW_SZ_BYTE: BinaryOut_Byte(BlackCPU::OPCODE_MOVE_REG_IND_B, Asd);
                                              BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                              break;
                             case KW_SZ_WORD: BinaryOut_Byte(BlackCPU::OPCODE_MOVE_REG_IND_W, Asd);
                                              BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                              break;
                             case KW_SZ_LONG: BinaryOut_Byte(BlackCPU::OPCODE_MOVE_REG_IND_L, Asd);
                                              BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                              break;
                           }
                         }
                         else if (Token_Register2.KeyWordId >= KW_REG_R0 && Token_Register2.KeyWordId <= KW_REG_R15)
                         {
                           RegisterNumber2 = Token_Register2.KeyWordId - KW_REG_R0;
                           switch(Token_Size.KeyWordId)
                           {
                             case KW_SZ_BYTE: BinaryOut_Byte(BlackCPU::OPCODE_MOVE_REG_REG_B, Asd);
                                              BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                              break;
                             case KW_SZ_WORD: BinaryOut_Byte(BlackCPU::OPCODE_MOVE_REG_REG_B, Asd);
                                              BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                              break;
                             case KW_SZ_LONG: BinaryOut_Byte(BlackCPU::OPCODE_MOVE_REG_REG_B, Asd);
                                              BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                              break;
                           }
                         }
                         else {ThrowError(Asd, &Token_Register, ERROR_UNEPECTEDSTATMENT); return(false);}
                         break;

      case KW_PNK_OPENPARENT:
                         if (Token_Register.KeyWordId < KW_REG_R0 || Token_Register.KeyWordId > KW_REG_R15) {ThrowError(Asd, &Token_Register, ERROR_EXPECTEDREGISTER); return(false); }
                         RegisterNumber = Token_Register.KeyWordId - KW_REG_R0;
                         Asd->Parser.GetToken(Token_Separator); if (Token_Separator.TokenType != ZParser_Token::TK_KEYWORD || Token_Separator.KeyWordId != KW_PNK_CLOSEPARENT) {ThrowError(Asd, &Token_Separator, ERROR_EXPECTEDCLOSEPARENT); return(false);}
                         Asd->Parser.GetToken(Token_Separator); if (Token_Separator.TokenType != ZParser_Token::TK_KEYWORD || Token_Separator.KeyWordId != KW_PNK_VIRG) {ThrowError(Asd, &Token_Separator, ERROR_EXPECTEDVIRGULE); return(false);}
                         Asd->Parser.GetToken(Token_Register2); if (Token_Register2.TokenType != ZParser_Token::TK_KEYWORD) {ThrowError(Asd, &Token_Register2, ERROR_UNEPECTEDSTATMENT); return(false);}
                         if (Token_Register2.KeyWordId < KW_REG_R0 || Token_Register2.KeyWordId > KW_REG_R15) {ThrowError(Asd, &Token_Register2, ERROR_EXPECTEDREGISTER); return(false); }
                         RegisterNumber2 = Token_Register2.KeyWordId - KW_REG_R0;
                         switch(Token_Size.KeyWordId)
                         {
                           case KW_SZ_BYTE: BinaryOut_Byte(BlackCPU::OPCODE_MOVE_REG_IND_B, Asd);
                                            BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                            break;
                           case KW_SZ_WORD: BinaryOut_Byte(BlackCPU::OPCODE_MOVE_REG_IND_W, Asd);
                                            BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                            break;
                           case KW_SZ_LONG: BinaryOut_Byte(BlackCPU::OPCODE_MOVE_REG_IND_L, Asd);
                                            BinaryOut_Byte(RegisterNumber2 || RegisterNumber << 4, Asd);
                                            break;
                         }
                         break;

      default: ThrowError(Asd, &Token_Arg, ERROR_EXPECTEDOPERAND); return(false);

    }

    return(true);
  }

  bool inline AssembleSection_Token_Movex(AssemblyData * Asd)
  {
    ZParser_Token Token_Size, Token_Separator, Token_Register;
    ULong ImmediateValue;

    // .x size

    if (!AssembleSection_OperandSize(Asd,Token_Size)) return(false);

    // # sharp sign.

    Asd->Parser.GetToken(Token_Separator); if (Token_Separator.TokenType != ZParser_Token::TK_KEYWORD || Token_Separator.KeyWordId != KW_PNK_SHARP ) {ThrowError(Asd, &Token_Separator, ERROR_EXPECTEDREGISTER); return(false);}

    // Immediate constant expression.

    if (!GetExpression(Asd, ImmediateValue)) return(false);

    // ',' separator

    Asd->Parser.GetToken(Token_Separator); if (Token_Separator.TokenType != ZParser_Token::TK_KEYWORD || Token_Separator.KeyWordId != KW_PNK_VIRG) {ThrowError(Asd, &Token_Separator, ERROR_EXPECTEDVIRGULE); return(false);}

    // Destination register.

    Asd->Parser.GetToken(Token_Register);
    if (   Token_Register.TokenType != ZParser_Token::TK_KEYWORD
        || Token_Register.KeyWordId < KW_REG_R0
        || Token_Register.KeyWordId > KW_REG_R15                ) { ThrowError(Asd, &Token_Register, ERROR_EXPECTEDREGISTER); return(false); }

    // Code output

    switch(Token_Size.KeyWordId)
    {
      case KW_SZ_UBYTE: BinaryOut_Byte(BlackCPU::OPCODE_MOVE_REG_IND_W, Asd);
                        break;
    }
    return(true);
  }

  bool inline AssembleSection_Token(AssemblyData * Asd, ZParser_Token &Token)
  {
    switch(Token.KeyWordId)
    {
      case KW_META_ORG: break;
      case KW_OP_MOVE: AssembleSection_Token_Move(Asd); break;

      default: break;
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

        case ZParser_Token::TK_IDENTIFIER: break;
        default: break;

      }

    }
    return(true);
  }

  public:

  bool Assemble(ZString & Text)
  {

    AssemblyData AsData, * Asd;

    Asd = &AsData;



    Asd->Text = &Text;

    RegisterKeywords(&Asd->KeyworSet);
    Asd->Parser.Init(Asd->Text->String, Asd->Text->Len, &Asd->KeyworSet);

    Asd->Address = 0;

    if (!AssembleSection(Asd)) return(false);
    return (true);
  }


};


#endif /* Z_MACROASSEMBLER_H */
