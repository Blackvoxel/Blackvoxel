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
 * ZVoxelType_UserTextureTransformer.cpp
 *
 *  Created on: 28 févr. 2012
 *      Author: laurent
 */

#include "ZVoxelType_UserTextureTransformer.h"
#include "ZGame.h"

#ifndef Z_ZVOXELEXTENSION_USERTEXTURETRANSFORMER_H
#  include "ZVoxelExtension_UserTextureTransformer.h"
#endif


ZVoxelExtension * ZVoxelType_UserTextureTransformer::CreateVoxelExtension(bool IsLoadingPhase)
{
  ZVoxelExtension * NewVoxelExtension = 0;

  NewVoxelExtension = new ZVoxelExtension_UserTextureTransformer;

  return (NewVoxelExtension);
}

void  ZVoxelType_UserTextureTransformer::UserAction_Activate(ZMemSize VoxelInfo, Long x, Long y, Long z)
{
  if (GameEnv->GameWindow_UserTextureTransformer->Is_Shown()) return;
  GameEnv->GameWindow_UserTextureTransformer->SetVoxelExtension((ZVoxelExtension *)VoxelInfo);
  GameEnv->GameWindow_UserTextureTransformer->Show();
}
