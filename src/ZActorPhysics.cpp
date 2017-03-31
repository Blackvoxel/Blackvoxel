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
 * ZActorPhysics.cpp
 *
 *  Created on: 19 janv. 2011
 *      Author: laurent
 */

#include "ZActorPhysics.h"
#include <math.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "ZHighPerfTimer.h"

#include "ZVoxelType.h"
#include "ZGame.h"

ZActor::ZActor()
{
  ULong i;
  Next = Pred = 0;

  Location.x = Location.y = Location.z = 0.0;
  Velocity.x = Velocity.y = Velocity.z = 0.0;
  Deplacement.x = Deplacement.y = Deplacement.z = 0.0;
  ViewDirection.yaw = ViewDirection.pitch = ViewDirection.roll = ViewDirection.Len = 0.0;
  CollideWithVoxels = false;
  CollideWithActors = false;
  Flag_ActivateAntiFall = false;
  TakesGravity      = true;
  DammageThreshold  = 599.0;
  LifePoints = 1000;
  JumpDebounce = 0;
  BuildingMaterial = 1;
  for (i=0; i<8; i++) MouseButtonMatrix[i]=0;
  Inventory = 0;
  Camera.ColoredVision.Activate = false;
  Camera.ColoredVision.Blue = 1.0f;
  Camera.ColoredVision.Red  = 1.0f;
  Camera.ColoredVision.Green= 1.0f;
  KeepControlOnJumping = true;
  IsDead = false;
  IsOnGround = false;

  order=false;
  PhysicsEngine = 0;
  DeathChronometer = 0.0;
  GameEnv = 0;

  Time_TotalGameTime = 0;
  Time_ElapsedTimeSinceLastRespawn = 0;
  LearningModePage = 0;

}

void ZActor::TakeDammage(double Dammage)
{
  if (Dammage > DammageThreshold) LifePoints -= (Long) (Dammage / 6.0);
  if (LifePoints < 0) {LifePoints = 0;}
}

void ZActor::SetPosition( ZVector3d &NewLocation )
{
  Location.x = NewLocation.x;
  Location.y = NewLocation.y;
  Location.z = NewLocation.z;
}




/*
void ZActor::Action_MouseMove(Long Delta_x, Long Delta_y)
{
  ViewDirection.yaw-=Delta_x;
  if (ViewDirection.yaw >= 360.0) ViewDirection.yaw -= 360.0;
  if (ViewDirection.yaw <0 ) ViewDirection.yaw += 360.0;
  ViewDirection.pitch-=Delta_y;
  if (ViewDirection.pitch >= 360.0) ViewDirection.pitch -= 360.0;
  if (ViewDirection.pitch <0 ) ViewDirection.pitch += 360.0;
}

void ZActor::Action_MouseButtonClick(ULong Button)
{
  MouseButtonMatrix[Button] = 1;
  if (Button==0) PhysicsEngine->World->SetVoxel_WithCullingUpdate(PointedVoxel.PredPointedVoxel.x, PointedVoxel.PredPointedVoxel.y, PointedVoxel.PredPointedVoxel.z, BuildingMaterial,true);
  if (Button!=0) PhysicsEngine->World->SetVoxel_WithCullingUpdate(PointedVoxel.PointedVoxel.x, PointedVoxel.PointedVoxel.y, PointedVoxel.PointedVoxel.z,0,true);
}

void ZActor::Action_MouseButtonRelease(ULong Button)
{
  MouseButtonMatrix[Button] = 0;
}
*/

bool ZActor::Action_StillEvents( bool * MouseMatrix, UByte * KeyboardMatrix)
{
  return(true);
}

void ZActor::Action_NextBuildingMaterial()
{
  ZVoxelTypeManager * VoxelTypeManager = this->PhysicsEngine->GetVoxelTypeManager();
  ZVoxelType * VoxelType;
  if (!VoxelTypeManager) return;
  BuildingMaterial++;
  if ( 0==(VoxelType = VoxelTypeManager->GetVoxelType(BuildingMaterial)) )
  {
    BuildingMaterial--;
  }
  else
  {
    printf("Selected Material %u: %s\n", BuildingMaterial, VoxelType->VoxelTypeName.String);
  }
}

