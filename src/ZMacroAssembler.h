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

#include <string.h>

class ZMacroAssembler
{
  public:
    class ZError
    {
      public:
        bool     CantLoadFile;
        ZString  FileText;
        ZString  FileName;
        ZMemSize ErrorLine;
        ZMemSize Offset_ErrorLine;
        ZMemSize Offset_ErrorStart;
        ZMemSize Offset_ErrorEnd;
        ZString  ErrorMessage;

        void AppendLinesBefore(ZString & Out, ULong LineCount)
        {
          ULong i;
          ZMemSize Pos;

          Pos = Offset_ErrorLine;

          for (i=0;i<=LineCount;i++)
          {
            FileText.SearchPredLineStart(Pos);
          }

          Out.Append_Mids(&Out, Pos,Offset_ErrorLine );

        }

        void AppendLinesAfter(ZString & Out, ULong LineCount);
        void AppendErrorLine_Part1(ZString & Out);
        void AppendErrorLine_Part2(ZString & Out);
        void AppendErrorLine_Part3(ZString & Out);
    } Error;

    class ZCompilationStatus
    {
      public:
        bool     CompilationResult;
        ZError * Error;
        ZString  CompilationTextOut;

        ZCompilationStatus()
        {
          Error = 0;
          CompilationResult = false;
        }
        ~ZCompilationStatus()
        {
          if (Error) delete Error;
          Error = 0;
        }
        void Clear()
        {
          if (Error) delete Error;
          Error = 0;
        }
    };

  protected:
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

  /*
          printf("Search:[%s]...", Name);
          if (strcmp(Name, "PIA_SERVO_DIRECTION")==0)
          {
            printf("R");
          }
  */
          if (!(Entry = FindEntry(Name))) {/*printf("FAIL\n");*/ return(false);}

          LabelValue = Entry->Value;
  /*      printf("Success %d\n", LabelValue); */

