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
 * ZVoxelType_LightTransmitter.cpp
 *
 *  Created on: 1 déc. 2012
 *      Author: laurent
 */



#include "ZVoxelType_LightTransmitter.h"
#include "ZGame.h"

#ifndef Z_ZFASTRANDOM_H
#  include "ZFastRandom.h"
#endif


ZVoxelType_LightTransmitter::ZBlocPosN ZVoxelType_LightTransmitter::nbp6[6] =
{
  {-1, 0, 0},
  { 1, 0, 0},
  { 0,-1, 0},
  { 0, 1, 0},
  { 0, 0,-1},
  { 0, 0, 1}
};

ULong ZVoxelType_LightTransmitter::Interface_PushBlock_Push( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count )
{
  ZVector3L Location, CommingDirection;
  ZTransmitterContext Context;

  Location.y = (DestLocation->Sector->Pos_y << ZVOXELBLOCSHIFT_Y) + (DestLocation->Offset & (ZVOXELBLOCMASK_Y)) ;
  Location.x = (DestLocation->Sector->Pos_x << ZVOXELBLOCSHIFT_X) + ((DestLocation->Offset & (ZVOXELBLOCMASK_X << ZVOXELBLOCSHIFT_Y)) >> ZVOXELBLOCSHIFT_Y);
  Location.z = (DestLocation->Sector->Pos_z << ZVOXELBLOCSHIFT_Z) + ((DestLocation->Offset & (ZVOXELBLOCMASK_Z << (ZVOXELBLOCSHIFT_Y + ZVOXELBLOCSHIFT_X ) )) >> (ZVOXELBLOCSHIFT_Y + ZVOXELBLOCSHIFT_X ));

  // MANUAL_BREAKPOINT;
/*
  Location.x = (DestLocation->Sector->Pos_x << ZVOXELBLOCSHIFT_X) + ( DestLocation->Offset & ZVOXELBLOCMASK_X);
  Location.y = (DestLocation->Sector->Pos_y << ZVOXELBLOCSHIFT_Y) + ((DestLocation->Offset & (ZVOXELBLOCMASK_Y << ZVOXELBLOCSHIFT_Y)) >> ZVOXELBLOCSHIFT_Y) ;
  Location.z = (DestLocation->Sector->Pos_z << ZVOXELBLOCSHIFT_Z) + ((DestLocation->Offset & (ZVOXELBLOCMASK_Z << ZVOXELBLOCSHIFT_Z)) >> ZVOXELBLOCSHIFT_Z);
*/
  CommingDirection = 0;
  // printf("-----------Start Searching endpoints--------------\n");
  LightTransmitter_FindEndPoints(&Location, &CommingDirection, &Context);
  // printf("-----------End Searching endpoints : %ld--------------\n", Context.EndPointTable.GetCount());

  ZMemSize nElements,i,j;
  ZVector3L * Element, NewPos;
  ZVoxelLocation NewLocation;
  UShort Voxel;
  ULong ChoosenElement, PlacedCount;

  PlacedCount = 0;
  nElements = Context.EndPointTable.GetCount();
  for (i=0; i<nElements; i++)
  {
    ChoosenElement = GameEnv->Random.GetNumber() % (nElements);
    Element = Context.EndPointTable.GetElement(ChoosenElement);
    for (j=0;j<6;j++)
    {
      NewPos.x = Element->x + nbp6[j].x; NewPos.y = Element->y + nbp6[j].y; NewPos.z = Element->z + nbp6[j].z;
      Voxel = GameEnv->World->GetVoxel(NewPos.x,NewPos.y,NewPos.z);

      if (GameEnv->VoxelTypeManager.VoxelTable[Voxel]->Is_Interface_PushBlock)
      {

        GameEnv->World->GetVoxelLocation( &NewLocation,NewPos.x, NewPos.y ,NewPos.z );
        PlacedCount += GameEnv->VoxelTypeManager.VoxelTable[Voxel]->Interface_PushBlock_Push(&NewLocation, VoxelType, Count);
        if ( (Count - PlacedCount) == 0 ) return(PlacedCount);
      }
      else if (Voxel==248 || Voxel == 98) // Materializer
      {
        NewPos.x = NewPos.x + nbp6[j].x; NewPos.y = NewPos.y + nbp6[j].y; NewPos.z = NewPos.z + nbp6[j].z;
        Voxel = GameEnv->World->GetVoxel(&NewPos);
        if (Voxel == 0)
        {
           PlacedCount++;
           GameEnv->World->SetVoxel_WithCullingUpdate(NewPos.x, NewPos.y,NewPos.z, VoxelType, ZVoxelSector::CHANGE_CRITICAL ,true);
        }
        if ( (Count - PlacedCount) == 0 ) return(PlacedCount);
      }
    }
  }
  return(PlacedCount);
}

