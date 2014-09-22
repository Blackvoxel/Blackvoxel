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
 * ZCharFilter.h
 *
 *  Created on: Pre-2000 code.
 *      Author: laurent
 */

#ifndef Z_ZCHARFILTER_H
#define Z_ZCHARFILTER_H

// #ifndef Z_ZCHARFILTER_H
// #  include "ZCharFilter.h"
// #endif

#ifndef Z_STYPES_H
#  include "ZTypes.h"
#endif

// ***********************************************************************
// *                        Caracter Conversion Filter                   *
// ***********************************************************************

class ZCharFilter 
{
  UByte Array[256];

  public:
    ZCharFilter();
    ZCharFilter( const ZCharFilter & Filter );

    ZCharFilter & operator = (const ZCharFilter & Filter);
    ZCharFilter   operator + (const ZCharFilter & Filter);
    ZCharFilter   operator - (const ZCharFilter & Filter);
    ZCharFilter & operator +=(const ZCharFilter & Filter);
    ZCharFilter & operator -=(const ZCharFilter & Filter);

    void Set(UByte CharToReplace, UByte ValueForThisChar);
    void SetMap(UByte StartChar, UByte EndChar, UByte StartCharReplacement);
    void Fill(UByte StartChar, UByte EndChar, UByte FillChar );

    inline char Convert( char Car ) 
    {
      return(Array[(UByte)Car]);
    }
};


#endif
