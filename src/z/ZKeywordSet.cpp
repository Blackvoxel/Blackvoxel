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
 * ZKeywordSet.cpp
 *
 *  Created on: Pre-2000 code.
 *      Author: laurent
 */


#include "ZKeywordSet.h"


bool ZFastKeyWordSet::Init()
{
  
  return(true);
}

ZFastKeyWordSet::KeyEntry * ZFastKeyWordSet::NewKeyTable()
{
  ULong i;
  KeyEntry * KeyTable;
  if ((KeyTable = new KeyEntry[256]))
  {
    for(i=0;i<256;i++)
    {
      KeyTable[i].NextKey=0;
      KeyTable[i].KeyInfo=0;
    }
  }
  return(KeyTable);
}

bool ZFastKeyWordSet::AddKeyTable(ZKeyWordTable * KeyTable)
{
  if (!KeyTable) return(false);
  while (KeyTable->Name && KeyTable->Value)
  {
    if (!AddKeyWord(KeyTable->Name,KeyTable->Value)) return(false);
    KeyTable++;
  }
  return(true);
}

bool ZFastKeyWordSet::AddKeyWord(const char * KeyWord,ULong AssociatedValue)
{
  KeyEntry * Kp;
  KeyEntry * Key;
  ULong Offset = 0;
  Kp = KeyTable;
  ULong c;

  while(KeyWord[Offset])
  {
    c = KeyWord[Offset];
    Key = &Kp[c];
    if (!KeyWord[Offset+1])
    {
      Key->KeyInfo = AssociatedValue;
      return(true);
    }
    if (Key->NextKey)
    {
      Kp = Key->NextKey;
    }
    else
    {
      if (!(Key->NextKey = NewKeyTable())) return(false);
      Kp = Key->NextKey;
    }
    Offset++;
  }
  return(false);
}

ULong ZFastKeyWordSet::SearchKey(char ** File)
{
  KeyEntry * Kp = KeyTable;
  char * Fp = *File;
  UByte c;
  KeyEntry * Key;
  KeyEntry * LastFound = 0;
  char * LastFoundFp =0;

  while(true)
  {
    c = (UByte)Filter.Convert(*(Fp++));
    Key = &Kp[c];
    if (Key->NextKey)
    {
      if (Key->KeyInfo)
      {
        LastFound = Key;
        LastFoundFp = Fp;
      }
      Kp = Key->NextKey;
    }
    else
    {
      if (Key->KeyInfo)
      {
        *File = Fp;
        return(Key->KeyInfo);
      }
      else
      {
        if (LastFound)
        {
          *File = LastFoundFp;
          return(LastFound->KeyInfo);
        }
        else return(0);
      }
    }
  }
  return(0);
}

void ZFastKeyWordSet::DestroyKey(KeyEntry * Key)
{
  ULong i;
  ZString As;

  As.SetLen(1);
  for (i=0;i<256;i++) if (Key[i].NextKey) DestroyKey(Key[i].NextKey);
  delete [] Key;
}

void ZFastKeyWordSet::DestroyKeyTable()
{
  if (KeyTable)
  {
    DestroyKey(KeyTable);
  }
}
