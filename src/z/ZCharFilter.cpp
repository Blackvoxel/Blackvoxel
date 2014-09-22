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

#include "ZCharFilter.h"

ZCharFilter::ZCharFilter() 
{ 
  ULong i;
  
  for (i=0;i<256;i++) Array[i]=(UByte)i;
}

ZCharFilter::ZCharFilter( const ZCharFilter & Filter )
{
  ULong i;
  
  for (i=0;i<256;i++) Array[i]=Filter.Array[i];
}

ZCharFilter & ZCharFilter::operator = (const ZCharFilter & Filter)
{
  ULong i;
  
  for (i=0;i<256;i++) Array[i]=Filter.Array[i];
  
  return(*this);
}

ZCharFilter   ZCharFilter::operator + (const ZCharFilter & Filter)
{
  ZCharFilter Result;
  ULong i;
  
  for(i=0;i<256;i++) Result.Array[i] = Filter.Array[ Array[i] ];
  
  return(Result);
}

ZCharFilter   ZCharFilter::operator - (const ZCharFilter & Filter)
{
  ZCharFilter Result;
  ULong i;

  for(i=0;i<256;i++) Result.Array[i] = Array[Filter.Array[i]];

  return(Result);
}

ZCharFilter & ZCharFilter::operator +=(const ZCharFilter & Filter)
{
  ULong i;

  for(i=0;i<256;i++) Array[i] = Filter.Array[ Array[i] ];

  return(*this);
}

ZCharFilter & ZCharFilter::operator -=(const ZCharFilter & Filter)
{
  ULong i;

  for(i=0;i<256;i++) Array[i] = Array[Filter.Array[i]];

  return(*this);
}

void ZCharFilter::Set(UByte CharToReplace, UByte ValueForThisChar)
{
  Array[CharToReplace] = ValueForThisChar;
}

void ZCharFilter::SetMap(UByte StartChar, UByte EndChar, UByte StartCharReplacement)
{
  UByte i,j;

  for (i = StartChar,j = 0; i<=EndChar; i++, j++)
  {
    Array[i] = StartCharReplacement + j;
  }
}

void ZCharFilter::Fill(UByte StartChar, UByte EndChar, UByte FillChar )
{
  UByte i;
  for (i=StartChar;i<=EndChar;i++) Array[i] = FillChar;
}
