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
 * ZTool_Constructor.cpp
 *
 *  Created on: 15 juil. 2011
 *      Author: laurent
 */

#include "ZTool_Constructor.h"
#include "ZActorPhysics.h"
#include "ZGame.h"

#ifndef Z_ZSOUND_H
#  include "ZSound.h"
#endif


bool ZTool_Constructor::Tool_MouseButtonClick(ULong Button)
{
  ZActor_Player * Actor;

  Actor = (ZActor_Player *)GameEnv->PhysicEngine->GetSelectedActor();

  if (Actor)
  {
    switch(Button)
    {
      case 2:  // Right mouse button

               if (Actor->PointedVoxel.Collided)
               {
                 ZInventory::Entry * InventorySlot;
                 UShort VoxelType;
                 ZVoxelLocation VLoc;
                 ZVector3d VoxelCenter, VoxelDistance;
                 bool AllowBuild;

                 // Ensure you are not too close to the voxel position you want to build.

                 Actor->PointedVoxel.PredPointedVoxel.GetVoxelCenterCoords(&VoxelCenter);
                 VoxelDistance = VoxelCenter - Actor->Location;

                 if (    (VoxelDistance.y < 512.0 && VoxelDistance.y > -127.0)
                      && (VoxelDistance.x < 203.0 && VoxelDistance.x >-203.0)
                      && (VoxelDistance.z < 203.0 && VoxelDistance.z >-203.0) )
                 {
                   GameEnv->GameWindow_Advertising->Advertise("TOO CLOSE", ZGameWindow_Advertising::VISIBILITY_HIGH,0, 2000, 1000);
                   break;
                 }

                 InventorySlot = Actor->Inventory->GetActualItemSlot();
                 VoxelType = InventorySlot->VoxelType;

                 AllowBuild = (COMPILEOPTION_ALLOWJUMPANDBUILD==1) ? true : Actor->IsOnGround;

                 if ( VoxelType == 0 || InventorySlot->Quantity == 0 || AllowBuild==false ) break;

                 if (1UL == Actor->Inventory->UnstoreBlocks(VoxelType,1UL))
                 {
                   if (GameEnv->World->SetVoxel_WithCullingUpdate(Actor->PointedVoxel.PredPointedVoxel.x, Actor->PointedVoxel.PredPointedVoxel.y, Actor->PointedVoxel.PredPointedVoxel.z, VoxelType, ZVoxelSector::CHANGE_CRITICAL, true, &VLoc)) VLoc.Sector ->Flag_HighPriorityRefresh = true;
                   GameEnv->Sound->PlaySound(7);
                 }
               }
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
               UShort Voxel;
               ZVoxelType * VoxelType;
               ZVoxelLocation Loc;
               ZString Reason;

               // Does a voxel is pointed

               if (!Actor->PointedVoxel.Collided) break;

               // Get the voxel informations

               if ( !GameEnv->World->GetVoxelLocation(&Loc, Actor->PointedVoxel.PointedVoxel.x, Actor->PointedVoxel.PointedVoxel.y, Actor->PointedVoxel.PointedVoxel.z )) break;
               Voxel = Loc.Sector->Data[Loc.Offset];
               VoxelType = GameEnv->VoxelTypeManager.GetVoxelType(Voxel);

               // Does this tool can break the class type of this voxel ?

               if (! ToolCompatibleTypes[VoxelType->MiningType])
               {
                 GameEnv->GameWindow_Advertising->Advertise("TOO HARD", ZGameWindow_Advertising::VISIBILITY_MEDIUM, 1, 1000, 200);
                 break;
               }

               // Does the voxel accept to be destroyed ?

               if (!VoxelType->UserAction_TryToDestroy(&Loc, &Reason))
               {
                 GameEnv->GameWindow_Advertising->Clear();
                 GameEnv->GameWindow_Advertising->Advertise(Reason.String, ZGameWindow_Advertising::VISIBILITY_MEDIUM, 1, 1000, 200);
                 break;
               }

               // Everything is right, so we start the mining process...

               MiningProgress.Start(VoxelType->MiningHardness, Actor->PointedVoxel.PointedVoxel);
             }
             break;

      case 3: // Mouse scroll down
             Actor->Inventory->Select_PreviousItem();

             break;
      case 4: // Mouse scroll up
             Actor->Inventory->Select_NextItem();

             break;

    }
  }
  return(true);
}

bool ZTool_Constructor::Tool_MouseButtonRelease(ULong Button)
{
  ZActor * Actor;

  Actor = GameEnv->PhysicEngine->GetSelectedActor();

  if (Actor)
  {
    switch(Button)
    {
      case 2:  // Right mouse button
               break;

      case 1:  // Wheel Button
               break;

      case 0:  // Left button
               if (MiningProgress.Is_InProgress()) MiningProgress.Stop();
               break;

      case 3:  // Mouse scroll down
               break;

      case 4:  // Mouse scroll up
               break;
    }
  }
  return(true);
}


