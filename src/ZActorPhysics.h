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
 * ZActorPhysics.h
 *
 *  Created on: 18 janv. 2011
 *      Author: laurent
 */

#ifndef Z_ZACTORPHYSICS_H
#define Z_ZACTORPHYSICS_H

//#ifndef Z_ZACTORPHYSICS_H
//#  include "ZActorPhysics.h"
//#endif

#ifndef Z_ZWORLD_H
#  include "ZWorld.h"
#endif

#ifndef Z_ZCAMERA_H
#  include "ZCamera.h"
#endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

#ifndef Z_ZINVENTORY_H
#  include "ZInventory.h"
#endif

#ifndef Z_ZTOOLS_H
#  include "ZTools.h"
#endif

#ifndef Z_ZSTREAM_SPECIALRAMSTREAM_H
#  include "z/ZStream_SpecialRamStream.h"
#endif

class ZActorPhysicEngine;
class ZGame;

class ZActor
{
  private:
  public:
    ZGame  * GameEnv;
    ZActor * Next;
    ZActor * Pred;
    ZActorPhysicEngine * PhysicsEngine;
    ZVector3d Location;        // The player position.
    ZVector3d Location_Old;    // The old player position.
    ZVector3d Velocity;        // Player velocity
    ZVector3d Deplacement;     // Deplacement is legs movement speed.
    ZPolar3d  ViewDirection;   // Player viewing and displacement Direction.
    ZVector3d EyesPosition;    // Player Eyes relative to the foot center.
    double    DammageThreshold;
    double    LifePoints;
    Bool      CollideWithVoxels;
    Bool      CollideWithActors;
    bool      Flag_ActivateAntiFall;
    bool      KeepControlOnJumping;
    bool      IsDead;
    bool      IsOnGround;
    Bool      TakesGravity;
    ULong     JumpDebounce;
    bool      order;
    double    DeathChronometer;

    ZCamera      Camera;


    char      MouseButtonMatrix[8];

    ZInventory * Inventory;

    UShort BuildingMaterial;
    UShort LearningModePage;

    // Game Time

    UELong     Time_TotalGameTime;
    UELong     Time_ElapsedTimeSinceLastRespawn;

    ZActor();
    virtual ~ZActor() {}

    virtual void Init(bool Death=false) {}
    virtual void TakeDammage(double Dammage);
    virtual void SetPosition( ZVector3d &NewLocation );
    virtual void GetPosition( ZVector3L &BlocLocation ) {BlocLocation.x = ((Long)Location.x) >> 8; BlocLocation.y = ((Long)Location.y) >> 8; BlocLocation.z = ((Long)Location.z) >> 8; }
    virtual void Action_SetActorMode(ULong ActorMode) {};
    virtual void SetGameEnv(ZGame * GameEnv) { this->GameEnv = GameEnv; }
    virtual bool Save( ZStream_SpecialRamStream * OutStream ) {return(true);}
    virtual bool Load( ZStream_SpecialRamStream * InStream  ) {return(true);}

/*
    virtual void DoPhysic(double CycleTime)
    {
      ZVector3d NewLocation;
      ZVector3d P[32];
      Bool PEnable[32];
      ULong i;

      NewLocation.x = Deplacement.x / 25.0 + Location.x + Velocity.x * CycleTime / 1000.0;
      NewLocation.y = Deplacement.y / 25.0 + Location.y + Velocity.y * CycleTime / 1000.0;
      NewLocation.z = Deplacement.z / 25.0 + Location.z + Velocity.z * CycleTime / 1000.0;

      P[0] = NewLocation + ZVector3d(-75.0,+0.0,+75.0);
      P[1] = NewLocation + ZVector3d(+75.0,+0.0,+75.0);
      P[2] = NewLocation + ZVector3d(+75.0,+0.0,-75.0);
      P[3] = NewLocation + ZVector3d(-75.0,+0.0,-75.0);

      P[4] = NewLocation + ZVector3d(-75.0,+500.0,+75.0);
      P[5] = NewLocation + ZVector3d(+75.0,+500.0,+75.0);
      P[6] = NewLocation + ZVector3d(+75.0,+500.0,-75.0);
      P[7] = NewLocation + ZVector3d(-75.0,+500.0,-75.0);

      P[8]  = NewLocation + ZVector3d(+85.0,+128.0,+85.0);
      P[9]  = NewLocation + ZVector3d(-85.0,+128.0,+85.0);
      P[10] = NewLocation + ZVector3d(+85.0,+384.0,+85.0);
      P[11] = NewLocation + ZVector3d(-85.0,+384.0,+85.0);

      P[12] = NewLocation + ZVector3d(+85.0,+128.0,+85.0);
      P[13] = NewLocation + ZVector3d(+85.0,+128.0,-85.0);
      P[14] = NewLocation + ZVector3d(+85.0,+384.0,+85.0);
      P[15] = NewLocation + ZVector3d(+85.0,+384.0,-85.0);

      P[16] = NewLocation + ZVector3d(+85.0,+128.0,-85.0);
      P[17] = NewLocation + ZVector3d(-85.0,+128.0,-85.0);
      P[18] = NewLocation + ZVector3d(+85.0,+384.0,-85.0);
      P[19] = NewLocation + ZVector3d(-85.0,+384.0,-85.0);

      P[20] = NewLocation + ZVector3d(-85.0,+128.0,+85.0);
      P[21] = NewLocation + ZVector3d(-85.0,+128.0,-85.0);
      P[22] = NewLocation + ZVector3d(-85.0,+384.0,+85.0);
      P[23] = NewLocation + ZVector3d(-85.0,+384.0,-85.0);

      for (i=0;i<32;i++) PEnable[i] = false;

      if (NewLocation.x > Location.x) { PEnable[12] = true; PEnable[13] = true; PEnable[14] = true; PEnable[15] = true; }
      if (NewLocation.x < Location.x) { PEnable[20] = true; PEnable[21] = true; PEnable[22] = true; PEnable[23] = true; }

      if (NewLocation.z > Location.z) { PEnable[8] = true; PEnable[9] = true; PEnable[10] = true; PEnable[11] = true; }
      if (NewLocation.z < Location.z) { PEnable[16] = true; PEnable[17] = true; PEnable[18] = true; PEnable[19] = true; }

    }
*/

