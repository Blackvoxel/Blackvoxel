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
 * ZVoxelGfx_Tree_Giant.cpp
 *
 *  Created on: 18 févr. 2012
 *      Author: laurent
 */

#include "ZVoxelGfx_Tree_Giant.h"
#include <stdio.h>

ULong ZVoxelGfx_Tree_Giant::Seed = 0;

void ZVoxelGfx_Tree_Giant::DrawBranch(ZVector3d * Position, ZPolar3d * BranchVector, double TotalBranchLen)
{
 ZVector3d NewPosition;
 ZPolar3d NewBranchVector;
 ZRect3L Rect;
 ZRect1d Thickness;
 ULong nSubDivisions;
 double Angle1,Angle2, NewTotalBranchLen;


 NewTotalBranchLen = TotalBranchLen + BranchVector->Len;
 if (NewTotalBranchLen > MaxBranchLenght) return;

 NewPosition = *Position + *BranchVector;
 Rect.Start = *Position + 0.5;
 Rect.End   = NewPosition + 0.5;
 Thickness.Start = ThicknessInterpolator_Branch.Interpolate(TotalBranchLen); Thickness.End = ThicknessInterpolator_Branch.Interpolate(NewTotalBranchLen);
 // Thickness.Start = 0.5; Thickness.End = 0.5;
 // printf("S:%lu (%ld,%ld,%ld = %ld,%ld,%ld)\n", nStep, Rect.Start.x, Rect.Start.y, Rect.Start.z, Rect.End.x, Rect.End.y, Rect.End.z );
 if (TotalBranchLen > 0) Sector->Draw_safe_VoxelLine( &Rect, &Thickness, 72 );
  if (HasFolliage && NewTotalBranchLen > FolliageLenght) Sector->Draw_safe_Sphere(Rect.End.x, Rect.End.y, Rect.End.z, 10.0, 70, false);


 if (RandomSubdivs) nSubDivisions = MinSubDivisionsPerNode + Random.GetNumber() % MaxSubDivisionsPerNode;
 else               nSubDivisions = MinSubDivisionsPerNode;
  //if (TotalBranchLen > (MaxBranchLenght/10.0) && nSubDivisions > 2 ) nSubDivisions=2;
  //if (nSubDivisions>2) nSubDivisions = 2;

  //nSubDivisions = 2;


 switch (nSubDivisions)
 {

   case 1:
   case 2:

     NewBranchVector = *BranchVector;
     NewBranchVector.Len = BrancheLen;
     Angle1 = (double) ((Long)Random.GetNumber()) / 23860929 / 6; Angle2 = (double) ((Long)Random.GetNumber()) / 23860929 / 6;
     NewBranchVector.pitch += Angle1; NewBranchVector.yaw   += Angle2;
     DrawBranch(&NewPosition, &NewBranchVector, NewTotalBranchLen);
     break;
   case 3:
   case 4:
   default:
     NewBranchVector = *BranchVector;
     NewBranchVector.Len = BrancheLen;
     do
     {
       Angle1 = (double) ((Long)Random.GetNumber()) / 23860929 / 3;
       Angle2 = (double) ((Long)Random.GetNumber()) / 23860929 / 3;
     } while (fabs(Angle1)<15.0 && fabs(Angle2)<15.0);


     NewBranchVector.pitch += Angle1; NewBranchVector.yaw   += Angle2;
     DrawBranch(&NewPosition, &NewBranchVector, NewTotalBranchLen);
     NewBranchVector = *BranchVector;
     NewBranchVector.pitch -= Angle1; NewBranchVector.yaw   -= Angle2;
     DrawBranch(&NewPosition, &NewBranchVector, NewTotalBranchLen);
     break;
   }


}

