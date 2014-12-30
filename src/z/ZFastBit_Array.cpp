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
 * ZFastBit_Array.cpp
 *
 *  Created on: 29 déc. 2014
 *      Author: laurent
 */

#include "ZFastBit_Array.h"

#ifndef Z_ZSTREAM_SPECIALRAMSTREAM_H
#  include "ZStream_SpecialRamStream.h"
#endif

bool ZFastBit_Array::Save (ZStream_SpecialRamStream * Stream)
{
  ZMemSize i;
  Stream->Put((UELong)StorageSize);
  for (i=0;i<StorageSize;i++) Stream->Put(Storage[i]);
  return(true);
}

bool ZFastBit_Array::Load (ZStream_SpecialRamStream * Stream, ZMemSize MinimumSize)
{
  ZMemSize i;
  UELong Size;
  bool Ok;

  // Get Size of the stored data

  Ok = Stream->Get(Size);
  StorageSize = (ZMemSize)Size;

  if (MinimumSize && (StorageSize < MinimumSize)) StorageSize = MinimumSize;
  // Reserve appropriate storage for it.

  if (Storage) delete [] Storage;
  Storage = new ULong[StorageSize]; if (!Storage) return(false);

  // Load Data

  for (i=0;i<Size;i++) Ok &= Stream->Get(Storage[i]);

  return(Ok);
}
