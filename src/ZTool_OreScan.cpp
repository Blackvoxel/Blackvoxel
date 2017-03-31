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
 * ZTool_OreScan.cpp
 *
 *  Created on: 24 déc. 2016
 *      Author: laurent
 */


#ifndef Z_ZTOOL_SCAN_H
#  include "ZTool_OreScan.h"
#endif

#ifndef Z_ZACTORPHYSICS_H
#  include "ZActorPhysics.h"
#endif

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif


void ZTool_OreScan::MakeScan(ZVector3L * ScanPoint, ZVector3L * ScanDimensions, ZString * Result)
{
  ULong Table[65536];
  ULong i;
  UShort VoxelType;
  ULong TotalVoxels;
  ULong TotalNonVoidVoxels;
  Long x,y,z;
  ZString Line;

  TotalVoxels = 0;
  TotalNonVoidVoxels = 0;

  // Clear Table

  for (i=0;i<65536;i++) Table[i]=0;

  // Count voxels

  for ( z = ScanPoint->z-ScanDimensions->z ; z < ScanPoint->z+ScanDimensions->z ; z++)
  {
    for (x = ScanPoint->x-ScanDimensions->x ; x < ScanPoint->x+ScanDimensions->x ; x++)
    {
      for (y = ScanPoint->y-ScanDimensions->y ; y < ScanPoint->y ; y++)
      {
        VoxelType = GameEnv->World->GetVoxel(x,y,z);
        Table[VoxelType]++;
        TotalVoxels++;
        if (VoxelType) TotalNonVoidVoxels++;
      }
    }
  }

  // Make display text

  for (i=1;i<65536;i++)
  {
    if (Table[i] != 0)
    {
      Line.Clear();
      double Pc = 100.0 / ((double)TotalNonVoidVoxels) * ((double)Table[i]);
      Line.Append_Double(Pc, 2, true);
      Line.PadUptoLen(' ',6);
      Line << GameEnv->VoxelTypeManager.GetVoxelType(i)->VoxelTypeName;
#ifdef COMPILEOPTION_DEBUGFACILITY
      Line << " [" << i << "]";
#endif
      Line << "\n";
      *Result << Line;
    }
  }


}

void ZTool_OreScan::DisplayHeader()
{
  LastAnalysis.Clear();
  //                                   012345678901234567890123456789
  LastAnalysis.Append_pchar("~1:FF0000:       Ground Scanner~1:FFFFFF:\n\n");

}


bool ZTool_OreScan::Tool_MouseButtonClick(ULong Button)
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
              {
                ZVector3L PlayerLocation;

                // Clear text display buffer and display Headers

                DisplayHeader();

                // Get Player location in voxel.

                GameEnv->World->Convert_Coords_PlayerToVoxel(&Actor->Location, &PlayerLocation );

                // Display Location and size informations.

                LastAnalysis<<"~1:00FF00:-----------Location-----------~1:FFFFFF:\n";
                LastAnalysis<<"Location : "<<PlayerLocation.x<<","<<PlayerLocation.y<<","<<PlayerLocation.z<<"\n";
                LastAnalysis<<"Size     : 64x64x32 Voxels\n";
                LastAnalysis<<"~1:00FF00:-----------Analysis-----------~1:FFFFFF:\n";

                // Launch Analysis

                ZVector3L ScannedZone(32,32,32);
                this->MakeScan(&PlayerLocation, &ScannedZone, &LastAnalysis );

                printf("%s\n", LastAnalysis.String);

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

bool ZTool_OreScan::Tool_MouseButtonRelease(ULong Button)
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

void ZTool_OreScan::Start_Tool()
{
  if (!GameEnv->GameWindow_Scan->Is_Shown()) GameEnv->GameWindow_Scan->Show();
  //MiningInProgress = false;
  //if (!GameEnv->VoxelTypeBar->Is_Shown()) GameEnv->VoxelTypeBar->Show();
  DisplayHeader();
  LastAnalysis.Append_pchar("\nClick to start a new Analysis\n");

}

void ZTool_OreScan::End_Tool()
{
  if (GameEnv->GameWindow_Scan->Is_Shown()) GameEnv->GameWindow_Scan->Hide();
  //if (GameEnv->VoxelTypeBar->Is_Shown()) GameEnv->VoxelTypeBar->Hide();
}

void ZTool_OreScan::Display()
{
  ZActor_Player * Actor;
  ZVoxelLocation Loc;
  ZString  Tx;

  Actor = (ZActor_Player *)GameEnv->PhysicEngine->GetSelectedActor();
  if (Actor->PointedVoxel.Collided)
  {
    if (GameEnv->World->GetVoxelLocation(&Loc, Actor->PointedVoxel.PointedVoxel.x, Actor->PointedVoxel.PointedVoxel.y, Actor->PointedVoxel.PointedVoxel.z))
    {
      /*
      Voxel = Loc.Sector->Data[Loc.Offset];
      VoxelInfo = Loc.Sector->OtherInfos[Loc.Offset];
      VoxelType = GameEnv->VoxelTypeManager.GetVoxelType(Voxel);
      Tx.Append_pchar("~1:00FF00:-----------VOXEL TYPE---------\n");
      Tx << "~1:FF0000:" << VoxelType->VoxelTypeName << "~1:FFFFFF:\n";
      Tx << Voxel << "\n";
      Tx.Append_pchar("-------------INFO------------\n");
      VoxelType->GetScanInformations(&Actor->PointedVoxel.PointedVoxel, Voxel, VoxelInfo, Tx);
      */
      GameEnv->GameWindow_Scan->SetText(&LastAnalysis);
    }
  }
}
