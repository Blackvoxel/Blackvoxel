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
 * ZActor_Player.h
 *
 *  Created on: 19 janv. 2011
 *      Author: laurent
 */

#ifndef Z_ZACTOR_PLAYER_H
#define Z_ZACTOR_PLAYER_H

// #ifndef Z_ZACTOR_PLAYER_H
// #  include "ZActor_Player.h"
// #endif

#ifndef Z_ZACTORPHYSICS_H
#  include "ZActorPhysics.h"
#endif

#ifndef Z_ZCAMERA_H
#  include "ZCamera.h"
#endif



class ZActor_Player : public ZActor
{
    ULong     ActorMode;       // s

    bool      IsInLiquid;      // s
    bool      IsFootInLiquid;  // s
    bool      IsHeadInLiquid;  // s
    bool      IsWalking;
    double    LocationDensity; // s Density of the voxels where the player is.
    double    Speed_Walk;

    void *    WalkSoundHandle;


    double    PlayerDensity;
    ZVector3d PlayerSurface;

    double    PlaneMinThrust[4];
    double    PlaneMaxThrust[4];
    double    PlaneSpeed;                  // s
    double    PlaneCommandResponsiveness;  // s
    double    PlaneEngineThrust;           // s
    bool      PlaneEngineOn;               // s
    bool      PlaneTakenOff;               // s
    double    PlaneLandedCounter;          // s
    bool      PlaneToohighAlt;             // s
    double    PlaneFreeFallCounter;        // s
    ZString   PlaneFreeFallMessage;
    bool      PlaneWaitForRectorStartSound;
    void *    PlaneReactorSoundHandle;

    ZVector3L LastHelpVoxel;
    double    LastHelpTime;
    ULong     Test_T1;

    // Riding Voxel
    UShort     Riding_Voxel;
    UShort     Vehicle_Subtype; // Plane type Z0,Z1
    ZMemSize   Riding_VoxelInfo;
    bool       Riding_IsRiding;

    // Backup for inventory in order to detect changes for powers.

    UShort     * PreviousVoxelTypes;

  public:
    ZRayCast_out PointedVoxel;
    ZRayCast_out PointedVoxel_Previous;

    virtual void Init(bool Death=false);
    virtual void SetInitialInventory(bool Death);
    ZActor_Player();

    virtual ~ZActor_Player();
    virtual void SetPosition( ZVector3d &NewLocation );
    virtual void Action_MouseMove( Long Delta_x, Long Delta_y);
    virtual void Action_MouseButtonClick( ULong Button);
    virtual void Action_MouseButtonRelease( ULong Button);
    virtual bool Action_StillEvents( bool * MouseMatrix, UByte * KeyboardMatrix);
    virtual void Action_SetActorMode(ULong ActorMode) {this->ActorMode = ActorMode;}


    virtual void Event_Collision(double RelativeVelocity );

    virtual void DoPhysic(UELong FrameTime);

    virtual void DoPhysic_GroundPlayer(double CycleTime);
    virtual void DoPhysic_Plane(double CycleTime);
    virtual void DoPhysic_Plane_Old(double CycleTime);
    virtual void DoPhysic_SupermanPlayer(double CycleTime);

    virtual bool Save( ZStream_SpecialRamStream * OutStream );
    virtual bool Load( ZStream_SpecialRamStream * InStream  );

    virtual void Action_GoFastForward( double speed);
    virtual void Action_GoForward();
    virtual void Action_GoBackward();
    virtual void Action_GoLeftStraff();
    virtual void Action_GoRightStraff();
    virtual void Action_GoUp();
    virtual void Action_GoDown();
    virtual void Action_Jump();


    virtual void Start_Riding(Long x, Long y, Long z);
    virtual void Stop_Riding();
    virtual void Action_GetInOutOfVehicle()
    {
      if ( Riding_IsRiding
           && (Velocity.x<1.0 && Velocity.x>-1.0)
           && (Velocity.y<1.0 && Velocity.y>-1.0)
           && (Velocity.z<1.0 && Velocity.z>-1.0)
          ) Stop_Riding();
    }

    virtual void Event_Death();
    virtual void Event_PlaneCrash();
    virtual void Event_DeadlyFall();

    virtual void Process_Powers();
};

#endif /* ZACTOR_PLAYER_H_ */