void ZVoxelGfx_Tree_Giant::DrawSecundaryTrunc(ZVector3d * Position, ZPolar3d * BranchVector, double TotalBranchLen)
{
   ZVector3d NewPosition;
   ZPolar3d NewBranchVector;
   ZRect3L Rect;
   ZRect1d Thickness;
   double Angle1,Angle2, NewTotalBranchLen;


   NewTotalBranchLen = TotalBranchLen + BranchVector->Len;
   if (NewTotalBranchLen > MaxBranchLenght) return;

   NewPosition = *Position + *BranchVector;
   Rect.Start = *Position + 0.5;
   Rect.End   = NewPosition + 0.5;
   Thickness.Start = ThicknessInterpolator_SecundTrunk.Interpolate(TotalBranchLen); Thickness.End = ThicknessInterpolator_SecundTrunk.Interpolate(NewTotalBranchLen);
   if (TotalBranchLen > 0) Sector->Draw_safe_VoxelLine( &Rect, &Thickness, 72 );

   // la branche

   NewBranchVector = *BranchVector;
   NewBranchVector.Len = BrancheLen;
   Angle1 = (double) ((Long)Random.GetNumber()) / 23860929 / 3;
   Angle2 = (double) ((Long)Random.GetNumber()) / 23860929 / 3;
   if (Angle1 > 0.0) Angle1+=15.0;
   if (Angle1 < 0.0) Angle1-=15.0;
   if (Angle2 > 0.0) Angle2+=15.0;
   if (Angle2 < 0.0) Angle2-=15.0;
   NewBranchVector.pitch += Angle1;
   NewBranchVector.yaw   += Angle2;

   DrawBranch(&NewPosition, &NewBranchVector, NewTotalBranchLen/4.0+350.0);

   // Le tronc central

    NewBranchVector = *BranchVector;
    NewBranchVector.Len = SecundaryTruncSegmentLenght;
    DrawSecundaryTrunc(&NewPosition, &NewBranchVector, NewTotalBranchLen);


}


void ZVoxelGfx_Tree_Giant::DrawTrunc(ZVector3d * Position, ZPolar3d * Direction)
{
  ZRect3L Rect;
  //ZRect1d Thickness;
  //ULong Number;

  double ThickTable[]= {25.0,20.0,18.0,18.0,18.0,20.0,20.0};

  //Thickness.Start = 5.0;
  //Thickness.End   = 5.0;

  //Number = Random.GetNumber();
  //TruncHeight = 10+( Number / 71582788);

  Rect.Start = *Position + 0.5;
  *Position  = *Position + ((ZVector3d)*Direction) * TruncHeight;
  Rect.End   = *Position + 0.5;

  //Sector->Draw_safe_VoxelLine(&Rect,&Thickness,67);
  Sector->Draw_safe_VoxelLine_TickCtl(&Rect,(double*)&ThickTable,7,72);

}


void ZVoxelGfx_Tree_Giant::DrawTree(ZVoxelSector * Sector, ZVector3d * BasePosition)
{
  ZVector3d Position;
  double ThicknessTable_Trunk[]={20.0,1.0};
  double ThicknessTable_Branch[]={3.0,0.0};

  //printf("-----------------------Draw Tree---------------------------\n");
  //printf("Seed : %lu",Seed);
  Random.Init(Seed);

  ThicknessInterpolator_SecundTrunk.SetValuesTable(ThicknessTable_Trunk,2);
  ThicknessInterpolator_SecundTrunk.SetBounds(0.0,MaxBranchLenght);
  ThicknessInterpolator_Branch.SetValuesTable(ThicknessTable_Branch,2);

  this->Sector = Sector;
  Position = *BasePosition;
  DrawTrunc( &Position, &StartingDirection);
  DrawSecundaryTrunc( &Position, &StartingDirection,0.0);
}

ZVoxelGfx_Tree_Giant::ZVoxelGfx_Tree_Giant()
{
  // Trunc starting direction : Vertical absolute.
  StartingDirection.Len   = 1.0;
  StartingDirection.pitch = 90.0;
  StartingDirection.yaw   = 90.0;
  StartingDirection.roll  = 0.0;
  TruncHeight = 128.0; // 40.0
  MinSubDivisionsPerNode = 1;
  MaxSubDivisionsPerNode = 4;
  RandomSubdivs = true;
  BrancheLen = 20.0;
  SecundaryTruncSegmentLenght = 10.0;
  MaxBranchLenght = 500.0;
  FolliageLenght  = 485.0; // 60.0
  HasFolliage = true;
}






