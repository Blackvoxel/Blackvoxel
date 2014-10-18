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
 * LParser2.cpp
 *
 *  Created on: Pre-2000 code.
 *      Author: laurent
 */

#include "ZParser.h"
#include <stdio.h>
/* */


// ***********************************************************************
// *                           Parser System                             *
// ***********************************************************************

ZParser::ZParser()
{
  SeparatorSet_StartString += '\"';
  SeparatorSet_EndString   += '\"';
  SeparatorSet_NextLine += '\n'; // Line Feed.
  SeparatorSet_Blank += '\r'; // Carriage Return
  SeparatorSet_Blank += '\n'; // Line Feed
  SeparatorSet_Blank += '\t'; // Horizontal Tabulation
  SeparatorSet_Blank += '\f'; // Form Feed
  SeparatorSet_Blank += ' ';  // Space
  SeparatorSet_Identifier.IncludeRange('0','9');
  SeparatorSet_Identifier.IncludeRange('A','Z');
  SeparatorSet_Identifier.IncludeRange('a','z');
  SeparatorSet_Identifier.Include('_');
  StripBlank = true;
  ParseKeyWords = true;
  ParseConst = true;
  ParseIdent = true;
  NoErrorOnEof = false;
  ErrorPoint = 0;

  Tp = 0;
  LineCount = 0;
  TotalLen = 0;
  LineP = 0;
  KeyWordSet = 0;
  RLen = 0;
  DocumentP = 0;
}

Bool ZParser::Init(char * Text,ULong Len, ZKeyWordSet * KeyWordSet)
{
  LineP = Text;
  Tp    = Text;
  RLen  = Len;
  LineCount = 1;
  this->KeyWordSet = KeyWordSet;
  return(true);
}

Bool ZParser::GetNumConst(Long &Number)
{
  Bool NegSign = false;
  ULong Len = RLen;
  Number= 0;
  char * Txt = Tp;
  if (!Len) return(false);
  if (*Txt=='0' && Txt[1] =='x') return (GetNumHexConst(Number));
  if (*Txt=='-' && Txt[1] =='0' && Txt[2] =='x') return (GetNumHexConst(Number));
  if (*Txt == '-') {NegSign = true;Txt++;Len--;}
  if (*Txt=='.') {Number=1;return(false);}
  do
  {
    if (!Len) {Number=0;return(false);}
    if (*Txt>'9' || *Txt<'0') {Number=0; return(false);}
    Number *= 10;
    Number += *Txt-'0';
    Len--;
    Txt++;
    if (*Txt=='.') {Number=1;return(false);}
  } while (Len && *Txt<='9' && *Txt>='0');
  if (NegSign) Number = -Number;
  RLen = Len;
  Tp = Txt;
  return(true);
}

Bool ZParser::GetNumHexConst(Long &Number)
{
  Bool NegSign = false;
  ULong Len = RLen;
  Number= 0;
  char * Txt = Tp;
  if (!Len) return(false);
  if (*Txt == '-') {NegSign = true;Txt++;Len--;}
  if (*Txt=='.') {Number=1;return(false);}
  if (*Txt != '0' || Txt[1] != 'x') return(false);
  Txt+=2;
  Len-=2;
  do
  {
    if (!Len) {Number=0;return(false);}
    if ( (*Txt>'9' || *Txt<'0') && (*Txt>'F' || *Txt<'A') && (*Txt>'f' || *Txt<'a') ) {Number=0; return(false);}
    Number = Number << 4;
    if (*Txt<='9' && *Txt>='0') Number += *Txt-'0';
    if (*Txt<='F' && *Txt>='A') Number += *Txt-'A' + 10;
    if (*Txt<='f' && *Txt>='a') Number += *Txt-'a' + 10;

    Len--;
    Txt++;
    if (*Txt=='.') {Number=1;return(false);}
  } while (Len && ( (*Txt<='9' && *Txt>='0') ||
                    (*Txt<='F' && *Txt>='A') ||
                    (*Txt<='f' && *Txt>='a')    )  );
  if (NegSign) Number = -Number;
  RLen = Len;
  Tp = Txt;
  return(true);
}

Bool ZParser::GetNumDoubleConst(double &Number)
{
  char * Txt = Tp;
  ULong Len = RLen;
  ZString Num;
  Bool PointFlag = true;
  float NumFloat;

  Number = 0;
  if (!Len) return(false);
  if (*Txt == '-' || *Txt =='+') {Num<<*Txt;Txt++;Len--;}
  do
  {
    if (!Len)      {return(false);}
    if (*Txt=='.' && PointFlag) PointFlag = false;
    else if (*Txt>'9' || *Txt<'0')  return(false);
    Num<<*Txt;
    Txt++;Len--;
  } while (Len && ((*Txt<='9' && *Txt>='0') || (*Txt=='.' && PointFlag)));

  if (1!=sscanf(Num.String, "%f",&NumFloat)) return(false);
  Number = NumFloat;
  RLen = Len;
  Tp   = Txt;
  return(true);
}

Bool ZParser::GetAlphaConst(ZString &Const)
{
  char * Txt = Tp;
  ULong Len = RLen;

  Const.Clear();
  if (!(Len>=2)) return(false);
  if (SeparatorSet_StartString == *Txt)
  {
    Txt++;Len--;
    while(Len)
    {
      if (SeparatorSet_EndString == *Txt)
      {
        Txt++;
        Len--;
        RLen = Len;
        Tp = Txt;
        return(true);
      }
      Const<<*Txt;
      Txt++;Len--;
    }
  }
  return(false);
}

