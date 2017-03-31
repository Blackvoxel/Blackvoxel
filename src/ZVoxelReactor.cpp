/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2015 Laurent Thiebaut & Olivia Merle
 *
 * (See below for other contributors)
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
 *
 *
 * ------------------------- Contributors --------------------------
 *
 * d3x0r : Water performance improvement patch.
 *
*/


/*
 * ZVoxelReactor.cpp
 *
 *  Created on: 15 sept. 2012
 *      Author: laurent
 */

#ifndef Z_ZVOXELREACTOR_H
#  include "ZVoxelReactor.h"
#endif

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

#ifndef Z_ZVOXELEXTENSION_BLASTFURNACE_H
#  include "ZVoxelExtension_BlastFurnace.h"
#endif

#ifndef Z_ZVOXELEXTENSION_FUSIONELEMENT_H
#  include "ZVoxelExtension_FusionElement.h"
#endif

#ifndef Z_ZVOXELEXTENSION_MININGROBOT_XR1_H
#  include "ZVoxelExtension_MiningRobot_xr1.h"
#endif

#ifndef Z_VOXELEXTENSION_SEQUENCER_H
#  include "ZVoxelExtension_Sequencer.h"
#endif

#ifndef Z_ZVOXELEXTENSION_EGMY_T1_H
#  include "ZVoxelExtension_Egmy_T1.h"
#endif

#ifndef Z_VOXELEXTENSIONTYPE_VOXELFLUID_H
#  include "ZVoxelExtensionType_VoxelFluid.h"
#endif

#ifndef Z_ZGENERICTABLE_H
#  include "z/ZGenericTable.h"
#endif

#ifndef Z_ZACTIVEVOXELINTERFACE_H
#  include "ZActiveVoxelInterface.h"
#endif





ZVoxelReactor::ZBlocPos ZVoxelReactor::bp6[6] =
{
    {0,1,1},
    {2,1,1},
    {1,0,1},
    {1,2,1},
    {1,1,0},
    {1,1,2},
};

ZVoxelReactor::ZBlocPos ZVoxelReactor::xbp6[6] = // Les 4 autours et ensuite celui du dessus et ensuite du dessous. (Ordre standardise).
{
    { 1, 1, 2 }, // 0 = Front of the robot.
    { 2, 1, 1 }, // 1 = Right of the robot.
    { 1, 1, 0 }, // 2 = Back of the robot.
    { 0, 1, 1 }, // 3 = Left of the robot.
    { 1, 2, 1 }, // 4 = Above the robot.
    { 1, 0, 1 }  // 5 = Under the robot.
};

ZVoxelReactor::ZBlocPosN ZVoxelReactor::xbp6_nc[6] = // Meme chose que xnp6 mais avec range -1,1 au lieu de 0,2.
{
    { 0, 0, 1 }, // 0 = Front of the robot.
    { 1, 0, 0 }, // 1 = Right of the robot.
    { 0, 0,-1 }, // 2 = Back of the robot.
    {-1, 0, 0 }, // 3 = Left of the robot.
    { 0, 1, 0 }, // 4 = Above the robot.
    { 0,-1, 0 }  // 5 = Under the robot.
};

ZVoxelReactor::ZBlocPos ZVoxelReactor::bft[8] = // Les 4 autours et les 4 sur l'étage d'en dessous
{
    {2,1,1},
    {0,1,1},
    {1,1,2},
    {1,1,0},
    {2,0,1},
    {0,0,1},
    {1,0,2},
    {1,0,0}
};

ZVoxelReactor::ZBlocPos ZVoxelReactor::bft6[10] = // Les 6 autours et les 4 de la rangée du dessous.
{
    {2,1,1},
    {0,1,1},
    {1,1,2},
    {1,1,0},
    {1,2,1},
    {1,0,1},
    {2,0,1},
    {0,0,1},
    {1,0,2},
    {1,0,0}
};

ZVoxelReactor::ZBlocPosN ZVoxelReactor::nbp6[6] =
{
  {-1, 0, 0},
  { 1, 0, 0},
  { 0,-1, 0},
  { 0, 1, 0},
  { 0, 0,-1},
  { 0, 0, 1}
};

UByte ZVoxelReactor::BlocOpposite[6] =
{
  1,0,3,2,5,4
};

UByte ZVoxelReactor::DirCodeTable[16] =
{
    0,  // 0000
    3,  // 0001
    2,  // 0010
    2,  // 0011
    1,  // 0100
    1,  // 0101
    1,  // 0110
    1,  // 0111
    0, // 1000
    0, // 1001
    0, // 1010
    0, // 1011
    0, // 1100
    0, // 1101
    0, // 1110
    0  // 1111
};
   /*
ZVoxelReactor::ZBlocPos ZVoxelReactor::bft[8] = // Les 4 autours et les 4 sur l'étage d'en dessous
{    x z
    {1 , 0}, 0
    {-1, 0}, 1
    {0 , 1}, 2
    {0 , -1}, 3
};
*/
ZVoxelReactor::ZVoxelReactor()
{
  ULong i;

  PlayerPosition.x = PlayerPosition.y = PlayerPosition.z = 0.0;
  GameEnv = 0;
  World = 0;
  VoxelTypeManager = 0;
  CycleNum = 0;

  // Active Table

  ActiveTable = new ULong[2048];

  // Dummy Sector

  DummySector = new ZVoxelSector();
  DummySector->Fill(0xFFFF);

  // Multiplexing Sector Tables for fast access to voxels

  Of_x[0]=0; Of_x[ZVOXELBLOCSIZE_X+1]=2;for (i=1;i<=ZVOXELBLOCSIZE_X;i++) Of_x[i]=1;
  Of_y[0]=0; Of_y[ZVOXELBLOCSIZE_Y+1]=8;for (i=1;i<=ZVOXELBLOCSIZE_Y;i++) Of_y[i]=4;
  Of_z[0]=0; Of_z[ZVOXELBLOCSIZE_Z+1]=32;for (i=1;i<=ZVOXELBLOCSIZE_Z;i++) Of_z[i]=16;

  // Multiplexing Voxel Tables for fast access to voxels

  If_x[0]=(ZVOXELBLOCSIZE_X-1)*ZVOXELBLOCSIZE_Y;
  If_x[ZVOXELBLOCSIZE_X+1]=0;
  for (i=0;i<ZVOXELBLOCSIZE_X;i++) If_x[i+1]=i*ZVOXELBLOCSIZE_Y;
  If_y[0]=(ZVOXELBLOCSIZE_Y-1);
  If_y[ZVOXELBLOCSIZE_Y+1]=0;
  for (i=0;i<ZVOXELBLOCSIZE_Y;i++) If_y[i+1]=i;
  If_z[0]=(ZVOXELBLOCSIZE_Z-1)*ZVOXELBLOCSIZE_Y * ZVOXELBLOCSIZE_X;
  If_z[ZVOXELBLOCSIZE_Z+1]=0;
  for (i=0;i<ZVOXELBLOCSIZE_Z;i++) If_z[i+1]=i*ZVOXELBLOCSIZE_Y*ZVOXELBLOCSIZE_X;


  // FireMine
  FireMineTime = 0;
  //

  Random.Init(0);

  // Reaction table
  ReactionTable = new ZVoxelReaction * [65536];
  for (i=0;i<65536;i++) ReactionTable[i]=0;

  // Green acid reaction
  ReactionTable[86] = new ZVoxelReaction(89,0);
  // ReactionTable[86]->SetReaction(1,10,10);

  //ReactionTable[86]->Set(1,10);

  // Robot SC Probabilities.

  RobotSC_Proba.AddEntry( 26,0,0.1); // Gold.
  RobotSC_Proba.AddEntry( 44,0,0.1); // X-Material
  RobotSC_Proba.AddEntry( 60,0,3.0); // Carbon
  RobotSC_Proba.AddEntry( 74,0,1.0); // Iron Ore
  RobotSC_Proba.AddEntry(110,0,0.1); // Nickel Ore
  RobotSC_Proba.AddEntry(112,0,1.0); // Copper Ore
  RobotSC_Proba.AddEntry(121,0,0.2); // Chrome Ore
  RobotSC_Proba.AddEntry(127,0,0.1); // Tin Ore

  RobotSC_Proba.AddEntry(122,0,0.001);// Titanium Ore
  RobotSC_Proba.AddEntry(150,0,0.5);  // Lead ORe
  RobotSC_Proba.AddEntry(197,0,0.1);  // Aluminium Ore
  RobotSC_Proba.ComputeProbabilities(1.0);

}

ZVoxelReactor::~ZVoxelReactor()
{
  if (ActiveTable) {delete ActiveTable; }
  if (DummySector) {delete DummySector; DummySector = 0;}
}

void ZVoxelReactor::Init(ZGame * GameEnv)
{
  this->GameEnv = GameEnv;
  this->World = GameEnv->World;
  this->VoxelTypeManager = &GameEnv->VoxelTypeManager;
  PlayerPosition.x = PlayerPosition.y = PlayerPosition.z = 0.0;
}

void ZVoxelReactor::LightTransmitter_FindEndPoints(ZVector3L * Location, ZVector3L * CommingDirection)
{
  UShort VoxelType;
  ZVector3L NewPos, Direction, NewCommingDirection;
  ULong i;

  for (i=0;i<6;i++)
  {
    Direction.x = nbp6[i].x; Direction.y = nbp6[i].y; Direction.z = nbp6[i].z;
    if (Direction == *CommingDirection) continue;
    NewPos.x = Location->x + nbp6[i].x; NewPos.y = Location->y + nbp6[i].y; NewPos.z = Location->z + nbp6[i].z;


      VoxelType = GameEnv->World->GetVoxel(NewPos.x, NewPos.y,NewPos.z);
      if (VoxelType == 99) LightTransmitter_FollowTransmitter(&NewPos, &Direction );
      if (VoxelType == 102)
      {
        NewCommingDirection.x = -Direction.x; NewCommingDirection.y = -Direction.y; NewCommingDirection.z = -Direction.z;
        LightTransmitter_FindEndPoints( &NewPos, &NewCommingDirection );
      }
  }
}

void ZVoxelReactor::LightTransmitter_FollowTransmitter(ZVector3L * Location, ZVector3L * FollowingDirection)
{
  UShort VoxelType;
  ZVector3L NewPos;
  ZVector3L CommingDirection;

  NewPos = *Location;

  while (true)
  {
    VoxelType = GameEnv->World->GetVoxel(NewPos.x, NewPos.y,NewPos.z);
    switch(VoxelType)
    {
      case 99:  break; // Continue
      case 101: /*printf("EndPoint %d,%d,%d\n",NewPos.x,NewPos.y,NewPos.z);*/ break;
      case 102: CommingDirection.x = - FollowingDirection->x; CommingDirection.y = FollowingDirection->y; CommingDirection.z = FollowingDirection->z;
                LightTransmitter_FindEndPoints(&NewPos,&CommingDirection);
                break;
      default:  return;
    }
    NewPos += *FollowingDirection;
  }

}


struct ZonePressure
{
  Long YMin, YMax;
  ULong MaxPressure;
  UShort VoxelType;
  bool EvenCycle;
};

void ZVoxelReactor::VoxelFluid_ComputeVolumePressure_Recurse(ZVector3L * Location, ZonePressure * Pr  )
{
  ZVoxelLocation Loc;
  UShort VoxelType;
  ZVoxelExtensionType_VoxelFluid * Ext;
  ULong i;
  ZVector3L NewLocation;

  GameEnv->World->GetVoxelLocation( &Loc, Location );

  // Get infos from the voxel

  VoxelType = Loc.Sector->Data[Loc.Offset];
  Ext = (ZVoxelExtensionType_VoxelFluid *)&Loc.Sector->OtherInfos[Loc.Offset];

  // If voxeltype is not of the required type, stop with it

  if (VoxelType != Pr->VoxelType) return;


  // If voxel is already processed in this function or in this cycle, stop with this voxel and return.

  if (Ext->Cycle != Pr->EvenCycle || Ext->Marked == true) return;
  // printf("[%d,%d,%d]", Location->x, Location->y, Location->z);
  // Mark the voxel as processed by this function

  Ext->Marked = true;

  // Gather min and max location of the volume as it will be used to compute pressure.

  if (Location->y > Pr->YMax) Pr->YMax = Location->y;
  if (Location->y < Pr->YMin) Pr->YMin = Location->y;

  // Look at neighboring voxels

  for (i=0;i<6;i++)
  {
    NewLocation.x = Location->x + xbp6_nc[i].x; NewLocation.y = Location->y + xbp6_nc[i].y; NewLocation.z = Location->z + xbp6_nc[i].z;
    VoxelFluid_ComputeVolumePressure_Recurse(&NewLocation, Pr);
  }
}

void ZVoxelReactor::VoxelFluid_SetVolumePressure_Recurse(ZVector3L * Location, ZonePressure * Pr  )
{
  ZVoxelLocation Loc;
  UShort VoxelType;
  ZVoxelExtensionType_VoxelFluid * Ext;
  ULong i;
  ZVector3L NewLocation;

  GameEnv->World->GetVoxelLocation( &Loc, Location );

  // Get infos from the voxel

  VoxelType = Loc.Sector->Data[Loc.Offset];
  Ext = (ZVoxelExtensionType_VoxelFluid *)&Loc.Sector->OtherInfos[Loc.Offset];

  // If voxeltype is not of the required type, stop with it

  if (VoxelType != Pr->VoxelType) return;

  // If voxel is already processed in this function or in this cycle, stop with this voxel and return.

  if (Ext->Cycle != Pr->EvenCycle || Ext->Marked == false) return; //

  // Mark the voxel as processed by this function

  Ext->Marked = false;

  // Set the pressure depending on the height.

  Ext->Pressure = Pr->YMax - Location->y;

  // Look at neighboring voxels

  for (i=0;i<6;i++)
  {
    NewLocation.x = Location->x + xbp6_nc[i].x; NewLocation.y = Location->y + xbp6_nc[i].y; NewLocation.z = Location->z + xbp6_nc[i].z;
    VoxelFluid_SetVolumePressure_Recurse(&NewLocation, Pr);
  }
}

bool ZVoxelReactor::VoxelFluid_ComputeVolumePressure(ZVector3L * Location, UShort VoxelType, bool EvenCycle)
{
  ZonePressure Zp;

  Zp.EvenCycle = EvenCycle;
  Zp.VoxelType = VoxelType;
  Zp.YMin =  2000000000;
  Zp.YMax = -2000000000;
  Zp.MaxPressure = 0;

  VoxelFluid_ComputeVolumePressure_Recurse(Location, &Zp);
  if (Zp.YMin == 2000000000 || Zp.YMax == -2000000000) return(false);
  VoxelFluid_SetVolumePressure_Recurse(Location, &Zp);

  //printf("\n");
  return(true);
}






