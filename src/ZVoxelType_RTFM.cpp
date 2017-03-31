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
 * ZVoxelType_RTFM.cpp
 *
 *  Created on: 20 janv. 2017
 *      Author: laurent
 */

#include "ZVoxelType_RTFM.h"

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

void ZVoxelType_RTFM::Event_Start_Selected(ZVoxelLocation * Loc, ZVector3L * Coords)
{
  if (!GameEnv->GameWindow_RTFM->Is_Shown()) GameEnv->GameWindow_RTFM->Show();
  // printf("Started %d %d %d\n",Coords->x, Coords->y, Coords->z);
}

void ZVoxelType_RTFM::Event_Is_Selected(ZVoxelLocation * Loc, ZVector3L * Coords)
{

}

void ZVoxelType_RTFM::Event_End_Selected(ZVoxelLocation * Loc, ZVector3L * Coords)
{
  GameEnv->GameWindow_RTFM->Hide();
  // printf("Ended %d %d %d\n",Coords->x, Coords->y, Coords->z);
}