Bool  ZParser::GetAlphaInlineConst(ZString &Const)
{
  char * Txt = Tp;
  ULong Len = RLen;
  char EndChar;

  Const.Clear();
  if (!(Len>=6)) return(false);
  if ( *Txt!='<' || *(Txt+1)!='<') return(false);
  EndChar = *(Txt+2);
  if (EndChar == '<') EndChar = '>';
  Txt+=3;Len-=3;
  while(Len)
  {
    if (EndChar == *Txt)
    {
      if ('>' == *(Txt+1) && '>' == *(Txt+2))
      {
        Txt+=3;
        Len-=3;
        RLen = Len;
        Tp = Txt;
        return(true);
      }
    }
    Const<<*Txt;
    Txt++;Len--;
  }
  return(false);
}


Bool ZParser::GetIdentifier(ZString &Ident)
{
  char * Txt = Tp;
  ULong Len = RLen;
  Ident.Clear();
  if (SeparatorSet_Identifier != *Txt) return(false);
  do
  {
    Ident<<*Txt;
    Len--;
    Txt++;
  } while(Len && SeparatorSet_Identifier==*Txt);
  Tp = Txt;
  RLen = Len;
  return(true);
}

void ZParser::StripLeadBlank()
{
  while (RLen && SeparatorSet_Blank == *Tp)
  {
    if (SeparatorSet_NextLine == *Tp) {LineP = Tp+1;LineCount++;}
    RLen--;Tp++;
  }
}

Bool ZParser::StripNextBlankLine()
{
  ULong cRLen = RLen;
  char * cTp = Tp;

  while (cRLen && SeparatorSet_Blank == *cTp)
  {
    if ( *cTp == 0xD && *(cTp+1) == 0xA) 
    {
      RLen = cRLen;
      Tp = cTp;
      RLen-=2;
      Tp+=2;
      LineP = Tp;LineCount++;
      return(true);
    }
    cRLen--;cTp++;
  }
  return(false);
}

ULong ZParser::GetKeyWord()
{
  ULong KeyWordId;
  char * OldTp;

  OldTp = Tp;
  if ((KeyWordId = KeyWordSet->SearchKey(&Tp)))
  {
    if (RLen >= (ULong)(Tp-OldTp))
    {
      RLen -= Tp-OldTp;
      return(KeyWordId);
    }
    Tp = OldTp;
  }
  return(0);
}

Bool ZParser::GetToken(ZParser_Token &Token)
{
  Long   Number;
  double DNumber;

  // Save position for BackTrack.

  Token.SText      = Tp;
  Token.SLineStart = LineP;
  Token.SLen       = RLen;
  Token.SLineCount = LineCount;

  // Strip blank space.

  if (StripBlank) StripLeadBlank();
  Token.LineStart = LineP;
  Token.LineCount = LineCount;
  Token.TokenStart= Tp;

  // If Eof reached, then return TK_EOF

  if (!RLen)
  {
    Token.TokenType = ZParser_Token::TK_EOF;
    Token.TokenEnd= Tp;
    if (NoErrorOnEof) return(true);   // Warning, Modification must be enabled.
    else              return(false);  // ... do not lead to compatibility problems.
  }

  // Try parsing keywords.

  if (ParseKeyWords)
  {
    if ((Token.KeyWordId = GetKeyWord()))
    {
      Token.TokenType = ZParser_Token::TK_KEYWORD;
      Token.TokenEnd = Tp;
      return(true);
    }
  }

  // Try parsing constants

  if (ParseConst)
  {
    if( GetNumConst(Number) )
    {
      Token.TokenType = ZParser_Token::TK_CONST;
      Token.TokenEnd = Tp;
      Token.ConstType = ZParser_Token::CT_INTEGER;
      Token.ConstValue.ValInteger = Number;
      return(true);
    }
    if (Number==1) //Detected Floating point Constant in Last Step
    {
      if( GetNumDoubleConst(DNumber) )
      {
        Token.TokenType = ZParser_Token::TK_CONST;
        Token.TokenEnd = Tp;
        Token.ConstType = ZParser_Token::CT_DOUBLE;
        Token.ConstValue.ValDouble = DNumber;
        return(true);
      }
    }
    if (GetAlphaConst(Token.AlphaValue))
    {
      Token.TokenType = ZParser_Token::TK_CONST;
      Token.TokenEnd = Tp;
      Token.ConstType = ZParser_Token::CT_ALPHA;
      return(true);
    }
    if (GetAlphaInlineConst(Token.AlphaValue))
    {
      Token.TokenType = ZParser_Token::TK_CONST;
      Token.TokenEnd = Tp;
      Token.ConstType = ZParser_Token::CT_ALPHA;
      return(true);
    }
  }

  // Parsing Identifiers

  if (ParseIdent)
  {
    if (GetIdentifier(Token.AlphaValue))
    {
      Token.TokenType = ZParser_Token::TK_IDENTIFIER;
      Token.TokenEnd = Tp;
      return(true);
    }
  }

  // No Token Matching (NTM);

  Token.TokenType = ZParser_Token::TK_NONE;
  Token.TokenStart = Tp;
  Token.TokenEnd   = Tp+1;
  return(false);
}


