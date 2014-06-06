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
 * ZVoxelType_BlastFurnace.cpp
 *
 *  Created on: 4 févr. 2013
 *      Author: laurent
 */

#ifndef Z_VOXELTYPE_BLASTFURNACE_H
#  include "ZVoxelType_BlastFurnace.h"
#endif

#ifndef Z_ZVOXELEXTENSION_BLASTFURNACE_H
#  include "ZVoxelExtension_BlastFurnace.h"
#endif


ZVoxelExtension * ZVoxelType_BlastFurnace::CreateVoxelExtension(bool IsLoadingPhase)
{
  ZVoxelExtension * NewVoxelExtension = 0;

  NewVoxelExtension = new ZVoxelExtension_BlastFurnace;

  return (NewVoxelExtension);
}



