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
 * ZVoxelType_Sequencer.cpp
 *
 *  Created on: 1 mai 2013
 *      Author: laurent
 */

#include "ZVoxelType_Sequencer.h"
#include "ZGame.h"

#ifndef Z_ZVOXELEXTENSION_SEQUENCER_H
#  include "ZVoxelExtension_Sequencer.h"
#endif


ZVoxelExtension * ZVoxelType_Sequencer::CreateVoxelExtension(bool IsLoadingPhase)
{
  ZVoxelExtension * NewVoxelExtension = 0;

  NewVoxelExtension = new ZVoxelExtension_Sequencer;

  return (NewVoxelExtension);
}

void  ZVoxelType_Sequencer::UserAction_Activate(ZMemSize VoxelInfo, Long x, Long y, Long z)
{
  if (GameEnv->GameWindow_Sequencer->Is_Shown()) return;
  GameEnv->GameWindow_Sequencer->SetVoxelExtension((ZVoxelExtension *)VoxelInfo);
  GameEnv->GameWindow_Sequencer->Show();
}

ULong ZVoxelType_Sequencer::Interface_PushBlock_Push( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count )
{

  return(0);
}


ULong ZVoxelType_Sequencer::Interface_PushBlock_Pull( ZVoxelLocation * DestLocation,  UShort * VoxelType, ULong Count )
{

  return(0);
}