void ZActor::Action_PrevBuildingMaterial()
{
  ZVoxelTypeManager * VoxelTypeManager = this->PhysicsEngine->GetVoxelTypeManager();
  ZVoxelType * VoxelType;
  if (!VoxelTypeManager) return;
  if (BuildingMaterial >1) BuildingMaterial--;
  if ( 0==(VoxelType = VoxelTypeManager->GetVoxelType(BuildingMaterial)) )
  {
    BuildingMaterial++;
  }
  else
  {
    printf("Selected Material %u: %s\n", BuildingMaterial, VoxelType->VoxelTypeName.String);
  }
}

void ZActor::Action_NextTool()
{
  Inventory->NextTool();
}

void ZActor::Action_PrevTool()
{
  Inventory->PrevTool();
}


void ZActorPhysicEngine::DoPhysic(UELong FrameTime)
{
  ZActor * Actor;
  ZVector3d NewLocation;
  Actor = ActorList;


  // double Dammage;


  while (Actor)
  {
    Actor->DoPhysic(FrameTime);
    if (FrameTime < 1000000)
    {
      Actor->Time_TotalGameTime += FrameTime;
      Actor->Time_ElapsedTimeSinceLastRespawn += FrameTime;
    }

    //return;
/*
    if (Actor->JumpDebounce > 0) Actor->JumpDebounce--;

    for (i=0;i<25;i++)
    {
      //if (Actor
      NewLocation.x = Actor->Deplacement.x / 25.0 + Actor->Location.x + Actor->Velocity.x * 1 / 1000.0;
      NewLocation.y = Actor->Deplacement.y / 25.0 + Actor->Location.y + Actor->Velocity.y * 1 / 1000.0;
      NewLocation.z = Actor->Deplacement.z / 25.0 + Actor->Location.z + Actor->Velocity.z * 1 / 1000.0;

      Voxel = World->GetVoxel(NewLocation.x,NewLocation.y,NewLocation.z);


      P[0] = World->GetVoxelPlayerCoord(NewLocation.x-75.0,NewLocation.y-1.0, NewLocation.z + 65.0);
      P[1] = World->GetVoxelPlayerCoord(NewLocation.x+75.0,NewLocation.y-1.0, NewLocation.z + 65.0);
      P[2] = World->GetVoxelPlayerCoord(NewLocation.x+75.0,NewLocation.y-1.0, NewLocation.z - 65.0);
      P[3] = World->GetVoxelPlayerCoord(NewLocation.x-75.0,NewLocation.y-1.0, NewLocation.z - 65.0);

      P[4] = World->GetVoxelPlayerCoord(NewLocation.x-75.0,NewLocation.y+500.0, NewLocation.z + 75.0);
      P[5] = World->GetVoxelPlayerCoord(NewLocation.x+75.0,NewLocation.y+500.0, NewLocation.z + 75.0);
      P[6] = World->GetVoxelPlayerCoord(NewLocation.x+75.0,NewLocation.y+500.0, NewLocation.z - 75.0);
      P[7] = World->GetVoxelPlayerCoord(NewLocation.x-75.0,NewLocation.y+500.0, NewLocation.z - 75.0);

      P[8] = World->GetVoxelPlayerCoord(NewLocation.x+85.0,NewLocation.y+128.0, NewLocation.z + 90.0);
      P[9] = World->GetVoxelPlayerCoord(NewLocation.x-85.0,NewLocation.y+128.0, NewLocation.z + 90.0);
      P[10] = World->GetVoxelPlayerCoord(NewLocation.x+85.0,NewLocation.y+384.0, NewLocation.z + 90.0);
      P[11] = World->GetVoxelPlayerCoord(NewLocation.x-85.0,NewLocation.y+384.0, NewLocation.z + 90.0);

      P[12] = World->GetVoxelPlayerCoord(NewLocation.x+90.0,NewLocation.y+128.0, NewLocation.z + 85.0);
      P[13] = World->GetVoxelPlayerCoord(NewLocation.x+90.0,NewLocation.y+128.0, NewLocation.z - 85.0);
      P[14] = World->GetVoxelPlayerCoord(NewLocation.x+90.0,NewLocation.y+384.0, NewLocation.z + 85.0);
      P[15] = World->GetVoxelPlayerCoord(NewLocation.x+90.0,NewLocation.y+384.0, NewLocation.z - 85.0);

      P[16] = World->GetVoxelPlayerCoord(NewLocation.x+85.0,NewLocation.y+128.0, NewLocation.z - 90.0);
      P[17] = World->GetVoxelPlayerCoord(NewLocation.x-85.0,NewLocation.y+128.0, NewLocation.z - 90.0);
      P[18] = World->GetVoxelPlayerCoord(NewLocation.x+85.0,NewLocation.y+384.0, NewLocation.z - 90.0);
      P[19] = World->GetVoxelPlayerCoord(NewLocation.x-85.0,NewLocation.y+384.0, NewLocation.z - 90.0);

      P[20] = World->GetVoxelPlayerCoord(NewLocation.x-90.0,NewLocation.y+128.0, NewLocation.z + 85.0);
      P[21] = World->GetVoxelPlayerCoord(NewLocation.x-90.0,NewLocation.y+128.0, NewLocation.z - 85.0);
      P[22] = World->GetVoxelPlayerCoord(NewLocation.x-90.0,NewLocation.y+384.0, NewLocation.z + 85.0);
      P[23] = World->GetVoxelPlayerCoord(NewLocation.x-90.0,NewLocation.y+384.0, NewLocation.z - 85.0);

     // if (i==0) printf("P0:%d P1:%d P2:%d P3:%d P4:%d P5:%d P6:%d P7:%d P8:%d P9:%d P10:%d P11:%d P12:%d P13:%d P14:%d P15:%d P16:%d P17:%d P18:%d P19:%d P20:%d P21:%d P22:%d P23:%d \n",
     //       P[0], P[1], P[2], P[3], P[4], P[5], P[6], P[7], P[8], P[9], P[10], P[11], P[12], P[13], P[14], P[15], P[16], P[17], P[18], P[19], P[20], P[21], P[22], P[23]);


      Dx = fabs(NewLocation.x - Actor->Location.x);
      Dz = fabs(NewLocation.z - Actor->Location.z);

      // Collision on Bottom side

      if ( (Actor->Velocity.y < 0.0) && ( ( P[0]!=0 ) || (P[1]!=0) || (P[2]!=0) || (P[3]!=0) ) )
      {
        Actor->Velocity.y = 0.0;
        NewLocation.y = (double)(((ELong)(NewLocation.y + 128.0)) & 0xFFFFFFFFFFFFFF00LL);
        //Actor->JumpDebounce=0;
      }
      // Collision on top side

      if ( (Actor->Velocity.y > 0.0) && ( ( P[4]!=0 ) || (P[5]!=0) || (P[6]!=0) || (P[7]!=0) ) )
      {
        Actor->Velocity.y = 0.0;
        NewLocation.y = (double)(((ELong)(NewLocation.y + 128.0)) & 0xFFFFFFFFFFFFFF00LL);
      }

      if ( (NewLocation.z > Actor->Location.z ) && ( (P[8]!=0)  || (P[9]!=0)  || P[10]!=0 || P[11]!=0 )) NewLocation.z = Actor->Location.z;
      if ( (NewLocation.z < Actor->Location.z ) && ( (P[16]!=0) || (P[17]!=0) || P[18]!=0 || P[19]!=0 )) NewLocation.z = Actor->Location.z;
      if ( (NewLocation.x > Actor->Location.x ) && ( (P[12]!=0) || (P[13]!=0) || P[14]!=0 || P[15]!=0 )) NewLocation.x = Actor->Location.x;
      if ( (NewLocation.x < Actor->Location.x ) && ( (P[20]!=0) || (P[21]!=0) || P[22]!=0 || P[23]!=0 )) NewLocation.x = Actor->Location.x;

      if (false)
      {
      if (Dz > Dx)
      {

        // Collision on Face side
        if ( (NewLocation.z > Actor->Location.z ) && ( ( P[8]!=0 ) || (P[9]!=0) || (P[10]!=0) || (P[11]!=0) ) )
        {
          NewLocation.z = ((double)(((ELong)(NewLocation.z + 128.0)) & 0xFFFFFFFFFFFFFF00LL)) - 85.0;
          Actor->Velocity.z = 0.0;
        }

      // Collision on Back side

        if ( (NewLocation.z < Actor->Location.z ) && (  ( P[16]!=0 ) || (P[17]!=0) || (P[18]!=0) || (P[19]!=0) ) )
        {
          NewLocation.z = ((double)(((ELong)(NewLocation.z + 128.0)) & 0xFFFFFFFFFFFFFF00LL)) + 85.0;
          Actor->Velocity.z = 0.0;
        }
      }
      else
      {
        // Collision on Left side
        if ( (NewLocation.x < Actor->Location.x ) && ( ( P[20]!=0 ) || (P[21]!=0) || (P[22]!=0) || (P[23]!=0) ) )
        {
          NewLocation.x = ((double)(((ELong)(NewLocation.x + 128.0)) & 0xFFFFFFFFFFFFFF00LL)) + 85.0;
          Actor->Velocity.x = 0.0;
        }

        // Collision on Right side

        if ( (NewLocation.x > Actor->Location.x ) && (  ( P[12]!=0 ) || (P[13]!=0) || (P[14]!=0) || (P[15]!=0) ) )
        {
          NewLocation.x = ((double)(((ELong)(NewLocation.x + 128.0)) & 0xFFFFFFFFFFFFFF00LL)) - 85.0;
          Actor->Velocity.x = 0.0;
        }
      }
      }
      Actor->SetPosition(NewLocation);

     // if (i==0) printf("Actor x: %lf %lf %lf NewLocation %lf %lf %lf  Deplacement: %lf %lf %lf \n", Actor->Location.x, Actor->Location.y, Actor->Location.z, NewLocation.x, NewLocation.y, NewLocation.z, Actor->Deplacement.x, Actor->Deplacement.y, Actor->Deplacement.z);


      if ( (P[0] == 0) && (P[1] == 0) && (P[2] == 0) && (P[3] == 0) )
      {
        //Actor->Velocity.x += (Gravity.x * CycleTime / 1000.0);
        Actor->Velocity.y += (Gravity.y * 0.72 / 1000.0);
        //Actor->Velocity.z += (Gravity.z * CycleTime / 1000.0);
      }

    }
    Actor->Deplacement.x = 0; Actor->Deplacement.y = 0; Actor->Deplacement.z = 0;
*/

    Actor = Actor->Next;
  }

}


