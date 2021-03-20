/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2021 Laurent Thiebaut & Olivia Merle
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
 * ZVoxelExtension_LiftL.h
 *
 *  Created on: 21 févr. 2021
 *      Author: laurent
 */

#ifndef Z_ZVOXELEXTENSION_LIFTL_H
#define Z_ZVOXELEXTENSION_LIFTL_H

//#ifndef Z_ZVOXELEXTENSION_LIFTL_H
//#  include "ZVoxelExtension_LiftL.h"
//#endif

#ifndef Z_ZVOXELEXTENSION_H
#  include "ZVoxelExtension.h"
#endif

class ZVoxelExtension_LiftL : public ZVoxelExtension
{
  public:
  enum {Storage_NumSlots = 5};

  UShort VoxelType[Storage_NumSlots];
  ULong  VoxelQuantity[Storage_NumSlots];

  public:

  ZVoxelExtension_LiftL();

  virtual ZVoxelExtension * GetNewCopy();

  virtual ULong GetExtensionID()    { return( MulticharConst('L','I','F','T') ); }

  virtual bool Save(ZStream_SpecialRamStream * Stream);
  virtual bool Load(ZStream_SpecialRamStream * Stream);

  virtual bool FindSlot(UShort VoxelType, ULong &Slot);
  virtual bool FindFreeSlot(ULong &Slot);

  virtual ULong StoreBlocks(UShort VoxelType, ULong VoxelQuantity);

};



#endif /* Z_ZVOXELEXTENSION_LIFTL_H */
