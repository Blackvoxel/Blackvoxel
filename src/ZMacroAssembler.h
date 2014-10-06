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
         KW_SZ_LONG
       };
  enum { ERROR_EXPECTEDOPERANDSIZE, ERROR_EXPECTEDOPERAND };


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
    Ks->AddKeyWord(".b",         KW_SZ_BYTE     );
    Ks->AddKeyWord(".w",         KW_SZ_WORD     );
    Ks->AddKeyWord(".l",         KW_SZ_LONG     );

    // Ks->AddKeyWord("")
  }

  void ThrowError(AssemblyData * Asd, ZParser_Token * Token, ULong MessageNum)
  {
     ZString As;
     switch (MessageNum)
     {
       case ERROR_EXPECTEDOPERANDSIZE: As = "Expected operand size : '.b', '.w' or '.l')."; break;
       case ERROR_EXPECTEDOPERAND:      As = "Valid operand expected : '.b', '.w' or '.l')."; break;
       default: As = "Unkown Error."; break;
     }
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

  bool inline AssembleSection_Token_Move(AssemblyData * Asd)
  {
    ZParser_Token Token_Size, Token_Arg;
    ZParser_State Pos;

    if (!AssembleSection_OperandSize(Asd,Token_Size)) return(false);

    Asd->Parser.SavePos(Pos);
    Asd->Parser.GetToken(Token_Arg);
    if (Token_Arg.TokenType != ZParser_Token::TK_KEYWORD) {ThrowError(Asd, &Token_Arg, ERROR_EXPECTEDOPERAND); return(false);}



    return(true);
  }

  bool inline AssembleSection_Token(AssemblyData * Asd, ZParser_Token &Token)
  {
    switch(Token.KeyWordId)
    {
      case KW_META_ORG: break;
      case KW_OP_MOVE: break;

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
        case ZParser_Token::TK_KEYWORD:

        case ZParser_Token::TK_IDENTIFIER: break;
        default: break;

      }

    }
    return(true);
  }

  bool Assemble(ZString & Text, AssemblyData * Asd)
  {
    Asd->Text = &Text;

    Asd->Parser.Init(Asd->Text->String, Asd->Text->Len, &Asd->KeyworSet);

    Asd->Address = 0;

    if (!AssembleSection(Asd)) return(false);
    return (true);
  }


};


#endif /* Z_MACROASSEMBLER_H */
