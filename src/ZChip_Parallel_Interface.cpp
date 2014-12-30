/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2014 Laurent Thiebaut & Olivia Merle
 *    Contributions by: d3x0r
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
 * ZChip_Parallel_Interface.cpp
 *
 *  Created on: 26 déc. 2014
 *      Author: laurent
 */

#include "ZChip_Parallel_Interface.h"

#ifndef Z_ZSTREAM_SPECIALRAMSTREAM_H
#  include "z/ZStream_SpecialRamStream.h"
#endif

bool ZChip_Parallel_Interface::Save(ZStream_SpecialRamStream * Stream)
{
  ULong * BlockSize;
  ULong   StartLen;

  BlockSize = Stream->GetPointer_ULong();
  Stream->Put(0u);        // The size of the block (defered storage).
  StartLen = Stream->GetActualBufferLen();
  Stream->Put((UShort)1); // Data Block Version;

  // Storage informations.

  Stream->Put(Data);
  Stream->Put(Direction);
  Stream->Put(Interrupt_EnableMask);
  Stream->Put(Interrupt_Level);
  Stream->Put(Control_Register);
  Stream->Put(In);
  Stream->Put(Out);
  Stream->Put(Out_Changed);

  // Compute Block Len and write it to start of the block.

  *BlockSize = Stream->GetActualBufferLen() - StartLen;

  return(true);
}

bool ZChip_Parallel_Interface::Load(ZStream_SpecialRamStream * Stream)
{
  bool Ok;

  ULong  BlockSize;
  UShort BlockVersion;
  UByte  Temp_Byte;

  Ok = Stream->Get(BlockSize);
  Ok&= Stream->Get(BlockVersion);     if(!Ok) return(false);

  // Check for supported block version. If unsupported new version, throw content and continue with a blank dataset.

  if (BlockVersion!=1) { BlockSize-=2; for(ZMemSize i=0;i<BlockSize;i++) Ok = Stream->Get(Temp_Byte); if (Ok) return(true); else return(false);}

  Ok &= Stream->Get(Data);
  Ok &= Stream->Get(Direction);
  Ok &= Stream->Get(Interrupt_EnableMask);
  Ok &= Stream->Get(Interrupt_Level);
  Ok &= Stream->Get(Control_Register);
  Ok &= Stream->Get(In);
  Ok &= Stream->Get(Out);
  Ok &= Stream->Get(Out_Changed);

  return(Ok);
}
