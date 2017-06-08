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
#ifndef Z_ZSTRING_H
#define Z_ZSTRING_H

//#ifndef Z_ZSTRING_H
//#  include "ZString.h"
//#endif


#ifndef Z_ZMEMPOOL_H
#  include "ZMemPool.h"
#endif

#ifndef Z_ZMEMPOOL_OPTIMIZED_H
#include "ZMemPool_Optimized.h"
#endif

#ifndef Z_ZNUMBERFORMAT_H
#  include "ZNumberFormat.h"
#endif

#define ZSTRING_CONVERSIONSIZE 128
#define ZSTRING_MINIMUMSTRINGSIZE 256

class ZString
{
  public:

    char * String;
    ZMemSize Len;
    ZMemSize MemLen;
    ZMemoryPool * MemPool;

  private:
    struct{ UShort  Display_TrailZero:1;
            UShort  Display_AlwaysShowSign:1;
            UShort  Base:6;
    } Settings;


  // Private Utility functions
    inline ZMemSize   GetOldCStringLen(const char * Texte)   { ZMemSize Len; for (Len=0;Texte[Len];Len++); return(Len); }
    char *            ULongToAsc(ULong Num, ZMemSize &EffectiveMemSize, ZMemSize & Len);
    char *            LongToAsc(long Num,   ZMemSize &EffectiveMemSize, ZMemSize & Len);
    bool inline       AtoULong(char * Txt, ZMemSize &Len, ULong &Value);
    bool inline       AtoLong(char * Txt, ZMemSize &Len, Long &Value);

    struct ZStringBaseData{ULong Base;ULong Divider;};
    static ZStringBaseData _ZStringBaseConvert[];
    static char * _ZStringCharTable;
    static UByte DigitCount_Unsigned[];
    static UByte DigitCount_Signed[];

    static unsigned char  _ZStringLookupTable[];

  public:

    void Set_DisplayBase(UShort Base)
    {
      Settings.Base = Base;
    }

    void Set_DisplayTrailingZero(bool Flag = true)
    {
      Settings.Display_TrailZero = (Flag) ? 0:1;
    }

    void Set_DisplayAlwaysShowSign(bool Flag = true)
    {
      Settings.Display_AlwaysShowSign = (Flag) ? 1:0;
    }

    void SetMemPool(ZMemoryPool * MemPool) { this->MemPool = MemPool; }

    inline void RaiseMem_DiscardContent( ZMemSize NewSize)
    {
      NewSize ++;
      if (NewSize < ZSTRING_MINIMUMSTRINGSIZE) NewSize = ZSTRING_MINIMUMSTRINGSIZE;
      if (MemLen>0) MemPool->FreeMem(String,MemLen);
      String = (char *)MemPool->AllocMem(NewSize,NewSize);
      MemLen = NewSize;
    }

    inline void RaiseMem_GetOldContent( ZMemSize NewSize )
    {
      char * NewString;
      ZMemSize i;

      NewSize ++;
      if (NewSize < ZSTRING_MINIMUMSTRINGSIZE) NewSize = ZSTRING_MINIMUMSTRINGSIZE;
      NewString = (char *)MemPool->AllocMem(NewSize,NewSize);
      for(i=0;i<Len;i++) NewString[i] = String[i];
      //String[i]=0;
      NewString[i]=0;
      if (MemLen) MemPool->FreeMem(String,MemLen);
      String = NewString;
      MemLen = NewSize;
    }

    void SetLen( ZMemSize NewSize )
    {
      if (NewSize >= this->MemLen) RaiseMem_GetOldContent(NewSize);
      Len = NewSize;
      String[Len]=0;
    }

    void Fill(char c)
    {
      ZMemSize i;

      for (i=0;i<Len;i++) String[i]=c;
    }

    void PadUptoLen(char c, ZMemSize PadLen)
    {
      ZMemSize PadCount;
      ZMemSize NewSize;

      if (Len >= PadLen) return;
      PadCount = PadLen - Len;
      NewSize = Len+PadLen+1;
      if ( NewSize >= MemLen) RaiseMem_GetOldContent(NewSize);
      while (PadCount--) String[Len++] = c;
      String[Len]=0;
    }

    void PadOnLeftUptoLen(char c, ZMemSize PadLen)
    {
      ZMemSize PadCount;
      ZMemSize NewSize, i, s, d;

      if (Len >= PadLen) return;
      PadCount = PadLen - Len;
      NewSize = Len+PadLen+1;
      if ( NewSize >= MemLen) RaiseMem_GetOldContent(NewSize);
      s=Len;d=Len+PadCount;i = Len;
      Len+=PadCount;
      while (i--) {String[--d] = String[--s]; }
      for (i=0; PadCount--;i++) String[i]=c;
      String[Len]=0;
    }

