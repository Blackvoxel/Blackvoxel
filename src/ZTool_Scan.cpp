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
 * ZTool_Scan.cpp
 *
 *  Created on: 14 juil. 2016
 *      Author: laurent
 */

#ifndef Z_ZTOOL_SCAN_H
#  include "ZTool_Scan.h"
#endif

#ifndef Z_ZACTORPHYSICS_H
#  include "ZActor_Player.h"
#endif

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif


bool ZTool_Scan::Tool_MouseButtonClick(ULong Button)
{
  ZActor_Player * Actor;

  Actor = (ZActor_Player *)GameEnv->PhysicEngine->GetSelectedActor();

  // printf("Click : %ld\n",Button);

  if (Actor)
  {
    switch(Button)
    {
      case 2: // Right mouse button
              break;

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
              break;

      case 3: // Mouse scroll down
              break;

      case 4: // Mouse scroll up
              break;


    }
  }
  return(true);
}

bool ZTool_Scan::Tool_MouseButtonRelease(ULong Button)
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

void ZTool_Scan::Start_Tool()
{
  if (!GameEnv->GameWindow_Scan->Is_Shown()) GameEnv->GameWindow_Scan->Show();
  //MiningInProgress = false;
  //if (!GameEnv->VoxelTypeBar->Is_Shown()) GameEnv->VoxelTypeBar->Show();
}

void ZTool_Scan::End_Tool()
{
  if (GameEnv->GameWindow_Scan->Is_Shown()) GameEnv->GameWindow_Scan->Hide();
  //if (GameEnv->VoxelTypeBar->Is_Shown()) GameEnv->VoxelTypeBar->Hide();
}

void ZTool_Scan::Display()
{
  ZActor_Player * Actor;
  UShort   Voxel;
  ZVoxelLocation Loc;
  ZVoxelType * VoxelType;
  ZMemSize     VoxelInfo;
  ZString  Tx;

  Actor = (ZActor_Player *)GameEnv->PhysicEngine->GetSelectedActor();
  if (Actor->PointedVoxel.Collided)
  {
    if (GameEnv->World->GetVoxelLocation(&Loc, Actor->PointedVoxel.PointedVoxel.x, Actor->PointedVoxel.PointedVoxel.y, Actor->PointedVoxel.PointedVoxel.z))
    {
      Voxel = Loc.Sector->Data[Loc.Offset];
      VoxelInfo = Loc.Sector->OtherInfos[Loc.Offset];
      VoxelType = GameEnv->VoxelTypeManager.GetVoxelType(Voxel);
      Tx.Append_pchar("~1:00FF00:-----------VOXEL TYPE---------\n");
      Tx << "~1:FF0000:" << VoxelType->VoxelTypeName << "~1:FFFFFF:\n";
      Tx << Voxel << "\n";
      Tx.Append_pchar("-------------INFO------------\n");
      VoxelType->GetScanInformations(&Actor->PointedVoxel.PointedVoxel, Voxel, VoxelInfo, Tx);
      GameEnv->GameWindow_Scan->SetText(&Tx);
    }
  }
}
