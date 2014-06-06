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
#ifndef Z_ZVOXELTYPE_YELLOWGAS_H
#define Z_ZVOXELTYPE_YELLOWGAS_H

//#ifndef Z_ZVOXELTYPE_YELLOWGAS_H
//#  include "ZVoxelType_YellowGas.h"
//#endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

class ZVoxelType_YellowGas : public ZVoxelType
{
  public:
    ZVoxelType_YellowGas(ULong VoxelType) : ZVoxelType(VoxelType)
    {
      Is_VoxelExtension = true;
      Is_HasAllocatedMemoryExtension = false;
    }

    // Voxel Extensions

    virtual ZVoxelExtension * CreateVoxelExtension(bool IsLoadingPhase = true);

};


#endif /* Z_ZVOXELTYPE_YELLOWGAS_H */