    void AdjustLenToZeroTerminated()
    {
      ZMemSize i;

      for(i=0;i<Len;i++) if (String[i]==0) {Len=i;return;}
    }

    ZString();


    ZString( const char * SourceString );
    ZString( const ZString & Str );
    ZString( const float Number );
    ZString( const ULong Number );
    ZString( const Long Number );


    ZString & operator = (const char * Str);
    ZString & operator = (const ZString & Str);
    ZString & operator = (const float Number);
    ZString & operator = (const double Number);
    ZString & operator = (const ULong Number);
    ZString & operator = (const Long  Number);

    bool      operator == (const char * Str);
    bool      operator == (const ZString & Str);
    bool      operator != (const char * Str);
    bool      operator != (const ZString & Str);

    ZString & operator << (ZString const & Str);
    ZString & operator << (ULong const Number);
    ZString & operator << (Long const Number);
    ZString & operator << (bool const Boolean);
    ZString & operator << (double const Number);
    ZString & operator << (char const * String );

    // Specific fast functions (for video game).

    ZString & Append_pchar(char const * Str);
    ZString & Append_ULong(ULong Number);
    ZString & Append_Long(Long Number );
    ZString & Append_char(char c);
    ZString & Append_CStringPart(char const * Str, ZMemSize PartLen);
    ZString & Append_CStringUpToEOL(char const * Str);
    ZString & Append_Formated_Long (Long Number , ZNumberFormat * Format);
    ZString & Append_Formated_ULong(ULong Number, ZNumberFormat * Format);
    ZString & Append_Formated_Float(float Number, ZNumberFormat * Format);
    ZString & Append_BinaryNumber_UByte(UByte Number);
    ZString & Append_BinaryNumber_UShort(UShort Number);
    ZString & Append_BinaryNumber_ULong(ULong Number);
    ZString & Append_HexNumber_Nibble(UByte Number);
    ZString & Append_HexNumber_UByte(UByte Number);
    ZString & Append_HexNumber_UShort(UShort Number);
    ZString & Append_HexNumber_ULong(ULong Number);
    ZString & Append_Double(double Number);
    ZString & Append_Double(double Number, UShort Digits, bool ForceDecimals);
    ZString & Append_Mids(ZString const * InString, ZMemSize Start, ZMemSize End);
    ZString & Append_SubString(ZString const * InString, ZMemSize Start, ZMemSize Len);



    ZString & AddToPath(char const * Str);
    ZString & AddToPath(ZString const & Str);

    inline ZString Path(char const * Str)    { ZString Result; Result = *this; Result.AddToPath( Str ); return(Result); }
    inline ZString Path(ZString const & Str) { ZString Result; Result = *this; Result.AddToPath( Str ); return(Result); }

    ZString Rights(ULong NumChar);
    ZString Lefts(ULong NumChar);

    void    MakeUpper();
    void    MakeLower();

    ULong   GetULong();
    Long    GetLong();
    double  GetDouble();

    Bool    LoadFromFile(const char * FileName);
    bool    SaveToFile  (const char * FileName);

    Bool Split(char c, ZString & FirstPart);
    void StripAll(char c);
    void StripLeading(char c);
    void StripTrailling(char c);

    ZString & SearchReplace(char Searched, char Replacement);
    bool SearchInString_Forward (ZString const * StringToFind, ZMemSize & OutPosition, ZMemSize Start);
    bool SearchInString_Backward(ZString const * StringToFind, ZMemSize & OutPosition, ZMemSize Start);
    bool SearchLineEnd(ZMemSize & OutPos);
    bool SearchNextLineStart(ZMemSize & OutPos);
    bool SearchPredLineStart(ZMemSize & OutPos);
    bool SearchPredLineEnd(ZMemSize & OutPos);
    bool SearchNextLineEnd(ZMemSize & OutPos);
    bool GetKeywordValue(ZString const * Keyword, ZString & KeywordValue);

    inline ZString & Clear() {if ((Len)) {Len=0;*String=0;} return(*this);}
    ZString HexDump(ULong nChar = 10)
    {
      ZString Out;
      ZMemSize Ln;
      ULong Col;
      char * P;


      for (Ln = Len, P = String, Col=0; Ln > 0; Ln--,P++, Col++)
      {
        Out.Append_HexNumber_UByte(*P);
        Out.Append_char(' ');
        if (Col>=nChar) Out.Append_pchar(NewLine());
      }

      return(Out);
    }

#ifdef ZENV_OS_WINDOWS
    inline char * NewLine() { return ( (char *)"\r\n"); }
#else
    inline char * NewLine() { return ( (char *)"\n"); }
#endif

static inline char * GetPathSeparator()
    {
      #ifdef ZENV_OS_WINDOWS
      return((char *)"\\");
      #else
      return((char *)"/");
      #endif
    }

    virtual ~ZString();

};





#endif
