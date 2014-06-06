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
 * ZVoxelExtensionType_VoxelFluid.h
 *
 *  Created on: 28 mars 2014
 *      Author: laurent
 */

#ifndef Z_VOXELEXTENSIONTYPE_VOXELFLUID_H
#define Z_VOXELEXTENSIONTYPE_VOXELFLUID_H

//#ifndef Z_VOXELEXTENSIONTYPE_VOXELFLUID_H
//#  include "ZVoxelExtensionType_VoxelFluid.h"
//#endif

struct ZVoxelExtensionType_VoxelFluid
{
  unsigned int Pressure:16;
  bool Marked:1;
  bool Cycle:1;
};

#endif /* Z_VOXELEXTENSIONTYPE_VOXELFLUID_H */
