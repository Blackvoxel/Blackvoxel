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
 * ZString.cpp
 *
 *  Created on: 28 févr. 2011
 *      Author: laurent
 */

#include "ZString.h"
#include "stdio.h"


//ZBasicMemoryPool ZStringBasicMemoryPool;

ZMemPool_Optimized ZStringBasicMemoryPool;

ZMemoryPool * ZStringCurrentMemoryPool=0;

ZString::ZStringBaseData ZString::_ZStringBaseConvert[] =
{
  {0,0},{1,0},
  {2,0x80000000},{3,0xcfd41b91},{4,0x40000000},{5,0x48c27395},
  {6,0x81bf1000},{7,0x75db9c97},{8,0x40000000},{9,0xcfd41b91},
  {10,0x3b9aca00},{11,0x8c8b6d2b},{12,0x19a10000},{13,0x309f1021},
  {14,0x57f6c100},{15,0x98c29b81},{16,0x10000000},{17,0x18754571},
  {18,0x247dbc80},{19,0x3547667b},{20,0x4c4b4000},{21,0x6b5a6e1d},
  {22,0x94ace180},{23,0xcaf18367},{24,0xb640000},{25,0xe8d4a51},
  {26,0x1269ae40},{27,0x17179149},{28,0x1cb91000},{29,0x23744899},
  {30,0x2b73a840},{31,0x34e63b41},{32,0x40000000},{33,0x4cfa3cc1},
  {34,0x5c13d840},{35,0x6d91b519},{36,0x81bf1000}
};

char * ZString::_ZStringCharTable = (char *)"0123456789ABCDEFGHiJKLMNoPQRSTUVWXYZ";

unsigned char ZString::_ZStringLookupTable[] =
{
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,
  -1,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
  25,26,27,28,29,30,31,32,33,34,35,-1,-1,-1,-1,-1,
  -1,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
  25,26,27,28,29,30,31,32,33,34,35,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
};

char *  ZString::ULongToAsc(ULong Num, ZMemSize &EffectiveMemSize, ZMemSize &Len)
{
  char * Buffer;
  ULong Divider,Base;
  Bool TrailFlag;

  Base = Settings.Base;
  if (Base <2 || Base>36) Base = 10;
  Divider = _ZStringBaseConvert[Settings.Base].Divider;
  Buffer = (char *)MemPool->AllocMem(ZSTRING_CONVERSIONSIZE,EffectiveMemSize);

  Len=0;TrailFlag=(Bool)Settings.Display_TrailZero;
  if (Settings.Display_SignAnyway) Buffer[Len++]='+';
  if (!Num && TrailFlag) {Buffer[Len++] = '0';Buffer[Len]=0;return(Buffer);}
  do
  {
    if ((Buffer[Len]=_ZStringCharTable[Num/Divider])!='0') TrailFlag = false;
    if (!TrailFlag) Len++;
    Num%=Divider;
    Divider/=Base;
  } while (Divider);
  Buffer[Len]=0;
  return(Buffer);
}

char *  ZString::LongToAsc(long Num, ZMemSize &EffectiveMemSize, ZMemSize & Len)
{
  char * Buffer;
  ULong Divider, Base;
  Bool TrailFlag;

  Base = Settings.Base;
  if (Base <2 || Base>36) Base = 10;
  Divider = _ZStringBaseConvert[Settings.Base].Divider;
  Buffer = (char *)MemPool->AllocMem(ZSTRING_CONVERSIONSIZE,EffectiveMemSize);
  Len=0;TrailFlag=(Bool)Settings.Display_TrailZero;
  if      (Num<0)                  {Buffer[Len++]='-'; Num= -Num;}
  else if (Settings.Display_SignAnyway) Buffer[Len++]='+';
  if (!Num && TrailFlag) {Buffer[Len++] = '0';Buffer[Len]=0;return(Buffer);}
  do
  {
    if ((Buffer[Len]=_ZStringCharTable[Num/Divider])!='0') TrailFlag = false;
    if (!TrailFlag) Len++;
    Num%=Divider;
    Divider/=Base;
  } while (Divider);
  Buffer[Len]=0;
  return(Buffer);
}

