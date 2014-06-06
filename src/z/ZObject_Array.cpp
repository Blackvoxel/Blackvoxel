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
/*******************************************************************************
*                                                                              *
*                         GNU GPL V3 Free Software                             *
*                                                                              *
*  (C) 1997-2011 Laurent Thiebaut & Olivia Merle                               *
*                                                                              *
*  This file is part of EXecutePage.                                           *
*                                                                              *
*  EXecutePage is free software: you can redistribute it and/or modify         *
*  it under the terms of the GNU General Public License as published by        *
*  the Free Software Foundation, either version 3 of the License, or           *
*  (at your option) any later version.                                         *
*                                                                              *
*  NodeKiller is distributed in the hope that it will be useful,               *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of              *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
*  GNU General Public License for more details.                                *
*                                                                              *
*  You should have received a copy of the GNU General Public License           *
*  along with EXecutePage.  If not, see <http://www.gnu.org/licenses/>.        *
*                                                                              *
*******************************************************************************/

#include "ZObject_Array.h"

ZObjectArray::ZObjectArray()
{
  ExpandableArray=0;
  MemSize=0;
  MaxIndex=0;
  NavIndex=0;
}

ZObjectArray::~ZObjectArray()
{
  ZSize i;
  
  if (ExpandableArray) 
  {
    for (i=0;i<MaxIndex;i++) if (ExpandableArray[i]) delete ExpandableArray[i];
    delete [] ExpandableArray;
  }
}

void ZObjectArray::Clear()
{
  ZSize i;
  
  if (ExpandableArray) 
  {
    for (i=0;i<MaxIndex;i++) if (ExpandableArray[i]) delete ExpandableArray[i];
    delete ExpandableArray;
    ExpandableArray = 0;
  }
  MemSize=0;MaxIndex=0;
}

ZObject * ZObjectArray::GetEntry(ZSize Index)
{
  if (!ExpandableArray) return(0);
  if (Index>=MaxIndex) return(0);
  return(ExpandableArray[Index]);
}

Bool ZObjectArray::SetEntry(ZSize Index,ZObject & Data)
{
  ZObject ** NewArray;
  ZSize i;
  ZSize NewSize;

  if (!ExpandableArray)
  {
    NewSize = (INITIALSIZE < (Index)) ? Index + INITIALSIZE : INITIALSIZE;
    if (!(ExpandableArray = new ZObject *[NewSize])) return(ZFALSE);
    MemSize = NewSize;
    for (i=0;i<MemSize;i++) ExpandableArray[i]=0;
  }
  if (Index >= MemSize)
  {
    NewSize = ((MemSize * 2) <= (Index)) ? Index + INITIALSIZE : MemSize * 2; 
    if (!(NewArray = new ZObject *[NewSize])) return(ZFALSE);
    for (i=0;i<MemSize;i++) NewArray[i] = ExpandableArray[i];
    for (i=MemSize;i<MemSize*2;i++) NewArray[i] = 0;
    MemSize=NewSize;
    delete ExpandableArray;
    ExpandableArray = NewArray;
  }
  if (ExpandableArray[Index]) delete (ExpandableArray[Index]);
  ExpandableArray[Index]=&Data;
  MaxIndex = ((Index+1)>MaxIndex) ? Index+1:MaxIndex;
  return(ZTRUE);
}

Bool ZObjectArray::Add(ZObject & Data)
{
  return(SetEntry(MaxIndex,Data));
}

Bool ZObjectArray::Enum(ZObject **Entry) 
{
  if (NavIndex<MaxIndex)
  {
    *Entry = ExpandableArray[NavIndex++];
    return (ZTRUE);
  }
  return(ZFALSE);
}

ULong ZObjectArray::GetLastEnumOffset() 
{
  if (!NavIndex) return(0);
  return(NavIndex-1);
}

ULong ZObjectArray::GetCount()
{
  return(MaxIndex);
}

