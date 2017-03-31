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
 * ZVoxelType_WirelessTransmitter.cpp
 *
 *  Created on: 29 déc. 2016
 *      Author: laurent
 */

#include "ZVoxelType_WirelessTransmitter.h"

#ifndef Z_ZVOXELEXTENSION_WIRELESSTRANSMITTER_H
#  include "ZVoxelExtension_WirelessTransmitter.h"
#endif

#ifndef Z_ZVOXELSECTOR_H
#  include "ZVoxelSector.h"
#endif

#ifndef Z_ZWORLD_H
#  include "ZWorld.h"
#endif

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

#ifndef Z_ZTOOLSPOINTER_H
#  include "ZToolsPointer.h"
#endif


ZVoxelExtension * ZVoxelType_WirelessTransmitter::CreateVoxelExtension(bool IsLoadingPhase)
{
  ZVoxelExtension * NewVoxelExtension = 0;

  NewVoxelExtension = new ZVoxelExtension_WirelessTransmitter;

  return (NewVoxelExtension);
}

bool ZVoxelType_WirelessTransmitter::Interface_SetPointingLocation(ZVoxelLocation * VLoc, ZToolsPointer * ToolsPointer, ULong Slot, ZVector3L * Location, ZString * OutMessage )
{
  //ZVoxelExtension_WirelessTransmitter * Extension;

  //Extension = (ZVoxelExtension_WirelessTransmitter *)VLoc->Sector->OtherInfos[VLoc->Offset];

  switch (Slot)
  {
    case 0:  ToolsPointer->StartLocation = *Location;
             *OutMessage = "SOURCE SET";
             break;
    case 2:  *OutMessage = "CAN'T SET DESTINATION HERE";
             break;
    default: break;
  }

  return(true);
}



ULong ZVoxelType_WirelessTransmitter::Interface_PushBlock_Push( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count )
{
  ZVoxelExtension_WirelessTransmitter * Ext;
  ZVoxelExtension_WirelessTransmitter::DestPoint Dp;
  ULong PlacedCount;
  ZVoxelLocation Loc;
  ZVoxelWorld * World;
  UShort Voxel;
  ZVector3L NewLocation;

  // Get Extension

  Ext = (ZVoxelExtension_WirelessTransmitter *)DestLocation->Sector->OtherInfos[DestLocation->Offset];
  if (!Ext) return (0);

  // Get
  ZMemSize PntCount = Ext->GetConsummerCount();
  if (Ext->NextUsed>=PntCount) Ext->NextUsed = 0;
  PlacedCount = 0;

  World = GameEnv->World;
  for (ZMemSize i = 0; i<PntCount && Count; i++)
  {
    // Get Next Endpoint coordinates

    if (!Ext->GetConsummerInfo(Ext->NextUsed++, Dp)) return(PlacedCount);
    if (Ext->NextUsed>=PntCount) Ext->NextUsed = 0;

    // Get Voxel for EndPoint
    if (World->GetVoxelLocation(&Loc,&Dp.Location))
    {
      Voxel = Loc.Sector->Data[Loc.Offset];

      // If it isn't an endpoint voxel, remove it from list and abort.

      if (Voxel!=255 && Voxel!=65535) { Ext->RemoveConsummer(&Dp.Location); return(PlacedCount); }

      // Look voxels around endpoint in order to find input interface or a materializer.

      for (ULong Dir=0; Dir<6;Dir++)
      {
         NewLocation = Dp.Location + ZVoxelWorld::ProxLoc[Dir];

         // Look at these locations around

         if (World->GetVoxelLocation(&Loc, &NewLocation))
         {
           Voxel = Loc.Sector->Data[Loc.Offset];

           // If it's a bloc with an input interface.

           if (VoxelTypeManager->VoxelTable[Voxel]->Is_Interface_PushBlock)
           {
             ULong Cnt = VoxelTypeManager->VoxelTable[Voxel]->Interface_PushBlock_Push(&Loc, VoxelType, Count);
             PlacedCount += Cnt;
             Count -= Cnt;
             if (!Count) return(PlacedCount);
           }
           // Or a Materializer directly at output...
           else if (Voxel==248 || Voxel == 98)
           {
             NewLocation+= ZVoxelWorld::ProxLoc[Dir];
             if (World->GetVoxelLocation(&Loc, &NewLocation))
               if (Loc.Sector->Data[Loc.Offset]==0)
                 if (GameEnv->World->SetVoxel_WithCullingUpdate(NewLocation.x, NewLocation.y,NewLocation.z, VoxelType, ZVoxelSector::CHANGE_CRITICAL ,true))
                 {
                   PlacedCount++;
                   Count--;
                 }
             if (!Count) return(PlacedCount);
           }

         }

      }


    }

  }

  return(0);
}