bool inline ZString::AtoULong(char * Txt,ZMemSize &Len, ULong &Value)
{
  ULong Val=0;
  ULong Base = Settings.Base;
  Len = 0;
  while(*Txt==' ') {Txt++;Len++;}
  if (*Txt=='+') {Txt++;Len++;}
  if ((_ZStringLookupTable[(unsigned char)*Txt]==0xFF) ||
      (_ZStringLookupTable[(unsigned char)*Txt]>=Base)    ) return(false);
  while( (_ZStringLookupTable[(unsigned char)*Txt] !=0xFF) &&
         (_ZStringLookupTable[(unsigned char)*Txt] <Base) )
  {
    Val*=Base;
    Val+=_ZStringLookupTable[(unsigned char)*(Txt++)];Len++;
  }
  Value=Val;
  return(true);
}

bool inline ZString::AtoLong(char * Txt,ZMemSize &Len, Long &Value)
{
  long Val=0;
  ULong Base = Settings.Base;
  Len=0;
  Bool SignMoins = false;
  while(*Txt==' ') {Txt++;Len++;}
  if (*Txt=='+') {Txt++;Len++;}
  if (*Txt=='-') {Txt++;Len++;SignMoins = true;}

  if ((_ZStringLookupTable[(unsigned char)*Txt]==0xFF) ||
      (_ZStringLookupTable[(unsigned char)*Txt]>=Base)    ) return(false);
  while( (_ZStringLookupTable[(unsigned char)*Txt] !=0xFF) &&
         (_ZStringLookupTable[(unsigned char)*Txt] <Base) )
  {
    Val*=Base;
    Val+=_ZStringLookupTable[(unsigned char)*(Txt++)];Len++;
  }
  if (SignMoins) Val=-Val;
  Value=Val;
  return(true);
}

ULong ZString::GetULong()
{
  ZMemSize Len;
  char * Txt;
  ULong Result;
  Txt = String;
  if (!AtoULong(Txt,Len,Result)) return(0);
  return(Result);
}

Long ZString::GetLong()
{
  ZMemSize Len;
  char * Txt;
  Long Result;
  Txt = String;
  if (!AtoLong(Txt,Len,Result)) return(0);
  return(Result);
}

double ZString::GetDouble()
{
  double Result;

  if (1!=sscanf(String,"%lf",&Result)) return(0.0);

  return(Result);
}

ZString::ZString()
{
  if (ZStringCurrentMemoryPool == 0) ZStringCurrentMemoryPool = &ZStringBasicMemoryPool;
  MemPool = ZStringCurrentMemoryPool;

  String = (char *)"";
  Len = 0;
  MemLen = 0;
  Settings.Display_TrailZero = 1;
  Settings.Display_SignAnyway= 0;
  Settings.Base = 10;
}

ZString::~ZString()
{
  if (MemLen>0) {MemPool->FreeMem(String,MemLen); MemLen = 0; Len = 0;}
}

ZString::ZString( const char * SourceString )
{
  ZMemSize i, StrLen;

  if (ZStringCurrentMemoryPool == 0) ZStringCurrentMemoryPool = &ZStringBasicMemoryPool;
  MemPool = ZStringCurrentMemoryPool;

  MemLen = 0;
  StrLen = GetOldCStringLen(SourceString);
  RaiseMem_DiscardContent(StrLen);

  for (i=0;i<StrLen;i++) String[i]=SourceString[i];
  String[i]=0;
  Len = StrLen;

  Settings.Display_TrailZero = 1;
  Settings.Display_SignAnyway= 1;ZString::
  Settings.Base = 10;
}

ZString::ZString( const ZString & Str )
{
  ZMemSize i;

  if (ZStringCurrentMemoryPool == 0) ZStringCurrentMemoryPool = &ZStringBasicMemoryPool;
  MemPool = ZStringCurrentMemoryPool;

  MemLen = 0;
  RaiseMem_DiscardContent(Str.Len);
  for(i=0;i<Str.Len;i++) String[i]=Str.String[i];
  String[i] = 0;
  Len = i;

  Settings.Display_TrailZero = 1;
  Settings.Display_SignAnyway= 0;
  Settings.Base = 10;
}