void ZVoxelReactor::ProcessSectors( double LastLoopTime )
{
  Long x,y,z;

  // register ULong MainOffset asm ("r8");
  register ULong MainOffset;
  UShort VoxelType;
  ZVoxelSector * SectorTable[64];
  ZVoxelSector * Sector;
  bool LowActivityTrigger;
  ZActor * SelectedActor;
  ZActiveVoxelInterface ActiveVoxelInterface;

  ZVector3d PlayerLocation;

  // Active voxel interface init.

  ActiveVoxelInterface.GameEnv = GameEnv;
  ActiveVoxelInterface.VoxelTypeManager = VoxelTypeManager;
  ActiveVoxelInterface.World = World;
  ActiveVoxelInterface._xbp = (ZActiveVoxelInterface::ZBlocPosN *)xbp6_nc;

  // FireMine

  if (FireMineTime > 0) FireMineTime--;


  // Get the player location (in multithreading friendly way)

  do
  {
    SelectedActor = GameEnv->PhysicEngine->GetSelectedActor();
    PlayerLocation = SelectedActor->Location;
  } while (PlayerLocation != GameEnv->PhysicEngine->GetSelectedActor()->Location);

  // Cycle Counter is incremented at each MVI's cycle. This is used in cycle dependent operations.

  CycleNum++;

  // Egmy Wave Manager

  EgmyWaveManager.SwapList();

  //

  Sector = World->SectorList;
  Long Sx,Sy,Sz;


  ULong Test=0;

  while ((Sector))
  {
    LowActivityTrigger = Sector->Flag_IsActiveLowRefresh && (((Sector->Pos_x + CycleNum) & Sector->LowRefresh_Mask) == 0);
    if (Sector->Flag_IsActiveVoxels | LowActivityTrigger)
    {
      // Find All the
      Sx = Sector->Pos_x - 1;
      Sy = Sector->Pos_y - 1;
      Sz = Sector->Pos_z - 1;

      for (x = 0; x <= 2; x++)
        for (y = 0; y <= 2; y++)
          for (z = 0; z <= 2; z++)
          {
            MainOffset = x + (y << 2) + (z << 4);
            if (!(SectorTable[MainOffset] = World->FindSector(Sx + x, Sy + y, Sz + z))) SectorTable[MainOffset] = DummySector;
            SectorTable[MainOffset]->ModifTracker.SetActualCycleNum(CycleNum);
          }

      // Make the sector table
      register UShort * DisplayData, * VoxelP;
      VoxelP = DisplayData = Sector->Data;
      ZFastBit_Array_64k * ActiveTable = VoxelTypeManager->ActiveTable;
      bool                 IsActiveVoxels = false;
      bool                 IsLowActivityVoxels = false;
      bool                 IsMovedVoxels  = false;
      bool                 IsDebug        = false;
      MainOffset = 0;
      Long RSx = Sector->Pos_x << ZVOXELBLOCSHIFT_X;
      Long RSy = Sector->Pos_y << ZVOXELBLOCSHIFT_Y;
      Long RSz = Sector->Pos_z << ZVOXELBLOCSHIFT_Z;

      for (z = 0; z < ZVOXELBLOCSIZE_Z; z++)
        for (x = 0; x < ZVOXELBLOCSIZE_X; x++)
          for (y = 0; y < ZVOXELBLOCSIZE_Y; y++)
          {
            VoxelType = *(VoxelP);
            if (ActiveTable->Get(VoxelType))
            {
              if (!Sector->ModifTracker.Get( MainOffset ) ) // If voxel is already processed, don't process it once more in the same cycle.
              {
                switch(VoxelType)
                {
                  /*
                  case 47:
                          {
                            ZVoxelSector * St[32];
                            UShort * Vp[32];
                            ULong i;
                            for(i=0;i<5;i++)
                            {
                              register Long cx,cy,cz;
                              register ULong SecondaryOffset;

                              cx = x+bp6[i].x; cy = y+bp6[i].y; cz = z+bp6[i].z;
                              SecondaryOffset = If_x[cx]+If_y[cy]+If_z[cz];
                              St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                              Vp[i] = &St[i]->Data[ SecondaryOffset ];
                              if ( *Vp[i]==1 )
                                if (!St[i]->ModifTracker.Get(SecondaryOffset))
                                {
                                  // Main voxel is modified
                                  *(VoxelP) = 22;
                                  Sector->Flag_IsModifiedByPlayer = true; Sector->Flag_Render_Dirty = true; Sector->ModifTracker.Set(MainOffset);
                                  // Secondary voxel is also modified
                                  *Vp[i]=22;
                                  St[i]->Flag_IsModifiedByPlayer = true; St[i]->Flag_Render_Dirty = true; St[i]->ModifTracker.Set(SecondaryOffset);
                                  break;
                                }
                            }
                          }
                          break;
                    */
                  case 84: // Pierre qui roule
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[32];
                            UShort * Vp[32];
                            ULong SecondaryOffset[32];
                            ULong i,vCount,j;
                            bool  DirEn[4];
                            register Long cx,cy,cz;

                            // Test if we can fall downward
                              i=0;
                              cx = x+1 ; cy = y ; cz = z+1; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_Water)
                              {
                                World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y-1, RSz + z, 84, ZVoxelSector::CHANGE_IMPORTANT);
                                World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, ZVoxelSector::CHANGE_IMPORTANT);
                                St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                break;
                              }
                              for(i=0,j=4,vCount=0;i<4;i++,j++)
                              {
                                cx = x+bft[i].x ; cy = y+bft[i].y ; cz = z+bft[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                                cx = x+bft[j].x ; cy = y+bft[j].y ; cz = z+bft[j].z ; SecondaryOffset[j] = If_x[cx]+If_y[cy]+If_z[cz];St[j] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[j] = &St[j]->Data[ SecondaryOffset[j] ];
                                if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_Water && VoxelTypeManager->VoxelTable[*Vp[j]]->Is_CanBeReplacedBy_Water) {vCount++; DirEn[i]=true;}
                                else DirEn[i]=false;
                              }

                              if (vCount>0)
                              {
                                j = (Random.GetNumber() % vCount) +1;
                                for (i=0;i<4;i++)
                                {
                                  if (DirEn[i]) j--;
                                  if (!j)
                                  {
                                    World->SetVoxel_WithCullingUpdate(RSx + x + bft[i].x-1, RSy + y, RSz + z + bft[i].z-1 , 84, ZVoxelSector::CHANGE_IMPORTANT);
                                    World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, ZVoxelSector::CHANGE_IMPORTANT);
                                    St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                    break;
                                  }
                                }
                              }
                              break;
                            }


                  case  85:
                          // Eau qui coule.
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[32];
                            UShort * Vp[32];
                            ULong SecondaryOffset[32];
                            ULong i,vCount,WaveCount,j;
                            bool  DirEn[4],WaveDirEn[4];
                            register Long cx,cy,cz;

                            // Test if we can fall downward
                              i=0;
                              cx = x+1 ; cy = y ; cz = z+1; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_Water)
                              {
                                World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y-1, RSz + z, 85, ZVoxelSector::CHANGE_UNIMPORTANT);
                                World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, ZVoxelSector::CHANGE_UNIMPORTANT);
                                St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                break;
                              }


                              // d3x0r performance patch

                              for(i=0,j=4,vCount=0,WaveCount=0;i<4;i++,j++)
                              {
                                cx = x+bft[i].x ; cy = y+bft[i].y ; cz = z+bft[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                                if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_Water )
                                {
                                  cx = x+bft[j].x ; cy = y+bft[j].y ; cz = z+bft[j].z ; SecondaryOffset[j] = If_x[cx]+If_y[cy]+If_z[cz];St[j] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[j] = &St[j]->Data[ SecondaryOffset[j] ];
                                  if( VoxelTypeManager->VoxelTable[*Vp[j]]->Is_CanBeReplacedBy_Water) {vCount++; DirEn[i]=true;}
                                  else DirEn[i]=false;
                                  WaveCount++;
                                  WaveDirEn[i] = true;
                                }
                                else {WaveDirEn[i] = false; DirEn[i]=false;}
                              }

                              // Old code