ULong ZVoxelType_LightTransmitter::Interface_PushBlock_PushTest( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count )
{
  ZVector3L Location, CommingDirection;
  ZTransmitterContext Context;

  Location.y = (DestLocation->Sector->Pos_y << ZVOXELBLOCSHIFT_Y) + (DestLocation->Offset & (ZVOXELBLOCMASK_Y)) ;
  Location.x = (DestLocation->Sector->Pos_x << ZVOXELBLOCSHIFT_X) + ((DestLocation->Offset & (ZVOXELBLOCMASK_X << ZVOXELBLOCSHIFT_Y)) >> ZVOXELBLOCSHIFT_Y);
  Location.z = (DestLocation->Sector->Pos_z << ZVOXELBLOCSHIFT_Z) + ((DestLocation->Offset & (ZVOXELBLOCMASK_Z << (ZVOXELBLOCSHIFT_Y + ZVOXELBLOCSHIFT_X ) )) >> (ZVOXELBLOCSHIFT_Y + ZVOXELBLOCSHIFT_X ));

  // MANUAL_BREAKPOINT;
/*
  Location.x = (DestLocation->Sector->Pos_x << ZVOXELBLOCSHIFT_X) + ( DestLocation->Offset & ZVOXELBLOCMASK_X);
  Location.y = (DestLocation->Sector->Pos_y << ZVOXELBLOCSHIFT_Y) + ((DestLocation->Offset & (ZVOXELBLOCMASK_Y << ZVOXELBLOCSHIFT_Y)) >> ZVOXELBLOCSHIFT_Y) ;
  Location.z = (DestLocation->Sector->Pos_z << ZVOXELBLOCSHIFT_Z) + ((DestLocation->Offset & (ZVOXELBLOCMASK_Z << ZVOXELBLOCSHIFT_Z)) >> ZVOXELBLOCSHIFT_Z);
*/
  CommingDirection = 0;
  // printf("-----------Start Searching endpoints--------------\n");
  LightTransmitter_FindEndPoints(&Location, &CommingDirection, &Context);
  // printf("-----------End Searching endpoints : %ld--------------\n", Context.EndPointTable.GetCount());

  ZMemSize nElements,i,j;
  ZVector3L * Element, NewPos;
  ZVoxelLocation NewLocation;
  UShort Voxel;
  ULong ChoosenElement, PlacedCount;

  PlacedCount = 0;
  nElements = Context.EndPointTable.GetCount();
  for (i=0; i<nElements; i++)
  {
    ChoosenElement = GameEnv->Random.GetNumber() % (nElements);
    Element = Context.EndPointTable.GetElement(ChoosenElement);
    for (j=0;j<6;j++)
    {
      NewPos.x = Element->x + nbp6[j].x; NewPos.y = Element->y + nbp6[j].y; NewPos.z = Element->z + nbp6[j].z;
      Voxel = GameEnv->World->GetVoxel(&NewPos);

      if (GameEnv->VoxelTypeManager.VoxelTable[Voxel]->Is_Interface_PushBlock)
      {

        GameEnv->World->GetVoxelLocation( &NewLocation,NewPos.x, NewPos.y ,NewPos.z );
        PlacedCount += GameEnv->VoxelTypeManager.VoxelTable[Voxel]->Interface_PushBlock_PushTest(&NewLocation, VoxelType, Count);
        if ( (Count - PlacedCount) == 0 ) return(PlacedCount);
      }
      else if (Voxel == 248 || Voxel == 98) // Materializer
      {
        NewPos.x = NewPos.x + nbp6[j].x; NewPos.y = NewPos.y + nbp6[j].y; NewPos.z = NewPos.z + nbp6[j].z;
        Voxel = GameEnv->World->GetVoxel(&NewPos);
        if (Voxel == 0) PlacedCount++;
        if ( (Count - PlacedCount) == 0 ) return(PlacedCount);
      }
    }
  }
  return(PlacedCount);
}


