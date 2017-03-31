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
 * ZTool_Rotate.cpp
 *
 *  Created on: 6 déc. 2016
 *      Author: laurent
 */

#ifndef Z_ZTOOL_ROTATE_H
#  include "ZTool_Rotate.h"
#endif

#ifndef Z_ZACTORPHYSICS_H
#  include "ZActorPhysics.h"
#endif

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

bool ZTool_Rotate::Tool_MouseButtonClick(ULong Button)
{
  ZActor_Player * Actor;
  ZVoxelLocation Loc;
  UShort VoxelType;
  bool Change;
  UShort NewVoxelType;

  Actor = (ZActor_Player *)GameEnv->PhysicEngine->GetSelectedActor();

  // printf("Click : %ld\n",Button);

  if (Actor)
  {
    switch(Button)
    {
      case 1: // Wheel Button
              if (Actor->PointedVoxel.Collided)
              {
                ZMemSize OtherInfos=0;
                UShort VoxelType;
                Long x,y,z;
                x = Actor->PointedVoxel.PointedVoxel.x;
                y = Actor->PointedVoxel.PointedVoxel.y;
                z = Actor->PointedVoxel.PointedVoxel.z;
                VoxelType = GameEnv->World->GetVoxelExt( x, y, z, OtherInfos );
                GameEnv->VoxelTypeManager.GetVoxelType(VoxelType)->UserAction_Activate(OtherInfos,x,y,z);
              }
              break;

      case 0: // Left button
              if (GameEnv->World->GetVoxelLocation(&Loc, Actor->PointedVoxel.PointedVoxel.x, Actor->PointedVoxel.PointedVoxel.y, Actor->PointedVoxel.PointedVoxel.z))
              {
                VoxelType = Loc.Sector->Data[Loc.Offset];
                Change = false;
                switch(VoxelType)
                {
                  case 103: Change = true; NewVoxelType = 106; break;
                  case 104: Change = true; NewVoxelType = 105; break;
                  case 105: Change = true; NewVoxelType = 103; break;
                  case 106: Change = true; NewVoxelType = 104; break;

                  case 244: Change = true; NewVoxelType = 247; break;
                  case 245: Change = true; NewVoxelType = 246; break;
                  case 246: Change = true; NewVoxelType = 244; break;
                  case 247: Change = true; NewVoxelType = 245; break;
                  default: break;
                }
                if (Change)
                {
                  if (GameEnv->World->SetVoxel_WithCullingUpdate(Actor->PointedVoxel.PointedVoxel.x, Actor->PointedVoxel.PointedVoxel.y, Actor->PointedVoxel.PointedVoxel.z, NewVoxelType, ZVoxelSector::CHANGE_CRITICAL, true, 0)) Loc.Sector ->Flag_HighPriorityRefresh = true;
                  GameEnv->Sound->PlaySound(7);
                }

              }

              break;
      case 2: // Right mouse button
              if (GameEnv->World->GetVoxelLocation(&Loc, Actor->PointedVoxel.PointedVoxel.x, Actor->PointedVoxel.PointedVoxel.y, Actor->PointedVoxel.PointedVoxel.z))
              {
                VoxelType = Loc.Sector->Data[Loc.Offset];
                Change = false;
                switch(VoxelType)
                {
                  case 103: Change = true; NewVoxelType = 105; break;
                  case 104: Change = true; NewVoxelType = 106; break;
                  case 105: Change = true; NewVoxelType = 104; break;
                  case 106: Change = true; NewVoxelType = 103; break;

                  case 244: Change = true; NewVoxelType = 246; break;
                  case 245: Change = true; NewVoxelType = 247; break;
                  case 246: Change = true; NewVoxelType = 245; break;
                  case 247: Change = true; NewVoxelType = 244; break;
                  default: break;
                }
                if (Change)
                {
                  if (GameEnv->World->SetVoxel_WithCullingUpdate(Actor->PointedVoxel.PointedVoxel.x, Actor->PointedVoxel.PointedVoxel.y, Actor->PointedVoxel.PointedVoxel.z, NewVoxelType, ZVoxelSector::CHANGE_CRITICAL, true, 0)) Loc.Sector ->Flag_HighPriorityRefresh = true;
                  GameEnv->Sound->PlaySound(7);
                }

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

bool ZTool_Rotate::Tool_MouseButtonRelease(ULong Button)
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

void ZTool_Rotate::Start_Tool()
{
  // Display the rotate message.

  GameEnv->GameWindow_Advertising->Clear();
  GameEnv->GameWindow_Advertising->Advertise("ROTATE", ZGameWindow_Advertising::VISIBILITY_LOW,0, 500, 500);
}

void ZTool_Rotate::End_Tool()
{

}

void ZTool_Rotate::Display()
{

}