/*
                              for(i=0,j=4,vCount=0,WaveCount=0;i<4;i++,j++)
                              {
                                cx = x+bft[i].x ; cy = y+bft[i].y ; cz = z+bft[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                                cx = x+bft[j].x ; cy = y+bft[j].y ; cz = z+bft[j].z ; SecondaryOffset[j] = If_x[cx]+If_y[cy]+If_z[cz];St[j] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[j] = &St[j]->Data[ SecondaryOffset[j] ];
                                if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_Water && VoxelTypeManager->VoxelTable[*Vp[j]]->Is_CanBeReplacedBy_Water) {vCount++; DirEn[i]=true;}
                                else DirEn[i]=false;
                                if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_Water) {WaveCount++;WaveDirEn[i] = true;}
                                else                                                                {WaveDirEn[i] = false;}
                              }
*/

                              // End

                              if (vCount>0)
                              {
                                j = (Random.GetNumber() % vCount) +1;
                                for (i=0;i<4;i++)
                                {
                                  if (DirEn[i]) j--;
                                  if (!j)
                                  {
                                    World->SetVoxel_WithCullingUpdate(RSx + x + bft[i].x-1, RSy + y, RSz + z + bft[i].z-1 , 85, ZVoxelSector::CHANGE_UNIMPORTANT);
                                    World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, ZVoxelSector::CHANGE_UNIMPORTANT);
                                    St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                    break;
                                  }
                                }
                              }
                              else if ( WaveCount > 0 && WaveCount < 4 )
                              {
                                j = (Random.GetNumber() % WaveCount) +1;
                                for (i=0;i<4;i++)
                                 {
                                   if (WaveDirEn[i]) j--;
                                   if (!j)
                                   {
                                     World->SetVoxel_WithCullingUpdate(RSx + x + bft[i].x-1, RSy + y, RSz + z + bft[i].z-1 , 85, ZVoxelSector::CHANGE_UNIMPORTANT);
                                     World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, ZVoxelSector::CHANGE_UNIMPORTANT);
                                     St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                     break;
                                   }
                                 }
                              }
                              break;
                            }

                  case 86:
                            // Acide Vert
                            IsActiveVoxels = true;
                            {
                              ZVoxelSector * St[32];
                              UShort * Vp[32];
                              ZVoxelReaction::ZReaction * Reaction;
                              ULong SecondaryOffset[32];
                              ULong i,j, FallCount,SnoopCount, GrindCount;
                              bool  FallEn[4],SnoopEn[4],GrindEn[6];
                              register Long cx,cy,cz;
                              ZVoxelLocation Location;

                              // Test if we can fall downward
                                i=0;
                                cx = x+1 ; cy = y ; cz = z+1; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                                if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_GreenAcid)
                                {
                                  World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y-1, RSz + z, 86, ZVoxelSector::CHANGE_UNIMPORTANT);
                                  World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, ZVoxelSector::CHANGE_UNIMPORTANT);
                                  St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                  break;
                                }

                                // Fetch Voxels for all 10 Voxels arounds
                                for (i=0;i<10;i++) { cx = x+bft6[i].x ; cy = y+bft6[i].y ; cz = z+bft6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ]; }


                                for(i=0,j=6,FallCount=SnoopCount=GrindCount=0 ; i<4 ; i++,j++)
                                {
                                  if (( GrindEn[i] = VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CombinableWith_GreenAcid ))            GrindCount++;
                                  if (( SnoopEn[i] = VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_GreenAcid ))           SnoopCount++;
                                  if (( FallEn[i]=SnoopEn[i] && VoxelTypeManager->VoxelTable[*Vp[j]]->Is_CanBeReplacedBy_GreenAcid)) FallCount++;
                                }
                                for(i=4;i<6;i++)
                                {
                                  if (( GrindEn[i] = VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CombinableWith_GreenAcid )) GrindCount++;
                                }

                                // If the voxel under acid is grindable, blast it

                                if (GrindEn[5])
                                {
                                  Reaction = ReactionTable[86]->GetReaction(*Vp[5]);
                                  World->SetVoxel_WithCullingUpdate(RSx + x + bft6[5].x - 1, RSy + y + bft6[5].y - 1 , RSz + z + bft6[5].z - 1 , Reaction->ActorTrans , ZVoxelSector::CHANGE_UNIMPORTANT);
                                  World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, Reaction->DrivedTrans, ZVoxelSector::CHANGE_UNIMPORTANT, true, &Location);
                                  // Location.Sector->OtherInfos[Location.Offset] = 1000;

                                  St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                  break;
                                }

                                // Fall To the side of a voxel.

                                if ((FallCount))
                                {
                                  j = (Random.GetNumber() % FallCount) +1;
                                  for (i=0;i<4;i++)
                                  {
                                    if (FallEn[i]) j--;
                                    if (!j)
                                    {
                                      World->SetVoxel_WithCullingUpdate(RSx + x + bft6[i].x - 1, RSy + y, RSz + z + bft6[i].z - 1 , 86, ZVoxelSector::CHANGE_UNIMPORTANT);
                                      World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, ZVoxelSector::CHANGE_UNIMPORTANT);
                                      St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                      break;
                                    }
                                  }
                                  break;
                                }

                                // Test if it can break any voxel

                                if ((GrindCount))
                                {
                                  j = (Random.GetNumber() % GrindCount) +1;
                                  for(i=0;i<6;i++)
                                  {
                                    if (GrindEn[i]) j--;
                                    if (!j)
                                    {
                                      Reaction = ReactionTable[86]->GetReaction(*Vp[i]);
                                      World->SetVoxel_WithCullingUpdate(RSx + x + bft6[i].x - 1, RSy + y + bft6[i].y - 1, RSz + z + bft6[i].z - 1 , Reaction->ActorTrans , ZVoxelSector::CHANGE_UNIMPORTANT, true, &Location);
                                      //{ Location.Sector->OtherInfos[Location.Offset] = 1000; }
                                      World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, Reaction->DrivedTrans , ZVoxelSector::CHANGE_UNIMPORTANT);
                                      St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                      break;
                                    }
                                  }
                                  break;
                                }

                                // If we can flow down or break anything, try to go in random horizontal direction

                                if ((SnoopCount))
                                {
                                  j = (Random.GetNumber() % SnoopCount) +1;
                                  for (i=0;i<4;i++)
                                  {
                                    if (SnoopEn[i]) j--;
                                    if (!j)
                                    {
                                      World->SetVoxel_WithCullingUpdate(RSx + x + bft6[i].x - 1, RSy + y, RSz + z + bft6[i].z - 1 , 86, ZVoxelSector::CHANGE_UNIMPORTANT);
                                      World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, ZVoxelSector::CHANGE_UNIMPORTANT);
                                      St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                      break;
                                    }
                                  }
                                  break;
                                }

                                break;
                              }

                  case 87: // Source d'eau
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[32];
                            UShort * Vp[32];
                            ULong SecondaryOffset[32];
                            ULong i;

                            register Long cx,cy,cz;

                            // Test if we can fall downward
                              i=0;
                              cx = x+1 ; cy = y ; cz = z+1; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                              for(i=0;i<6;i++)
                              {
                                cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                                if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_Water)
                                {
                                  World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x-1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z-1 , 85, ZVoxelSector::CHANGE_UNIMPORTANT);
                                  St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                }
                              }
                              break;
                            }
                  case 88: // Absorbeur de liquide
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[32];
                            UShort * Vp[32];
                            ULong SecondaryOffset[32];
                            ULong i;

                            register Long cx,cy,cz;

                              i=0;
                              cx = x+1 ; cy = y+1 ; cz = z+1;

                              for(i=0;i<6;i++)
                              {
                                cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                                if (*Vp[i]==85)
                                {
                                  // if (!St[i]->ModifTracker.Get(SecondaryOffset[i]))
                                  {
                                    World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x - 1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z - 1 , 0, ZVoxelSector::CHANGE_UNIMPORTANT);
                                    St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                  }
                                }
                              }

                              break;
                            }


                  case 89:
                            // Gaz moutarde
                            IsActiveVoxels = true;
                            {
                              ZVoxelSector * St[32];
                              UShort * Vp[32];
                              ULong SecondaryOffset[32];
                              ULong i,j, MoveCount;
                              bool  MoveEn[6];
                              register Long cx,cy,cz;

                              // If it reaches end of life destroy it...

                              if ( Sector->OtherInfos[MainOffset] == 0 )
                              {
                                World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, ZVoxelSector::CHANGE_UNIMPORTANT);
                              }
                              else Sector->OtherInfos[MainOffset]--;

                              // Fetch Voxels for all 6 Voxels arounds main.
                              for (i=MoveCount=0;i<6;i++)
                              {
                                cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                                MoveCount += 1 & ( MoveEn[i] = VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_MustardGaz );
                              }
                              if (MoveCount == 6) { MoveEn[3] = false; MoveCount--; }

                              if ((MoveCount))
                              {
                                j = (Random.GetNumber() % MoveCount) +1;
                                for(i=0;i<6;i++)
                                {
                                  if (MoveEn[i]) j--;
                                  if (!j)
                                  {
                                    World->MoveVoxel( RSx + x, RSy + y, RSz + z,
                                                      RSx + x + bp6[i].x - 1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z - 1, 0, ZVoxelSector::CHANGE_UNIMPORTANT);
                                    //World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x - 1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z - 1 ,89, true);
                                    //World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, true);
                                    St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                    break;
                                  }
                                }
                              }

                              break;
                            }

                           // Buggy but fun
                          /*
                          {
                            ZVoxelSector * St[32];
                            UShort * Vp[32];
                            ULong i,vCount,j;
                            register Long cx,cy,cz;
                            register ULong SecondaryOffset;
                            // Test if we can fall downward
                              i=0;
                              cx = x+1 ; cy = y ; cz = z+1; SecondaryOffset = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset ];
                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_Water)
                              {
                                World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y-1, RSz + z, 10, true);
                                World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, true);
                              }
                              for(i=0,j=4,vCount=0;i<4;i++,j++)
                              {
                                cx = x+bft[i].x ; cy = y+bft[i].y ; cz = z+bft[i].y ; SecondaryOffset = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset ];
                                cx = x+bft[j].x ; cy = y+bft[j].y ; cz = z+bft[j].y ; SecondaryOffset = If_x[cx]+If_y[cy]+If_z[cz];St[j] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[j] = &St[j]->Data[ SecondaryOffset ];
                                if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_Water && VoxelTypeManager->VoxelTable[*Vp[j]]->Is_CanBeReplacedBy_Water) vCount++;
                              }

                              if (vCount>0)
                              {
                                i = Random.GetNumber() % vCount;
                                World->SetVoxel_WithCullingUpdate(RSx + x + bft[i].x-1, RSy + y, RSz + z + bft[i].z-1 , 10, true);
                                World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, true);
                              }

                              break;
                           }
                           */
                  case 90:
                            // Bombe
                            IsActiveVoxels = true;
                            {
                              ZVoxelSector * St[32];
                              UShort * Vp[32];
                              ULong SecondaryOffset[32];
                              ULong i, TriggerCount;
                              register Long cx,cy,cz;
                              ZVoxelLocation VxLoc;

                              Short BlastPower = 30;
                              UShort LifeTime  = 200;

                              // Fetch Voxels for all 6 Voxels arounds main.
                              for (i=TriggerCount=0;i<6;i++)
                              {
                                cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                                TriggerCount += 1 & ( VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanTriggerBomb
                                    && (!St[i]->ModifTracker.Get(SecondaryOffset[i]))   ); //
                              }

                              if ((TriggerCount))
                              {
                                if (!World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 91, ZVoxelSector::CHANGE_IMPORTANT,true, & VxLoc)) break;
                                VxLoc.Sector->OtherInfos[VxLoc.Offset] = ((ULong)BlastPower) | ( (ULong)LifeTime << 16 );
                              }

                              break;
                            }

                  case 91:
                            // Explosion de magma
                            IsActiveVoxels = true;
                            {
                              ZVoxelSector * St[32];
                              UShort * Vp[32];
                              ULong SecondaryOffset[32];
                              ULong i,j, MoveCount;
                              bool  MoveEn[6];
                              register Long cx,cy,cz;

                              Short BlastPower;
                              UShort LifeTime;
                              ZVoxelLocation VxLoc;



                              // Unpack data

                              BlastPower = (Short)(Sector->OtherInfos[MainOffset] & 0xffff );
                              LifeTime   = (UShort)(Sector->OtherInfos[MainOffset] >> 16 & 0xffff);

                              // If Lifetime is elapsed, kill this voxel

                              if (LifeTime < 2 || LifeTime > 32768 ) World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, ZVoxelSector::CHANGE_IMPORTANT);

                              // Repack data with decreased lifetime counter

                              Sector->OtherInfos[MainOffset] = ((ULong)BlastPower) | ( (ULong)(LifeTime-1) << 16 );

                              // Fetch Voxels for all 6 Voxels arounds main.
                              for (i=MoveCount=0;i<6;i++)
                              {
                                cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                                MoveCount += 1 & ( MoveEn[i] = VoxelTypeManager->VoxelTable[*Vp[i]]->BlastResistance <= BlastPower );
                              }

                              if ((MoveCount))
                              {
                                j = (Random.GetNumber() % MoveCount) +1;
                                for(i=0;i<6;i++)
                                {
                                  if (MoveEn[i]) j--;
                                  if (!j)
                                  {
                                    if (!World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x - 1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z - 1 ,91, ZVoxelSector::CHANGE_IMPORTANT , true, &VxLoc)) break;
                                    VxLoc.Sector->OtherInfos[VxLoc.Offset] = ((ULong)BlastPower - 1) | ( ((ULong)(LifeTime-4)) << 16 );
                                    St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                    break;
                                  }
                                }
                              }

                              break;
                            }

                  case 92: // Source d'acide
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[32];
                            UShort * Vp[32];
                            ULong SecondaryOffset[32];
                            ULong i;

                            register Long cx,cy,cz;

                              i=0;
                              cx = x+1 ; cy = y ; cz = z+1; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                              for(i=0;i<6;i++)
                              {
                                cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                                if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_Water)
                                {
                                  World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x-1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z-1 , 86, ZVoxelSector::CHANGE_UNIMPORTANT);
                                  St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                }
                              }
                              break;
                            }

                  case 93: // DEBUG BLOCK
                          IsActiveVoxels = true;
                          {
                            IsDebug = true;
                            Sector->Flag_Render_Dirty = true;
                            break;
                          }
                  case 94:  // Pompe à liquide T1
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[8];
                            UShort * Vp[8];
                            ULong SecondaryOffset[8];
                            ULong i,j, Pumpable_Count, Pushable_Count;
                            ULong Pumpable[8], Pushable[8];
                            ZVoxelLocation VInfo;


                            register Long cx,cy,cz;

                            Pumpable_Count = Pushable_Count = 0;
                            for(i=0;i<6;i++)
                            {
                              cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_Pumpable_ByPump_T1)  { Pumpable[Pumpable_Count++] = i; }
                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_Interface_PushBlock) { Pushable[Pushable_Count++] = i; }
                            }

                            if (Pumpable_Count>0 && Pushable_Count>0)
                            {
                              j = (Random.GetNumber() % Pumpable_Count);
                              for (i=0;i<Pushable_Count;i++)
                              {
                                VInfo.Sector = St[ Pushable[i] ];
                                VInfo.Offset = SecondaryOffset[ Pushable[i] ];
                                if (1==VoxelTypeManager->VoxelTable[ *Vp[ Pushable[i]] ]->Interface_PushBlock_Push(&VInfo, *Vp[ Pumpable[j] ], 1 ))
                                {
                                  i = Pumpable[j];
                                  World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x-1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z-1 , 0, ZVoxelSector::CHANGE_IMPORTANT);
                                  St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                  break;
                                }
                              }
                            }
                            break;
                          }

                  case 95:  // Pompe à liquide T2
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[8];
                            UShort * Vp[8];
                            ULong SecondaryOffset[8];
                            ULong i,j, Pumpable_Count, Pushable_Count;
                            ULong Pumpable[8], Pushable[8];
                            ZVoxelLocation VInfo;


                            register Long cx,cy,cz;

                            Pumpable_Count = Pushable_Count = 0;
                            for(i=0;i<6;i++)
                            {
                              cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_Pumpable_ByPump_T2)  { Pumpable[Pumpable_Count++] = i; }
                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_Interface_PushBlock) { Pushable[Pushable_Count++] = i; }
                            }

                            if (Pumpable_Count>0 && Pushable_Count>0)
                            {
                              j = (Random.GetNumber() % Pumpable_Count);
                              for (i=0;i<Pushable_Count;i++)
                              {
                                VInfo.Sector = St[ Pushable[i] ];
                                VInfo.Offset = SecondaryOffset[ Pushable[i] ];
                                if (1==VoxelTypeManager->VoxelTable[ *Vp[ Pushable[i]] ]->Interface_PushBlock_Push(&VInfo, *Vp[ Pumpable[j] ], 1 ))
                                {
                                  i = Pumpable[j];
                                  World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x-1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z-1 , 0, ZVoxelSector::CHANGE_IMPORTANT);
                                  St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                  break;
                                }
                              }
                            }
                            break;
                          }


                  case 97:  // Loader
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[8];
                            UShort * Vp[8];
                            ULong SecondaryOffset[8];
                            ULong i, Opposite;
                            ZVoxelLocation VInfo;


                            register Long cx,cy,cz;

                            for(i=0;i<6;i++)
                            {
                              cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_Interface_PushBlock)
                              {
                                Opposite = BlocOpposite[i];
                                cx = x+bp6[Opposite].x ; cy = y+bp6[Opposite].y ; cz = z+bp6[Opposite].z ; SecondaryOffset[Opposite] = If_x[cx]+If_y[cy]+If_z[cz];St[Opposite] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[Opposite] = &St[Opposite]->Data[ SecondaryOffset[Opposite] ];
                                if (VoxelTypeManager->VoxelTable[*Vp[Opposite]]->Is_Loadable_ByLoader_L1 && ( (!St[Opposite]->ModifTracker.Get(SecondaryOffset[Opposite]) || ( VoxelTypeManager->VoxelTable[*Vp[Opposite]]->BvProp_FastMoving) ) ))
                                {
                                  VInfo.Sector = St[i];
                                  VInfo.Offset = SecondaryOffset[i];
                                  if (1==VoxelTypeManager->VoxelTable[ *Vp[i] ]->Interface_PushBlock_Push(&VInfo, *Vp[ Opposite ], 1 ))
                                  {
                                    World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x-1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z-1 , 0, ZVoxelSector::CHANGE_IMPORTANT);
                                    St[Opposite]->ModifTracker.Set(SecondaryOffset[Opposite]);
                                    break;
                                  }

                                }

                              }
                            }

                            break;
                          }


                  case 98:  // Voxel Materializer
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[8];
                            UShort * Vp[8];
                            ULong SecondaryOffset[8];
                            ULong i, Opposite;
                            ZVoxelLocation VInfo, VInfo2;
                            UShort VoxelType2, VoxelType3;


                            register Long cx,cy,cz;


                            for(i=0;i<6;i++)
                            {
                              cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_Interface_PullBlock)
                              {
                                Opposite = BlocOpposite[i];
                                cx = x+bp6[Opposite].x ; cy = y+bp6[Opposite].y ; cz = z+bp6[Opposite].z ; SecondaryOffset[Opposite] = If_x[cx]+If_y[cy]+If_z[cz];St[Opposite] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[Opposite] = &St[Opposite]->Data[ SecondaryOffset[Opposite] ];
                                if (*Vp[Opposite]==0)
                                {
                                  VInfo.Sector = St[i];
                                  VInfo.Offset = SecondaryOffset[i];
                                  if (1==VoxelTypeManager->VoxelTable[ *Vp[i] ]->Interface_PushBlock_Pull(&VInfo, &VoxelType2 , 1 ))
                                  {
                                    World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x-1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z-1 , VoxelType2, ZVoxelSector::CHANGE_CRITICAL);
                                    St[Opposite]->ModifTracker.Set(SecondaryOffset[Opposite]);
                                    break;
                                  }

                                }
                                else if (*Vp[Opposite] == 97) // Is it a loader just in face.
                                {
                                  if (World->GetVoxelLocation(&VInfo2, RSx+x+bp6[Opposite].x+bp6[Opposite].x - 2 ,RSy+y+bp6[Opposite].y+bp6[Opposite].y - 2 ,RSz+z+bp6[Opposite].z+bp6[Opposite].z - 2))
                                  {
                                    VoxelType2 = VInfo2.Sector->Data[VInfo2.Offset];
                                    //VoxelType = World->GetVoxel( RSx+x+bp6[Opposite].x+bp6[Opposite].x - 2 ,RSy+y+bp6[Opposite].y+bp6[Opposite].y - 2 ,RSz+z+bp6[Opposite].z+bp6[Opposite].z - 2 );
                                    if (VoxelTypeManager->VoxelTable[VoxelType2]->Is_Interface_PushBlock)
                                    {
                                       VInfo.Sector = St[i];
                                       VInfo.Offset = SecondaryOffset[i];
                                       if (1==VoxelTypeManager->VoxelTable[ *Vp[i] ]->Interface_PushBlock_PullTest(&VInfo, &VoxelType3 , 1 )) // Test if we can pull a voxel and which type.
                                       {
                                         if (1==VoxelTypeManager->VoxelTable[VoxelType2]->Interface_PushBlock_Push(&VInfo2, VoxelType3, 1)) // Push it.
                                         {
                                           VoxelTypeManager->VoxelTable[ *Vp[i] ]->Interface_PushBlock_Pull(&VInfo, &VoxelType3 , 1 );      // If pushed ok, then pull it.
                                         }
                                       }
                                    }
                                  }
                                }

                              }
                            }

                            break;
                          }

                  case 100:  // Tube StartPoint
                          IsActiveVoxels = true;
                          {
                            ZVector3L Position,CommingDir;

                            CommingDir = 0;
                            Position.x = RSx+x; Position.y = RSy+y; Position.z = RSz+z;
                            // LightTransmitter_FindEndPoints(&Position,&CommingDir);
                            break;
                          }

                  case 103:  // Mover Right
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[2];
                            UShort * Vp[2];
                            ULong SecondaryOffset[2];

                            register Long cx,cy,cz;
                            ULong i;
                            Long Vx,Vy,Vz;
                            Vx = RSx+x; Vy = RSy+y; Vz = RSz+z;

                            if (CycleNum & 1) break;
                            i=0;
                            cx = x+1 ; cy = y+2 ; cz = z+1 ;
                            SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];
                            St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                            Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                            if (VoxelTypeManager->VoxelTable[*Vp[i]]->BvProp_MoveableByTreadmill && !St[i]->ModifTracker.Get(SecondaryOffset[i]))
                            {
                              i=1;

                              cx = x+1 ; cy = y+2 ; cz = z+2 ;
                              SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];
                              St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                              Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_PlayerCanPassThrough)
                              {
                                World->ExchangeVoxels(Vx,Vy+1,Vz,Vx,Vy+1,Vz+1,ZVoxelSector::CHANGE_CRITICAL, true);
                                /*
                                World->MoveVoxel(Vx,Vy+1,Vz,Vx,Vy+1,Vz+1,0,ZVoxelSector::CHANGE_CRITICAL);
                                St[0]->ModifTracker.Set(SecondaryOffset[0]);
                                St[1]->ModifTracker.Set(SecondaryOffset[1]);
                                */
                              }
                            }
                            break;
                          }
                  case 104:  // Mover Left
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[2];
                            UShort * Vp[2];
                            ULong SecondaryOffset[2];

                            register Long cx,cy,cz;
                            ULong i;
                            Long Vx,Vy,Vz;
                            Vx = RSx+x; Vy = RSy+y; Vz = RSz+z;

                            if (CycleNum & 1) break;
                            i=0;
                            cx = x+1 ; cy = y+2 ; cz = z+1 ;
                            SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];
                            St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                            Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                            if (VoxelTypeManager->VoxelTable[*Vp[i]]->BvProp_MoveableByTreadmill && !St[i]->ModifTracker.Get(SecondaryOffset[i]))
                            {
                              i=1;

                              cx = x+1 ; cy = y+2 ; cz = z ;
                              SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];
                              St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                              Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_PlayerCanPassThrough)
                              {
                                World->ExchangeVoxels(Vx,Vy+1,Vz,Vx,Vy+1,Vz-1,ZVoxelSector::CHANGE_CRITICAL, true);
/*
                                World->MoveVoxel(Vx,Vy+1,Vz,Vx,Vy+1,Vz-1,0,ZVoxelSector::CHANGE_CRITICAL);
                                St[0]->ModifTracker.Set(SecondaryOffset[0]);
                                St[1]->ModifTracker.Set(SecondaryOffset[1]);
*/
                              }
                            }
                            break;
                          }
                  case 105:  // Mover Top
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[2];
                            UShort * Vp[2];
                            ULong SecondaryOffset[2];

                            register Long cx,cy,cz;
                            ULong i;
                            Long Vx,Vy,Vz;
                            Vx = RSx+x; Vy = RSy+y; Vz = RSz+z;

                            if (CycleNum & 1) break;
                            i=0;
                            cx = x+1 ; cy = y+2 ; cz = z+1 ;
                            SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];
                            St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                            Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                            if (VoxelTypeManager->VoxelTable[*Vp[i]]->BvProp_MoveableByTreadmill && !St[i]->ModifTracker.Get(SecondaryOffset[i]))
                            {
                              i=1;

                              cx = x ; cy = y+2 ; cz = z+1 ;
                              SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];
                              St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                              Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_PlayerCanPassThrough)
                              {
                                World->ExchangeVoxels(Vx,Vy+1,Vz,Vx-1,Vy+1,Vz,ZVoxelSector::CHANGE_CRITICAL, true);
/*
                                World->MoveVoxel(Vx,Vy+1,Vz,Vx-1,Vy+1,Vz,0,ZVoxelSector::CHANGE_CRITICAL);
                                St[0]->ModifTracker.Set(SecondaryOffset[0]);
                                St[1]->ModifTracker.Set(SecondaryOffset[1]);
*/
                              }
                            }
                            break;
                          }
                  case 106:  // Mover Bottom
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[2];
                            UShort * Vp[2];
                            ULong SecondaryOffset[2];

                            register Long cx,cy,cz;
                            ULong i;
                            Long Vx,Vy,Vz;
                            Vx = RSx+x; Vy = RSy+y; Vz = RSz+z;

                            if (CycleNum & 1) break;
                            i=0;
                            cx = x+1 ; cy = y+2 ; cz = z+1 ;
                            SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];
                            St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                            Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                            if (VoxelTypeManager->VoxelTable[*Vp[i]]->BvProp_MoveableByTreadmill && !St[i]->ModifTracker.Get(SecondaryOffset[i]))
                            {
                              i=1;

                              cx = x+2 ; cy = y+2 ; cz = z+1 ;
                              SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];
                              St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                              Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_PlayerCanPassThrough)
                              {
                                World->ExchangeVoxels(Vx,Vy+1,Vz,Vx+1,Vy+1,Vz,ZVoxelSector::CHANGE_CRITICAL, true);
/*
                                World->MoveVoxel(Vx,Vy+1,Vz,Vx+1,Vy+1,Vz,0,ZVoxelSector::CHANGE_CRITICAL);
                                St[0]->ModifTracker.Set(SecondaryOffset[0]);
                                St[1]->ModifTracker.Set(SecondaryOffset[1]); */
                              }
                            }
                            break;
                          }
