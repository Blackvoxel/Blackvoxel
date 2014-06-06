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
 * ZStream_SpecialRamStream.cpp
 *
 *  Created on: 2 nov. 2011
 *      Author: laurent
 */

#include "ZStream_SpecialRamStream.h"
#include <stdio.h>


ZMonoSizeMemoryPool ZStream_SpecialRamStream::MemPool;

void ZStream_SpecialRamStream::Debug_MemoryOut(Long Offset)
{
  ZMemSize Offset_Neg, Offset_Pos;

  Offset_Neg = Offset_Pos = 0;

  if (Offset<0) Offset_Neg = (ZMemSize) -Offset;
  if (Offset>0) Offset_Pos = (ZMemSize) Offset;

  ULong i,j;
  for (j=0;j<8;j++)
  {
    for (i=0;i<8;i++)
    {
      printf("%08x ",  *((ULong *)(Buffer + Pointer - Offset_Neg + Offset_Pos + (i*4) + (j * 4 * 8) )));
    }
    printf("\n");
  }
}
