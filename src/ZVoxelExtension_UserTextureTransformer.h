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
 * ZVoxelExtension_UserTextureTransformer.h
 *
 *  Created on: 28 févr. 2012
 *      Author: laurent
 */

#ifndef Z_ZVOXELEXTENSION_USERTEXTURETRANSFORMER_H
#define Z_ZVOXELEXTENSION_USERTEXTURETRANSFORMER_H

//#ifndef Z_ZVOXELEXTENSION_USERTEXTURETRANSFORMER_H
//#  include "ZVoxelExtension_UserTextureTransformer.h"
//#endif

#ifndef Z_ZVOXELEXTENSION_H
#  include "ZVoxelExtension.h"
#endif

class ZVoxelExtension_UserTextureTransformer : public ZVoxelExtension
{
  public:
  enum {Storage_NumSlots = 10};


  UShort VoxelType[Storage_NumSlots];
  ULong  VoxelQuantity[Storage_NumSlots];
  UShort ActualVoxel;
  public:

  virtual ZVoxelExtension * GetNewCopy()
  {
    ZVoxelExtension_UserTextureTransformer * NewCopy;
    NewCopy = new ZVoxelExtension_UserTextureTransformer(*this);
    return(NewCopy);
  }

  ZVoxelExtension_UserTextureTransformer()
  {
    ULong i;
    ExtensionType = Extension_UserTextureTransformer;
    for (i=0;i<Storage_NumSlots;i++) {VoxelType[i] = 0; VoxelQuantity[i] = 0;}
    ActualVoxel = 32768;
  }

  virtual ULong GetExtensionID()
  {
    return( MulticharConst('U','T','T','R') );
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
    Stream->Put(ActualVoxel);
    Stream->PutZone(&VoxelType,sizeof(VoxelType));
    Stream->PutZone(&VoxelQuantity,sizeof(VoxelQuantity));

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

    Stream->Get(ActualVoxel);
    Stream->GetZone(&VoxelType, sizeof(VoxelType));
    Stream->GetZone(&VoxelQuantity, sizeof(VoxelQuantity));
    return(Ok);
  }
};

#endif /* Z_ZVOXELEXTENSION_USERTEXTURETRANSFORMER_H */
