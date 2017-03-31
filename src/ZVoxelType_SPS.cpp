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
 * ZVoxelType_SPS.cpp
 *
 *  Created on: 29 juin 2016
 *      Author: laurent
 */

#include "ZVoxelType_SPS.h"

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

void ZVoxelType_SPS::Power_Start() {GameEnv->GameWindow_SPS->Show();}
void ZVoxelType_SPS::Power_End()   {GameEnv->GameWindow_SPS->Hide();}

void ZVoxelType_SPS::Power_DoWork()
{
  ZString As;

  ZActor_Player * Actor;
  ZVector3L Coords;

  Actor = (ZActor_Player *)GameEnv->PhysicEngine->GetSelectedActor();

  GameEnv->World->Convert_Coords_PlayerToVoxel(&Actor->Location, &Coords);

  As.Append_pchar("[");
  As.Append_Long(Coords.x);
  As.Append_char(',');
  As.Append_Long(Coords.y);
  As.Append_char(',');
  As.Append_Long(Coords.z);
  As.Append_pchar("] [");
  As.Append_Long(Coords.x >> 8);
  As.Append_char(',');
  As.Append_Long(Coords.z >> 8);
  As.Append_pchar("] [");
  As.Append_Long((Long)Actor->ViewDirection.yaw);
  As.Append_pchar("]");

  GameEnv->GameWindow_SPS->SetText(&As);
}