void ZVoxelType_LightTransmitter::LightTransmitter_FindEndPoints(ZVector3L * Location, ZVector3L * CommingDirection, ZTransmitterContext * Context )
{
  UShort VoxelType;
  ZVector3L NewPos, Direction, NewCommingDirection;
  ULong i;


  // printf("Start at %d,%d,%d\n",Location->x, Location->y, Location->z);
  for (i=0;i<6;i++)
  {
    Direction.x = nbp6[i].x; Direction.y = nbp6[i].y; Direction.z = nbp6[i].z;
    if (Direction == *CommingDirection) continue;
    NewPos.x = Location->x + nbp6[i].x; NewPos.y = Location->y + nbp6[i].y; NewPos.z = Location->z + nbp6[i].z;


      VoxelType = GameEnv->World->GetVoxel(NewPos.x, NewPos.y,NewPos.z);
      switch(VoxelType)
      {
        case  99:  LightTransmitter_FollowTransmitter(&NewPos, &Direction, Context ); break;
        case 101:  Context->EndPointTable.AddToTable(&NewPos);
                   break;
        case 102:  if (!Context->NodeTable.IsElementInTable(&NewPos))
                   {
                     Context->NodeTable.AddToTable(&NewPos);
                     NewCommingDirection.x = -Direction.x; NewCommingDirection.y = -Direction.y; NewCommingDirection.z = -Direction.z;
                     LightTransmitter_FindEndPoints( &NewPos, &NewCommingDirection, Context );
                   }
                   break;
        default:   break;
      }
  }
}

void ZVoxelType_LightTransmitter::LightTransmitter_FollowTransmitter(ZVector3L * Location, ZVector3L * FollowingDirection, ZTransmitterContext * Context)
{
  UShort VoxelType;
  ZVector3L NewPos;
  ZVector3L CommingDirection;

  NewPos = *Location;

  // printf("Follow Transmitter From %d,%d,%d\n",Location->x, Location->y,Location->z);
  while (true)
  {
    VoxelType = GameEnv->World->GetVoxel(NewPos.x, NewPos.y,NewPos.z);
    switch(VoxelType)
    {
      case 99:  break; // Continue
      case 101: // printf("OutPoint at %d,%d,%d\n",NewPos.x,NewPos.y,NewPos.z);
                Context->EndPointTable.AddToTable(&NewPos);
                return; // printf("EndPoint %d,%d,%d\n",NewPos.x,NewPos.y,NewPos.z); break;
      case 102: // printf("Split at %d,%d,%d\n",NewPos.x,NewPos.y,NewPos.z);
                if (Context->NodeTable.IsElementInTable(&NewPos)) return;
                Context->NodeTable.AddToTable(&NewPos);
                CommingDirection.x = -FollowingDirection->x; CommingDirection.y =-FollowingDirection->y; CommingDirection.z = -FollowingDirection->z;
                LightTransmitter_FindEndPoints(&NewPos,&CommingDirection, Context);
                return;
      default:  return;
    }
    NewPos += *FollowingDirection;
  }

}