          return(true);
        }

        bool StoreLabel(char const * Name, ULong Value)
        {
          ZEntry * Entry;
          ZMemSize Index;
  /*
          printf("Store:[%s]=%d\n",Name, Value);
          if (strcmp(Name, "PIA_SERVO_DIRECTION")==0)
          {
            printf("R");
          }
  */
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
       ZCompilationStatus * CompilationStatus;
       ZLabelStore LabelStore;
       ZFastKeyWordSet KeyworSet;
       ZParser Parser;
       ZString * Text;
       ULong    Address;
       ULong    SectionSize;
       ZStream_SpecialRamStream Stream;
       ZMemSize Stream_SectionPos_Save;
       ULong *  Stream_SectionSize_Ptr;
       ZString  OutLine;
       ULong    AssemblyPass;
       Long     LastPrintedLine;
       ZError   Error;
    };

  enum { KW_META_ORG=1,
         KW_META_ALIGN,
         KW_META_DATA,
         KW_META_RESERVE,
         KW_OP_NOP,
         KW_OP_SLEEP,
         KW_OP_BREAK,
         KW_OP_MOVE,
         KW_OP_MOVEX,
         KW_OP_PUSHREGS,
         KW_OP_POPREGS,
         KW_OP_INC,
         KW_OP_DEC,
         KW_OP_INCLR,
         KW_OP_DECLR,
         KW_OP_ADD,
         KW_OP_SUB,
         KW_OP_AND,
         KW_OP_OR,
         KW_OP_XOR,
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
         KW_OP_BHS_U,
         KW_OP_BLS_U,
         KW_OP_BLO_U,
         KW_OP_BHI_S,
         KW_OP_BHS_S,
         KW_OP_BLS_S,
         KW_OP_BLO_S,
         KW_OP_BSO,
         KW_OP_RTS,
         KW_OP_RTI,
         KW_OP_EXT_BW,
         KW_OP_EXT_BL,
         KW_OP_EXT_WL,
         KW_OP_RSR,
         KW_OP_WSR,
         KW_OP_BCND,
         KW_OP_BNCND,
         KW_FLAG_ZSET,
         KW_FLAG_ZCLEAR,
         KW_FLAG_NSET,
         KW_FLAG_NCLEAR,
         KW_FLAG_VSET,
         KW_FLAG_VCLEAR,
         KW_FLAG_CSET,
         KW_FLAG_CCLEAR,
         KW_SZ_BYTE,
         KW_SZ_WORD,
         KW_SZ_LONG,
         KW_SZ_UBYTE,
         KW_SZ_SBYTE,
         KW_SZ_UWORD,
         KW_SZ_SWORD,
         KW_SZ_UNIBBLE,
         KW_SZ_SNIBBLE,

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
         KW_PNK_TWOPOINTS,
         KW_PNK_SEMICOLON,
         KW_PNK_ADD,
         KW_PNK_SUB,
         KW_PNK_MUL,
         KW_PNK_DIV,
         KW_PNK_OR,
         KW_PNK_XOR,
         KW_PNK_AND,
         KW_PNK_NOT,
         KW_PNK_EQUAL
       };
  enum { ERROR_EXPECTEDOPERANDSIZE, ERROR_EXPECTEDOPERAND, ERROR_EXPECTEDEXPRESSION, ERROR_EXPECTEDINTEGERVALUE, ERROR_EXPECTEDREGISTER, ERROR_EXPECTEDVIRGULE,
         ERROR_EXPECTEDOPENPARENT, ERROR_EXPECTEDCLOSEPARENT, ERROR_UNEPECTEDSTATMENT, ERROR_EXPECTEDSHARP, ERROR_EXPECTEDTWOPOINTSOREQUAL, ERROR_OUTOFRANGEBRANCH,
         ERROR_INVALIDOPSIZE_EXPECT_L, ERROR_MULTIPLIEROUTOFRANGE, ERROR_EXPECTFLAGKEYWORD };


  void RegisterKeywords(ZKeyWordSet * Ks)
  {
    Ks->AddKeyWord(".org",       KW_META_ORG   );
    Ks->AddKeyWord(".align",     KW_META_ALIGN );
    Ks->AddKeyWord(".data",      KW_META_DATA  );
    Ks->AddKeyWord(".reserve",   KW_META_RESERVE );
    Ks->AddKeyWord("nop",        KW_OP_NOP     );
    Ks->AddKeyWord("brk",        KW_OP_BREAK   );
    Ks->AddKeyWord("sleep",      KW_OP_SLEEP   );
    Ks->AddKeyWord("move",       KW_OP_MOVE    );
    Ks->AddKeyWord("movex",      KW_OP_MOVEX   );
    Ks->AddKeyWord("pushregs",   KW_OP_PUSHREGS);
    Ks->AddKeyWord("popregs",    KW_OP_POPREGS );
    Ks->AddKeyWord("inc",        KW_OP_INC     );
    Ks->AddKeyWord("dec",        KW_OP_DEC     );
    Ks->AddKeyWord("inclr",      KW_OP_INCLR   );
    Ks->AddKeyWord("declr",      KW_OP_DECLR   );
    Ks->AddKeyWord("add",        KW_OP_ADD     );
    Ks->AddKeyWord("sub",        KW_OP_SUB     );
    Ks->AddKeyWord("and",        KW_OP_AND     );
    Ks->AddKeyWord("or",         KW_OP_OR      );
    Ks->AddKeyWord("xor",        KW_OP_XOR     );
    Ks->AddKeyWord("asr",        KW_OP_ASR     );
    Ks->AddKeyWord("lsl",        KW_OP_LSL     );
    Ks->AddKeyWord("lsr",        KW_OP_LSR     );
    Ks->AddKeyWord("rol",        KW_OP_ROL     );
    Ks->AddKeyWord("ror",        KW_OP_ROR     );
    Ks->AddKeyWord("umul",       KW_OP_UMUL    );
    Ks->AddKeyWord("smul",       KW_OP_SMUL    );
    Ks->AddKeyWord("cmp",        KW_OP_CMP     );
    Ks->AddKeyWord("jmp",        KW_OP_JMP     );
    Ks->AddKeyWord("jsr",        KW_OP_JSR     );
    Ks->AddKeyWord("bra",        KW_OP_BRA     );
    Ks->AddKeyWord("beq",        KW_OP_BEQ     );
    Ks->AddKeyWord("bne",        KW_OP_BNE     );
    Ks->AddKeyWord("bcc",        KW_OP_BCC     );
    Ks->AddKeyWord("bcs",        KW_OP_BCS     );
    Ks->AddKeyWord("bpl",        KW_OP_BPL     );
    Ks->AddKeyWord("bmi",        KW_OP_BMI     );
    Ks->AddKeyWord("bvs",        KW_OP_BVS     );
    Ks->AddKeyWord("bvc",        KW_OP_BVC     );
    Ks->AddKeyWord("bhi",        KW_OP_BHI_U   );
    Ks->AddKeyWord("bhs",        KW_OP_BHS_U   );
    Ks->AddKeyWord("bhe.s",      KW_OP_BLS_U   );
    Ks->AddKeyWord("blo.s",      KW_OP_BLO_U   );
    Ks->AddKeyWord("bhi.s",      KW_OP_BHI_S   );
    Ks->AddKeyWord("ble.s",      KW_OP_BHS_S   );
    Ks->AddKeyWord("ble.s",      KW_OP_BLS_S   );
    Ks->AddKeyWord("ble.s",      KW_OP_BLO_S   );
    Ks->AddKeyWord("ble.s",      KW_OP_BSO     );
    Ks->AddKeyWord("rts",        KW_OP_RTS     );
    Ks->AddKeyWord("rti",        KW_OP_RTI     );
    Ks->AddKeyWord("ext.bw",     KW_OP_EXT_BW  );
    Ks->AddKeyWord("ext.bl",     KW_OP_EXT_BL  );
    Ks->AddKeyWord("ext.wl",     KW_OP_EXT_WL  );
    Ks->AddKeyWord("rsr",        KW_OP_RSR     );
    Ks->AddKeyWord("wsr",        KW_OP_WSR     );
    Ks->AddKeyWord("bcnd",       KW_OP_BCND    );
    Ks->AddKeyWord("bncnd",      KW_OP_BNCND   );
    Ks->AddKeyWord("ZCLEAR",     KW_FLAG_ZCLEAR    );
    Ks->AddKeyWord("ZSET",       KW_FLAG_ZSET    );
    Ks->AddKeyWord("NCLEAR",     KW_FLAG_NCLEAR    );
    Ks->AddKeyWord("NSET",       KW_FLAG_NSET    );
    Ks->AddKeyWord("VCLEAR",     KW_FLAG_VCLEAR    );
    Ks->AddKeyWord("VSET",       KW_FLAG_VSET    );
    Ks->AddKeyWord("CCLEAR",     KW_FLAG_CCLEAR    );
    Ks->AddKeyWord("CSET",       KW_FLAG_CSET    );
    Ks->AddKeyWord(".b",         KW_SZ_BYTE    );
    Ks->AddKeyWord(".w",         KW_SZ_WORD    );
    Ks->AddKeyWord(".l",         KW_SZ_LONG    );
    Ks->AddKeyWord(".ub",        KW_SZ_UBYTE   );
    Ks->AddKeyWord(".sb",        KW_SZ_SBYTE   );
    Ks->AddKeyWord(".uw",        KW_SZ_UWORD   );
    Ks->AddKeyWord(".sw",        KW_SZ_SWORD   );
    Ks->AddKeyWord(".n",         KW_SZ_UNIBBLE );
    Ks->AddKeyWord(".un",        KW_SZ_UNIBBLE );
    Ks->AddKeyWord(".sn",        KW_SZ_SNIBBLE );

    Ks->AddKeyWord("#",          KW_PNK_SHARP  );
    Ks->AddKeyWord(",",          KW_PNK_VIRG   );
    Ks->AddKeyWord("(",          KW_PNK_OPENPARENT);
    Ks->AddKeyWord(")",          KW_PNK_CLOSEPARENT);
    Ks->AddKeyWord(":",          KW_PNK_TWOPOINTS );
    Ks->AddKeyWord(";",          KW_PNK_SEMICOLON);
    Ks->AddKeyWord("+",          KW_PNK_ADD );
    Ks->AddKeyWord("-",          KW_PNK_SUB );
    Ks->AddKeyWord("*",          KW_PNK_MUL );
    Ks->AddKeyWord("/",          KW_PNK_DIV );
    Ks->AddKeyWord("=",          KW_PNK_EQUAL);

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
     ZString As,Bs;
     ZMemSize i, Count;

     switch (MessageNum)
     {
       case ERROR_EXPECTEDOPERANDSIZE:     As = "Expected operand size : '.b', '.w' or '.l')."; break;
       case ERROR_INVALIDOPSIZE_EXPECT_L:  As = "Invalid operand size : '.l' is the only option here.";  break;
       case ERROR_EXPECTEDOPERAND:         As = "Valid operand expected : '.b', '.w' or '.l')."; break;
       case ERROR_EXPECTEDEXPRESSION:      As = "Expression is expected."; break;
       case ERROR_EXPECTEDINTEGERVALUE:    As = "Integer value expected."; break;
       case ERROR_EXPECTEDREGISTER:        As = "Expected register"; break;
       case ERROR_EXPECTEDVIRGULE:         As = "',' separator expected."; break;
       case ERROR_EXPECTEDOPENPARENT:      As = "'(' expected."; break;
       case ERROR_EXPECTEDCLOSEPARENT:     As = "')' expected."; break;
       case ERROR_UNEPECTEDSTATMENT:       As = "Unexpected statment."; break;
       case ERROR_EXPECTEDSHARP:           As = "'#' Sharp sign expected."; break;
       case ERROR_EXPECTEDTWOPOINTSOREQUAL:As = "':' or '=' signs expected."; break;
       case ERROR_OUTOFRANGEBRANCH:        As = "Out of range relative branch."; break;
       case ERROR_MULTIPLIEROUTOFRANGE:    As = "Offset multiplier out of range (Allowed values are 1, 2, 4 or 8)."; break;
       case ERROR_EXPECTFLAGKEYWORD:       As = "Expect flag keyword list or ',' for separator."; break;
       default: As = "Unkown Error."; break;
     }

     // Display error

     printf("Assembler error at line %d : %s\n", Token->LineCount, As.String);
     Count = Token->TokenStart - Token->LineStart;
     Bs.Clear();
     Bs.Append_CStringUpToEOL(Token->LineStart);
     printf("%s\n",Bs.String);
     Bs.Clear();
     for (i=0;i<Count;i++) Bs.Append_char(' ');
     Bs.Append_char('^');
     printf("%s\n",Bs.String);

     // Fill Error structure

     if (!Asd->CompilationStatus->Error) Asd->CompilationStatus->Error = new ZError();
     Asd->CompilationStatus->Error->CantLoadFile = false;
     Asd->CompilationStatus->Error->FileText = *Asd->Text;
     Asd->CompilationStatus->Error->ErrorMessage = As;
     Asd->CompilationStatus->Error->ErrorLine = Token->LineCount;
     Asd->CompilationStatus->Error->Offset_ErrorLine = Token->LineStart  - Asd->Text->String;
     Asd->CompilationStatus->Error->Offset_ErrorStart= Token->TokenStart - Asd->Text->String;
     Asd->CompilationStatus->Error->Offset_ErrorEnd  = Token->TokenEnd   - Asd->Text->String;
  }

  void BinaryOut_Byte(UByte Code, AssemblyData * Asd)
  {
    Asd->Stream.Put(Code);
    Asd->Address++;
    Asd->SectionSize++;

    Display_AssembledByte(Asd, Code);
  }

  void BinaryOut_Word(UShort Code, AssemblyData * Asd)
  {
    Asd->Stream.Put(Code);
    Asd->Address+=2;
    Asd->SectionSize+=2;

    Display_AssembledShort(Asd, Code);
  }

  void BinaryOut_Long(ULong Code, AssemblyData * Asd)
  {
    Asd->Stream.Put(Code);
    Asd->Address+=4;
    Asd->SectionSize+=4;

    Display_AssembledLong(Asd, Code);
  }


  bool inline AssembleSection_OperandSize(AssemblyData * Asd, ZParser_Token & Token)
  {
    if (!Asd->Parser.GetToken(Token))                 {ThrowError(Asd, &Token, ERROR_EXPECTEDOPERANDSIZE); return(false);}
    if (Token.TokenType != ZParser_Token::TK_KEYWORD) {ThrowError(Asd, &Token,ERROR_EXPECTEDOPERANDSIZE); return(false);}
    switch(Token.KeyWordId)
    {
      case KW_SZ_UNIBBLE:
      case KW_SZ_SNIBBLE:
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

     Asd->Parser.GetToken(Token);

     switch(Token.TokenType)
     {
       case ZParser_Token::TK_IDENTIFIER: GetIdentifierValue(Asd, Token.AlphaValue, ExpressionValue);
                                          break;

       case ZParser_Token::TK_CONST:      if (Token.ConstType!= ZParser_Token::CT_INTEGER)  { ThrowError(Asd, &Token, ERROR_EXPECTEDINTEGERVALUE); return(false);}
                                          ExpressionValue = Token.ConstValue.ValULong;
                                          break;
       case ZParser_Token::TK_KEYWORD:    switch(Token.KeyWordId)
                                          {
                                            case KW_PNK_OPENPARENT: GetExpression(Asd, ExpressionValue);
                                                                    if (!ExpectToken(Asd, KW_PNK_OPENPARENT, KW_PNK_CLOSEPARENT)) return(false);
                                                                    break;

                                            case KW_PNK_SUB:        GetExpression(Asd,ExpressionValue);
                                                                    ExpressionValue = - ExpressionValue;
                                                                    break;
                                            default:                ThrowError(Asd, &Token, ERROR_EXPECTEDEXPRESSION); return(false);
                                          }

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

  bool inline ExpectToken_GetIfMatch(AssemblyData * Asd, ULong Opcode)
  {
    ZParser_Token Token;
    ZParser_State Pos;

    // Save state

    Asd->Parser.SavePos(Pos);

    // Fetch Token

    Asd->Parser.GetToken(Token);

    // Ensure token is what's expected

    if ( Token.TokenType != ZParser_Token::TK_KEYWORD || Token.KeyWordId != Opcode )
    {
      Asd->Parser.RestorePos(Pos);
      return(false);
    }

    // Token is matching

    return(true);
  }


  bool inline ExpectRegisterOnToken(AssemblyData * Asd, ZParser_Token & Token_Register, ULong & Register)
  {
    // Ensure token is a register number.

    if (   Token_Register.TokenType != ZParser_Token::TK_KEYWORD
        || Token_Register.KeyWordId < KW_REG_R0
        || Token_Register.KeyWordId > KW_REG_R15 )
    {
      ThrowError(Asd, &Token_Register, ERROR_EXPECTEDREGISTER);
      Register = 0;
      return(false);
    }

    // Convert token to register number

    Register = Token_Register.KeyWordId - KW_REG_R0;

    return(true);
  }

  bool inline ExpectRegister(AssemblyData * Asd, ULong & Register)
  {
    ZParser_Token Token_Register;

    // Fetch Token

    Asd->Parser.GetToken(Token_Register);

    // Parse Token and return result

    return( ExpectRegisterOnToken(Asd, Token_Register, Register) );

  }

  bool inline AssembleSection_Token_Move_Reg_ComplexForm(AssemblyData * Asd)
  {
    ZParser_Token Token_Separator, Token_Size;
    ULong Value, Register_Number_1, Register_Number_2, Register_Number_3;

    // Size

    if (!AssembleSection_OperandSize(Asd,Token_Size)) return(false);

    // Source register

    if (!ExpectRegister(Asd, Register_Number_1 )) return(false);

    // ,

    if (!ExpectToken(Asd, KW_PNK_VIRG, ERROR_EXPECTEDVIRGULE)) return(false);

    // Displacement

    if (!GetExpression(Asd,Value)) return(false);

    // (

    if (!ExpectToken(Asd, KW_PNK_OPENPARENT, ERROR_EXPECTEDOPENPARENT)) return(false);

    // Register

    if (!ExpectRegister(Asd, Register_Number_2 )) return(false);

    // + or ) :

    Asd->Parser.GetToken(Token_Separator); if (Token_Separator.TokenType != ZParser_Token::TK_KEYWORD) {ThrowError(Asd, &Token_Separator, ERROR_UNEPECTEDSTATMENT); return(false);}

    // move rx, displacement(ry) form

    if (Token_Separator.KeyWordId == KW_PNK_CLOSEPARENT)
    {
      // Code Output

      switch(Token_Size.KeyWordId)
      {
        case KW_SZ_BYTE: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_INDDEP_B, Asd);
                         BinaryOut_Byte(Register_Number_2 | Register_Number_1 << 4, Asd);
                         BinaryOut_Word(Value, Asd);
                         break;
        case KW_SZ_WORD: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_INDDEP_W, Asd);
                         BinaryOut_Byte(Register_Number_2 | Register_Number_1 << 4, Asd);
                         BinaryOut_Word(Value, Asd);

                         break;
        case KW_SZ_LONG: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_INDDEP_L, Asd);
                         BinaryOut_Byte(Register_Number_2 | Register_Number_1 << 4, Asd);
                         BinaryOut_Word(Value, Asd);
                         break;
        default:         ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);
      }
    }

    // move rx, displacement(ry+rz*m) form

    else if (Token_Separator.KeyWordId == KW_PNK_ADD)
    {
      ULong Shift = 0;

      // rz

      if (!ExpectRegister(Asd, Register_Number_3 )) return(false);

      // *m Multiplier if any

      if (ExpectToken_GetIfMatch(Asd, KW_PNK_MUL))
      {
        if (!this->GetExpression(Asd, Shift)) return(false);
        switch(Shift)
        {
          case 0: Shift = 0; break;
          case 1: Shift = 0; break;
          case 2: Shift = 1; break;
          case 4: Shift = 2; break;
          case 8: Shift = 4; break;
          default: ThrowError(Asd, &Token_Size,ERROR_MULTIPLIEROUTOFRANGE); return(false);
        }
      }

      // )

      if (!ExpectToken(Asd, KW_PNK_CLOSEPARENT, ERROR_EXPECTEDCLOSEPARENT)) return(false);

      // Code Output

      switch(Token_Size.KeyWordId)
      {
        case KW_SZ_BYTE: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_INDDEPOFF_B, Asd);
                         BinaryOut_Byte(Register_Number_2 | Register_Number_1 << 4, Asd);
                         BinaryOut_Byte(Register_Number_3 | Shift << 6, Asd);
                         BinaryOut_Word(Value, Asd);
                         break;
        case KW_SZ_WORD: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_INDDEPOFF_W, Asd);
                         BinaryOut_Byte(Register_Number_2 | Register_Number_1 << 4, Asd);
                         BinaryOut_Byte(Register_Number_3 | Shift << 6, Asd);
                         BinaryOut_Word(Value, Asd);

                         break;
        case KW_SZ_LONG: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_INDDEPOFF_L, Asd);
                         BinaryOut_Byte(Register_Number_2 | Register_Number_1 << 4, Asd);
                         BinaryOut_Byte(Register_Number_3 | Shift << 6, Asd);
                         BinaryOut_Word(Value, Asd);
                         break;
        default:         ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);
      }

    }

    // Error if not one of the two forms

    else
    {
      ThrowError(Asd, &Token_Size,ERROR_UNEPECTEDSTATMENT); return(false);
    }

    return(true);
  }

  bool inline AssembleSection_Token_Move_ComplexForm_Reg(AssemblyData * Asd)
  {
    ZParser_Token Token_Separator, Token_Size;
    ULong Value, Register_Number_1, Register_Number_2, Register_Number_3;

    // Size

    if (!AssembleSection_OperandSize(Asd,Token_Size)) return(false);

    // Displacement

    if (!GetExpression(Asd,Value)) return(false);

    // (

    if (!ExpectToken(Asd, KW_PNK_OPENPARENT, ERROR_EXPECTEDOPENPARENT)) return(false);

    // Register

    if (!ExpectRegister(Asd, Register_Number_1 )) return(false);

    // + or ) :

    Asd->Parser.GetToken(Token_Separator); if (Token_Separator.TokenType != ZParser_Token::TK_KEYWORD) {ThrowError(Asd, &Token_Separator, ERROR_UNEPECTEDSTATMENT); return(false);}

    // move displacement(rx),ry form
    if (Token_Separator.KeyWordId == KW_PNK_CLOSEPARENT)
    {
      // ,

      if (!ExpectToken(Asd, KW_PNK_VIRG, ERROR_EXPECTEDVIRGULE)) return(false);

      // rx

      if (!ExpectRegister(Asd, Register_Number_2 )) return(false);

      // Code Output

      switch(Token_Size.KeyWordId)
      {
        case KW_SZ_BYTE: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_INDDEP_REG_B, Asd);
                         BinaryOut_Byte(Register_Number_2 | Register_Number_1 << 4, Asd);
                         BinaryOut_Word(Value, Asd);
                         break;
        case KW_SZ_WORD: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_INDDEP_REG_W, Asd);
                         BinaryOut_Byte(Register_Number_2 | Register_Number_1 << 4, Asd);
                         BinaryOut_Word(Value, Asd);

                         break;
        case KW_SZ_LONG: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_INDDEP_REG_L, Asd);
                         BinaryOut_Byte(Register_Number_2 | Register_Number_1 << 4, Asd);
                         BinaryOut_Word(Value, Asd);
                         break;
        default:         ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);
      }

    }

    // move displacement(rx+rz*m),ry form

    else if (Token_Separator.KeyWordId == KW_PNK_ADD)
    {
      ULong Shift = 0;

      // rz

      if (!ExpectRegister(Asd, Register_Number_3 )) return(false);

      // *m Multiplier if any

      if (ExpectToken_GetIfMatch(Asd, KW_PNK_MUL))
      {
        if (!this->GetExpression(Asd, Shift)) return(false);
        switch(Shift)
        {
          case 0: Shift = 0; break;
          case 1: Shift = 0; break;
          case 2: Shift = 1; break;
          case 4: Shift = 2; break;
          case 8: Shift = 4; break;
          default: ThrowError(Asd, &Token_Size,ERROR_MULTIPLIEROUTOFRANGE); return(false);
        }
      }

      // )

      if (!ExpectToken(Asd, KW_PNK_CLOSEPARENT, ERROR_EXPECTEDCLOSEPARENT)) return(false);

      // ,

      if (!ExpectToken(Asd, KW_PNK_VIRG, ERROR_EXPECTEDVIRGULE)) return(false);

      // rx

      if (!ExpectRegister(Asd, Register_Number_2 )) return(false);

      // Code Output

      switch(Token_Size.KeyWordId)
      {
        case KW_SZ_BYTE: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_INDDEPOFF_REG_B, Asd);
                         BinaryOut_Byte(Register_Number_2 | Register_Number_1 << 4, Asd);
                         BinaryOut_Byte(Register_Number_3 | Shift << 6, Asd);
                         BinaryOut_Word(Value, Asd);
                         break;
        case KW_SZ_WORD: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_INDDEPOFF_REG_W, Asd);
                         BinaryOut_Byte(Register_Number_2 | Register_Number_1 << 4, Asd);
                         BinaryOut_Byte(Register_Number_3 | Shift << 6, Asd);
                         BinaryOut_Word(Value, Asd);

                         break;
        case KW_SZ_LONG: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_INDDEPOFF_REG_L, Asd);
                         BinaryOut_Byte(Register_Number_2 | Register_Number_1 << 4, Asd);
                         BinaryOut_Byte(Register_Number_3 | Shift << 6, Asd);
                         BinaryOut_Word(Value, Asd);
                         break;
        default:         ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);
      }
    }

    // Error if not one of the two forms

    else
    {
      ThrowError(Asd, &Token_Size,ERROR_UNEPECTEDSTATMENT); return(false);
    }

    return(true);
  }

  bool inline AssembleSection_Token_Move(AssemblyData * Asd)
  {
    ZParser_Token Token_Size, Token_Arg, Token_Register,Token_Register2, Token_Separator;

    ULong ImmediateValue;
    ULong RegisterNumber, RegisterNumber2;
    ZParser_State Pos;

    Asd->Parser.SavePos(Pos);

    if (!AssembleSection_OperandSize(Asd,Token_Size)) return(false);
    Asd->Parser.GetToken(Token_Arg);

    // Complex Forms
    if (    Token_Arg.TokenType == ZParser_Token::TK_IDENTIFIER
        || (Token_Arg.TokenType == ZParser_Token::TK_CONST && Token_Arg.ConstType == ZParser_Token::CT_INTEGER) )
    {
      Asd->Parser.RestorePos(Pos);
      return(AssembleSection_Token_Move_ComplexForm_Reg(Asd));
    }

    if (Token_Arg.TokenType != ZParser_Token::TK_KEYWORD) {ThrowError(Asd, &Token_Size,ERROR_UNEPECTEDSTATMENT); return(false);}

    switch(Token_Arg.KeyWordId)
    {
      case KW_PNK_SHARP: if (!GetExpression(Asd, ImmediateValue)) return(false);
                         if (!ExpectToken(Asd, KW_PNK_VIRG, ERROR_EXPECTEDVIRGULE)) return(false);
                         if (!ExpectRegister(Asd, RegisterNumber)) return(false);

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

                         // Complex form
                         Asd->Parser.GetToken(Token_Register2);
                         if (    Token_Register2.TokenType == ZParser_Token::TK_IDENTIFIER
                             || (Token_Register2.TokenType == ZParser_Token::TK_CONST && Token_Register2.ConstType == ZParser_Token::CT_INTEGER) )
                         {
                           Asd->Parser.RestorePos(Pos);
                           return(AssembleSection_Token_Move_Reg_ComplexForm(Asd));
                         }

                         if (Token_Register2.TokenType != ZParser_Token::TK_KEYWORD) {ThrowError(Asd, &Token_Register2, ERROR_UNEPECTEDSTATMENT); return(false);}
                         if (Token_Register2.KeyWordId == KW_PNK_SUB) { Asd->Parser.RestorePos(Pos); return(AssembleSection_Token_Move_Reg_ComplexForm(Asd)); }
                         if (Token_Register2.KeyWordId == KW_PNK_OPENPARENT)
                         {
                           if (!ExpectRegister(Asd, RegisterNumber2)) return(false);
                           if (!ExpectToken(Asd, KW_PNK_CLOSEPARENT, ERROR_EXPECTEDCLOSEPARENT)) return(false);

                           switch(Token_Size.KeyWordId)
                           {
                             case KW_SZ_BYTE: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_IND_B, Asd);
                                              BinaryOut_Byte(RegisterNumber2 | RegisterNumber << 4, Asd);
                                              break;
                             case KW_SZ_WORD: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_IND_W, Asd);
                                              BinaryOut_Byte(RegisterNumber2 | RegisterNumber << 4, Asd);
                                              break;
                             case KW_SZ_LONG: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_IND_L, Asd);
                                              BinaryOut_Byte(RegisterNumber2 | RegisterNumber << 4, Asd);
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
                                              BinaryOut_Byte(RegisterNumber2 | RegisterNumber << 4, Asd);
                                              break;
                             case KW_SZ_WORD: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_REG_B, Asd);
                                              BinaryOut_Byte(RegisterNumber2 | RegisterNumber << 4, Asd);
                                              break;
                             case KW_SZ_LONG: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_REG_REG_B, Asd);
                                              BinaryOut_Byte(RegisterNumber2 | RegisterNumber << 4, Asd);
                                              break;
                             default:         ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);
                           }
                         }
                         else {ThrowError(Asd, &Token_Register2, ERROR_UNEPECTEDSTATMENT); return(false);}
                         break;

      case KW_PNK_OPENPARENT:
                         if (!ExpectRegister(Asd, RegisterNumber)) return(false);
                         if (!ExpectToken(Asd, KW_PNK_CLOSEPARENT, ERROR_EXPECTEDCLOSEPARENT)) return(false);
                         if (!ExpectToken(Asd, KW_PNK_VIRG, ERROR_EXPECTEDVIRGULE)) return(false);
                         if (!ExpectRegister(Asd, RegisterNumber2)) return(false);

                         switch(Token_Size.KeyWordId)
                         {
                           case KW_SZ_BYTE: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_IND_REG_B, Asd);
                                            BinaryOut_Byte(RegisterNumber2 | RegisterNumber << 4, Asd);
                                            break;
                           case KW_SZ_WORD: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_IND_REG_W, Asd);
                                            BinaryOut_Byte(RegisterNumber2 | RegisterNumber << 4, Asd);
                                            break;
                           case KW_SZ_LONG: BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVE_IND_REG_L, Asd);
                                            BinaryOut_Byte(RegisterNumber2 | RegisterNumber << 4, Asd);
                                            break;
                           default:         ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);
                         }
                         break;

      default: ThrowError(Asd, &Token_Arg, ERROR_EXPECTEDOPERAND); return(false);

    }

    return(true);
  }

  bool inline AssembleSection_Token_NibbleImm(AssemblyData * Asd, ULong OpCode, Long Offset)
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

    if (!ExpectRegister(Asd, Register)) return(false);

    // Code output

    switch(Token_Size.KeyWordId)
    {
      case KW_SZ_LONG: BinaryOut_Byte(OpCode, Asd);
                       BinaryOut_Byte( Register | ((((ImmediateValue + Offset) & 0xf) << 4)), Asd);
                       break;

      case KW_SZ_BYTE:
      case KW_SZ_UBYTE:
      case KW_SZ_SBYTE:
      case KW_SZ_WORD:
      case KW_SZ_UWORD:
      case KW_SZ_SWORD:ThrowError(Asd, &Token_Size,ERROR_INVALIDOPSIZE_EXPECT_L);
                       return(false);


      default:         ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE);
                       return(false);

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

    if (!ExpectRegister(Asd, Register)) return(false);

    // Code output

    switch(Token_Size.KeyWordId)
    {
      case KW_SZ_UNIBBLE:BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVEX_IMM_UN, Asd);
                         BinaryOut_Byte( Register | (ImmediateValue<<4), Asd);
                         break;
      case KW_SZ_SNIBBLE:BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVEX_IMM_SN, Asd);
                         BinaryOut_Byte( Register | (ImmediateValue<<4), Asd);
                         break;
      case KW_SZ_BYTE:
      case KW_SZ_UBYTE:  BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVEX_IMM_UB, Asd);
                         BinaryOut_Byte( Register, Asd);
                         BinaryOut_Byte(ImmediateValue, Asd);
                         break;
      case KW_SZ_SBYTE:  BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVEX_IMM_SB, Asd);
                         BinaryOut_Byte( Register, Asd);
                         BinaryOut_Byte(ImmediateValue, Asd);
                         break;
      case KW_SZ_WORD:
      case KW_SZ_UWORD:  BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVEX_IMM_UW, Asd);
                         BinaryOut_Byte( Register, Asd);
                         BinaryOut_Word(ImmediateValue, Asd);
                         break;
      case KW_SZ_SWORD:  BinaryOut_Byte(BlackCPU<int>::OPCODE_MOVEX_IMM_SW, Asd);
                         BinaryOut_Byte( Register, Asd);
                         BinaryOut_Word(ImmediateValue, Asd);
                         break;
      default:           ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);

    }
    return(true);
  }

  bool inline AssembleSection_Token_PushPopRegs(AssemblyData * Asd, ULong Opcode)
  {
    ZParser_Token Token;
    ULong  RegisterMask, RegisterNum, RegisterNum2,Tmp,i;
    ZParser_State Pos;


    RegisterMask = 0;
    do
    {
      if (!ExpectRegister( Asd, RegisterNum )) return(false);

      Asd->Parser.SavePos(Pos);
      Asd->Parser.GetToken(Token);
      if ( (Token.TokenType == ZParser_Token::TK_KEYWORD) && ( Token.KeyWordId == KW_PNK_SUB )  )
      {
        if (!ExpectRegister( Asd, RegisterNum2 )) return(false);
        if (RegisterNum>RegisterNum2) {Tmp = RegisterNum; RegisterNum = RegisterNum2; RegisterNum2 = Tmp; }
        for (i=RegisterNum; i<=RegisterNum2;i++) RegisterMask |= 1 << i;
        Asd->Parser.SavePos(Pos);
        Asd->Parser.GetToken(Token);
      }

    } while ( (Token.TokenType == ZParser_Token::TK_KEYWORD) & (Token.KeyWordId==KW_PNK_VIRG));

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

    if (!ExpectRegister( Asd, DestinationRegister )) return(false);

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
                        BinaryOut_Byte(DestinationRegister | ( SourceRegister<< 4) , Asd);
                        break;
      case KW_SZ_WORD:  BinaryOut_Byte(Opcode+64, Asd);
                        BinaryOut_Byte(DestinationRegister | ( SourceRegister<< 4) , Asd);
                        break;
      case KW_SZ_LONG:  BinaryOut_Byte(Opcode+128, Asd);
                        BinaryOut_Byte(DestinationRegister | ( SourceRegister<< 4) , Asd);
                        break;
      default:          ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);
    }

    return(true);
  }




  bool inline AssembleSection_Token_IndirectJump(AssemblyData * Asd, ULong Opcode)
  {
    ZParser_Token Token_Size, Token_Separator, Token_Register;
    ULong Register=0;

    // Open parenthesis

    if (!ExpectToken(Asd, KW_PNK_OPENPARENT, ERROR_EXPECTEDOPENPARENT)) return(false);

    // Source register.

    if (!ExpectRegister(Asd, Register)) return(false);

    // Close parenthesis

    if (!ExpectToken(Asd, KW_PNK_CLOSEPARENT, ERROR_EXPECTEDCLOSEPARENT)) return(false);

    // Code output

    BinaryOut_Byte(Opcode, Asd);
    BinaryOut_Byte(Register , Asd);

    return(true);
  }

  bool inline AssembleSection_Token_ConditionalBranch(AssemblyData * Asd, ZParser_Token &Token ,  ULong Opcode)
  {
    ULong ImmediateValue;
    Long Displacement;

    // Immediate constant expression.

    if (!GetExpression(Asd, ImmediateValue)) return(false);

    // Compute branch location

    Displacement = (Long)(ImmediateValue - Asd->Address);
    Displacement-= 3; // Compensation for PC increasing in branch instruction.

    // Verify branch range.

    if (Displacement > 32767 || Displacement < -32768) { ThrowError(Asd, &Token,ERROR_OUTOFRANGEBRANCH); return(false); }

    // Code output

    BinaryOut_Byte(Opcode, Asd);
    BinaryOut_Word( (UShort)Displacement , Asd);

    return(true);
  }


  bool inline AssembleSection_Token_BranchCond(AssemblyData * Asd, ZParser_Token &OpcodeToken ,  ULong Opcode)
  {
    ZParser_Token Token;

    ULong ImmediateValue, CondMask,Cond;
    Long Displacement;
    bool Continue;

    CondMask = Cond = 0;
    Continue = true;
    while(Continue)
    {
      Asd->Parser.GetToken(Token);
      if (Token.TokenType != ZParser_Token::TK_KEYWORD) {ThrowError(Asd, &Token, ERROR_EXPECTFLAGKEYWORD); return(false);}
      switch(Token.KeyWordId)
      {
        case KW_FLAG_ZSET:   CondMask |= 1; Cond |=  1; break;
        case KW_FLAG_ZCLEAR: CondMask |= 1; Cond &= ~1; break;
        case KW_FLAG_NSET:   CondMask |= 2; Cond |=  2; break;
        case KW_FLAG_NCLEAR: CondMask |= 2; Cond &= ~2; break;
        case KW_FLAG_VSET:   CondMask |= 4; Cond |=  4; break;
        case KW_FLAG_VCLEAR: CondMask |= 4; Cond &= ~4; break;
        case KW_FLAG_CSET:   CondMask |= 8; Cond |=  8; break;
        case KW_FLAG_CCLEAR: CondMask |= 8; Cond &= ~8; break;
        case KW_PNK_DIV:     break;
        case KW_PNK_VIRG:    Continue = false;
                             break;
        default: ThrowError(Asd, &Token, ERROR_EXPECTFLAGKEYWORD); return(false);
      }
    }

    // Immediate constant expression.

    if (!GetExpression(Asd, ImmediateValue)) return(false);

    // Compute branch location

    Displacement = (Long)(ImmediateValue - Asd->Address);
    Displacement-= 4; // Compensation for PC increasing in branch instruction.

    // Verify branch range.

    if (Displacement > 32767 || Displacement < -32768) { ThrowError(Asd, &OpcodeToken,ERROR_OUTOFRANGEBRANCH); return(false); }

    // Code output

    BinaryOut_Byte(Opcode, Asd);
    BinaryOut_Byte( (Cond & 0xf) | ((CondMask&0xf) << 4) , Asd);
    BinaryOut_Word( (UShort)Displacement , Asd);

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

  bool inline AssembleSection_Token_Reserve(AssemblyData * Asd)
  {
    ZParser_Token Token_Size;
    ULong Value, NewStartAddress;

    // .x size

    if (!AssembleSection_OperandSize(Asd,Token_Size)) return(false);

    // Size of the uninitialized space.

    if (!GetExpression(Asd, Value)) return(false);

    // Open a BSS section

    CloseSection(Asd);
    OpenNewSection(Asd, MulticharConst('S','P','C','E') , Asd->Address);

    // Data lenght.

    NewStartAddress = Asd->Address;
    switch(Token_Size.KeyWordId)
    {
      case KW_SZ_BYTE: BinaryOut_Long(0, Asd ); NewStartAddress += Value; break;
      case KW_SZ_WORD: BinaryOut_Long(1, Asd ); NewStartAddress += Value << 1; break;
      case KW_SZ_LONG: BinaryOut_Long(2, Asd ); NewStartAddress += Value << 2; break;
      default:         ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);
    }

    // Number of space to reserve.

    BinaryOut_Long(Value, Asd );

    // Close space section and open new code section

    CloseSection(Asd);
    OpenNewSection(Asd, MulticharConst('C','O','D','E') , NewStartAddress);

    return(true);
  }

  bool inline AssembleSection_Token_Align(AssemblyData * Asd)
  {
    ULong Value, Remain, i;

    if (!GetExpression(Asd, Value)) return(false);

    Remain = Asd->Address % Value;

    if (Remain) for (i= Value - Remain; i>0;i--) BinaryOut_Byte(0, Asd);

    return(true);
  }

  bool inline AssembleSection_Token_Data(AssemblyData * Asd)
  {
    ZParser_Token Token_Size, Token_Separator, Token_String;
    ZParser_State SavePoint;
    ZMemSize i;
    ULong Value;

    // .x size

    if (!AssembleSection_OperandSize(Asd,Token_Size)) return(false);

    // Open Parent

    if (!ExpectToken(Asd, KW_PNK_OPENPARENT, ERROR_EXPECTEDOPENPARENT)) return(false);

    // Data

    do
    {
      // Strings processing

      Asd->Parser.SavePos(SavePoint);
      Asd->Parser.GetToken(Token_String);
      if (Token_String.TokenType == ZParser_Token::TK_CONST && Token_String.ConstType == ZParser_Token::CT_ALPHA)
      {
        for (i=0;i<Token_String.AlphaValue.Len;i++)
        {
          Value = (ULong)Token_String.AlphaValue.String[i];
          switch(Token_Size.KeyWordId)
          {
            case KW_SZ_BYTE: BinaryOut_Byte(Value, Asd );    break;
            case KW_SZ_WORD: BinaryOut_Word(Value, Asd );    break;
            case KW_SZ_LONG: BinaryOut_Long(Value, Asd );    break;
            default:         ThrowError(Asd, &Token_Size,ERROR_EXPECTEDOPERANDSIZE); return(false);
          }
        }
        Asd->Parser.GetToken(Token_Separator);
        if ( Token_Separator.TokenType != ZParser_Token::TK_KEYWORD ) { ThrowError(Asd, &Token_Separator, ERROR_EXPECTEDCLOSEPARENT); return(false); }

        continue;
      }
      Asd->Parser.RestorePos(SavePoint);

      // If not a string, try parsing expression.

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

  bool AssembleComment(AssemblyData * Asd)
  {
    Asd->Parser.StripUToEol();
    return(true);
  }

  void inline Display_AssemblingAddress(AssemblyData * Asd)
  {
    // Output line

    Asd->OutLine.Clear();
    Asd->OutLine.Set_DisplayBase(16);
    Asd->OutLine.Set_DisplayTrailingZero();
    Asd->OutLine << "[" << Asd->Address << "] ";
  }

  void Display_AssembledByte(AssemblyData * Asd, UByte Data)
  {
    ZString Number;

    Number.Set_DisplayBase(16);
    Number.Set_DisplayTrailingZero();

    Number = (ULong)Data;

    Asd->OutLine << Number.String + 6 << " ";
  }

  void Display_AssembledShort(AssemblyData * Asd, UShort Data)
  {
    ZString Number;

    Number.Set_DisplayBase(16);
    Number.Set_DisplayTrailingZero();

    Number = (ULong)(Data & 0xff);
    Asd->OutLine << Number.String + 6 << " ";
    Number = (ULong)(Data >> 8);
    Asd->OutLine << Number.String + 6 << " ";

  }

  void Display_AssembledLong(AssemblyData * Asd, ULong Data)
  {
    ZString Number;

    Number.Set_DisplayBase(16);
    Number.Set_DisplayTrailingZero();

    Number = (ULong)(Data & 0xff);
    Asd->OutLine << Number.String + 6 << " ";
    Number = (ULong)((Data >> 8) & 0xff);
    Asd->OutLine << Number.String + 6 << " ";
    Number = (ULong)((Data >> 16) & 0xff);
    Asd->OutLine << Number.String + 6 << " ";
    Number = (ULong)((Data >> 24) & 0xff);
    Asd->OutLine << Number.String + 6 << " ";
  }

  void Display_Align(AssemblyData * Asd)
  {
    Asd->OutLine.PadUptoLen(' ', 29 );
  }

  void Display_TokenLine(AssemblyData * Asd, ZParser_Token &Token)
  {
    Asd->OutLine.Append_CStringUpToEOL(Token.LineStart);
  }

  bool inline AssembleSection_Token(AssemblyData * Asd, ZParser_Token &Token)
  {
    bool Ok;

    // Display address

    if (Asd->AssemblyPass == 2)
    {
      Asd->OutLine.Clear();
      Display_AssemblingAddress(Asd);
    }

    // Process instructions

    switch(Token.KeyWordId)
    {
      case KW_META_ORG:      Ok = AssembleSection_Token_Org(Asd);                                                   break;
      case KW_PNK_SEMICOLON: Ok = AssembleComment(Asd);                                                             break;
      case KW_META_ALIGN:    Ok = AssembleSection_Token_Align(Asd);                                                 break;
      case KW_META_DATA:     Ok = AssembleSection_Token_Data(Asd);                                                  break;
      case KW_META_RESERVE:  Ok = AssembleSection_Token_Reserve(Asd);                                               break;
      case KW_OP_NOP:        Ok = AssembleSection_Token_SingleInstruction(Asd, BlackCPU<int>::OPCODE_NOP);          break;
      case KW_OP_BREAK:      Ok = AssembleSection_Token_SingleInstruction(Asd, BlackCPU<int>::OPCODE_BREAK);        break;
      case KW_OP_SLEEP:      Ok = AssembleSection_Token_SingleInstruction(Asd, BlackCPU<int>::OPCODE_SLEEP);        break;
      case KW_OP_MOVE:       Ok = AssembleSection_Token_Move(Asd);                                                  break;
      case KW_OP_MOVEX:      Ok = AssembleSection_Token_Movex(Asd);                                                 break;
      case KW_OP_PUSHREGS:   Ok = AssembleSection_Token_PushPopRegs(Asd, BlackCPU<int>::OPCODE_PUSHREGS);           break;
      case KW_OP_POPREGS:    Ok = AssembleSection_Token_PushPopRegs(Asd, BlackCPU<int>::OPCODE_POPREGS);            break;
      case KW_OP_INC:        Ok = AssembleSection_Token_NibbleImm(Asd,   BlackCPU<int>::OPCODE_INC, -1 );           break;
      case KW_OP_DEC:        Ok = AssembleSection_Token_NibbleImm(Asd,   BlackCPU<int>::OPCODE_DEC, -1 );           break;
      case KW_OP_INCLR:      Ok = AssembleSection_Token_SingleInstruction(Asd, BlackCPU<int>::OPCODE_INCLR);        break;
      case KW_OP_DECLR:      Ok = AssembleSection_Token_SingleInstruction(Asd, BlackCPU<int>::OPCODE_DECLR);        break;
      case KW_OP_ADD:        Ok = AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_ADD_B );   break;
      case KW_OP_SUB:        Ok = AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_SUB_B );   break;
      case KW_OP_AND:        Ok = AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_AND_B );   break;
      case KW_OP_OR:         Ok = AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_OR_B  );   break;
      case KW_OP_XOR:        Ok = AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_XOR_B );   break;
      case KW_OP_ASR:        Ok = AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_ASR_B );   break;
      case KW_OP_LSL:        Ok = AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_LSL_B );   break;
      case KW_OP_LSR:        Ok = AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_LSR_B );   break;
      case KW_OP_ROL:        Ok = AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_ROL_B );   break;
      case KW_OP_ROR:        Ok = AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_ROR_B );   break;
      case KW_OP_UMUL:       Ok = AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_UMUL_B );  break;
      case KW_OP_SMUL:       Ok = AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_SMUL_B );  break;
      case KW_OP_CMP:        Ok = AssembleSection_Token_TwoParameterAritmetic(Asd, BlackCPU<int>::OPCODE_CMP_B );   break;
      case KW_OP_JMP:        Ok = AssembleSection_Token_IndirectJump(Asd, BlackCPU<int>::OPCODE_JMP_IND );          break;
      case KW_OP_JSR:        Ok = AssembleSection_Token_IndirectJump(Asd, BlackCPU<int>::OPCODE_JSR_IND );          break;
      case KW_OP_BRA:        Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BRA ); break;
      case KW_OP_BEQ:        Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BEQ ); break;
      case KW_OP_BNE:        Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BNE ); break;
      case KW_OP_BCS:        Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BCS ); break;
      case KW_OP_BCC:        Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BCC ); break;
      case KW_OP_BMI:        Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BMI ); break;
      case KW_OP_BPL:        Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BPL ); break;
      case KW_OP_BVS:        Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BVS ); break;
      case KW_OP_BVC:        Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BVC ); break;
      case KW_OP_BHI_U:      Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BHI_U);break;
      case KW_OP_BHS_U:      Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BHS_U);break;
      case KW_OP_BLS_U:      Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BLS_U);break;
      case KW_OP_BLO_U:      Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BLO_U);break;
      case KW_OP_BHI_S:      Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BHI_S);break;
      case KW_OP_BHS_S:      Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BHS_S);break;
      case KW_OP_BLS_S:      Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BLS_S);break;
      case KW_OP_BLO_S:      Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BLO_S);break;
      case KW_OP_BSO:        Ok = AssembleSection_Token_ConditionalBranch(Asd, Token, BlackCPU<int>::OPCODE_BRANCH_BSO  );break;
      case KW_OP_RTS:        Ok = AssembleSection_Token_SingleInstruction(Asd, BlackCPU<int>::OPCODE_RTS);         break;
      case KW_OP_RTI:        Ok = AssembleSection_Token_SingleInstruction(Asd, BlackCPU<int>::OPCODE_RTI);         break;
      case KW_OP_EXT_BW:     Ok = AssembleSection_Token_OneParameterAritmetic(Asd, BlackCPU<int>::OPCODE_EXT_BW);  break;
      case KW_OP_EXT_BL:     Ok = AssembleSection_Token_OneParameterAritmetic(Asd, BlackCPU<int>::OPCODE_EXT_BL);  break;
      case KW_OP_EXT_WL:     Ok = AssembleSection_Token_OneParameterAritmetic(Asd, BlackCPU<int>::OPCODE_EXT_WL);  break;
      case KW_OP_RSR:        Ok = AssembleSection_Token_NibbleImm(Asd,  BlackCPU<int>::OPCODE_RSR, 0);               break;
      case KW_OP_WSR:        Ok = AssembleSection_Token_NibbleImm(Asd,  BlackCPU<int>::OPCODE_WSR, 0);               break;
      case KW_OP_BCND:       Ok = AssembleSection_Token_BranchCond(Asd, Token, BlackCPU<int>::OPCODE_BCND);  break;
      case KW_OP_BNCND:      Ok = AssembleSection_Token_BranchCond(Asd, Token, BlackCPU<int>::OPCODE_BNCND);  break;


      default: Ok = false; break;
    }

    // Display Line

    if (Asd->AssemblyPass == 2)
    {
      Display_Align(Asd);
      Display_TokenLine(Asd, Token);
      if ((Long)Token.LineCount > Asd->LastPrintedLine)
      {
        Asd->LastPrintedLine = (Long)Token.LineCount;
        printf("%s\n",Asd->OutLine.String);
      }
    }

    return(Ok);
  }

  bool inline AssembleSection_Label(AssemblyData * Asd, ZParser_Token &Token)
  {
    ZParser_Token Token_Separator;
    ULong Value;

    // Separator :

    Asd->Parser.GetToken(Token_Separator);
    if ( Token_Separator.TokenType != ZParser_Token::TK_KEYWORD ) {ThrowError(Asd, &Token_Separator, ERROR_EXPECTEDTWOPOINTSOREQUAL); return(false); }
    switch (Token_Separator.KeyWordId)
    {
      case KW_PNK_TWOPOINTS: Asd->LabelStore.StoreLabel(Token.AlphaValue.String, Asd->Address);
                             break;
      case KW_PNK_EQUAL:     if (!GetExpression(Asd, Value)) return(false);
                             Asd->LabelStore.StoreLabel(Token.AlphaValue.String, Value);
                             break;

      default:               ThrowError(Asd, &Token_Separator, ERROR_EXPECTEDTWOPOINTSOREQUAL); return(false);
    }



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
        case ZParser_Token::TK_KEYWORD: if (!AssembleSection_Token(Asd, Token)) return(false);
                                        break;

        case ZParser_Token::TK_IDENTIFIER: if (!AssembleSection_Label(Asd, Token)) return(false);
                                           break;
        default: return(false);

      }

    }
    return(true);
  }

  public:

  bool Assembly_Pass(AssemblyData * Asd, ULong AssemblyPass)
  {
    bool Result;
    ULong * TotalSize;

    Asd->AssemblyPass = AssemblyPass;
    Asd->Parser.Init(Asd->Text->String, Asd->Text->Len, &Asd->KeyworSet);
    Asd->Address = 0;
    Asd->LastPrintedLine = -1;
    Asd->Stream.OpenWrite();
    Asd->Stream.Put(MulticharConst('Z','B','I','N'));
    TotalSize = Asd->Stream.GetPointer_ULong();
    Asd->Stream.Put((ULong)0);
      OpenNewSection(Asd, MulticharConst('C','O','D','E') ,0);
        Result = AssembleSection(Asd);
      CloseSection(Asd);
    Asd->Stream.Put(MulticharConst('E','N','D','*'));
    *TotalSize = Asd->Stream.GetActualBufferLen() - 8;
    return(Result);
  }

  bool Assemble(ZString & Text,ZString & BinaryOutput, ZCompilationStatus & CompilationStatus)
  {
    AssemblyData AsData, * Asd;
    ZStream_SpecialRamStream Stream;

    // End test

    Asd = &AsData;

    Asd->LabelStore.Clear();
    Asd->Text = &Text;
    RegisterKeywords(&Asd->KeyworSet);
    Asd->OutLine.SetLen(4096);

    Asd->CompilationStatus = & CompilationStatus;

    // Pass 1 : Fill the label table.

    if (!Assembly_Pass(Asd, 1)) return(false);

    // Pass 2 : Final pass with right label values.

    if (!Assembly_Pass(Asd, 2)) return(false);

    // Output

    Asd->Stream.BufferToString(BinaryOutput);

    return(true);
  }


};


#endif /* Z_MACROASSEMBLER_H */