ZString::ZString( const float Number )
{
  if (ZStringCurrentMemoryPool == 0) ZStringCurrentMemoryPool = &ZStringBasicMemoryPool;
  MemPool = ZStringCurrentMemoryPool;

  MemLen = 0;
  RaiseMem_DiscardContent(ZSTRING_CONVERSIONSIZE-1);

  sprintf(String,"%f",Number);
  Len = GetOldCStringLen(String);

  Settings.Display_TrailZero = 1;
  Settings.Display_SignAnyway= 0;
  Settings.Base = 10;
}

ZString::ZString( const ULong Number )
{
  if (ZStringCurrentMemoryPool == 0) ZStringCurrentMemoryPool = &ZStringBasicMemoryPool;
  MemPool = ZStringCurrentMemoryPool;
  MemLen = 0;
  Settings.Display_TrailZero = 1;
  Settings.Display_SignAnyway= 0;
  Settings.Base = 10;

  String = ULongToAsc(Number, MemLen, Len);
}

ZString::ZString( const Long Number )
{
  if (ZStringCurrentMemoryPool == 0) ZStringCurrentMemoryPool = &ZStringBasicMemoryPool;
  MemPool = ZStringCurrentMemoryPool;

  MemLen = 0;


  Settings.Display_TrailZero = 1;
  Settings.Display_SignAnyway= 0;
  Settings.Base = 10;

  String = LongToAsc(Number, MemLen, Len);
}

ZString & ZString::operator = (const char * Str)
{
  ZMemSize StrLen;
  ZMemSize i;

  StrLen = GetOldCStringLen(Str);

  if (MemLen<=StrLen) RaiseMem_DiscardContent(StrLen);

  for (i=0;i<StrLen;i++) String[i]=Str[i];
  String[i]=0;
  Len = i;

  return (*this);
}

ZString & ZString::operator = (const ZString & Str)
{
  ZMemSize i;

  if (MemLen<=Str.Len)RaiseMem_DiscardContent(Str.Len);
  for (i=0;i<Str.Len;i++) String[i] = Str.String[i];
  String[i] = 0;
  Len = i;

  return(*this);
}

ZString & ZString::operator = (const float Number)
{
  if (MemLen<ZSTRING_CONVERSIONSIZE) RaiseMem_DiscardContent(ZSTRING_CONVERSIONSIZE);
  sprintf(String, "%f",Number);
  Len = GetOldCStringLen(String);
  return(*this);
}

ZString & ZString::operator = (const double Number)
{
  if (MemLen<ZSTRING_CONVERSIONSIZE) RaiseMem_DiscardContent(ZSTRING_CONVERSIONSIZE);
  sprintf(String, "%lf",Number);
  Len = GetOldCStringLen(String);
  return(*this);
}

ZString & ZString::operator = (const ULong Number)
{
  if (MemLen) MemPool->FreeMem(String,MemLen);
  String = ULongToAsc(Number, MemLen, Len);
  return(*this);
}

ZString & ZString::operator = (const Long  Number)
{
  if (MemLen) MemPool->FreeMem(String,MemLen);
  String = LongToAsc(Number, MemLen, Len);
  return(*this);
}

bool ZString::operator == (const char * Str)
{
  ZMemSize i;

  for (i=0; i<Len; i++) if (String[i]!=Str[i]) return(false);
  if (Str[i]!=0) return(false);
  return(true);
}

bool ZString::operator == (const ZString & Str)
{
  ZMemSize i;

  if (Len!=Str.Len) return(false);
  for(i=0;i<Len;i++) if (String[i]!=Str.String[i]) return(false);
  return(true);
}

bool ZString::operator != (const char * Str)
{
  ZMemSize i;

   for (i=0; i<Len; i++) if (String[i]!=Str[i]) return(true);
   if (Str[i]!=0) return(true);
   return(false);
}

bool ZString::operator != (const ZString & Str)
{
   ZMemSize i;

   if (Len!=Str.Len) return(true);
   for(i=0;i<Len;i++) if (String[i]!=Str.String[i]) return(true);
   return(false);
}


ZString & ZString::operator << (const ZString & Str)
{
  ZMemSize TotalLen,i,j;

  TotalLen = Len + Str.Len;

  if (MemLen<=TotalLen) RaiseMem_GetOldContent(TotalLen);

  for(i=Len,j=0;i<TotalLen;i++,j++) String[i]=Str.String[j];
  String[i]=0;
  Len = i;

  return(*this);
}