void ZActorPhysicEngine::AddActor(ZActor * Actor)
{
  Actor->PhysicsEngine = this;
  if (!ActorList)
  {
    ActorList = Actor;
    Actor->Next = 0;
    Actor->Pred = 0;
    return;
  }
  Actor->Next = ActorList;
  Actor->Pred = 0;
  ActorList->Pred = Actor;
  ActorList = Actor;
}

void ZActorPhysicEngine::RemoveActor(ZActor * Actor)
{
  if (Actor == ActorList) {ActorList = Actor->Next; Actor->Next = 0; Actor->Pred = 0; return;}
  Actor->Next->Pred = Actor->Pred;
  Actor->Pred->Next = Actor->Next;
  Actor->Next = 0;
  Actor->Pred = 0;
}

ZActorPhysicEngine::ZActorPhysicEngine()
{
  ActorList = 0;
  SelectedActor = 0;
  Gravity.x = 0.0;
  Gravity.y = -9810.0;
  Gravity.z = 0.0;
  VoxelTypeManager = 0;
  World = 0;
  ToolManager = 0;
}

ZActorPhysicEngine::~ZActorPhysicEngine()
{
  ZActor * Actor, * NextActor;

  // Freeing actors in Actor list.

  Actor = ActorList;
  while (Actor)
  {
    NextActor = Actor->Next;
    delete Actor;
    Actor = NextActor;
  }
  ActorList = 0;
}





