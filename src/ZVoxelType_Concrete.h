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
 * ZVoxelType_Concrete.h
 *
 *  Created on: 16 mars 2014
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPE_CONCRETE_H
#define Z_ZVOXELTYPE_CONCRETE_H

//#ifndef Z_ZVOXELTYPE_CONCRETE_H
//#  include "ZVoxelType_Concrete.h"
//#endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

class ZVoxelType_Concrete : public ZVoxelType
{
  public:

    ZVoxelType_Concrete(ULong VoxelType) : ZVoxelType(VoxelType)
    {
      Is_VoxelExtension = true;
      Is_HasAllocatedMemoryExtension = false;
    }

    // Voxel Extensions

    virtual ZVoxelExtension * CreateVoxelExtension(bool IsLoadingPhase = true);
    virtual void  GetBlockInformations(ZVoxelLocation * DestLocation, ZString & Infos);

};


#endif /* Z_ZVOXELTYPE_CONCRETE_H */