ULong ZVoxelType_WirelessTransmitter::Interface_PushBlock_PushTest( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count )
{
  ZVoxelExtension_WirelessTransmitter * Ext;
  ZVoxelExtension_WirelessTransmitter::DestPoint Dp;
  ULong PlacedCount;
  ZVoxelLocation Loc;
  ZVoxelWorld * World;
  UShort Voxel;
  ZVector3L NewLocation;
  ULong CpNextUsed;

  // Get Extension

  Ext = (ZVoxelExtension_WirelessTransmitter *)DestLocation->Sector->OtherInfos[DestLocation->Offset];
  if (!Ext) return (0);

  // Get
  ZMemSize PntCount = Ext->GetConsummerCount();
  PlacedCount = 0;

  CpNextUsed = Ext->NextUsed;
  if (CpNextUsed >= PntCount) CpNextUsed = 0;
  World = GameEnv->World;
  for (ZMemSize i = 0; i<PntCount && Count; i++)
  {
    // Get Next Endpoint coordinates

    if (!Ext->GetConsummerInfo(CpNextUsed, Dp)) return(PlacedCount);
    if (CpNextUsed >= PntCount) CpNextUsed = 0;

    // Get Voxel for EndPoint
    if (World->GetVoxelLocation(&Loc,&Dp.Location))
    {
      Voxel = Loc.Sector->Data[Loc.Offset];

      // If it isn't an endpoint voxel, remove it from list and abort.

      if (Voxel!=255 && Voxel!=65535) { Ext->RemoveConsummer(&Dp.Location); return(PlacedCount); }

      // Look voxels around endpoint in order to find input interface or a materializer.

      for (ULong Dir=0; Dir<6;Dir++)
      {
         NewLocation = Dp.Location + ZVoxelWorld::ProxLoc[Dir];

         // Look at these locations around

         if (World->GetVoxelLocation(&Loc, &NewLocation))
         {
           Voxel = Loc.Sector->Data[Loc.Offset];
           if (VoxelTypeManager->VoxelTable[Voxel]->Is_Interface_PushBlock)
           {
             ULong Cnt = VoxelTypeManager->VoxelTable[Voxel]->Interface_PushBlock_PushTest(&Loc, VoxelType, Count);
             PlacedCount += Cnt;
             Count -= Cnt;
             if (!Count) return(PlacedCount);
           }
           else if (Voxel==248 || Voxel == 98) // Materializer
           {
             NewLocation+= ZVoxelWorld::ProxLoc[Dir];
             if (World->GetVoxelLocation(&Loc, &NewLocation))
               if (Loc.Sector->Data[Loc.Offset]==0)
               {
                 PlacedCount++;
                 Count--;
               }
             if (!Count) return(PlacedCount);
           }

         }

      }


    }

  }

  return(0);
}

void ZVoxelType_WirelessTransmitter::GetScanInformations(ZVoxelCoords * VoxelCoords, UShort VoxelType, ZMemSize VoxelInfo, ZString & Infos)
{
  ZVoxelExtension_WirelessTransmitter * Ext;
  ULong i;

  Ext = (ZVoxelExtension_WirelessTransmitter *)VoxelInfo;

  ZMemSize Count = Ext->GetConsummerCount();
  ZVoxelExtension_WirelessTransmitter::DestPoint Dp;

  for (i=0;i<Count;i++)
  {
    if (Ext->GetConsummerInfo(i, Dp ))
    {
      Infos << "POINT: [" << Dp.Location.x << "," << Dp.Location.y << "," << Dp.Location.z << "]\n";
    }
  }

}

