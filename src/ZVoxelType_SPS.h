/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2016 Laurent Thiebaut & Olivia Merle
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
 * ZVoxelType_SPS.h
 *
 *  Created on: 29 juin 2016
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPE_SPS_H
#define Z_ZVOXELTYPE_SPS_H

//#ifndef Z_ZVOXELTYPE_SPS_H
//  #include "ZVoxelType_SPS.h"
//#endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

class ZVoxelType_SPS : public ZVoxelType
{
 public:
    ZVoxelType_SPS(ULong VoxelType) : ZVoxelType(VoxelType)
    {
      Is_Power = true;
    }

    virtual void Power_Start();
    virtual void Power_End();
    virtual void Power_DoWork();
};

#endif /* Z_ZVOXELTYPE_SPS_H */