/*
                  case 107:  // Mother Machine
                          {
                            ZVoxelSector * St[8];
                            UShort * Vp[8];
                            ULong SecondaryOffset[8];
                            ULong i, Opposite;
                            UShort VoxelType2;
                            ZVoxelExtension_MotherMachine * Ext;

                            register Long cx,cy,cz;

                            Ext = (ZVoxelExtension_MotherMachine *)Sector->OtherInfos[MainOffset];
                            switch (Ext->MachineState )
                            {
                              case 0:
                                      for(i=0;i<6;i++)
                                      {
                                        cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                                        VoxelType2 = *Vp[i];
                                        if ( VoxelType2 == 1 || VoxelType2 == 26 || VoxelType2==74 )
                                        {

                                          switch(VoxelType2)
                                          {
                                            case 1:  Ext->VoxelType[0]=1 ; Ext->VoxelQuantity[0]++; break;
                                            case 26: Ext->VoxelType[1]=26; Ext->VoxelQuantity[1]++; break;
                                            case 74: Ext->VoxelType[2]=74; Ext->VoxelQuantity[2]++; break;
                                          }
                                          Ext->LastInDirection = i;
                                          World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x-1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z-1 , 0, true);

                                          // Is there the materials in it to fab the bloc

                                          if (Ext->VoxelQuantity[0] >=5)
                                          {
                                            if (Ext->VoxelQuantity[1] >= 5) { Ext->MachineState = 1; Ext->VoxelToOutput = 93; }
                                            if (Ext->VoxelQuantity[0] >=10) { Ext->MachineState = 2; }
                                          }

                                          if (Ext->MachineState == 1)
                                          {
                                            Opposite = BlocOpposite[Ext->LastInDirection];
                                            cx = x+bp6[Opposite].x ; cy = y+bp6[Opposite].y ; cz = z+bp6[Opposite].z ; SecondaryOffset[Opposite] = If_x[cx]+If_y[cy]+If_z[cz]; St[Opposite] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[Opposite] = &St[Opposite]->Data[ SecondaryOffset[Opposite] ];
                                            if (VoxelTypeManager->VoxelTable[*Vp[Opposite]]->Is_PlayerCanPassThrough)
                                            {
                                              World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x-1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z-1 , Ext->VoxelToOutput, true);
                                              for (i=0;i<ZVoxelExtension_MotherMachine::Storage_NumSlots;i++) { Ext->VoxelQuantity[i]=0; }
                                              Ext->MachineState = 0;
                                              Ext->VoxelToOutput = 0;
                                            }
                                            break;
                                          }

                                          break;
                                        }
                                      }
                                      break;
                              case 1:
                                      Opposite = BlocOpposite[Ext->LastInDirection];
                                      cx = x+bp6[Opposite].x ; cy = y+bp6[Opposite].y ; cz = z+bp6[Opposite].z ; SecondaryOffset[Opposite] = If_x[cx]+If_y[cy]+If_z[cz]; St[Opposite] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[Opposite] = &St[Opposite]->Data[ SecondaryOffset[Opposite] ];
                                      if (VoxelTypeManager->VoxelTable[*Vp[Opposite]]->Is_PlayerCanPassThrough)
                                      {
                                        World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x-1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z-1 , Ext->VoxelToOutput, true);
                                        Ext->MachineState = 0;
                                        Ext->VoxelToOutput = 0;
                                      }
                                      break;
                              case 2:
                                      bool NoBloc;
                                      Opposite = BlocOpposite[Ext->LastInDirection];
                                      cx = x+bp6[Opposite].x ; cy = y+bp6[Opposite].y ; cz = z+bp6[Opposite].z ; SecondaryOffset[Opposite] = If_x[cx]+If_y[cy]+If_z[cz]; St[Opposite] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[Opposite] = &St[Opposite]->Data[ SecondaryOffset[Opposite] ];
                                      NoBloc = true;
                                      if (VoxelTypeManager->VoxelTable[*Vp[Opposite]]->Is_PlayerCanPassThrough)
                                      {
                                        for (i=0; i<ZVoxelExtension_MotherMachine::Storage_NumSlots ; i++)
                                        {
                                          if (Ext->VoxelQuantity[i] >0)
                                          {
                                            Ext->VoxelQuantity[i]--;
                                            World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x-1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z-1 , Ext->VoxelType[i], true);
                                            NoBloc = false;
                                            break;
                                          }
                                        }

                                        if (NoBloc) { Ext->MachineState = 0; Ext->VoxelToOutput = 0; }
                                      }
                                      break;





                            }


                            break;
                          }
*/





                  case 107:  // Mother Machine (Base Machine)
                  case 130:  // Molding machine (Machine de moulage)
                  case 131:  // Sheet Manufacturing Unit ( ??? )
                  case 132:  // Wire winder (Bobineuse)
                  case 133:  // Integrated Circuit Engraver (Graveuse de circuits intégrés)
                  case 134:  // Automatic Forging Machine (Forgeuse automatique)
                  case 135:  // Extruder machine (Extrudeuse)
                  case 136:  // Assembly machine (Machine d'assemblage)
                  case 137:  // Breaker (Broyeur)
                  case 139:  // CNC milling machine (Machine CNC)
                  case 140:  // Lathe (Tour)
                  case 142:  // Drawing Machine (Tréfileuse)
                  case 143:  // X-Machine (Machine X)
                  case 172:  // Glass Furnace (Four à verre)
                  case 173:  // Arc Furnace (Four à arc)
                  case 182:  // Vapor Phase Deposition Unit (Unité de déposition en phase vapeur)
                  case 195:  // Laser Cutting Machine (Découpeuse Laser)
                  case 196:  // Versatile Furnace (Four polyvalent)
                  case 210:  // Printing Unit (



                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[8];
                            UShort * Vp[8];
                            ULong SecondaryOffset[8];
                            ULong i, j, TransNum, k, Opposite;
                            UShort VoxelType2;
                            ZVoxelExtension_FabMachine * Ext;
                            ZFabInfos * Fab;
                            bool VoxelFound, TransformationFound;
                            ZFabInfos::ZTransformation * Transformation = 0;

                            // Report this bloc to EgmyManager to be a possible target.
                            ZVector3L Vc;
                            Vc.x = x + RSx; Vc.y = y + RSy; Vc.z = z + RSz;
                            EgmyWaveManager.ReportObjective(&Vc, VoxelType);
                            //


                            register Long cx,cy,cz;
                            Ext = (ZVoxelExtension_FabMachine *)Sector->OtherInfos[MainOffset];
                            Fab = VoxelTypeManager->VoxelTable[VoxelType]->FabInfos;
                            for (bool Reloop = true; Reloop;)
                            {
                              Reloop = false;
                              switch (Ext->MachineState )
                              {
                                case 0: // Test for input voxels. If compatible voxels found, store them. If all conditions are meet, proceed to next state.
                                        for(i=0;i<6;i++)
                                        {
                                          if (i==2) continue;
                                          // Fetch voxel around.
                                          cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                                          VoxelType2 = *Vp[i];

                                          // Only take a voxel if it has not moved.
                                          if (St[i]->ModifTracker.Get(SecondaryOffset[i])) continue;
                                          // Compare to the allowed voxel list.
                                          for (j=0,VoxelFound = false ; j < Fab->MaterialCount ; j++ )
                                          {
                                            if (VoxelType2==Fab->MaterialTable[j].VoxelType) { Ext->VoxelQuantity[j]++; Ext->VoxelType[j]=VoxelType2; VoxelFound = true; break; }
                                          }
                                          // If a compatible voxel was found, continue
                                          if ( VoxelFound )
                                          {
                                            // Last direction is recorded to know the output direction
                                            Ext->LastInDirection = i;
                                            // Erase the inputing voxel. At this point, it's already stored.
                                            World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x-1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z-1 , 0, ZVoxelSector::CHANGE_CRITICAL);

                                            // Purge limit test

                                            if (Ext->VoxelQuantity[ Fab->PurgeSlot ] >= Fab->PurgeCount ) { Ext->MachineState = 2; Reloop = true; break; }

                                            // Transformation Limit Test

                                            if (( Fab->ValidationCount != 65536 ) && (Ext->VoxelQuantity[ Fab->ValidationSlot ] < Fab->ValidationCount )) { break; }

                                            // Is there the materials in it to fab the bloc
                                            TransformationFound = false;
                                            for (TransNum=0;TransNum<Fab->TransformationCount;TransNum++)
                                            {
                                              Transformation = &Fab->TransformationTable[TransNum];
                                              for (k=0, TransformationFound = true; k < Transformation->EntryCount ; k++)
                                              {
                                                if (Ext->VoxelQuantity[ Transformation->FabList[k].Index ] < Transformation->FabList[k].Quantity) { TransformationFound = false; break; }
                                              }
                                              if (TransformationFound) break;
                                            }

                                            if (TransformationFound)
                                            {
                                              // Substract voxel quantities to make the transformation.
                                              for (k=0; k < Transformation->EntryCount ; k++)
                                              {
                                                Ext->VoxelQuantity[ Transformation->FabList[k].Index ] -= Transformation->FabList[k].Quantity;
                                              }
                                              // Machine state change, voxels to output are noted.
                                              for (j=0;j<ZVoxelExtension_FabMachine::Output_NumSlots;j++)
                                              {
                                                Ext->VoxelToOutput[j] = Transformation->Result_VoxelType[j];
                                                Ext->VoxelToOutputQuantities[j] = Transformation->Result_Quantity[j];
                                              }
                                              Ext->MachineState = 1;
                                              Reloop = true;
                                            }
                                            break;
                                          }
                                        }
                                        break;
                                case 1: // Voxel output state;
                                        Opposite = BlocOpposite[Ext->LastInDirection];
                                        cx = x+bp6[Opposite].x ; cy = y+bp6[Opposite].y ; cz = z+bp6[Opposite].z ; SecondaryOffset[Opposite] = If_x[cx]+If_y[cy]+If_z[cz]; St[Opposite] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[Opposite] = &St[Opposite]->Data[ SecondaryOffset[Opposite] ];
                                        if (VoxelTypeManager->VoxelTable[*Vp[Opposite]]->Is_PlayerCanPassThrough)
                                        {
                                          for( j=0, TransformationFound=false ; j < ZVoxelExtension_FabMachine::Output_NumSlots ; j++ )
                                          {
                                            if (Ext->VoxelToOutputQuantities[j])
                                            {
                                              World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x-1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z-1 , Ext->VoxelToOutput[j], ZVoxelSector::CHANGE_CRITICAL);
                                              St[Opposite]->ModifTracker.Set(SecondaryOffset[Opposite]);
                                              if (! --Ext->VoxelToOutputQuantities[j]) Ext->VoxelToOutput[j]=0;
                                              TransformationFound = true;
                                              break;
                                            }
                                          }
                                          if (!TransformationFound) { Ext->MachineState = 2; Reloop = true; }
                                        }
                                        break;
                                case 2: // Ext->MachineState = 0; break;
                                        bool NoBloc;
                                        Opposite = BlocOpposite[Ext->LastInDirection];
                                        cx = x+bp6[Opposite].x ; cy = y+bp6[Opposite].y ; cz = z+bp6[Opposite].z ; SecondaryOffset[Opposite] = If_x[cx]+If_y[cy]+If_z[cz]; St[Opposite] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[Opposite] = &St[Opposite]->Data[ SecondaryOffset[Opposite] ];
                                        NoBloc = true;
                                        if (VoxelTypeManager->VoxelTable[*Vp[Opposite]]->Is_PlayerCanPassThrough)
                                        {
                                          for (i=0; i<ZVoxelExtension_FabMachine::Storage_NumSlots ; i++)
                                          {
                                            if (Ext->VoxelQuantity[i] >0)
                                            {
                                              Ext->VoxelQuantity[i]--;
                                              World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x-1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z-1 , Ext->VoxelType[i], ZVoxelSector::CHANGE_CRITICAL);
                                              if (Ext->VoxelQuantity[i]==0) Ext->VoxelType[i] = 0;
                                              NoBloc = false;
                                              break;
                                            }
                                          }

                                          if (NoBloc)
                                          {
                                            for( j=0 ; j < ZVoxelExtension_FabMachine::Output_NumSlots ; j++ ) { Ext->VoxelToOutput[j]=0; Ext->VoxelToOutputQuantities[j]=0;  }
                                            Ext->MachineState = 0; Reloop = true;
                                          }
                                        }
                                        break;





                              }
                            }

                            break;
                          }

                  case 108:  // Programmable Machine
                          IsActiveVoxels = true;
                          {
                            ZVector3L VoxelCoords;
                            ZVoxelExtension_Programmable * Ext;

                            VoxelCoords.x = RSx + x; VoxelCoords.y = RSy + y; VoxelCoords.z = RSz + z;
                            Ext = (ZVoxelExtension_Programmable *)Sector->OtherInfos[MainOffset];
                            Ext->SetVoxelPosition(&VoxelCoords);
                            Ext->SetGameEnv(GameEnv);
                            if (Ext->IsCompiledOk() & Ext->IsAllowedToRun)
                            {
                              for (ULong OverSteps=0; OverSteps <= Ext->Overclock; OverSteps++)
                              {
                                // If "stop robots" key is pressed, stop the robot."...
                                if (GameEnv->Stop_Programmable_Robots) { printf("Robot %d stopped on user request signal.\n",Ext->RobotSerialNumber); Ext->Script_Engine.RunScript((char *)"Voxel_Unload", true, ZScripting_Squirrel3::RUNCONTEXT_PROGRAMCHANGE); Ext->IsAllowedToRun = false;}
                                // Main robot program...
                                Ext->Script_Engine.RunScript((char *)"Voxel_Step", true, ZScripting_Squirrel3::RUNCONTEXT_NORMALSTEP);
                              }
                            }
                            break;
                          }
                   case 113: // Voxel Displacer
                          IsActiveVoxels = true;
                          {
                            /*
                            ZVector3L VoxelCoords, Vx;
                            ZVector3d * Location, NewLocation;
                            VoxelLocation Loc;

                            VoxelCoords.x = RSx + x; VoxelCoords.y = RSy + y; VoxelCoords.z = RSz + z;

                            for ( Vx.x = VoxelCoords.x - 5; Vx.x < VoxelCoords.x + 5; Vx.x ++ )
                              for (Vx.z = VoxelCoords.z - 5; Vx.z < VoxelCoords.z + 5; Vx.z ++)
                                for (Vx.y = VoxelCoords.y; Vx.y < VoxelCoords.y +5; Vx.y ++ )
                                {
                                  World->MoveVoxel(Vx.x, Vx.y, Vx.z, Vx.x, Vx.y, Vx.z - 1, 0, true);
                                }

                            World->GetVoxelLocation( &Loc, Vx.x+5, Vx.y, Vx.z );
                            Loc.Sector->Flag_HighPriorityRefresh = true;
                            World->GetVoxelLocation( &Loc, Vx.x-5, Vx.y, Vx.z );
                            Loc.Sector->Flag_HighPriorityRefresh = true;
                            World->GetVoxelLocation( &Loc, Vx.x, Vx.y, Vx.z+5 );
                            Loc.Sector->Flag_HighPriorityRefresh = true;
                            World->GetVoxelLocation( &Loc, Vx.x, Vx.y, Vx.z-5 );
                            Loc.Sector->Flag_HighPriorityRefresh = true;

                            Location = &GameEnv->PhysicEngine->GetSelectedActor()->Location;

                            NewLocation = *Location;
                            NewLocation.z -= 256.0;
                            GameEnv->PhysicEngine->GetSelectedActor()->SetPosition(NewLocation);

                            break;
                            */
                            ZVector3L VoxelCoords;

                            VoxelCoords.x = RSx + x; VoxelCoords.y = RSy + y; VoxelCoords.z = RSz + z;
                            GameEnv->ShipCenter = VoxelCoords;
                            break;
                          }
                   case 114:
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[8];
                            UShort * Vp[8];
                            ULong SecondaryOffset[8];

                            ULong i, Opposite;
                            ZVoxelExtension_BlastFurnace * Ext;
                            ZVoxelExtension_FusionElement * Ext2;
                            ZVoxelLocation Loc;

                            Ext = (ZVoxelExtension_BlastFurnace *)Sector->OtherInfos[MainOffset];


                            register Long cx,cy,cz;
                            bool Cont;

                            for (i=0, Cont=true; i<6 && Cont ; i++)
                            {
                              cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                              // Only take a voxel if it has not moved.
                              if (St[i]->ModifTracker.Get(SecondaryOffset[i])) continue;

                              switch (*Vp[i])
                              {
                                case 60: Ext->Quantity_Carbon += 1.0; //charbon
                                         World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x-1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z-1 , 0, ZVoxelSector::CHANGE_CRITICAL);
                                         Cont = false;
                                         break;
                                case 74: // Minerai de Fer
                                         if (Ext->Quantity_Carbon < 1.0) break;
                                           Opposite = BlocOpposite[i];
                                           if (0 != World->GetVoxel(RSx + x + bp6[Opposite].x - 1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z - 1) ) break;
                                           World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x-1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z-1 , 0, ZVoxelSector::CHANGE_CRITICAL);
                                           if (World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x - 1, RSy + y + bp6[Opposite].y - 1 , RSz + z + bp6[Opposite].z - 1 , 115, ZVoxelSector::CHANGE_CRITICAL,true,&Loc ))
                                           {
                                             Loc.Sector->ModifTracker.Set(Loc.Offset);
                                             Ext2 = (ZVoxelExtension_FusionElement *)Loc.Sector->OtherInfos[Loc.Offset];
                                             Ext2->Quantity[ZVoxelExtension_FusionElement::Material_Carbon] = Ext->Quantity_Carbon;
                                             Ext2->Quantity[ZVoxelExtension_FusionElement::Material_Iron] = 100.0 - Ext->Quantity_Carbon;
                                             Ext2->Temperature = 1500.0;
                                             Ext->Quantity_Carbon = 0;
                                             Cont = false;
                                           }
                                         break;
                                case 121: // Minerai de Chrome
                                         if (Ext->Quantity_Carbon < 1.0) break;
                                           Opposite = BlocOpposite[i];
                                           if (0 != World->GetVoxel(RSx + x + bp6[Opposite].x - 1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z - 1) ) break;
                                           World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x-1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z-1 , 0, ZVoxelSector::CHANGE_CRITICAL);
                                           if (World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x - 1, RSy + y + bp6[Opposite].y - 1 , RSz + z + bp6[Opposite].z - 1 , 115, ZVoxelSector::CHANGE_CRITICAL,true,&Loc ))
                                           {
                                             Loc.Sector->ModifTracker.Set(Loc.Offset);
                                             Ext2 = (ZVoxelExtension_FusionElement *)Loc.Sector->OtherInfos[Loc.Offset];
                                             Ext2->Quantity[ZVoxelExtension_FusionElement::Material_Carbon] = Ext->Quantity_Carbon;
                                             Ext2->Quantity[ZVoxelExtension_FusionElement::Material_Chrome] = 100.0 - Ext->Quantity_Carbon;
                                             Ext2->Temperature = 1500.0;
                                             Ext->Quantity_Carbon = 0;
                                             Cont = false;
                                           }
                                         break;
                                case 110: // Minerai de nickel
                                         if (Ext->Quantity_Carbon < 1.0) break;
                                           Opposite = BlocOpposite[i];
                                           if (0 != World->GetVoxel(RSx + x + bp6[Opposite].x - 1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z - 1) ) break;
                                           World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x-1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z-1 , 0, ZVoxelSector::CHANGE_CRITICAL);
                                           if (World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x - 1, RSy + y + bp6[Opposite].y - 1 , RSz + z + bp6[Opposite].z - 1 , 115, ZVoxelSector::CHANGE_CRITICAL,true,&Loc ))
                                           {
                                             Loc.Sector->ModifTracker.Set(Loc.Offset);
                                             Ext2 = (ZVoxelExtension_FusionElement *)Loc.Sector->OtherInfos[Loc.Offset];
                                             Ext2->Quantity[ZVoxelExtension_FusionElement::Material_Carbon] = Ext->Quantity_Carbon;
                                             Ext2->Quantity[ZVoxelExtension_FusionElement::Material_Nickel] = 100.0 - Ext->Quantity_Carbon;
                                             Ext2->Temperature = 1500.0;
                                             Ext->Quantity_Carbon = 0;
                                             Cont = false;
                                           }
                                         break;

                                case 112: // Minerai de Cuivre
                                          if (Ext->Quantity_Carbon < 1.0) break;
                                          Opposite = BlocOpposite[i];
                                          if (0 != World->GetVoxel(RSx + x + bp6[Opposite].x - 1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z - 1) ) break;
                                          World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x-1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z-1 , 0, ZVoxelSector::CHANGE_CRITICAL);
                                          if (World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x - 1, RSy + y + bp6[Opposite].y - 1 , RSz + z + bp6[Opposite].z - 1 , 115, ZVoxelSector::CHANGE_CRITICAL,true,&Loc ))
                                          {
                                            Loc.Sector->ModifTracker.Set(Loc.Offset);
                                            Ext2 = (ZVoxelExtension_FusionElement *)Loc.Sector->OtherInfos[Loc.Offset];
                                            Ext2->Quantity[ZVoxelExtension_FusionElement::Material_Carbon] = Ext->Quantity_Carbon;
                                            Ext2->Quantity[ZVoxelExtension_FusionElement::Material_Copper] = 100.0 - Ext->Quantity_Carbon;
                                            Ext2->Temperature = 1500.0;
                                            Ext->Quantity_Carbon = 0;
                                            Cont = false;
                                          }
                                         break;
                                case 122: // Minerai de Titane
                                          if (Ext->Quantity_Carbon < 1.0) break;
                                          Opposite = BlocOpposite[i];
                                          if (0 != World->GetVoxel(RSx + x + bp6[Opposite].x - 1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z - 1) ) break;
                                          World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x-1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z-1 , 0, ZVoxelSector::CHANGE_CRITICAL);
                                          if (World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x - 1, RSy + y + bp6[Opposite].y - 1 , RSz + z + bp6[Opposite].z - 1 , 115, ZVoxelSector::CHANGE_CRITICAL,true,&Loc ))
                                          {
                                            Loc.Sector->ModifTracker.Set(Loc.Offset);
                                            Ext2 = (ZVoxelExtension_FusionElement *)Loc.Sector->OtherInfos[Loc.Offset];
                                            Ext2->Quantity[ZVoxelExtension_FusionElement::Material_Carbon] = Ext->Quantity_Carbon;
                                            Ext2->Quantity[ZVoxelExtension_FusionElement::Material_Titanium] = 100.0 - Ext->Quantity_Carbon;
                                            Ext2->Temperature = 1500.0;
                                            Ext->Quantity_Carbon = 0;
                                            Cont = false;
                                          }
                                         break;
                                case 127: // Minerai d'Etain
                                          if (Ext->Quantity_Carbon < 1.0) break;
                                          Opposite = BlocOpposite[i];
                                          if (0 != World->GetVoxel(RSx + x + bp6[Opposite].x - 1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z - 1) ) break;
                                          World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x-1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z-1 , 0, ZVoxelSector::CHANGE_CRITICAL);
                                          if (World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x - 1, RSy + y + bp6[Opposite].y - 1 , RSz + z + bp6[Opposite].z - 1 , 115, ZVoxelSector::CHANGE_CRITICAL,true,&Loc ))
                                          {
                                            Loc.Sector->ModifTracker.Set(Loc.Offset);
                                            Ext2 = (ZVoxelExtension_FusionElement *)Loc.Sector->OtherInfos[Loc.Offset];
                                            Ext2->Quantity[ZVoxelExtension_FusionElement::Material_Carbon] = Ext->Quantity_Carbon;
                                            Ext2->Quantity[ZVoxelExtension_FusionElement::Material_Tin] = 100.0 - Ext->Quantity_Carbon;
                                            Ext2->Temperature = 1500.0;
                                            Ext->Quantity_Carbon = 0;
                                            Cont = false;
                                          }
                                         break;
                                case 150: // Minerai de plomb
                                          if (Ext->Quantity_Carbon < 1.0) break;
                                          Opposite = BlocOpposite[i];
                                          if (0 != World->GetVoxel(RSx + x + bp6[Opposite].x - 1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z - 1) ) break;
                                          World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x-1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z-1 , 0, ZVoxelSector::CHANGE_CRITICAL);
                                          if (World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x - 1, RSy + y + bp6[Opposite].y - 1 , RSz + z + bp6[Opposite].z - 1 , 115, ZVoxelSector::CHANGE_CRITICAL,true,&Loc ))
                                          {
                                            Loc.Sector->ModifTracker.Set(Loc.Offset);
                                            Ext2 = (ZVoxelExtension_FusionElement *)Loc.Sector->OtherInfos[Loc.Offset];
                                            Ext2->Quantity[ZVoxelExtension_FusionElement::Material_Carbon] = Ext->Quantity_Carbon;
                                            Ext2->Quantity[ZVoxelExtension_FusionElement::Material_Lead] = 100.0 - Ext->Quantity_Carbon;
                                            Ext2->Temperature = 1500.0;
                                            Ext->Quantity_Carbon = 0;
                                            Cont = false;
                                          }
                                         break;
                                case 197: // Minerai d'aluminium
                                          if (Ext->Quantity_Carbon < 1.0) break;
                                          Opposite = BlocOpposite[i];
                                          if (0 != World->GetVoxel(RSx + x + bp6[Opposite].x - 1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z - 1) ) break;
                                          World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x-1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z-1 , 0, ZVoxelSector::CHANGE_CRITICAL);
                                          if (World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x - 1, RSy + y + bp6[Opposite].y - 1 , RSz + z + bp6[Opposite].z - 1 , 115, ZVoxelSector::CHANGE_CRITICAL,true,&Loc ))
                                          {
                                            Loc.Sector->ModifTracker.Set(Loc.Offset);
                                            Ext2 = (ZVoxelExtension_FusionElement *)Loc.Sector->OtherInfos[Loc.Offset];
                                            Ext2->Quantity[ZVoxelExtension_FusionElement::Material_Carbon] = Ext->Quantity_Carbon;
                                            Ext2->Quantity[ZVoxelExtension_FusionElement::Material_Aluminium] = 100.0 - Ext->Quantity_Carbon;
                                            Ext2->Temperature = 1500.0;
                                            Ext->Quantity_Carbon = 0;
                                            Cont = false;
                                          }
                                         break;

                              }

                            }

                            break;
                          }

                   case 115: // Aliage en Fusion.
                   IsActiveVoxels = true;
                   {
                     ZVoxelSector * St[32];
                     UShort * Vp[32];
                     ULong SecondaryOffset[32];
                     ULong i,vCount,j;
                     bool  DirEn[4], GetOut;
                     register Long cx,cy,cz;

                     ZVoxelExtension_FusionElement * Ext, * Ext2;
                     Ext = (ZVoxelExtension_FusionElement *)Sector->OtherInfos[MainOffset];

                     // Refroidissement Naturel. Se fait au bout de 5 minutes.

                     Ext->Temperature -= 0.003333333 * LastLoopTime;
                     if (Ext->Temperature < 500.0)
                     {
                       // World->SetVoxel_WithCullingUpdate( RSx+cx, RSy+cy, RSz + cz, 0, true );
                       World->SetVoxel_WithCullingUpdate( RSx+x , RSy+y , RSz + z , Ext->Solidify(), ZVoxelSector::CHANGE_CRITICAL );
                       break;
                     }

                     // Blending
                     for (i=0, GetOut = false;i<6;i++)
                     {
                       cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                       switch (*Vp[i] )
                       {

                         case 115: // Another Fusion bloc, if yes, we blend.
                                   Ext2= (ZVoxelExtension_FusionElement *)St[i]->OtherInfos[ SecondaryOffset[i] ];
                                   Ext->Blend(Ext2);
                                   break;
                         case 85:  // Water, we cool down and transform.
                                   World->SetVoxel_WithCullingUpdate( RSx+cx-1, RSy+cy-1, RSz + cz-1, 0, ZVoxelSector::CHANGE_CRITICAL );
                                   World->SetVoxel_WithCullingUpdate( RSx+x , RSy+y , RSz + z , Ext->Solidify(), ZVoxelSector::CHANGE_CRITICAL );
                                   GetOut = true;
                                   break;
                       }
                       if (GetOut) break;
                     }
                     if (GetOut) break;

                     // Test if we can fall downward
                       i=0;
                       cx = x+1 ; cy = y ; cz = z+1; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                       if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_Water)
                       {
                         World->MoveVoxel(RSx + x, RSy + y, RSz + z, RSx + x, RSy + y-1, RSz + z, 0, ZVoxelSector::CHANGE_CRITICAL );
                         //World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y-1, RSz + z, 84, true);
                         //World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, true);
                         St[i]->ModifTracker.Set(SecondaryOffset[i]);
                         break;
                       }
                       for(i=0,j=4,vCount=0;i<4;i++,j++)
                       {
                         cx = x+bft[i].x ; cy = y+bft[i].y ; cz = z+bft[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                         cx = x+bft[j].x ; cy = y+bft[j].y ; cz = z+bft[j].z ; SecondaryOffset[j] = If_x[cx]+If_y[cy]+If_z[cz];St[j] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[j] = &St[j]->Data[ SecondaryOffset[j] ];
                         if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_Water && VoxelTypeManager->VoxelTable[*Vp[j]]->Is_CanBeReplacedBy_Water) {vCount++; DirEn[i]=true;}
                         else DirEn[i]=false;
                       }

                       if (vCount>0)
                       {
                         j = (Random.GetNumber() % vCount) +1;
                         for (i=0;i<4;i++)
                         {
                           if (DirEn[i]) j--;
                           if (!j)
                           {
                             World->MoveVoxel(RSx + x, RSy + y, RSz + z, RSx + x + bft[i].x-1, RSy + y, RSz + z + bft[i].z-1 , 0, ZVoxelSector::CHANGE_CRITICAL );
                             // World->SetVoxel_WithCullingUpdate(RSx + x + bft[i].x-1, RSy + y, RSz + z + bft[i].z-1 , 84, true);
                             // World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, true);
                             St[i]->ModifTracker.Set(SecondaryOffset[i]);
                             break;
                           }
                         }
                       }
                       break;
                     }

                   case 153:  // Mining Robot XR-1
                   case 154:  // Mining Robot XR-2
                   case 155:  // Mining Robot XR-3
                   case 156:  // Mining Robot XR-4
                   case 157:  // Mining Robot XR-5
                   case 158:  // Mining Robot XR-TEST
                   case 159:  // Unloading Robot
                   IsActiveVoxels = true;
                   {
                     ZVector3L VoxelCoords, StorageLocation;
                     ZVoxelExtension_MiningRobot_xr1  * Ext;
                     ZVoxelSector * St[32];
                     UShort * Vp[32];
                     ULong SecondaryOffset[32];
                     ULong i;
                     UShort Voxel;
                     bool Found;
                     Long cx,cy,cz;

                     VoxelCoords.x = RSx + x; VoxelCoords.y = RSy + y; VoxelCoords.z = RSz + z;
                     Ext = (ZVoxelExtension_MiningRobot_xr1 *)Sector->OtherInfos[MainOffset];

                     if (!Ext->State)
                     {
                       for (i=0, Found = false ;i<4 && (!Found) ;i++)
                       {
                         cx = x+xbp6[i].x ; cy = y+xbp6[i].y ; cz = z+xbp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                         Voxel = *Vp[i];
                         if (VoxelTypeManager->VoxelTable[Voxel]->Is_Interface_PushBlock)
                         {
                           Found = true;
                           StorageLocation.x = RSx + cx - 1; StorageLocation.y = RSy + cy - 1; StorageLocation.z = RSz + cz - 1;
                           switch(VoxelType)
                           {
                             case 153: Ext->Robot_Init( &StorageLocation, i,  8,  8, 16,  3,  1 , 1 ); break; // XR-1
                             case 154: Ext->Robot_Init( &StorageLocation, i,  8,  8, 32,  1,  2 , 2 ); break; // XR-2
                             case 155: Ext->Robot_Init( &StorageLocation, i, 16, 16, 32,  0,  8 , 3 ); break; // XR-3
                             case 156: Ext->Robot_Init( &StorageLocation, i, 16, 16, 64, -4,  4 , 4 ); break; // XR-4
                             case 157: Ext->Robot_Init( &StorageLocation, i, 32, 32, 64, -19, 64, 5 ); break; // XR-5
                             case 158: Ext->Robot_Init( &StorageLocation, i, 32, 32, 64, -37 ,16, 6 );break; // XR-Test
                             //case 159:
                             // Ext->Robot_Init( &StorageLocation, i,  8,  8, 64,  0,  4, 4 );
                             //case 157: Ext->Robot_Init( &StorageLocation, i, 16, 16, 64, -4 , 8, 5 ); break;
                             //case 158: Ext->Robot_Init( &StorageLocation, i, 16, 16, 64, -37 ,16, 6 ); break;
                           }
                         }
                       }
                     }
                     else Ext->Robot_Move(&VoxelCoords, GameEnv);

                     break;
                   }

                   case 198: // Sequencer
                             IsActiveVoxels = true;
                             {
                               ZVoxelSector * St[8];
                               UShort * Vp[8];
                               ULong SecondaryOffset[8];
                               ULong i, Count;
                               ZVoxelExtension_Sequencer * Ext;
                               ZMemSize Index;
                               bool CheckQuantities, MaterialsAreIn;

                               ZGenericTable<UShort,ZVoxelExtension_Sequencer::Input_NumSlots> AllowedVoxels;
                               ULong NeededQuantity[ZVoxelExtension_Sequencer::Input_NumSlots];

                               register Long cx,cy,cz;

                               Ext = (ZVoxelExtension_Sequencer *)Sector->OtherInfos[MainOffset];

                               switch (Ext->Mode)
                               {

                                 case 0: // Input mode

                                         // Scan the table for allowed voxels

                                         for ( i= ZVoxelExtension_Sequencer::Start_SequenceSlots; i < ZVoxelExtension_Sequencer::End_SequenceSlots ; i++)
                                         {
                                           if (Ext->VoxelType[i]==199 && Ext->VoxelQuantity[i]>0) break;
                                           if (!(Ext->VoxelType[i]==0 || Ext->VoxelQuantity[i]==0 ))
                                           {
                                             if (!AllowedVoxels.FindElement( &Ext->VoxelType[i], &Index ))
                                             {
                                               AllowedVoxels.AddToTable(&Ext->VoxelType[i]);
                                               if (AllowedVoxels.FindElement( &Ext->VoxelType[i], &Index )) NeededQuantity[Index] = Ext->VoxelQuantity[i];
                                             }
                                             else
                                             {
                                               NeededQuantity[Index]+=Ext->VoxelQuantity[i];
                                             }
                                           }
                                         }

                                         // If an allowed voxel is going around, then grab it !!!

                                         for (i=0, CheckQuantities = false;i<6;i++)
                                         {
                                           if (i == Ext->OutputLocation ) continue; // Don't input at the output position.
                                           cx = x+xbp6[i].x ; cy = y+xbp6[i].y ; cz = z+xbp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                                           if (AllowedVoxels.FindElement(Vp[i],&Index) && (!St[i]->ModifTracker.Get(SecondaryOffset[i])))
                                           {
                                             ULong SlotNum;
                                             if (Ext->FindSlot(*Vp[i],SlotNum))
                                             {
                                               if (Ext->VoxelQuantity[SlotNum]<NeededQuantity[Index])
                                               {
                                                 Ext->VoxelQuantity[SlotNum]++;
                                                 World->SetVoxel_WithCullingUpdate(RSx + x + xbp6[i].x-1, RSy + y + xbp6[i].y - 1, RSz + z + xbp6[i].z-1 , 0, ZVoxelSector::CHANGE_CRITICAL);
                                                 CheckQuantities = true;
                                               }
                                             }
                                             else if (Ext->FindFreeSlot(SlotNum))
                                             {
                                               Ext->VoxelType[SlotNum] = *Vp[i];
                                               Ext->StoreBlocks(*Vp[i],1);
                                               World->SetVoxel_WithCullingUpdate(RSx + x + xbp6[i].x-1, RSy + y + xbp6[i].y - 1, RSz + z + xbp6[i].z-1 , 0, ZVoxelSector::CHANGE_CRITICAL);
                                               CheckQuantities = true;
                                             }
                                           }
                                         }

                                         // Check

                                         if (CheckQuantities)
                                         {
                                           ULong Quantity, SlotNum;
                                           UShort VoxelType2;

                                           MaterialsAreIn = true;
                                           Count = AllowedVoxels.GetCount();
                                           CheckQuantities = (Count != 0);
                                           for (i=0; i<Count ;i++)
                                           {
                                             VoxelType2 = *AllowedVoxels.GetElement(i);
                                             Quantity = NeededQuantity[i];
                                             if (Ext->FindSlot(VoxelType2, SlotNum))
                                             {
                                               if (Ext->VoxelQuantity[SlotNum] < Quantity) MaterialsAreIn = false;
                                             }
                                             else MaterialsAreIn = false;
                                           }

                                           if (MaterialsAreIn)
                                           {
                                             Ext->Mode = 1; // Go to output mode
                                             Ext->SequencePosition = ZVoxelExtension_Sequencer::Start_SequenceSlots;
                                             Ext->PositionRemain = (Ext->VoxelQuantity[Ext->SequencePosition] && Ext->VoxelType[Ext->SequencePosition]) ? Ext->VoxelQuantity[Ext->SequencePosition] : 0;
                                             // printf("C'est bon.\n");
                                           }
                                         }

                                         break;

                                 case 1: // Output Mode
                                         bool Advance = false;
                                         if (!Ext->PositionRemain)
                                         {
                                           Advance = true;
                                         }
                                         else
                                         {
                                           UShort VoxelToOutput;

                                           VoxelToOutput = Ext->VoxelType[Ext->SequencePosition];

                                           // If we found an end mark, terminate sequence and swith to output mode.
                                           if (VoxelToOutput == 199 && Ext->VoxelQuantity[Ext->SequencePosition] > 0) { Ext->Mode = 0; Ext->SequencePosition = 0; Ext->PositionRemain = 0; break; }

                                           cx = x+xbp6[Ext->OutputLocation].x ; cy = y+xbp6[Ext->OutputLocation].y ; cz = z+xbp6[Ext->OutputLocation].z ; SecondaryOffset[Ext->OutputLocation] = If_x[cx]+If_y[cy]+If_z[cz];St[Ext->OutputLocation] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[Ext->OutputLocation] = &St[Ext->OutputLocation]->Data[ SecondaryOffset[Ext->OutputLocation] ];
                                           if (*Vp[Ext->OutputLocation] !=0) break; // If position is not free, wait for it to be.

                                           if (Ext->FindSlot(VoxelToOutput, i))
                                           {

                                             Ext->VoxelQuantity[i]--;
                                             if (Ext->VoxelQuantity[i] == 0) Ext->VoxelType[i]=0;
                                             World->SetVoxel_WithCullingUpdate(RSx + x + xbp6[Ext->OutputLocation].x-1, RSy + y + xbp6[Ext->OutputLocation].y - 1, RSz + z + xbp6[Ext->OutputLocation].z-1 , VoxelToOutput, ZVoxelSector::CHANGE_CRITICAL);
                                             St[Ext->OutputLocation]->ModifTracker.Set(SecondaryOffset[Ext->OutputLocation]);
                                             if (!(--Ext->PositionRemain))
                                             {
                                               Advance = true;
                                             }

                                           } else Ext->Mode = 0; // Reset to input sequence if there is something missing.
                                         }
                                         if (Advance)
                                         {
                                            Ext->SequencePosition++;
                                            if (Ext->SequencePosition >= ZVoxelExtension_Sequencer::End_SequenceSlots ) { Ext->Mode = 0; Ext->SequencePosition = 0; Ext->PositionRemain = 0; break; }
                                            Ext->PositionRemain = (Ext->VoxelQuantity[Ext->SequencePosition] && Ext->VoxelType[Ext->SequencePosition]) ? Ext->VoxelQuantity[Ext->SequencePosition] : 0;
                                         }
                                         break;


                             }
                             break;
                           }

                   case 200:
                           IsLowActivityVoxels = true; // Maintain the low activity flag.
                           Sector->LowRefresh_Mask = 0xF; // Get 1/16 cycle
                           {
                             ZVoxelLocation Loc;
                             ZVector3L VoxelCoords;
                             ZVector3d VoxelLocation;
                             double Distance;

                             VoxelCoords.x = RSx + x; VoxelCoords.y = RSy + y; VoxelCoords.z = RSz + z;
                             World->Convert_Coords_VoxelToPlayer( &VoxelCoords, &VoxelLocation );
                             VoxelLocation.x += 128.0;VoxelLocation.z += 128.0;VoxelLocation.y += 128.0;
                             Distance = VoxelLocation.Distance(PlayerLocation);
                             if (Distance < 4096.0)
                             {
                               IsLowActivityVoxels = false;
                               IsActiveVoxels = true;
                               World->SetVoxel_WithCullingUpdate(VoxelCoords.x, VoxelCoords.y - 1 , VoxelCoords.z , 201, ZVoxelSector::CHANGE_UNIMPORTANT, true, &Loc);
                               // Loc.Sector->OtherInfos[Loc.Offset] = 128;
                               if (FireMineTime) FireMineTime += 20;
                               else              FireMineTime =  64;
                               // printf("Distance %lf\n", Distance );
                             }

                           }
                           break;

                   case 201: // Raging fire.
                           IsActiveVoxels = true;
                           {
                             ZVoxelSector * St[8];
                             UShort * Vp[8];
                             ULong SecondaryOffset[8];

                             ULong i;

                             Long cx,cy,cz;

                             if (!FireMineTime)
                             {
                               if ((Random.GetNumber() & 64) == 0)
                               {
                                 World->SetVoxel_WithCullingUpdate(RSx + x , RSy + y , RSz + z ,202, ZVoxelSector::CHANGE_UNIMPORTANT); // 85
                                 Sector->ModifTracker.Set(MainOffset);
                               }
                             }
                             else
                             for (i=0;i<6;i++)
                             {
                               cx = x+xbp6[i].x ; cy = y+xbp6[i].y ; cz = z+xbp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                               if (! VoxelTypeManager->VoxelTable[*Vp[i]]->BvProp_AtomicFireResistant)
                               {
                                 World->SetVoxel_WithCullingUpdate(RSx + x + xbp6[i].x-1, RSy + y + xbp6[i].y - 1, RSz + z + xbp6[i].z-1 , 201, ZVoxelSector::CHANGE_UNIMPORTANT);
                                 St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                 // Loc.Sector->OtherInfos[Loc.Offset] = RemainingLifeTime-3;
                               }
                             }
                           }
                           break;

                   case 202: // Bloody fluid.
                           IsActiveVoxels = true;
                           {
                             ZVoxelSector * St[8];
                             UShort * Vp[8];
                             ULong SecondaryOffset[8];

                             ULong i;
                             UByte DirCode;

                             ZVoxelLocation Loc;

                             Long cx,cy,cz;

                           // Test if we can fall downward
                             i=0;
                             cx = x + 1 ; cy = y ; cz = z + 1; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz]; St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                             if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_Water)
                             {
                               World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y-1, RSz + z, 202, ZVoxelSector::CHANGE_UNIMPORTANT);
                               World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, ZVoxelSector::CHANGE_UNIMPORTANT);
                               St[i]->ModifTracker.Set(SecondaryOffset[i]);
                               break;
                             }
                             for(i=0, DirCode=0;i<4;i++)
                             {
                               cx = x+bft[i].x ; cy = y+bft[i].y ; cz = z+bft[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                               DirCode = (DirCode << 1) | (1 & (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_Water));
                             }
                             if (!DirCode) break;
                             DirCode = DirCode ^ (Random.GetNumber() & DirCode);
                             if (!DirCode) break;
                             DirCode = DirCodeTable[DirCode];

                             World->SetVoxel_WithCullingUpdate(RSx + x + bft[DirCode].x - 1 , RSy + y , RSz + z + bft[DirCode].z - 1, 202, ZVoxelSector::CHANGE_UNIMPORTANT, true, &Loc);
                             World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, ZVoxelSector::CHANGE_UNIMPORTANT);
                             Loc.Sector->ModifTracker.Set(Loc.Offset);
                             // St[i]->ModifTracker.Set(SecondaryOffset[i]);
                           }
                           break;

                   case 204: // Voxel Picker
                           IsActiveVoxels = true;
                           {
                             ZVoxelSector * St[8];
                             UShort * Vp[8];
                             ULong SecondaryOffset[8];
                             ULong i, j, OtherInfos;
                             UShort        LockedVoxel;
                             Long cx,cy,cz;
                             UByte Dir_In, Dir_Out;

                             // Extract values stored in extension

                             OtherInfos = Sector->OtherInfos[MainOffset];
                             Dir_In = (UByte)((OtherInfos >> 4)  & 0xf);
                             Dir_Out= (UByte)(OtherInfos & 0xf);
                             LockedVoxel = (Sector->OtherInfos[MainOffset] >> 16) & 0xffff;

                             // If directions are not fixed, try doing it.

                             if (Dir_In > 5 || Dir_Out > 5)
                             {
                               for(i=0;i<6;i++)
                               {
                                 cx = x+xbp6[i].x ; cy = y+xbp6[i].y ; cz = z+xbp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                                 if (*Vp[i] == 205) { Sector->OtherInfos[MainOffset] = (Sector->OtherInfos[MainOffset] & 0xffffffff0f) | ((i << 4 ) & 0xf0); }
                                 if (*Vp[i] == 206) { Sector->OtherInfos[MainOffset] = (Sector->OtherInfos[MainOffset] & 0xfffffffff0) | (i & 0xf); }

                               }
                               break;
                             }

                             // Fetch the Dir_In Voxel

                             i = Dir_In;
                             cx = x+xbp6[i].x ; cy = y+xbp6[i].y ; cz = z+xbp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                             // If the VoxelType to process is not fixed, try doing it.

                             if (!LockedVoxel)
                             {
                               if ( (*Vp[i] != 205) && (*Vp[i] != 206) )
                               {
                                 Sector->OtherInfos[MainOffset] = (Sector->OtherInfos[MainOffset] & 0x0000ffff) | ( (*Vp[i] << 16) & 0xffff0000);
                               }
                               break;
                             }

                             // Fetch the voxel at the output

                             j = Dir_Out;
                             cx = x+xbp6[j].x ; cy = y+xbp6[j].y ; cz = z+xbp6[j].z ; SecondaryOffset[j] = If_x[cx]+If_y[cy]+If_z[cz];St[j] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[j] = &St[j]->Data[ SecondaryOffset[j] ];

                             // Test if output is free and input is from the matching type

                             if ( (*Vp[j] != 0) || (*Vp[i] != LockedVoxel) || (St[i]->ModifTracker.Get(SecondaryOffset[i])) ) break;

                             // Move the Voxel...

                             World->MoveVoxel( RSx + x + xbp6[i].x - 1, RSy + y + xbp6[i].y - 1 , RSz + z + xbp6[i].z - 1,
                                               RSx + x + xbp6[j].x - 1, RSy + y + xbp6[j].y - 1 , RSz + z + xbp6[j].z - 1, 0, ZVoxelSector::CHANGE_CRITICAL);
                             St[j]->ModifTracker.Set(SecondaryOffset[j]);

                           }
                           break;



                   case 209: // Voxel Mover
                           IsActiveVoxels = true;
                           {
                             ZVoxelSector * St[8];
                             UShort * Vp[8];
                             ULong SecondaryOffset[8];
                             ULong i, j, OtherInfos;
                             Long cx,cy,cz;
                             UByte Dir_In, Dir_Out;

                             // Extract values stored in extension

                             OtherInfos = Sector->OtherInfos[MainOffset];
                             Dir_In = (UByte)((OtherInfos >> 4)  & 0xf);
                             Dir_Out= (UByte)(OtherInfos & 0xf);

                             // If directions are not fixed, try doing it.

                             if (Dir_In > 5 || Dir_Out > 5)
                             {
                               for(i=0;i<6;i++)
                               {
                                 cx = x+xbp6[i].x ; cy = y+xbp6[i].y ; cz = z+xbp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                                 if (*Vp[i] == 205) { Sector->OtherInfos[MainOffset] = (Sector->OtherInfos[MainOffset] & 0xffffffff0f) | ((i << 4 ) & 0xf0); }
                                 if (*Vp[i] == 206) { Sector->OtherInfos[MainOffset] = (Sector->OtherInfos[MainOffset] & 0xfffffffff0) | (i & 0xf); }
                               }
                               break;
                             }

                             // Fetch the Dir_In Voxel

                             i = Dir_In;
                             cx = x+xbp6[i].x ; cy = y+xbp6[i].y ; cz = z+xbp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                            // Fetch the voxel at the output

                             j = Dir_Out;
                             cx = x+xbp6[j].x ; cy = y+xbp6[j].y ; cz = z+xbp6[j].z ; SecondaryOffset[j] = If_x[cx]+If_y[cy]+If_z[cz];St[j] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[j] = &St[j]->Data[ SecondaryOffset[j] ];

                             // Test if output is free and input is from the matching type

                             if ( (*Vp[j] != 0 ) || (St[i]->ModifTracker.Get(SecondaryOffset[i])) ) break;

                             // Move the Voxel...

                             World->MoveVoxel( RSx + x + xbp6[i].x - 1, RSy + y + xbp6[i].y - 1 , RSz + z + xbp6[i].z - 1,
                                               RSx + x + xbp6[j].x - 1, RSy + y + xbp6[j].y - 1 , RSz + z + xbp6[j].z - 1, 0, ZVoxelSector::CHANGE_CRITICAL);
                             St[j]->ModifTracker.Set(SecondaryOffset[j]);

                           }
                           break;
                   case 213: // Egmy
                           IsActiveVoxels = true;
                           {
                             ZVoxelSector * St[8];
                             UShort * Vp[8];
                             ULong SecondaryOffset[8];
                             ZVector3L VoxelCoords;
                             Long cx,cy,cz;

                             ZVoxelExtension_Egmy_T1 * Ext;

                             ULong i;

                             VoxelCoords.x = RSx + x; VoxelCoords.y = RSy + y; VoxelCoords.z = RSz + z;
                             Ext = (ZVoxelExtension_Egmy_T1 *)Sector->OtherInfos[MainOffset];

                             // If no target acquired, try to get one.

                             if ((!Ext->EvasionMode) && Ext->TargetVoxelType == 0 )
                             {
                               ZEgmyTargetManager::ObjectiveEntry * Objective;
                               if (EgmyWaveManager.GetRandomObjective(Random.GetNumber(), Objective))
                               {
                                 Ext->TargetLocation  = Objective->Location;
                                 Ext->TargetVoxelType = Objective->VoxelType;
                               }
                             }

                             ZVector3L Displacement, Destination, Target;
                             ZVoxelType * VoxelRecord;
                             // If
                             if (Ext->EvasionMode)
                             {
                               Target = Ext->TargetLocation;
                             }
                             else
                             {
                               if (Ext->TargetVoxelType != 0)
                               {
                                 // Does the target still here ?
                                 if (World->GetVoxel(&Ext->TargetLocation) != Ext->TargetVoxelType ) { Ext->TargetVoxelType = 0; break; } // Find new target
                                 Target = Ext->TargetLocation;
                               }
                               else
                               {
                                 // Chase the player

                                 World->Convert_Coords_PlayerToVoxel(&PlayerLocation, &Target);

                                 // If the player died recently and egmys are too close, turn egmys into escaping mode.

                                 if (SelectedActor->Time_ElapsedTimeSinceLastRespawn < 60000000ULL)
                                 {
                                   if (Target.Distance(VoxelCoords) < 20.0)
                                   {
                                     Target = VoxelCoords - (Target - VoxelCoords);
                                   }
                                 }
                               }
                             }

                             // Get the displacement direction
                             Displacement = Target;
                             Displacement -= VoxelCoords;
                             Displacement.GetSign(Displacement);

                             // If it moves in several axes, choose only one of it in a random way.
                             ULong DepCount;
                             UByte DepTable[4];
                             DepCount = 0;
                             if (Displacement.x != 0) DepTable[DepCount++] = 0;
                             if (Displacement.y != 0) DepTable[DepCount++] = 1;
                             if (Displacement.z != 0) DepTable[DepCount++] = 2;
                             if (DepCount > 1)
                             {
                               switch ( DepTable[Random.GetNumber() % DepCount] )
                               {
                                 case 0: Displacement.y = Displacement.z = 0; break;
                                 case 1: Displacement.x = Displacement.z = 0; break;
                                 case 2: Displacement.x = Displacement.y = 0; break;
                               }
                             }

                             // I
                             if (Ext->EvasionMode && DepCount == 0) { Ext->EvasionMode = false; break; }

                             i=0;
                             cx = x + Displacement.x + 1 ; cy = y+Displacement.y+1 ; cz = z+Displacement.z+1 ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                             Destination = VoxelCoords; Destination += Displacement;
                             if (*Vp[i] == 65535) {break;}
                             VoxelRecord = VoxelTypeManager->VoxelTable[*Vp[i]];
                             if (VoxelRecord->Is_CanBeReplacedBy_Water)
                             {
                               World->MoveVoxel_Sm( &VoxelCoords, &Destination, 0 , ZVoxelSector::CHANGE_UNIMPORTANT);
                             }
                             else
                             {
                               if (Ext->EvasionCount < 5 || *Vp[0] == 213)
                               {
                                 // register ULong Rnd = Random.GetNumber();
                                 Ext->TargetLocation = VoxelCoords;
/*
                                 Ext->TargetLocation.x +=  (Rnd & 0x1F) - 16;
                                 Ext->TargetLocation.y +=  ((Rnd >>= 8) & 0x1F ) - 16;
                                 Ext->TargetLocation.z +=  ((Rnd >>= 8) & 0x1F ) - 16;
                                 */
                                 Ext->TargetLocation.x +=  (Random.GetNumber() & 0x1F) - 8;
                                 Ext->TargetLocation.y +=  (Random.GetNumber() & 0x1F) - 8;
                                 Ext->TargetLocation.z +=  (Random.GetNumber() & 0x1F) - 8;


                                 Ext->EvasionMode = true;
                                 Ext->TargetVoxelType = 0;
                                 Ext->EvasionCount++;
                                 break;
                               }
                               else if (Ext->EvasionMode) { Ext->EvasionMode = 0; break; }

                               if ((!VoxelRecord->BvProp_EgmyT1Resistant)) // Random.GetNumber() < (42949672 * 5) &&
                               {
                                 World->SetVoxel_WithCullingUpdate(VoxelCoords.x, VoxelCoords.y, VoxelCoords.z, 0, ZVoxelSector::CHANGE_UNIMPORTANT, false, 0 );
                                 World->SetVoxel_WithCullingUpdate(Destination.x, Destination.y, Destination.z, 0, ZVoxelSector::CHANGE_UNIMPORTANT, false, 0 );
                                 //World->MoveVoxel_Sm( &VoxelCoords, &Destination, 0 , true);
                               }
                             }
                           }
                           break;
                   case 235: // Liquid Concrete
                           IsActiveVoxels = true;
                           {
                             ZVoxelExtensionType_VoxelFluid * Ext;
                             ZVector3L VoxelCoords;

                             if (Test==0)
                             {
                               // printf(">");
                             }
                             Test++;

                             Ext = (ZVoxelExtensionType_VoxelFluid *)&Sector->OtherInfos[MainOffset];

                             VoxelCoords.x = RSx + x; VoxelCoords.y = RSy + y; VoxelCoords.z = RSz + z;

                             // If this voxel collection hasn't been processed at this cycle, do it.

                             if (Ext->Cycle == (CycleNum & 1)) VoxelFluid_ComputeVolumePressure(&VoxelCoords, VoxelType, CycleNum & 1 );


                             /*
                             ZVoxelSector * St[8];
                             UShort * Vp[8];
                             ULong SecondaryOffset[8];
                             UShort NeighborPressure[8], Pressure;
                             UShort NeighborDistance[8], Distance;
                             bool   CanGo[8];
                             ULong  PossibleDirections;
                             ULong  MaxPressure, MaxDistance;
                             ULong i,j;
                             UByte DirCode;
                             VoxelLocation Loc;
                             ZMemSize Info, Info2;

                             Long cx,cy,cz;

                             ZVector3L VoxelCoords; VoxelCoords.x = RSx + x; VoxelCoords.y = RSy + y; VoxelCoords.z = RSz + z;
                             if (VoxelCoords.x == 6 &&
                                 VoxelCoords.y == 0 &&
                                 VoxelCoords.z == 7)
                             {
                               printf("z");
                             }

                             // Extract field from the voxel info.
                             Info = Sector->OtherInfos[MainOffset];
                             Pressure = Info & 0xffff;
                             Distance = Info >> 16;

                             // Environnement pressure computing
                             PossibleDirections = 0;
                             MaxPressure = 1;
                             MaxDistance = 0;
                             bool isnonassigned;
                             for (i=0;i<6;i++)
                             {
                               cx = x+xbp6[i].x ; cy = y+xbp6[i].y ; cz = z+xbp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                               CanGo[i] = (Vp[i] == 0);
                               if (*Vp[i] == 235)
                               {
                                 Info2 = St[i]->OtherInfos[ SecondaryOffset[i] ];
                                 NeighborPressure[i] = Info2 & 0xffff;
                                 NeighborDistance[i] = Info2 >> 16;
                                 if (i==4) NeighborPressure[i] += 1; //
                                 if (i==5) NeighborPressure[i] -= 1;

                                 if ((NeighborPressure[i] >= Pressure) && (NeighborDistance[i]<=Distance || MaxDistance == 0) ) { MaxPressure = NeighborPressure[i]; MaxDistance = NeighborDistance[i]+1; }
                               }
                             }

                             Info  = ((ZMemSize)MaxPressure) & 0xFFFF;
                             Info |= ((ZMemSize)MaxDistance) << 16;
                             Sector->OtherInfos[MainOffset] = Info;



                             for(i=0,j=4, DirCode=0;i<4;i++,j++)
                             {
                               cx = x+bft[i].x ; cy = y+bft[i].y ; cz = z+bft[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                               Pressure = 0;
                               if (*Vp[i] == 235)
                               cx = x+bft[j].x ; cy = y+bft[j].y ; cz = z+bft[j].z ; SecondaryOffset[j] = If_x[cx]+If_y[cy]+If_z[cz];St[j] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[j] = &St[j]->Data[ SecondaryOffset[j] ];
                               DirCode = (DirCode << 1) | (1 & (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_CanBeReplacedBy_Water) & (VoxelTypeManager->VoxelTable[*Vp[j]]->Is_CanBeReplacedBy_Water) );
                             }
                             if (!DirCode) break;
                             DirCode = DirCode ^ (Random.GetNumber() & DirCode);
                             if (!DirCode) break;
                             DirCode = DirCodeTable[DirCode];

                             World->SetVoxel_WithCullingUpdate(RSx + x + bft[DirCode].x - 1 , RSy + y , RSz + z + bft[DirCode].z - 1, 202, ZVoxelSector::CHANGE_UNIMPORTANT, true, &Loc);
                             World->SetVoxel_WithCullingUpdate(RSx + x, RSy + y, RSz + z, 0, ZVoxelSector::CHANGE_UNIMPORTANT);
                             Loc.Sector->ModifTracker.Set(Loc.Offset);
                             // St[i]->ModifTracker.Set(SecondaryOffset[i]);

                              */
                           }
                           break;





                  case 244:  // Mover Right
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[2];
                            UShort * Vp[2];
                            ULong SecondaryOffset[2];

                            register Long cx,cy,cz;
                            ULong i;
                            Long Vx,Vy,Vz;
                            Vx = RSx+x; Vy = RSy+y; Vz = RSz+z;

                            if (CycleNum & 7) break;
                            i=0;
                            cx = x+1 ; cy = y+2 ; cz = z+1 ;
                            SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];
                            St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                            Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                            if (VoxelTypeManager->VoxelTable[*Vp[i]]->BvProp_MoveableByTreadmill && !St[i]->ModifTracker.Get(SecondaryOffset[i]))
                            {
                              i=1;

                              cx = x+1 ; cy = y+2 ; cz = z+2 ;
                              SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];
                              St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                              Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_PlayerCanPassThrough)
                              {
                                World->ExchangeVoxels(Vx,Vy+1,Vz,Vx,Vy+1,Vz+1,ZVoxelSector::CHANGE_CRITICAL, true);
                                /*
                                World->MoveVoxel(Vx,Vy+1,Vz,Vx,Vy+1,Vz+1,0,ZVoxelSector::CHANGE_CRITICAL);
                                St[0]->ModifTracker.Set(SecondaryOffset[0]);
                                St[1]->ModifTracker.Set(SecondaryOffset[1]);
                                */
                              }
                            }
                            break;
                          }
                  case 245:  // Mover Left
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[2];
                            UShort * Vp[2];
                            ULong SecondaryOffset[2];

                            register Long cx,cy,cz;
                            ULong i;
                            Long Vx,Vy,Vz;
                            Vx = RSx+x; Vy = RSy+y; Vz = RSz+z;

                            if (CycleNum & 7) break;
                            i=0;
                            cx = x+1 ; cy = y+2 ; cz = z+1 ;
                            SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];
                            St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                            Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                            if (VoxelTypeManager->VoxelTable[*Vp[i]]->BvProp_MoveableByTreadmill && !St[i]->ModifTracker.Get(SecondaryOffset[i]))
                            {
                              i=1;

                              cx = x+1 ; cy = y+2 ; cz = z ;
                              SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];
                              St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                              Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_PlayerCanPassThrough)
                              {
                                World->ExchangeVoxels(Vx,Vy+1,Vz,Vx,Vy+1,Vz-1,ZVoxelSector::CHANGE_CRITICAL, true);
/*
                                World->MoveVoxel(Vx,Vy+1,Vz,Vx,Vy+1,Vz-1,0,ZVoxelSector::CHANGE_CRITICAL);
                                St[0]->ModifTracker.Set(SecondaryOffset[0]);
                                St[1]->ModifTracker.Set(SecondaryOffset[1]);
*/
                              }
                            }
                            break;
                          }
                  case 246:  // Mover Top
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[2];
                            UShort * Vp[2];
                            ULong SecondaryOffset[2];

                            register Long cx,cy,cz;
                            ULong i;
                            Long Vx,Vy,Vz;
                            Vx = RSx+x; Vy = RSy+y; Vz = RSz+z;

                            if (CycleNum & 7) break;
                            i=0;
                            cx = x+1 ; cy = y+2 ; cz = z+1 ;
                            SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];
                            St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                            Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                            if (VoxelTypeManager->VoxelTable[*Vp[i]]->BvProp_MoveableByTreadmill && !St[i]->ModifTracker.Get(SecondaryOffset[i]))
                            {
                              i=1;

                              cx = x ; cy = y+2 ; cz = z+1 ;
                              SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];
                              St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                              Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_PlayerCanPassThrough)
                              {
                                World->ExchangeVoxels(Vx,Vy+1,Vz,Vx-1,Vy+1,Vz,ZVoxelSector::CHANGE_CRITICAL, true);
/*
                                World->MoveVoxel(Vx,Vy+1,Vz,Vx-1,Vy+1,Vz,0,ZVoxelSector::CHANGE_CRITICAL);
                                St[0]->ModifTracker.Set(SecondaryOffset[0]);
                                St[1]->ModifTracker.Set(SecondaryOffset[1]);
*/
                              }
                            }
                            break;
                          }
                  case 247:  // Mover Bottom
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[2];
                            UShort * Vp[2];
                            ULong SecondaryOffset[2];

                            register Long cx,cy,cz;
                            ULong i;
                            Long Vx,Vy,Vz;
                            Vx = RSx+x; Vy = RSy+y; Vz = RSz+z;

                            if (CycleNum & 7) break;
                            i=0;
                            cx = x+1 ; cy = y+2 ; cz = z+1 ;
                            SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];
                            St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                            Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                            if (VoxelTypeManager->VoxelTable[*Vp[i]]->BvProp_MoveableByTreadmill && !St[i]->ModifTracker.Get(SecondaryOffset[i]))
                            {
                              i=1;

                              cx = x+2 ; cy = y+2 ; cz = z+1 ;
                              SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];
                              St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ];
                              Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];

                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_PlayerCanPassThrough)
                              {
                                World->ExchangeVoxels(Vx,Vy+1,Vz,Vx+1,Vy+1,Vz,ZVoxelSector::CHANGE_CRITICAL, true);
/*
                                World->MoveVoxel(Vx,Vy+1,Vz,Vx+1,Vy+1,Vz,0,ZVoxelSector::CHANGE_CRITICAL);
                                St[0]->ModifTracker.Set(SecondaryOffset[0]);
                                St[1]->ModifTracker.Set(SecondaryOffset[1]); */
                              }
                            }
                            break;
                          }

                  case 248:  // Slow Voxel Materializer

                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[8];
                            UShort * Vp[8];
                            ULong SecondaryOffset[8];
                            ULong i, Opposite;
                            ZVoxelLocation VInfo, VInfo2;
                            UShort VoxelType2, VoxelType3;


                            register Long cx,cy,cz;

                            if (CycleNum & 15) break;
                            for(i=0;i<6;i++)
                            {
                              cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_Interface_PullBlock)
                              {
                                Opposite = BlocOpposite[i];
                                cx = x+bp6[Opposite].x ; cy = y+bp6[Opposite].y ; cz = z+bp6[Opposite].z ; SecondaryOffset[Opposite] = If_x[cx]+If_y[cy]+If_z[cz];St[Opposite] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[Opposite] = &St[Opposite]->Data[ SecondaryOffset[Opposite] ];
                                if (*Vp[Opposite]==0)
                                {
                                  VInfo.Sector = St[i];
                                  VInfo.Offset = SecondaryOffset[i];
                                  if (1==VoxelTypeManager->VoxelTable[ *Vp[i] ]->Interface_PushBlock_Pull(&VInfo, &VoxelType2 , 1 ))
                                  {
                                    World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x-1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z-1 , VoxelType2, ZVoxelSector::CHANGE_CRITICAL);
                                    St[Opposite]->ModifTracker.Set(SecondaryOffset[Opposite]);
                                    break;
                                  }

                                }
                                else if (*Vp[Opposite] == 249) // Is it a loader just in face.
                                {
                                  if (World->GetVoxelLocation(&VInfo2, RSx+x+bp6[Opposite].x+bp6[Opposite].x - 2 ,RSy+y+bp6[Opposite].y+bp6[Opposite].y - 2 ,RSz+z+bp6[Opposite].z+bp6[Opposite].z - 2))
                                  {
                                    VoxelType2 = VInfo2.Sector->Data[VInfo2.Offset];
                                    //VoxelType = World->GetVoxel( RSx+x+bp6[Opposite].x+bp6[Opposite].x - 2 ,RSy+y+bp6[Opposite].y+bp6[Opposite].y - 2 ,RSz+z+bp6[Opposite].z+bp6[Opposite].z - 2 );
                                    if (VoxelTypeManager->VoxelTable[VoxelType2]->Is_Interface_PushBlock)
                                    {
                                       VInfo.Sector = St[i];
                                       VInfo.Offset = SecondaryOffset[i];
                                       if (1==VoxelTypeManager->VoxelTable[ *Vp[i] ]->Interface_PushBlock_PullTest(&VInfo, &VoxelType3 , 1 )) // Test if we can pull a voxel and which type.
                                       {
                                         if (1==VoxelTypeManager->VoxelTable[VoxelType2]->Interface_PushBlock_Push(&VInfo2, VoxelType3, 1)) // Push it.
                                         {
                                           VoxelTypeManager->VoxelTable[ *Vp[i] ]->Interface_PushBlock_Pull(&VInfo, &VoxelType3 , 1 );      // If pushed ok, then pull it.
                                         }
                                       }
                                    }
                                  }
                                }

                              }
                            }

                            break;
                          }

                  case 249:  // Slow Voxel Dematerializer
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[8];
                            UShort * Vp[8];
                            ULong SecondaryOffset[8];
                            ULong i, Opposite;
                            ZVoxelLocation VInfo;


                            register Long cx,cy,cz;
                            if (CycleNum & 15) break;
                            for(i=0;i<6;i++)
                            {
                              cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_Interface_PushBlock)
                              {
                                Opposite = BlocOpposite[i];
                                cx = x+bp6[Opposite].x ; cy = y+bp6[Opposite].y ; cz = z+bp6[Opposite].z ; SecondaryOffset[Opposite] = If_x[cx]+If_y[cy]+If_z[cz];St[Opposite] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[Opposite] = &St[Opposite]->Data[ SecondaryOffset[Opposite] ];
                                if (VoxelTypeManager->VoxelTable[*Vp[Opposite]]->Is_Loadable_ByLoader_L1 && ( (!St[Opposite]->ModifTracker.Get(SecondaryOffset[Opposite]) || ( VoxelTypeManager->VoxelTable[*Vp[Opposite]]->BvProp_FastMoving) ) ))
                                {
                                  VInfo.Sector = St[i];
                                  VInfo.Offset = SecondaryOffset[i];
                                  if (1==VoxelTypeManager->VoxelTable[ *Vp[i] ]->Interface_PushBlock_Push(&VInfo, *Vp[ Opposite ], 1 ))
                                  {
                                    World->SetVoxel_WithCullingUpdate(RSx + x + bp6[Opposite].x-1, RSy + y + bp6[Opposite].y - 1, RSz + z + bp6[Opposite].z-1 , 0, ZVoxelSector::CHANGE_IMPORTANT);
                                    St[Opposite]->ModifTracker.Set(SecondaryOffset[Opposite]);
                                    break;
                                  }

                                }

                              }
                            }

                            break;
                          }





                 case 250:  // Slow Liquid Pump T0
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[8];
                            UShort * Vp[8];
                            ULong SecondaryOffset[8];
                            ULong i,j, Pumpable_Count, Pushable_Count;
                            ULong Pumpable[8], Pushable[8];
                            ZVoxelLocation VInfo;


                            register Long cx,cy,cz;

                            if (CycleNum & 31) break;
                            Pumpable_Count = Pushable_Count = 0;
                            for(i=0;i<6;i++)
                            {
                              cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_Pumpable_ByPump_T1)  { Pumpable[Pumpable_Count++] = i; }
                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_Interface_PushBlock) { Pushable[Pushable_Count++] = i; }
                            }

                            if (Pumpable_Count>0 && Pushable_Count>0)
                            {
                              j = (Random.GetNumber() % Pumpable_Count);
                              for (i=0;i<Pushable_Count;i++)
                              {
                                VInfo.Sector = St[ Pushable[i] ];
                                VInfo.Offset = SecondaryOffset[ Pushable[i] ];
                                if (1==VoxelTypeManager->VoxelTable[ *Vp[ Pushable[i]] ]->Interface_PushBlock_Push(&VInfo, *Vp[ Pumpable[j] ], 1 ))
                                {
                                  i = Pumpable[j];
                                  World->SetVoxel_WithCullingUpdate(RSx + x + bp6[i].x-1, RSy + y + bp6[i].y - 1, RSz + z + bp6[i].z-1 , 0, ZVoxelSector::CHANGE_IMPORTANT);
                                  St[i]->ModifTracker.Set(SecondaryOffset[i]);
                                  break;
                                }
                              }
                            }
                            break;
                          }

                 case 252:  // Mining Robot Super Six.
                          IsActiveVoxels = true;
                          {
                            ZVoxelSector * St[8];
                            UShort * Vp[8];
                            ULong SecondaryOffset[8];
                            ULong i,Pushable_Count;
                            ULong Pushable[8];
                            ZVoxelLocation VInfo;
                            UShort WonVoxelType;


                            register Long cx,cy,cz;

                            Pushable_Count = 0;

                            if (CycleNum & 3) break;

                            // Look around for all blocks having an interface for accepting blocks.

                            for(i=0;i<6;i++)
                            {
                              cx = x+bp6[i].x ; cy = y+bp6[i].y ; cz = z+bp6[i].z ; SecondaryOffset[i] = If_x[cx]+If_y[cy]+If_z[cz];St[i] = SectorTable[ Of_x[cx] + Of_y[cy] + Of_z[cz] ]; Vp[i] = &St[i]->Data[ SecondaryOffset[i] ];
                              if (VoxelTypeManager->VoxelTable[*Vp[i]]->Is_Interface_PushBlock) { Pushable[Pushable_Count++] = i; }
                            }

                            // Get VoxelType with computed probabilities.
                            WonVoxelType = RobotSC_Proba.GetVoxelType(RobotSC_Proba.GetTypeNum(Random.GetNumber()));

                            if (Pushable_Count>0)
                            {
                              for (i=0;i<Pushable_Count;i++)
                              {
                                VInfo.Sector = St[ Pushable[i] ];
                                VInfo.Offset = SecondaryOffset[ Pushable[i] ];
                                if (1==VoxelTypeManager->VoxelTable[ *Vp[ Pushable[i]] ]->Interface_PushBlock_Push(&VInfo, WonVoxelType , 1 ))
                                {
                                  break;
                                }
                              }
                            }
                            break;
                          }

                 case 254:  // Wireless Transmitter

                           // *** Active voxels mapped on voxeltype ***
                           //
                           // This was made to make modding easier to do.
                           //
                           // Unfortunately, that's also slower than adding a snippet of code in this switch.
                           // There is no fast access to neigbor voxels and a little overhead for parameters
                           // and function call.
                           // This method is perfectly suitable and the recommanded way for voxels that are only
                           // few in the world.
                           // For "massive" voxeltype presence where you want more speed, consider implementing it
                           // in the switch in this code file.


                   default:IsActiveVoxels = true;
                           ActiveVoxelInterface.Coords.x = RSx + x;
                           ActiveVoxelInterface.Coords.y = RSy + y;
                           ActiveVoxelInterface.Coords.z = RSz + z;
                           ActiveVoxelInterface.Location.Sector = Sector;
                           ActiveVoxelInterface.Location.Offset = VoxelP - DisplayData;
                           VoxelTypeManager->VoxelTable[VoxelType]->ActiveProcess(&ActiveVoxelInterface);
                           break;


                }
              } else { IsMovedVoxels = true; } // Voxel Have moved in this sector, so don't stop activity in it.
            }

            VoxelP++; MainOffset++;
          }


      // Update sector activity flag but don't deactivate if there is movement in this sector.

      Sector->Flag_IsActiveVoxels = IsActiveVoxels || (Sector->Flag_IsActiveVoxels && IsMovedVoxels); // If they are no active voxels in this sector, disable active voxel processing for it.
      Sector->Flag_IsActiveLowRefresh = IsLowActivityVoxels || (Sector->Flag_IsActiveLowRefresh && IsMovedVoxels); // If they are no low activity voxels in this sectir disable low activity processing.
      Sector->Flag_IsDebug        = IsDebug;
    }
    Sector = Sector->GlobalList_Next;
  }
}


