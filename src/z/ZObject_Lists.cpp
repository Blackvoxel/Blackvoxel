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
 * ZObject_Lists.cpp
 *
 *  Created on: 7 mars 2011
 *      Author: laurent
 */

#include "ZObject_Lists.h"
#include <stdio.h>

ZBasicMemoryPool ZList::ZListBasicMemPool;

ZListItem * ZList::AddToTail(ZObject * Object)
{
  ZListItem * Item;
  ZMemSize EffectiveSize;

  Item = (ZListItem * )MemPool->AllocMem(sizeof(ZListItem), EffectiveSize);
  Item->PoolMemorySize = EffectiveSize;
  Item->Object = Object;

  Item->Next = &Tail;
  Item->Prev = Tail.Prev;
  Tail.Prev->Next = Item;
  Tail.Prev = Item;

  return(Item);
}

ZListItem * ZList::AddToHead(ZObject * Object)
{
  ZListItem * Item;
  ZMemSize EffectiveSize;

  Item = (ZListItem * )MemPool->AllocMem(sizeof(ZListItem), EffectiveSize);
  Item->PoolMemorySize = EffectiveSize;
  Item->Object = Object;

  Item->Next = Head.Next;
  Item->Prev = &Head;
  Head.Next->Prev = Item;
  Head.Next = Item;

  return(Item);
}

void ZList::Dump()
{
  ZListItem * Item;

  Item = Head.Next;
  printf("(List:%016lx",(ZMemSize)this);
  while (Item != &Tail)
  {
    printf("[Item");
    if (Item->GetObject()) printf(":I");
    printf("]");
    Item = Item->Next;
  }
  printf(")\n");
}
