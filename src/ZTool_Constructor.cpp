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

  // printf("Click : %ld\n",Button);

  if (Actor)
  {
    switch(Button)
    {
      case 2: // Right mouse button
             if (Actor->PointedVoxel.Collided)
             {
               ZInventory::Entry * InventorySlot;
               UShort VoxelType;
               ZVoxelLocation VLoc;
               //ULong  OtherInfos;
               ZVector3d VoxelCenter, VoxelDistance;
               bool IsOnGround;;


               // Ensure you are not to close to the voxel position you want to build.
               bool NotTooClose;
               NotTooClose = true;
               Actor->PointedVoxel.PredPointedVoxel.GetVoxelCenterCoords(&VoxelCenter);
               VoxelDistance = VoxelCenter - Actor->Location;
               //printf("VoxelDistance : %lf,%lf,%lf\n", VoxelDistance.x, VoxelDistance.y, VoxelDistance.z);
               if (VoxelDistance.y < 512.0 && VoxelDistance.y > -127.0)
               {
                 if (    (VoxelDistance.x < 203.0 && VoxelDistance.x >-203.0)
                      && (VoxelDistance.z < 203.0 && VoxelDistance.z >-203.0) ) { NotTooClose = false; GameEnv->GameWindow_Advertising->Advertise("TOO CLOSE", ZGameWindow_Advertising::VISIBILITY_HIGH,0, 2000, 1000); }
               }



               InventorySlot = Actor->Inventory->GetActualItemSlot();
               VoxelType = InventorySlot->VoxelType;
               //OtherInfos = 0;
               // printf("Location (%lf,%lf,%lf) Voxel (%lf,%lf,%lf)\n", Actor->Location.x,Actor->Location.y,Actor->Location.z, VoxelCenter.x, VoxelCenter.y, VoxelCenter.z);

#if COMPILEOPTION_ALLOWJUMPANDBUILD==1
               IsOnGround = true;
#else
               IsOnGround = Actor->IsOnGround;
#endif


                 if (VoxelType > 0 && InventorySlot->Quantity > 0 && NotTooClose && IsOnGround )
                 {
                   if (1UL == Actor->Inventory->UnstoreBlocks(VoxelType,1UL))
                   {
                    //VoxelExtensionType = GameEnv->VoxelTypeManager.GetVoxelType(VoxelType)->ExtensionType;
                    //if (VoxelExtensionType!=0) OtherInfos = (ULong)GameEnv->World->ExtensionFactory.CreateVoxelExtension(VoxelExtensionType);
                     //GameEnv->World->SetVoxel_WithCullingUpdate(Actor->PointedVoxel.PredPointedVoxel.x, Actor->PointedVoxel.PredPointedVoxel.y, Actor->PointedVoxel.PredPointedVoxel.z, VoxelType, OtherInfos);

                     // new

                     if (GameEnv->World->SetVoxel_WithCullingUpdate(Actor->PointedVoxel.PredPointedVoxel.x, Actor->PointedVoxel.PredPointedVoxel.y, Actor->PointedVoxel.PredPointedVoxel.z, VoxelType, ZVoxelSector::CHANGE_CRITICAL, true, &VLoc)) VLoc.Sector ->Flag_HighPriorityRefresh = true;
                     GameEnv->Sound->PlaySound(7);
                   }
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
              // printf("Location x:%ld y:%ld z:%ld \n", x, y, z);
            }
            break;
      case 0: // Left button
             {
               UShort Voxel;
               ZVoxelType * VoxelType;
               ZVoxelLocation Loc;
               ZString Reason;

               if (Actor->PointedVoxel.Collided)
               {
                 if ( !GameEnv->World->GetVoxelLocation(&Loc, Actor->PointedVoxel.PointedVoxel.x, Actor->PointedVoxel.PointedVoxel.y, Actor->PointedVoxel.PointedVoxel.z )) break;
                 Voxel = Loc.Sector->Data[Loc.Offset];
                 VoxelType = GameEnv->VoxelTypeManager.GetVoxelType(Voxel);
                 if (ToolCompatibleTypes[VoxelType->MiningType])
                 {
                   // Does the voxel accept to be destroyed.
                   if (!VoxelType->UserAction_TryToDestroy(&Loc, &Reason))
                   {
                     GameEnv->GameWindow_Advertising->Clear();
                     GameEnv->GameWindow_Advertising->Advertise(Reason.String, ZGameWindow_Advertising::VISIBILITY_MEDIUM, 1, 1000, 200);
                     break;
                   }

                   // So do it...
                   Mining_MaterialResistanceCounter = VoxelType->MiningHardness;
                   MiningInProgress = true;
                   MinedVoxel = Actor->PointedVoxel.PointedVoxel;
                   GameEnv->GameProgressBar->SetCompletion(0.0f);
                   GameEnv->GameProgressBar->Show();
                   #if COMPILEOPTION_FNX_SOUNDS_1 == 1
                   if (SoundHandle == 0) SoundHandle = GameEnv->Sound->Start_PlaySound(5,true,true,1.0,0);
                   #endif
                 }
                 else
                 {
                   GameEnv->GameWindow_Advertising->Advertise("TOO HARD", ZGameWindow_Advertising::VISIBILITY_MEDIUM, 1, 1000, 200);
                 }

               }
               /*
               UShort VoxelType;
               VoxelType = GameEnv->World->GetVoxel(Actor->PointedVoxel.PointedVoxel.x, Actor->PointedVoxel.PointedVoxel.y, Actor->PointedVoxel.PointedVoxel.z);
               GameEnv->World->SetVoxel_WithCullingUpdate(Actor->PointedVoxel.PointedVoxel.x, Actor->PointedVoxel.PointedVoxel.y, Actor->PointedVoxel.PointedVoxel.z,0);
               Actor->Inventory->StoreBlocks(VoxelType,1);
               printf("Mining\n");
               */
             }
             break;
      case 3: // Mouse scroll down
             Actor->Inventory->Select_PreviousItem();

             // printf("Selected : %ld : %s\n", Actor->Inventory->GetActualItemSlotNum(), GameEnv->VoxelTypeManager.VoxelTable[Actor->Inventory->GetActualItemSlot()->VoxelType]->VoxelTypeName.String);
             break;
      case 4: // Mouse scroll up
             Actor->Inventory->Select_NextItem();

             // printf("Selected : %ld : %s\n", Actor->Inventory->GetActualItemSlotNum(), GameEnv->VoxelTypeManager.VoxelTable[Actor->Inventory->GetActualItemSlot()->VoxelType]->VoxelTypeName.String);
             break;


    }
  }
  return(true);
}

