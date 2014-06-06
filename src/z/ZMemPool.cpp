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
 * ZMemPool.cpp
 *
 *  Created on: 28 févr. 2011
 *      Author: laurent
 */

#include "ZMemPool.h"

ZBasicMemoryPool::ZBasicMemoryPool()
{
  ULong i;

  for(i=0;i<Z_ZBASICMEMORYPOOL_SIZE;i++) MemTable[i]=0;
}

ZBasicMemoryPool::~ZBasicMemoryPool()
{
  ULong i;
  PoolNode * Block, * OldBlock;

  for (i=0;i<Z_ZBASICMEMORYPOOL_SIZE;i++)
  {
    Block = MemTable[i];
    while (Block)
    {
      OldBlock = Block;
      Block = Block->Next;
      delete [] OldBlock;
    }
    MemTable[i]=0;
  }
}


void * ZBasicMemoryPool::AllocMem( const ZMemSize Size, ZMemSize & EffectiveSize)
{
  ULong i;
  void * MemBlock;

  // Debug
/*
  EffectiveSize = Size;
  MemBlock = new char[Size];
  return(MemBlock);
*/
  // End debug

  EffectiveSize = Z_ZBASICMEMORYPOOL_MINMEMSIZE;

  for (i=0;i<Z_ZBASICMEMORYPOOL_SIZE;i++)
  {
    if (EffectiveSize>=Size) break;
    EffectiveSize <<= 1;
  }

  if (MemTable[i])
  {
    MemBlock = MemTable[i];
    MemTable[i] = ((PoolNode *)MemBlock)->Next; // crash ?
  }
  else
  {
    MemBlock = new char [EffectiveSize];
  }

  return(MemBlock);
}

void ZBasicMemoryPool::FreeMem ( void * Block, const ZMemSize BlockSize)
{
  ULong i;
  ZMemSize EffectiveSize;

  // Debug
/*
  delete[] Block;
  return;
*/

  // End debug


  EffectiveSize = Z_ZBASICMEMORYPOOL_MINMEMSIZE;

  for (i=0;i<Z_ZBASICMEMORYPOOL_SIZE;i++)
  {
    if (EffectiveSize>=BlockSize) break;
    EffectiveSize <<= 1;
  }

  ((PoolNode *)Block)->Next = MemTable[i];
  MemTable[i] = (PoolNode *)Block;
}
