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
 * ZVoxelExtension_Egmy_T1.h
 *
 *  Created on: 22 sept. 2013
 *      Author: laurent
 */

#ifndef Z_ZVOXELEXTENSION_EGMY_T1_H
#define Z_ZVOXELEXTENSION_EGMY_T1_H

//#ifndef Z_ZVOXELEXTENSION_EGMY_T1_H
//#  include "ZVoxelExtension_Egmy_T1.h"
//#endif

#ifndef Z_ZVOXELEXTENSION_H
#  include "ZVoxelExtension.h"
#endif

class ZVoxelExtension_Egmy_T1 : public ZVoxelExtension
{
  public:

  ZVector3L TargetLocation;
  UShort    TargetVoxelType; // 0 = No valid target at this time.
  bool      EvasionMode;
  UShort    EvasionCount;

  virtual ZVoxelExtension * GetNewCopy()
  {
    ZVoxelExtension_Egmy_T1 * NewCopy;
    NewCopy = new ZVoxelExtension_Egmy_T1(*this);
    return(NewCopy);
  }

  ZVoxelExtension_Egmy_T1()
  {
    ExtensionType = Extension_Egmy_T1;
    TargetLocation = 0;
    TargetVoxelType = 0;
    EvasionMode = false;
    EvasionCount = 0;
  }

  virtual ULong GetExtensionID()
  {
    return( MulticharConst('E','M','Y','1') );
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
    Stream->Put(TargetLocation.x);
    Stream->Put(TargetLocation.y);
    Stream->Put(TargetLocation.z);
    Stream->Put(TargetVoxelType);

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

    Ok &= Stream->Get(TargetLocation.x);
    Ok &= Stream->Get(TargetLocation.y);
    Ok &= Stream->Get(TargetLocation.z);
    Ok &= Stream->Get(TargetVoxelType);

    return(Ok);
  }

};


#endif /* Z_ZVOXELEXTENSION_EGMY_T1_H */
