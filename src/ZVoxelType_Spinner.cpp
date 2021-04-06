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
 * ZVoxelType_Spinner.cpp
 *
 *  Created on: 4 avr. 2021
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPE_SPINNER_H
#  include "ZVoxelType_Spinner.h"
#endif

#include "ZGame.h"

void  ZVoxelType_Spinner::UserAction_Activate(ZMemSize VoxelInfo, Long x, Long y, Long z)
{
  ZActor * Actor;

  Actor = GameEnv->PhysicEngine->GetSelectedActor();
  Actor->Start_Riding(x,y,z);
  // GameEnv->GameWindow_Storage->SetVoxelExtension((ZVoxelExtension *)VoxelInfo);
  // GameEnv->GameWindow_Storage->Show();
}

