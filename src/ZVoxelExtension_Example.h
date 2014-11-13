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
 * ZVoxelExtension_Example.h
 *
 *  Created on: 11 nov. 2014
 *      Author: laurent
 */

#ifndef Z_ZVOXELEXTENSION_EXAMPLE_H
#define Z_ZVOXELEXTENSION_EXAMPLE_H

//#ifndef Z_ZVOXELEXTENSION_EXAMPLE_H
//#  include "ZVoxelExtension_Example.h"
//#endif

#ifndef Z_ZVOXELEXTENSION_H
#  include "ZVoxelExtension.h"
#endif


class ZVoxelExtension_Example : public ZVoxelExtension
{
  public:

    // Storage example, can be whatever you want.

    ULong Storage;


  // Create a copy of your extension.

  virtual ZVoxelExtension * GetNewCopy()
  {
    return( new ZVoxelExtension_Example(*this) );
  }

  // Constructor

  ZVoxelExtension_Example()
  {
    ExtensionType = Extension_GeneralModing;

    // Init the storage.

    Storage = 0;
  }

  // Extension save.
  //
  // You must add your data fields to be saved.
  // Don't forget to change version when adding fields.

  virtual bool Save(ZStream_SpecialRamStream * Stream)
  {
    ULong * ExtensionSize;
    ULong   StartLen;

    // Standard Header (Don't change it).

    ExtensionSize = Stream->GetPointer_ULong();
    Stream->Put(0u);       // The size of the extension (defered storage).
    StartLen = Stream->GetActualBufferLen();

    // Extension version : Increment it when you need to add/remove/change

    Stream->Put((UShort)1); // Extension Version;

    // Informations storage

    Stream->Put(Storage);

    // Standard header size update (Don't change it).

    *ExtensionSize = Stream->GetActualBufferLen() - StartLen;

    return(true);
  }

  // Extension load.
  //
  // You must add your data fields to be loaded.
  // Dont forget to use version number to keep compatibility when making changes.

  virtual bool Load(ZStream_SpecialRamStream * Stream)
  {
    bool Ok;
    ULong  ExtensionSize;
    UShort ExtensionVersion;

    // Standard header parsing (Don't change it).

    Ok = Stream->Get(ExtensionSize);
    Ok&= Stream->Get(ExtensionVersion);     if(!Ok) return(false);

    // Check for supported extension version. If this version is unsupported, throw content continue with a cleared one.

    if ( ExtensionVersion!=1 ) return( _ThrowExtension(Stream, ExtensionSize) );

    // Load extension data

    Stream->Get(Storage);

    return(Ok);
  }

};


#endif /* Z_ZVOXELEXTENSION_EXAMPLE_H */
