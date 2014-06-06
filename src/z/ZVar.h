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
 * ZVar.h
 *
 *  Created on: 4 nov. 2013
 *      Author: laurent
 */

#ifndef Z_VAR_H
#define Z_VAR_H

//#ifndef Z_VAR_H
//#  include "ZVar.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

#ifndef Z_ZSTRING_H
#  include "ZString.h"
#endif

class ZVar
{
  public:
    enum { TYPE_NONE, TYPE_INT, TYPE_UINT, TYPE_UINT8, TYPE_INT8, TYPE_UINT16, TYPE_INT16, TYPE_UINT32, TYPE_INT32, TYPE_UINT64, TYPE_INT64, TYPE_FLOAT, TYPE_DOUBLE, TYPE_BOOL, TYPE_CSTRING, TYPE_ZSTRING };

    union
    {
      char   Char;
      UByte  UInt_8;
      Byte   Int_8;
      UShort UInt_16;
      Short  Int_16;
      ULong  UInt_32;
      Long   Int_32;
      UELong UInt_64;
      ELong  Int_64;
      int    Int;
      unsigned int UInt;
      float  singlefloat;
      double doublefloat;
      bool   Bool;
      char * CString;
    } Scalar;
    ZString String;
    UByte VarType;

    ZVar() {VarType = TYPE_NONE;}

    inline void Set_Bool( bool Value )          { VarType = TYPE_BOOL; Scalar.Bool = Value; }
    inline void Set_String(const char * String) { VarType = TYPE_ZSTRING; this->String = String; }
    inline void Set_Int( int Value)             { VarType = TYPE_INT; this->Scalar.Int = Value; }
    inline void Set_Double( double Value)       { VarType = TYPE_DOUBLE; this->Scalar.doublefloat = Value; }
    inline void Set_CString(char * String)      { VarType = TYPE_CSTRING; this->Scalar.CString = String; }

};


#endif /* Z_VAR_H */
