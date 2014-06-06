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
 * ZVoxelExtension_BlastFurnace.h
 *
 *  Created on: 4 févr. 2013
 *      Author: laurent
 */

#ifndef Z_ZVOXELEXTENSION_BLASTFURNACE_H
#define Z_ZVOXELEXTENSION_BLASTFURNACE_H

//#ifndef Z_ZVOXELEXTENSION_BLASTFURNACE_H
//#  include "ZVoxelExtension_BlastFurnace.h"
//#endif

#ifndef Z_ZVOXELEXTENSION_H
#  include "ZVoxelExtension.h"
#endif

class ZVoxelExtension_BlastFurnace : public ZVoxelExtension
{
  public:
    double Quantity_Carbon;
  public:

  ZVoxelExtension_BlastFurnace()
  {
    ExtensionType = Extension_BlastFurnace;

    Quantity_Carbon   = 0.0;
  }

  virtual ZVoxelExtension * GetNewCopy()
  {
    ZVoxelExtension_BlastFurnace * NewCopy;
    NewCopy = new ZVoxelExtension_BlastFurnace(*this);
    return(NewCopy);
  }

  virtual ULong GetExtensionID()
  {
    return( MulticharConst('B','F','U','R') );
  }

  virtual bool Save(ZStream_SpecialRamStream * Stream)
  {
    ULong * ExtensionSize;
    ULong   StartLen;

    ExtensionSize = Stream->GetPointer_ULong();
    Stream->Put(0u);       // The size of the extension (defered storage).
    StartLen = Stream->GetActualBufferLen();
    Stream->Put((UShort)1); // Extension Version;

    // Storage informations.

    Stream->Put(Quantity_Carbon);

    *ExtensionSize = Stream->GetActualBufferLen() - StartLen;

    return(true);
  }

  virtual bool Load(ZStream_SpecialRamStream * Stream)
  {
    bool Ok;
    ULong  ExtensionSize;
    UShort ExtensionVersion;
    UByte  Temp_Byte;

    Ok = Stream->Get(ExtensionSize);
    Ok&= Stream->Get(ExtensionVersion);     if(!Ok) return(false);

    // Check for supported extension version. If unsupported new version, throw content and continue with a blank extension.

    if (ExtensionVersion!=1) { ExtensionSize-=2; for(ZMemSize i=0;i<ExtensionSize;i++) Ok = Stream->Get(Temp_Byte); if (Ok) return(true); else return(false);}

    Stream->Get(Quantity_Carbon);

    return(Ok);
  }

};

#endif /* ZVOXELEXTENSION_BLASTFURNACE_H_ */