ZString & ZString::operator << (const ULong Number)
{
  ZString As = Number;

  (*this)<<As;
  return(*this);
}

ZString & ZString::operator << (Long const Number)
{
  ZString As = Number;

  (*this)<<As;
  return(*this);
}

ZString & ZString::operator << (const bool Boolean)
{
  if (Boolean) (*this)<<"true";
  else         (*this)<<"false";
  return(*this);
}

ZString & ZString::operator << (const double Number)
{
  ZMemSize MinMem;

  MinMem = Len + ZSTRING_CONVERSIONSIZE;

  if (MemLen <= MinMem) RaiseMem_GetOldContent(MinMem);
  Len+= (ZMemSize)sprintf(String+Len,"%lf", Number);
  String[Len]=0;

  return(*this);
}

ZString & ZString::operator << (char const * Str )
{
  ZMemSize TotalLen,i,j;

  TotalLen = Len + GetOldCStringLen(Str);

  if (MemLen<=TotalLen) RaiseMem_GetOldContent(TotalLen);

  for(i=Len,j=0;i<TotalLen;i++,j++) String[i]=Str[j];
  String[i]=0;
  Len = i;
  return(*this);
}

ZString & ZString::Append_pchar(char const * Str)
{
  ZMemSize TotalLen,i,j;

  TotalLen = Len + GetOldCStringLen(Str);

  if (MemLen<=TotalLen) RaiseMem_GetOldContent(TotalLen);

  for(i=Len,j=0;i<TotalLen;i++,j++) String[i]=Str[j];
  String[i]=0;
  Len = i;
  return(*this);
}

ZString & ZString::Append_char(char c)
{

  if (MemLen<=(Len+1)) RaiseMem_GetOldContent(MemLen+1);
  String[Len++]=c;
  String[Len]=0;
  return(*this);
}

ZString & ZString::Append_ULong( ULong Number )
{
  char * Buffer;
  ULong Divider,Base;
  Bool TrailFlag;
  ZMemSize CLen;

  Base = Settings.Base;
  if (Base <2 || Base>36) Base = 10;
  Divider = _ZStringBaseConvert[Settings.Base].Divider;

  if (MemLen <= Len + ZSTRING_CONVERSIONSIZE) RaiseMem_GetOldContent(Len + ZSTRING_CONVERSIONSIZE);

  Buffer = &String[Len];

  CLen=0;TrailFlag=(Bool)Settings.Display_TrailZero;
  if (Settings.Display_SignAnyway) Buffer[CLen++]='+';
  if (!Number && TrailFlag) {Buffer[CLen++] = '0';Buffer[CLen]=0;Len+=CLen;return(*this);}
  do
  {
    if ((Buffer[CLen]=_ZStringCharTable[Number/Divider])!='0') TrailFlag = false;
    if (!TrailFlag) CLen++;
    Number%=Divider;
    Divider/=Base;
  } while (Divider);
  Buffer[CLen]=0;
  Len+=CLen;

  return(*this);
}

ZString & ZString::AddToPath(char const * Str)
{
  ZMemSize TotalLen,i,j;
  char Separator;
  //ZMemSize StringLen;

#ifdef ZENV_OS_WINDOWS
  Separator = '\\';
#else
  Separator = '/';
#endif

  TotalLen = Len + GetOldCStringLen(Str);;
  if (Len>0) TotalLen ++;

  if (MemLen<=TotalLen) RaiseMem_GetOldContent(TotalLen);

  if (Len>0) String[Len++] = Separator;

  for(i=Len,j=0;i<TotalLen;i++,j++) String[i]=Str[j];
  String[i]=0;
  Len = i;
  return(*this);
}

ZString & ZString::AddToPath(ZString const & Str)
{
  ZMemSize TotalLen,i,j;
  char Separator;


#ifdef ZENV_OS_WINDOWS
  Separator = '\\';
#else
  Separator = '/';
#endif

  TotalLen = Len + Str.Len;
  if (Len>0) TotalLen++;

  if (MemLen<=TotalLen) RaiseMem_GetOldContent(TotalLen);

  if (Len>0) String[Len++] = Separator;

  for(i=Len,j=0;i<TotalLen;i++,j++) String[i]=Str.String[j];
  String[i]=0;
  Len = i;
  return(*this);

}