    virtual void DoPhysic(UELong FrameTime);

    virtual void Action_GoFastForward( double speed) {}
    virtual void Action_GoForward() {}
    virtual void Action_GoBackward() {}
    virtual void Action_GoLeftStraff() {}
    virtual void Action_GoRightStraff() {}
    virtual void Action_Jump() {}
    virtual void Action_GoUp() {}
    virtual void Action_GoDown() {}
    virtual void Action_MouseMove( Long Delta_x, Long Delta_y) {}
    virtual void Action_MouseButtonClick( ULong Button) {}
    virtual void Action_MouseButtonRelease( ULong Button) {}
    virtual bool Action_StillEvents( bool * MouseMatrix, UByte * KeyboardMatrix);
    virtual void Action_NextBuildingMaterial();
    virtual void Action_PrevBuildingMaterial();
    virtual void Action_NextTool();
    virtual void Action_PrevTool();


    virtual void Event_Collision(double RelativeVelocity ) {}
    virtual void Event_Death() {}


    virtual void Start_Riding(Long x, Long y, Long z) {}
    virtual void Stop_Riding() {}
    virtual void Action_GetInOutOfVehicle() {}

    virtual void Process_Powers() {}
};



class ZActorPhysicEngine
{
  private:
    ZVector3d Gravity;
    ZActor * ActorList;
    ZActor * SelectedActor;
    ZVoxelTypeManager * VoxelTypeManager;
    ZToolManager      * ToolManager;
  public:
    ZVoxelWorld * World;


  void SetWorld(ZVoxelWorld * World) {this->World = World;}
  void DoPhysic(UELong FrameTime);
  void AddActor(ZActor * Actor);

  void SelectActor( ZActor * Actor) { SelectedActor = Actor; }
  ZActor * GetSelectedActor() { return(SelectedActor); }
  void SetVoxelTypeManager( ZVoxelTypeManager * VoxelTypeManager ) { this->VoxelTypeManager = VoxelTypeManager; }
  void SetToolManager(ZToolManager * ToolManager) { this->ToolManager = ToolManager; }
  ZVoxelTypeManager * GetVoxelTypeManager() { return (VoxelTypeManager); }
  ZToolManager      * GetToolManager()      { return (ToolManager); }

  void RemoveActor(ZActor * Actor);
  ZActorPhysicEngine();
  ~ZActorPhysicEngine();

};

#endif
