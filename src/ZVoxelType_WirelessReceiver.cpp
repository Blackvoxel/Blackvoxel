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
 * ZVoxelType_WirelessReceiver.cpp
 *
 *  Created on: 31 déc. 2016
 *      Author: laurent
 */


#include "ZVoxelType_WirelessReceiver.h"

#ifndef Z_ZVOXELEXTENSION_WIRELESSTRANSMITTER_H
#  include "ZVoxelExtension_WirelessTransmitter.h"
#endif

#ifndef Z_ZVOXELSECTOR_H
#  include "ZVoxelSector.h"
#endif

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

#ifndef Z_ZTOOLSPOINTER_H
#  include "ZToolsPointer.h"
#endif



bool ZVoxelType_WirelessReceiver::Interface_SetPointingLocation(ZVoxelLocation * VLoc, ZToolsPointer * ToolsPointer, ULong Slot, ZVector3L * Location, ZString * OutMessage )
{
  //ZVoxelExtension_WirelessTransmitter * Extension;

  //Extension = (ZVoxelExtension_WirelessTransmitter *)VLoc->Sector->OtherInfos[VLoc->Offset];

  switch (Slot)
  {
    case 0:  *OutMessage = "CAN'T SET SOURCE HERE";
             break;

    case 1:
             {
               UShort VoxelType;
               ZVoxelLocation SLoc;
               ZVoxelExtension_WirelessTransmitter * Extension;

               ToolsPointer->EndLocation = *Location;

               // Get Source Voxel

               if (!GameEnv->World->GetVoxelLocation(&SLoc,&ToolsPointer->StartLocation))
               {
                 *OutMessage = "SOURCE IS UNREACHEABLE";
                 return(false);
               }

               // Check Source Voxel Type.

               VoxelType = SLoc.Sector->Data[SLoc.Offset];
               if (VoxelType!=254)
               {
                 *OutMessage = "SOURCE NOT COMPATIBLE";
                 return(false);
               }

               // Get Voxel Extension

               Extension = (ZVoxelExtension_WirelessTransmitter * )SLoc.Sector->OtherInfos[SLoc.Offset];

               // Create a DestPoint

               ZVoxelExtension_WirelessTransmitter::DestPoint DestPoint;
               DestPoint.Location = ToolsPointer->EndLocation;

               // Test if Destpoint already registered ?

               if (!Extension->IsConsummerRegistered(&DestPoint))
               {
                 *OutMessage = "NOT REGISTERED";
                 return(false);
               }

               // Add the Dest Point to the list.

               if (!Extension->RemoveConsummer(&DestPoint.Location))
               {
                *OutMessage = "CANT REMOVE";
                return(false);
               }

               // Return with success.

               *OutMessage = "REMOVED";

               return(true);
             }
             break;

    case 2:  {
               UShort VoxelType;
               ZVoxelLocation SLoc;
               ZVoxelExtension_WirelessTransmitter * Extension;

               ToolsPointer->EndLocation = *Location;

               // Get Source Voxel

               if (!GameEnv->World->GetVoxelLocation(&SLoc,&ToolsPointer->StartLocation))
               {
                 *OutMessage = "SOURCE IS UNREACHEABLE";
                 return(false);
               }

               // Check Source Voxel Type.

               VoxelType = SLoc.Sector->Data[SLoc.Offset];
               if (VoxelType!=254)
               {
                 *OutMessage = "SOURCE NOT COMPATIBLE";
                 return(false);
               }

               // Get Voxel Extension

               Extension = (ZVoxelExtension_WirelessTransmitter * )SLoc.Sector->OtherInfos[SLoc.Offset];

               // Create a DestPoint

               ZVoxelExtension_WirelessTransmitter::DestPoint DestPoint;
               DestPoint.Location = ToolsPointer->EndLocation;

               // Test if Destpoint already registered ?

               if (Extension->IsConsummerRegistered(&DestPoint))
               {
                 *OutMessage = "ALREADY REGISTERED";
                 return(false);
               }

               // Add the Dest Point to the list.

               if (!Extension->AddConsummer(&DestPoint))
               {
                *OutMessage = "CAPACITY FULL";
                return(false);
               }

               // Return with success.

               *OutMessage = "REGISTERED";

               return(true);
             }
             break;
    default: break;
  }

  return(true);
}



ULong ZVoxelType_WirelessReceiver::Interface_PushBlock_Push( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count )
{
  ZVector3L Location, CommingDirection;

/*
  Location.y = (DestLocation->Sector->Pos_y << ZVOXELBLOCSHIFT_Y) + (DestLocation->Offset & (ZVOXELBLOCMASK_Y)) ;
  Location.x = (DestLocation->Sector->Pos_x << ZVOXELBLOCSHIFT_X) + ((DestLocation->Offset & (ZVOXELBLOCMASK_X << ZVOXELBLOCSHIFT_Y)) >> ZVOXELBLOCSHIFT_Y);
  Location.z = (DestLocation->Sector->Pos_z << ZVOXELBLOCSHIFT_Z) + ((DestLocation->Offset & (ZVOXELBLOCMASK_Z << (ZVOXELBLOCSHIFT_Y + ZVOXELBLOCSHIFT_X ) )) >> (ZVOXELBLOCSHIFT_Y + ZVOXELBLOCSHIFT_X ));


  CommingDirection = 0;
  // printf("-----------Start Searching endpoints--------------\n");
  LightTransmitter_FindEndPoints(&Location, &CommingDirection, &Context);
  // printf("-----------End Searching endpoints : %ld--------------\n", Context.EndPointTable.GetCount());

  ZMemSize nElements,i,j;
  ZVector3L * Element, NewPos;
  VoxelLocation NewLocation;
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
  */
  return(0);
}

ULong ZVoxelType_WirelessReceiver::Interface_PushBlock_PushTest( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count )
{
  ZVector3L Location, CommingDirection;

/*
  Location.y = (DestLocation->Sector->Pos_y << ZVOXELBLOCSHIFT_Y) + (DestLocation->Offset & (ZVOXELBLOCMASK_Y)) ;
  Location.x = (DestLocation->Sector->Pos_x << ZVOXELBLOCSHIFT_X) + ((DestLocation->Offset & (ZVOXELBLOCMASK_X << ZVOXELBLOCSHIFT_Y)) >> ZVOXELBLOCSHIFT_Y);
  Location.z = (DestLocation->Sector->Pos_z << ZVOXELBLOCSHIFT_Z) + ((DestLocation->Offset & (ZVOXELBLOCMASK_Z << (ZVOXELBLOCSHIFT_Y + ZVOXELBLOCSHIFT_X ) )) >> (ZVOXELBLOCSHIFT_Y + ZVOXELBLOCSHIFT_X ));

  // MANUAL_BREAKPOINT;

  CommingDirection = 0;
  // printf("-----------Start Searching endpoints--------------\n");
  LightTransmitter_FindEndPoints(&Location, &CommingDirection, &Context);
  // printf("-----------End Searching endpoints : %ld--------------\n", Context.EndPointTable.GetCount());

  ZMemSize nElements,i,j;
  ZVector3L * Element, NewPos;
  VoxelLocation NewLocation;
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
  */
  return(0);
}