bool ZTool_Constructor::Tool_MouseButtonRelease(ULong Button)
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
             {
               //UShort Voxel;
               if (MiningInProgress)
               {
                 Mining_MaterialResistanceCounter = 0;
                 MiningInProgress = false;
                 GameEnv->GameProgressBar->SetCompletion(0.0f);
                 GameEnv->GameProgressBar->Hide();
                 #if COMPILEOPTION_FNX_SOUNDS_1 == 1
                 if (SoundHandle != 0) { GameEnv->Sound->Stop_PlaySound(SoundHandle); SoundHandle = 0; }
                 #endif
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


bool ZTool_Constructor::Tool_StillEvents(double FrameTime, bool * MouseButtonMatrix, UByte * KeyboardMatrix )
{
  ZString Reason;
  ZActor_Player * Actor;
  Actor = (ZActor_Player *)GameEnv->PhysicEngine->GetSelectedActor(); if (!Actor) return(true);


  // Breaking material in progress

  if (MouseButtonMatrix[1] && MiningInProgress)
  {
    UShort Voxel;
    ZVoxelType * VoxelType;
    ZVoxelLocation VLoc;

    // Get actualy pointed voxel

    if (!Actor->PointedVoxel.Collided)
    {
      Mining_MaterialResistanceCounter = 1000;
      GameEnv->GameProgressBar->SetCompletion(0.0f);
      return(true);
    }


    if (!GameEnv->World->GetVoxelLocation(&VLoc, Actor->PointedVoxel.PointedVoxel.x, Actor->PointedVoxel.PointedVoxel.y, Actor->PointedVoxel.PointedVoxel.z)) return(true);

    Voxel = VLoc.Sector->Data[VLoc.Offset];
    VoxelType = GameEnv->VoxelTypeManager.GetVoxelType(Voxel);

    // Uhhh, the player has moved is tool on another voxel, so resetting mining.

    if (Actor->PointedVoxel.PointedVoxel != MinedVoxel)
    {
      // Does this tool can break this material ?
      if (ToolCompatibleTypes[VoxelType->MiningType])
      {
        // Does the voxel accept to be destroyed ?
        if (!VoxelType->UserAction_TryToDestroy(&VLoc, &Reason))
        {
          GameEnv->GameWindow_Advertising->Clear();
          GameEnv->GameWindow_Advertising->Advertise(Reason.String, ZGameWindow_Advertising::VISIBILITY_MEDIUM, 1, 1000, 200);
          return(true);
        }
        // So, do it...
        Mining_MaterialResistanceCounter = VoxelType->MiningHardness;
        MiningInProgress = true;
        MinedVoxel = Actor->PointedVoxel.PointedVoxel;

      }
    }

    // Material resistance is slowly going down

    Mining_MaterialResistanceCounter -= this->ToolForce[VoxelType->MiningType] * FrameTime;

    // printf("Resistance :%lf\n",Mining_MaterialResistanceCounter);
    GameEnv->GameProgressBar->SetCompletion( (100.0 / VoxelType->MiningHardness) * (VoxelType->MiningHardness - Mining_MaterialResistanceCounter)   );
    // Ok, it's breaking

    if (Mining_MaterialResistanceCounter < 0.0)
    {
      Mining_MaterialResistanceCounter =10000.0;
      if(Actor->Inventory->StoreBlocks(Voxel,1))
      {
        if ( GameEnv->World->SetVoxel_WithCullingUpdate(Actor->PointedVoxel.PointedVoxel.x, Actor->PointedVoxel.PointedVoxel.y, Actor->PointedVoxel.PointedVoxel.z,0,ZVoxelSector::CHANGE_CRITICAL,true,&VLoc) ) VLoc.Sector->Flag_HighPriorityRefresh = true;
      }
      #if COMPILEOPTION_FNX_SOUNDS_1 == 1
      GameEnv->Sound->PlaySound(6);
      if (SoundHandle != 0) { GameEnv->Sound->Stop_PlaySound(SoundHandle); SoundHandle = 0; }
      #endif
      // Sector->Flag_HighPriorityRefresh
    }
  }
  return(true);
};



void ZTool_Constructor::Start_Tool()
{
  MiningInProgress = false;
  if (!GameEnv->VoxelTypeBar->Is_Shown()) GameEnv->VoxelTypeBar->Show();
}

void ZTool_Constructor::End_Tool()
{
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