bool ZTool_Constructor::Tool_StillEvents(double FrameTime, bool * MouseButtonMatrix, UByte * KeyboardMatrix )
{
  ZString Reason;
  ZActor_Player * Actor;
  Actor = (ZActor_Player *)GameEnv->PhysicEngine->GetSelectedActor(); if (!Actor) return(true);

  // Breaking material in progress

  if (MouseButtonMatrix[1] && MiningProgress.Is_InProgress())
  {
    UShort Voxel;
    ZVoxelType * VoxelType;
    ZVoxelLocation VLoc;

    // If no pointed voxel, put system on pause and do nothing.

    if (!Actor->PointedVoxel.Collided)
    {
      MiningProgress.SetNoTarget();
      GameEnv->GameProgressBar->SetCompletion(0.0f);
      return(true);
    }

    // Get Pointed Voxel
    if (!GameEnv->World->GetVoxelLocation(&VLoc, Actor->PointedVoxel.PointedVoxel.x, Actor->PointedVoxel.PointedVoxel.y, Actor->PointedVoxel.PointedVoxel.z)) return(true);

    Voxel = VLoc.Sector->Data[VLoc.Offset];
    VoxelType = GameEnv->VoxelTypeManager.GetVoxelType(Voxel);

    // Uhhh, the player has moved is tool on another voxel, so resetting mining.


    if (Actor->PointedVoxel.PointedVoxel != MiningProgress.GetMinedVoxel())
    {
      // Does this tool can break this material ?
      if (ToolCompatibleTypes[VoxelType->MiningType])
      {
        // Does the voxel accept to be destroyed ?
        if (!VoxelType->UserAction_TryToDestroy(&VLoc, &Reason))
        {
          GameEnv->GameWindow_Advertising->Clear();
          GameEnv->GameWindow_Advertising->Advertise(Reason.String, ZGameWindow_Advertising::VISIBILITY_MEDIUM, 1, 1000, 200);

          MiningProgress.Stop();
          return(true);
        }
        // Ok, this voxel can be destroyed, so set it to be the new target in the continuous mining process.
        MiningProgress.SetNewtarget(VoxelType->MiningHardness, Actor->PointedVoxel.PointedVoxel);

      }
    }

    // Do mining of the voxel.

    if (MiningProgress.DoMine(this->ToolForce[VoxelType->MiningType] * FrameTime))
    {
      // The voxel is broken, so store it.
      if(Actor->Inventory->StoreBlocks(Voxel,1))
      {
        // If voxel is stored ok, destroy it in the world.
        if ( GameEnv->World->SetVoxel_WithCullingUpdate(Actor->PointedVoxel.PointedVoxel.x, Actor->PointedVoxel.PointedVoxel.y, Actor->PointedVoxel.PointedVoxel.z,0,ZVoxelSector::CHANGE_CRITICAL,true,&VLoc) ) VLoc.Sector->Flag_HighPriorityRefresh = true;
      }
    }
  }
  return(true);
};



void ZTool_Constructor::Start_Tool()
{
  // If mining is in progress, stop it.

  if (MiningProgress.Is_InProgress()) MiningProgress.Stop();

  // Show the voxel destruction progress bar.

  if (!GameEnv->VoxelTypeBar->Is_Shown()) GameEnv->VoxelTypeBar->Show();
}

void ZTool_Constructor::End_Tool()
{
  // If mining is in progress, stop it.

  if (MiningProgress.Is_InProgress()) MiningProgress.Stop();

  // Hide the voxel destruction progress bar.

  if (GameEnv->VoxelTypeBar->Is_Shown()) GameEnv->VoxelTypeBar->Hide();
}

void ZTool_Constructor::Display()
{
  ZGameWindow_VoxelTypeBar * VoxelTypeBar;
  ZActor * Actor;
  ULong ActualSlotNum, i;

  Long Slot;

  Actor = GameEnv->PhysicEngine->GetSelectedActor();
  VoxelTypeBar = GameEnv->VoxelTypeBar;
  ActualSlotNum = Actor->Inventory->GetActualItemSlotNum();

  //if (ActualSlotNum != OldToolNum)
  {


    // Descending order;

    Slot = (Long)ActualSlotNum;
    Slot -= 6; // Slot offset
    if (Slot < ZInventory::Inventory_StartSlot) {Slot-=ZInventory::Inventory_StartSlot; Slot = ZInventory::Inventory_EndSlot + Slot + 1; }

    for (i=0; i < 14 ; i++)
    {
      ZInventoryBox * Box;
      ZInventory::Entry * InvSlot;

      Box = VoxelTypeBar->GetInventoryBox(i);
      InvSlot = Actor->Inventory->GetSlotRef((ULong)Slot);

      Box->SetVoxelType( &InvSlot->VoxelType );
      Box->SetQuantity ( &InvSlot->Quantity  );

      Slot++; if (Slot > ZInventory::Inventory_EndSlot) Slot = ZInventory::Inventory_StartSlot;

    }

    OldToolNum = ActualSlotNum;
  }

}



