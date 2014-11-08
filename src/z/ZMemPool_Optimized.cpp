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
 * ZMemPool_Optimized.cpp
 *
 *  Created on: 17 nov. 2011
 *      Author: laurent
 */

#include "ZMemPool_Optimized.h"
#include <stdio.h>

#ifndef __GNUC__
#    define __sync_bool_compare_and_swap(a,b,c) (InterlockedCompareExchangePointer((void*volatile*)a,c,b), (*a)==(c))
#endif

bool ZMemPool_Optimized::Initialized = false;
char ZMemPool_Optimized::BitPosTable[65536];

void * ZMemPool_Optimized::AllocMem( const ZMemSize Size, ZMemSize & NewSize)
{
  PoolNode * NewBlock;

  ULong Index, BitPosition;

  if ((Index = Size >> 16)) BitPosition = 17 + BitPosTable[Index];
  else                      BitPosition = BitPosTable[Size];

  NewSize = 1 << BitPosition;
  #if COMPILEOPTION_BOUNDCHECKING
  if (NewSize < Size ) {MANUAL_BREAKPOINT}
  #endif

  while (true)
  {
    if ( (NewBlock = MemTable[BitPosition] ))
    {
      if (__sync_bool_compare_and_swap(&MemTable[BitPosition],NewBlock,NewBlock->Next))
	  {
        return((void *) (((char *)NewBlock) + 16));
      }
    }
    else
    {
      if ( (NewBlock = (PoolNode *)new char [ NewSize + 16 ]) )
      {
        NewBlock->Size = BitPosition;
        return(((char *)NewBlock)+16);
      }
    }
  }

  return(0);
};

void * ZMemPool_Optimized::AllocMem( const ZMemSize Size )
{
  ZMemSize NewSize;
  PoolNode * NewBlock;

  ULong Index, BitPosition;

  if ((Index = Size >> 16)) BitPosition = 16 + BitPosTable[Index];
  else                      BitPosition = BitPosTable[Size];

  NewSize = 1 << BitPosition;

  while (true)
  {
    if ( (NewBlock = MemTable[BitPosition] ))
    {
      if (__sync_bool_compare_and_swap(&MemTable[BitPosition],NewBlock,NewBlock->Next))
      {
        return((void *) (((char *)NewBlock) + 16));
      }
    }
    else
    {
      if ( (NewBlock = (PoolNode *)new char [ NewSize + 16 ]) )
      {
        NewBlock->Size = BitPosition;
        return(((char *)NewBlock)+16);
      }
    }
  }

  return(0);
};

void ZMemPool_Optimized::FreeMem( void * Block )
{
  PoolNode * MemBlock;
  ULong Index;

  MemBlock = (PoolNode * )((char *)Block - 16);
  Index = MemBlock->Size;

  do
  {
    MemBlock->Next = MemTable[Index];

  } while(! __sync_bool_compare_and_swap( &MemTable[Index] , MemBlock->Next , MemBlock ));

}