void ZActor::DoPhysic(UELong FrameTime)
{
  ZVector3d Dep,Dep2;
  ZVector3d Tmp;
  ZVector3d P[32];
  Bool PEnable[32];
  bool PInvert[32];
  UShort Voxel[32];
  bool   IsEmpty[32];
  ULong i;
  ZVoxelWorld * World;
  double DepLen;

  double CycleTime = FrameTime / 1000.0;

  return;
  //printf("Entry\n");

  // Define Detection points

  P[0] = Location + ZVector3d(-75.0,+0.0,+75.0);
  P[1] = Location + ZVector3d(+75.0,+0.0,+75.0);
  P[2] = Location + ZVector3d(+75.0,+0.0,-75.0);
  P[3] = Location + ZVector3d(-75.0,+0.0,-75.0);

  P[4] = Location + ZVector3d(-75.0,+128.0,+75.0);
  P[5] = Location + ZVector3d(+75.0,+128.0,+75.0);
  P[6] = Location + ZVector3d(+75.0,+128.0,-75.0);
  P[7] = Location + ZVector3d(-75.0,+128.0,-75.0);

  P[8] = Location + ZVector3d(-75.0,+384.0,+75.0);
  P[9] = Location + ZVector3d(+75.0,+384.0,+75.0);
  P[10] = Location + ZVector3d(+75.0,+384.0,-75.0);
  P[11] = Location + ZVector3d(-75.0,+384.0,-75.0);

  P[12] = Location + ZVector3d(-75.0,+500.0,+75.0);
  P[13] = Location + ZVector3d(+75.0,+500.0,+75.0);
  P[14] = Location + ZVector3d(+75.0,+500.0,-75.0);
  P[15] = Location + ZVector3d(-75.0,+500.0,-75.0);

  P[16] = Location + ZVector3d(-70.0,-5.0,-70.0); // # Detection points behind the player
  P[17] = Location + ZVector3d(+70.0,-5.0,-70.0); // # Used for Anti-Fall.
  P[18] = Location + ZVector3d(+70.0,-5.0,+70.0); // #
  P[19] = Location + ZVector3d(-70.0,-5.0,+70.0); // #

  P[20] = Location + ZVector3d(0,-5,0);  // # Detection point are only for voxel
  P[21] = Location + ZVector3d(0,0,0);   // #
  P[22] = Location + ZVector3d(0,128,0); // #
  P[23] = Location + ZVector3d(0,384,0); // #

  // Get the Voxel Informations

  World = PhysicsEngine->World;
  for (i=0;i<24;i++)
  {
    Voxel[i]=World->GetVoxelPlayerCoord(P[i].x,P[i].y,P[i].z);
    IsEmpty[i] = GameEnv->VoxelTypeManager.VoxelTable[Voxel[i]]->Is_PlayerCanPassThrough;
  }

  // Detect player is on ground

  if ( IsEmpty[16] && IsEmpty[17] && IsEmpty[18] && IsEmpty[19] ) IsOnGround = false;
  else                                                            IsOnGround = true;

  // Voxel collision and player damage.

  Camera.ColoredVision.Activate = false;
  bool harming = 0;
  //UShort HarmingVoxel;
  double MaxHarming = 0;
  double VoxelHarming;
  for(i=0;i<16;i++) if (GameEnv->VoxelTypeManager.VoxelTable[Voxel[i]]->Is_Harming)
  {
    harming |= true;
    //HarmingVoxel = Voxel[i];
    VoxelHarming = GameEnv->VoxelTypeManager.VoxelTable[Voxel[i]]->HarmingLifePointsPerSecond;
    if (VoxelHarming > MaxHarming) MaxHarming = VoxelHarming;
  }
  if ((harming))
  {
    Camera.ColoredVision.Activate = true;
    Camera.ColoredVision.Red      = 1.0f;
    Camera.ColoredVision.Green    = 0.0f;
    Camera.ColoredVision.Blue     = 0.0f;
    Camera.ColoredVision.Opacity  = 0.5f;
    // printf("LifePoints: %lf \n",LifePoints);
    LifePoints -= MaxHarming / 1000.0 * CycleTime;
    if (LifePoints < 0.0) LifePoints = 0.0;
  }

  if (LifePoints <= 0.0 && !IsDead)
  {
    this->IsDead = true;
    GameEnv->GameWindow_Advertising->Advertise("YOU ARE DEAD", 0, 0, 10000.0, 10000.0);
    DeathChronometer = 10000.0;
  }

  if (IsDead)
  {
    DeathChronometer -= CycleTime;
    if (DeathChronometer <= 0.0)
    {
      Event_Death();

    }
    return;
  }

  // Vision change for the head in voxel


  if (Voxel[23] == 52)
  {
    Camera.ColoredVision.Activate = true;
    Camera.ColoredVision.Red     = 1.0f;
    Camera.ColoredVision.Green   = 0.0f;
    Camera.ColoredVision.Blue    = 0.0f;
    Camera.ColoredVision.Opacity = 0.95f;
  }

  // Physical deplacement computation

  if (0)
  {
    if (CycleTime > 160.0) CycleTime = 160.0; // Limit frame time

    Velocity.y -= 5.0 * CycleTime;

    Dep.x = Deplacement.x / 1.0 + Velocity.x * CycleTime / 1000.0;
    Dep.y = Deplacement.y / 1.0 + Velocity.y * CycleTime / 1000.0;
    Dep.z = Deplacement.z / 1.0 + Velocity.z * CycleTime / 1000.0;
  }
  if (0)
  {
    ZVector3d Frottement, Sign;
    double FrictionCoef;
    if (CycleTime > 160.0) CycleTime = 160.0; // Limit frame time
    Velocity.y -= 50.0 * CycleTime;
    Velocity += Deplacement * 100.0;

    Sign.x = 1.0 ;if (Velocity.x < 0.0) Sign.x = -1.0;
    Sign.y = 1.0 ;if (Velocity.y < 0.0) Sign.y = -1.0;
    Sign.z = 1.0 ;if (Velocity.z < 0.0) Sign.z = -1.0;
    FrictionCoef = GameEnv->VoxelTypeManager.VoxelTable[Voxel[20]]->FrictionCoef;
    FrictionCoef+= GameEnv->VoxelTypeManager.VoxelTable[Voxel[22]]->FrictionCoef;
    FrictionCoef+= GameEnv->VoxelTypeManager.VoxelTable[Voxel[23]]->FrictionCoef;
    Frottement = (Velocity * Velocity * FrictionCoef / 1000000000);
    printf("Velocity: %lf %lf %lf  Frottement : %lf %lf %lf Friction: %lf\n",Velocity.x, Velocity.y, Velocity.z ,Frottement.x, Frottement.y, Frottement.z, FrictionCoef);
    Velocity = Velocity / (Frottement + 1.0);
    if (Velocity.x > 0.01)  Velocity.x-=0.01;
    if (Velocity.x < -0.01) Velocity.x+=0.01;
    if (Velocity.z > 0.01)  Velocity.z-=0.01;
    if (Velocity.z < -0.01) Velocity.z+=0.01;
    Dep = Velocity * CycleTime / 10000.0;
  }
  if (1)
  {
    ZVector3d Frottement, WalkSpeed, VelocityIncrease, MaxVelocityIncrease, GripFactor;
    double FrictionCoef;

    // Limit the frametime to avoid accidental stuttering nasty effects.

    if (CycleTime > 160.0) CycleTime = 160.0; // Limit frame time

    // Jumping from certain blocks won't permit you any motion control in space...

    bool ForceLostControll = false;;
    if (!GameEnv->VoxelTypeManager.VoxelTable[Voxel[20]]->Is_PlayerCanPassThrough)
    {
      KeepControlOnJumping = GameEnv->VoxelTypeManager.VoxelTable[Voxel[20]]->Is_KeepControlOnJumping;
    }
    else
    {
      if ( !KeepControlOnJumping ) {Deplacement = 0.0; ForceLostControll = true; } // Cancel space control if you jump from these blocks...
    }

    // The gravity...
    double Gravity, CubeY;
    CubeY = Location.y / 256.0;
    if      (CubeY > 10000.0 && CubeY < 15000.0) { Gravity = 5.0 - (( (CubeY-10000.0) * (CubeY-10000.0)) / 5000000.0); } //5000000.0;
    else if (CubeY <= 10000.0) { Gravity = 5.0; }
    else                       { Gravity = 0.0; }

    Velocity.y -= Gravity * CycleTime;
    //printf("Gravity %lf y: %lf\n",Gravity, CubeY);

    // Player legs action

    GripFactor.x = GameEnv->VoxelTypeManager.VoxelTable[Voxel[20]]->Grip_Horizontal;
    GripFactor.z = GripFactor.x;
    GripFactor.y = GameEnv->VoxelTypeManager.VoxelTable[Voxel[22]]->Grip_Vertical;
    WalkSpeed = Deplacement * 50.0;
    if (GameEnv->VoxelTypeManager.VoxelTable[Voxel[20]]->Is_SpaceGripType || ForceLostControll)
    {
      VelocityIncrease = WalkSpeed * (CycleTime / 16.0)* GripFactor;
      if (WalkSpeed.x > 0.0) {if (Velocity.x > WalkSpeed.x) if (VelocityIncrease.x>0.0) VelocityIncrease.x = 0.0; }
      if (WalkSpeed.x < 0.0) {if (Velocity.x < WalkSpeed.x) if (VelocityIncrease.x<0.0) VelocityIncrease.x = 0.0; }
      if (WalkSpeed.z > 0.0) {if (Velocity.z > WalkSpeed.z) if (VelocityIncrease.z>0.0) VelocityIncrease.z = 0.0; }
      if (WalkSpeed.z < 0.0) {if (Velocity.z < WalkSpeed.z) if (VelocityIncrease.z<0.0) VelocityIncrease.z = 0.0; }
    }
    else
    {
      MaxVelocityIncrease = (WalkSpeed - Velocity) * GripFactor;
      VelocityIncrease = MaxVelocityIncrease * (CycleTime / 16.0);
      VelocityIncrease.y = 0.0;
      if (fabs(VelocityIncrease.x) > fabs(MaxVelocityIncrease.x) ) VelocityIncrease.x = MaxVelocityIncrease.x;
      if (fabs(VelocityIncrease.y) > fabs(MaxVelocityIncrease.y) ) VelocityIncrease.y = MaxVelocityIncrease.y;
      if (fabs(VelocityIncrease.z) > fabs(MaxVelocityIncrease.z) ) VelocityIncrease.z = MaxVelocityIncrease.z;
    }
    Velocity += VelocityIncrease;

    // The viscous friction loss...

    FrictionCoef = GameEnv->VoxelTypeManager.VoxelTable[Voxel[20]]->FrictionCoef;
    FrictionCoef+= GameEnv->VoxelTypeManager.VoxelTable[Voxel[22]]->FrictionCoef;
    FrictionCoef+= GameEnv->VoxelTypeManager.VoxelTable[Voxel[23]]->FrictionCoef;
    Frottement = (Velocity * Velocity * FrictionCoef / 1000000000.0 * CycleTime) + 1.0;
    Velocity /= Frottement;

    // VelocityBooster.

    if (Voxel[20] == 16) Velocity *= 1.0 + CycleTime / 40.0;

    // Velocity to deplacement

    Dep = Velocity * CycleTime / 1000.0;
    DepLen = sqrt(Dep.x*Dep.x + Dep.y*Dep.y + Dep.z*Dep.z);
    // printf("Velocity %lf %lf %lf Increase: %lf %lf %lf CycleTime :%lf\n",Velocity.x, Velocity.y, Velocity.z, VelocityIncrease.x, VelocityIncrease.y, VelocityIncrease.z, CycleTime );

  }



  // Disable all control points. Mark anti fall points with invert flag.

  for (i=0;i<24;i++) {PEnable[i] = false; PInvert[i] = false; }
  for (i=16;i<20;i++){PInvert[i]=true;}

  // Vector direction enable some control points.

  if (Dep.x < 0.0 ) { PEnable[4]  = true; PEnable[7] = true; PEnable[8]  = true; PEnable[11] = true; }
  if (Dep.x > 0.0 ) { PEnable[5]  = true; PEnable[6] = true; PEnable[9]  = true; PEnable[10] = true; }
  if (Dep.z > 0.0 ) { PEnable[4]  = true; PEnable[5] = true; PEnable[8]  = true; PEnable[9]  = true; }
  if (Dep.z < 0.0 ) { PEnable[6]  = true; PEnable[7] = true; PEnable[10] = true; PEnable[11] = true; }
  if (Dep.y > 0.0 ) { PEnable[12] = true; PEnable[13]= true; PEnable[14] = true; PEnable[15] = true; }
  if (Dep.y < 0.0 ) { PEnable[0]  = true; PEnable[1] = true; PEnable[2]  = true; PEnable[3]  = true; }

  PEnable[0]  = true; PEnable[1] = true; PEnable[2]  = true; PEnable[3]  = true;

  // Anti Fall test point activation

  if (this->Flag_ActivateAntiFall)
  {
    if (Dep.x >0)
    {
       if   (Dep.z>0) {PEnable[16] = true;}
       else           {PEnable[19] = true;}
    }
    else
    {
      if   (Dep.z>0) {PEnable[17] = true;}
      else           {PEnable[18] = true;}
    }

    if ( Dep.y>0.0)                                                 {PEnable[16]=PEnable[17]=PEnable[18]=PEnable[19] = false; }
    if ( IsEmpty[19] && IsEmpty[18] && IsEmpty[17] && IsEmpty[16] ) {PEnable[16]=PEnable[17]=PEnable[18]=PEnable[19] = false; }
    if ( !IsEmpty[20] )                                             {PEnable[16]=PEnable[17]=PEnable[18]=PEnable[19] = false; }

    if ( IsEmpty[19] && IsEmpty[18] && IsEmpty[17] ) PEnable[16] = true;
    if ( IsEmpty[18] && IsEmpty[17] && IsEmpty[16] ) PEnable[19] = true;
    if ( IsEmpty[17] && IsEmpty[16] && IsEmpty[19] ) PEnable[18] = true;
    if ( IsEmpty[16] && IsEmpty[19] && IsEmpty[18] ) PEnable[17] = true;
  }

  // Collision detection loop and displacement correction

  ZRayCast_in In;
  ZRayCast_out Out[32];


  In.Camera = 0;
  In.MaxCubeIterations = ceil(DepLen / 256)+5; // 6;
  In.PlaneCubeDiff = In.MaxCubeIterations - 3;
  In.MaxDetectionDistance = 3000000.0;
  double DistanceMin;
  Long CollisionIndice;
  Bool Collided;
  Bool Continue;

  // ****

  Continue = true;
  if ( (Dep.x == 0) && (Dep.y == 0) && (Dep.z == 0.0) ) { Continue = false; return; }

  // printf("Loc: %lf %lf %lf\n",Location.x,Location.y,Location.z);


  while (Continue)
  {

    Collided = false;
    DistanceMin = 10000000.0;
    CollisionIndice = -1;
    for (i=0;i<20;i++)
    {

      if (PEnable[i]) // (PEnable[i])
      {
        bool ret;
        if (PInvert[i]) ret = World->RayCast_Vector_special(P[i],Dep , &In, &(Out[i]), PInvert[i]); // If anti fall points, use special routine and invert empty/full detection.
        else            ret = World->RayCast_Vector(P[i],Dep , &In, &(Out[i]), PInvert[i]);         // Normal points.

        if (ret)
        {
          if (Out[i].CollisionDistance < DistanceMin ) { Collided = true; DistanceMin = Out[i].CollisionDistance; CollisionIndice = i; }
        }
      }
    }
    // printf("\n");

    DepLen = sqrt(Dep.x*Dep.x + Dep.y*Dep.y + Dep.z*Dep.z);

    if (Collided && (DistanceMin < DepLen || DistanceMin <= 1.1) )
    {
      // printf("Collided(Loc:%lf %lf %lf dep: %lf %lf %lf Point : %lf %lf %lf Ind:%ld ))\n",Location.x,Location.y,Location.z, Dep.x,Dep.y,Dep.z,Out[CollisionIndice].CollisionPoint.x,Out[CollisionIndice].CollisionPoint.y, Out[CollisionIndice].CollisionPoint.z, CollisionIndice);
      // World->RayCast_Vector(P[CollisionIndice],Dep , &In, &(Out[CollisionIndice]));
      // Dep = Dep - (P[CollisionIndice] - Out[CollisionIndice].CollisionPoint);
      // SetPosition( Out[CollisionIndice].CollisionPoint );
      // Dep = 0.0;
      switch(Out[CollisionIndice].CollisionAxe)
      {
        case 0: Dep.x=0.0; Event_Collision(Velocity.x); Velocity.x = 0.0; break;
        case 1: Dep.y=0.0; Event_Collision(Velocity.y); Velocity.y = 0.0; JumpDebounce = 0;break;
        case 2: Dep.z=0.0; Event_Collision(Velocity.z); Velocity.z = 0.0; break;
      }
      //Deplacement = 0.0;
      //return;
    }
    else
    {
      ZVector3d NewLocation;

      NewLocation = Location + Dep;



        SetPosition( NewLocation );
        Deplacement = 0.0;
        Continue = false;

    }
  }

}