Bool ZString::LoadFromFile(const char * FileName)
{
  FILE * Fl;
  size_t ReadSize, TrueRead;
  ZMemSize TotalSize;

  if ((Fl = fopen(FileName, "rb")) == 0) return(false);

  ReadSize = 1024;
  TotalSize = 0;
  RaiseMem_DiscardContent(ReadSize);
  while ( 0!=(TrueRead = fread(String+TotalSize, 1,ReadSize-1,Fl)) )
  {
    TotalSize += TrueRead;
    if (TrueRead < (ReadSize-1)) break;
    ReadSize <<= 1;
    RaiseMem_GetOldContent(ReadSize);
  }
  Len = TotalSize;
  String[Len]=0;
  fclose(Fl);

  return(true);
}

bool ZString::SaveToFile  (const char * FileName)
{
  FILE * Fl;
  size_t WriteSize;

  if ((Fl = fopen(FileName, "wb")) == 0) return(false);
  WriteSize = fwrite(String,Len,1, Fl);
  fclose(Fl);

  if (WriteSize != Len) return(false);
  return(true);
}

Bool ZString::Split(char c, ZString & FirstPart)
{
  ZMemSize MatchPos,i,j;
  Bool Match;

  for (MatchPos=0, Match=false;MatchPos<Len;MatchPos++)
  {
    if (c == String[MatchPos]) {Match = true; break;}
  }
  if (!Match) return(false);

  if (FirstPart.MemLen <= MatchPos) FirstPart.RaiseMem_DiscardContent(MatchPos);
  for(i=0;i<MatchPos;i++) FirstPart.String[i]=String[i];
  FirstPart.String[i]=0;
  FirstPart.Len = MatchPos;
  MatchPos++;
  for (i=MatchPos,j=0;i<Len;i++,j++) String[j] = String[i];
  String[j]=0;
  Len -= MatchPos;
  return(true);
}

void ZString::StripAll(char c)
{
  ZMemSize i, NewSize;

  for (i=0, NewSize=0;i<Len;i++)
  {
    if (String[i]!=c) String[NewSize++] = String[i];
  }
  Len = NewSize;
}
void ZString::StripLeading(char c)
{
  ZMemSize i,j, MatchPoint;

  for (MatchPoint=0;MatchPoint<Len;MatchPoint++) if (String[MatchPoint]!=c) break;

  for (i=MatchPoint,j=0;i<Len;i++,j++) String[j]=String[i];
  String[j]=0;
  Len-=MatchPoint;
}

ZString ZString::Rights(ULong NumChar)
{
  ZString Result;
  ULong RLen,i,CharPos;

  RLen = (NumChar>Len) ? Len : NumChar;
  if (Result.MemLen<=RLen)
  {
    Result.RaiseMem_DiscardContent(RLen);
  }
  CharPos = Len-RLen;
  for (i=0;i<RLen;i++) Result.String[i] = String[CharPos+i];
  Result.String[i]=0;
  Result.Len = RLen;

  return(Result);
}

ZString ZString::Lefts(ULong NumChar)
{
  ULong RLen,i;
  ZString Result;

  RLen = (Len < NumChar) ? Len : NumChar;
  if (Result.MemLen <= RLen)
  {
    Result.RaiseMem_DiscardContent(RLen);
  }
  for (i=0;i<RLen;i++) Result.String[i] = String[i];
  Result.String[i] = 0;
  Result.Len = RLen;
  return(Result);
}

void ZString::MakeUpper()
{
  ZMemSize i;
  char c;

  for (i=0;i<Len;i++)
  {
    c = String[i];

    if (c>= 'a' && c<='z') c -= ('a'-'A');

    String[i] = c;
  }
}

void ZString::MakeLower()
{
  ZMemSize i;
  char c;

  for (i=0;i<Len;i++)
  {
    c = String[i];

    if (c>= 'A' && c<='Z') c += ('a'-'A');

    String[i] = c;
  }
}

