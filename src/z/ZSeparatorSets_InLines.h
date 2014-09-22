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
 * ZSeparatorSets_InLines.h
 *
 *  Created on: Pre-2000 code.
 *      Author: laurent
 */

// ************************** Inlines **********************

inline ZSeparatorSet::ZSeparatorSet()
{
  ULong i;
  for(i=0;i<256;i++) Array[i]=0;
}

inline ZSeparatorSet::ZSeparatorSet(ZSeparatorSet &Set)
{
  ULong i;
  for(i=0;i<256;i++) Array[i] = Set.Array[i];
}

inline void ZSeparatorSet::Include(char Separator)
{
  Array[(UByte)Separator]=-1;
}

inline void ZSeparatorSet::Exclude(char Separator)
{
  Array[(UByte)Separator]=0;
}

inline void ZSeparatorSet::IncludeRange(char Start,char End) 
{
  ULong i;
  if ((UByte)Start <= (UByte)End)
  {
    for(i=(ULong)Start;i<=(ULong)End;i++) Array[i]=-1;
  }
}

inline void ZSeparatorSet::ExcludeRange(char Start,char End) 
{
  ULong i;
  if ((UByte)Start<= (UByte)End)
  {
    for(i=(ULong)Start;i<=(ULong)End;i++) Array[i]=0;
  }
}

inline ZSeparatorSet &  ZSeparatorSet::operator=(ZSeparatorSet &Set)
{
  ULong i;
  for(i=0;i<256;i++) Array[i] = Set.Array[i];
  return(*this);
}

inline ZSeparatorSet  ZSeparatorSet::operator+(ZSeparatorSet &Set)
{
  ULong i;
  ZSeparatorSet NewSet;
  for(i=0;i<256;i++) NewSet.Array[i] = Array[i]|Set.Array[i];
  return(NewSet);
}

inline ZSeparatorSet ZSeparatorSet::operator-(ZSeparatorSet &Set)
{
  UShort i;
  ZSeparatorSet NewSet;
  for(i=0;i<256;i++) NewSet.Array[i] = Array[i]&(~Set.Array[i]);
  return(NewSet);
}

inline ZSeparatorSet ZSeparatorSet::operator+(char Separator)
{
  ZSeparatorSet NewSet(*this);
  NewSet.Array[(UByte)Separator] = -1;
  return(NewSet);
}

inline ZSeparatorSet ZSeparatorSet::operator-(char Separator)
{
  ZSeparatorSet NewSet(*this);
  NewSet.Array[(UByte)Separator] = 0;
  return(NewSet);
}

inline ZSeparatorSet ZSeparatorSet::operator&(ZSeparatorSet &Set)
{
  UShort i;
  ZSeparatorSet NewSet;
  for(i=0;i<256;i++) NewSet.Array[i] = Array[i] & Set.Array[i];
  return(NewSet);
}

inline ZSeparatorSet ZSeparatorSet::operator!()
{
  UShort i;
  ZSeparatorSet NewSet;
  for(i=0;i<256;i++) NewSet.Array[i] = ~Array[i];
  return(NewSet);
}

inline ZSeparatorSet ZSeparatorSet::operator~()
{
  UShort i;
  ZSeparatorSet NewSet;
  for(i=0;i<256;i++) NewSet.Array[i] = ~Array[i];
  return(NewSet);
}

inline ZSeparatorSet & ZSeparatorSet::operator+=(char Separator)
{
  Array[(UByte)Separator] = -1;
  return(*this);
}

inline ZSeparatorSet & ZSeparatorSet::operator-=(char Separator)
{
  Array[(UByte)Separator] = 0;
  return(*this);
}

inline ZSeparatorSet & ZSeparatorSet::operator+=(ZSeparatorSet &Set)
{
  UShort i;
  for(i=0;i<256;i++) Array[i] |= Set.Array[i];
  return(*this);
}

inline ZSeparatorSet &  ZSeparatorSet::operator-=(ZSeparatorSet &Set)
{
  UShort i;
  for(i=0;i<256;i++) Array[i] = Array[i] &(~Set.Array[i]);
  return(*this);
}

inline Bool ZSeparatorSet::IsIncluded   (char Char)
{
  return( Array[(UByte)Char] );
}

inline Bool ZSeparatorSet::IsNotIncluded(char Char)
{
  return (!Array[(UByte)Char]);
}

inline Bool ZSeparatorSet::operator==   (char Char)
{
  return( Array[(UByte)Char] );
}

inline Bool ZSeparatorSet::operator!=   (char Char) 
{
  return(!Array[(UByte)Char] );
}

inline void ZSeparatorSet::ClearAll()
{
  UShort i;
  for(i=0;i<256;i++) Array[i] = 0;
}

inline void ZSeparatorSet::SetAll()
{
  UShort i;
  for(i=0;i<256;i++) Array[i] = -1;
}
