/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2017 Laurent Thiebaut & Olivia Merle
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
 * ZTool_WirelessLinker.cpp
 *
 *  Created on: 30 déc. 2016
 *      Author: laurent
 */

#ifndef Z_ZTOOL_LINKER_H
#  include "ZTool_Linker.h"
#endif

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

ZToolsPointer ZTool_Linker::Pointer;

bool ZTool_Linker::Tool_MouseButtonClick(ULong Button)
{
  ZActor_Player * Actor;

  Actor = (ZActor_Player *)GameEnv->PhysicEngine->GetSelectedActor();

  // printf("Click : %ld\n",Button);

  if (Actor)
  {
    switch(Button)
    {
      case 0: // Left mouse button
      case 1: // Wheel Button
      case 2: // Right button

             if (Actor->PointedVoxel.Collided)
            {
              UShort VoxelType;
              ZVoxelLocation Loc;
              ZVector3L Pos;
              ZString Message;
              Pos.x = Actor->PointedVoxel.PointedVoxel.x; Pos.y = Actor->PointedVoxel.PointedVoxel.y; Pos.z = Actor->PointedVoxel.PointedVoxel.z;
              if (GameEnv->World->GetVoxelLocation(&Loc, &Pos))
              {
                VoxelType = Loc.Sector->Data[Loc.Offset];
                GameEnv->VoxelTypeManager.GetVoxelType(VoxelType)->Interface_SetPointingLocation(&Loc, &Pointer, Button, &Pos, &Message);
                GameEnv->GameWindow_Advertising->Clear();
                GameEnv->GameWindow_Advertising->Advertise(Message.String,ZGameWindow_Advertising::VISIBILITY_MEDLOW,0,1000,100);
              }


              // printf("Location x:%ld y:%ld z:%ld \n", x, y, z);
            }
             break;





      case 3: // Mouse scroll down

             break;
      case 4: // Mouse scroll up

             break;


    }
  }
  return(true);
}

bool ZTool_Linker::Tool_MouseButtonRelease(ULong Button)
{
  ZActor * Actor;

  Actor = GameEnv->PhysicEngine->GetSelectedActor();

  // printf("Release : %ld\n",Button);

  if (Actor)
  {
    switch(Button)
    {
      case 2: // Right mouse button
              break;
      case 1: // Wheel Button
              break;
      case 0: // Left button
              break;
      case 3: // Mouse scroll down
              break;
      case 4: // Mouse scroll up
              break;
    }
  }
  return(true);
}

void ZTool_Linker::Start_Tool()
{
  // Display the rotate message.

  GameEnv->GameWindow_Advertising->Clear();
  GameEnv->GameWindow_Advertising->Advertise("LINKER TOOL", ZGameWindow_Advertising::VISIBILITY_LOW,0, 1000, 1000);
}

void ZTool_Linker::End_Tool()
{

}

void ZTool_Linker::Display()
{

}
