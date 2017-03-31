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
 * ZVoxelExtension_MiningRobot_xr1.cpp
 *
 *  Created on: 23 févr. 2013
 *      Author: laurent
 */

#include <stdio.h>

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

#ifndef Z_ZVOXELEXTENSION_MININGROBOT_XR1_H
#  include "ZVoxelExtension_MiningRobot_xr1.h"
#endif

void ZVoxelExtension_MiningRobot_xr1::DebugOut()
{
  printf("--------------------------------ROBOT INFORMATIONS-------------------------\n");
  printf("State.............: %d\n", State);
  printf("StorageLocation...: %d,%d,%d\n", StorageLocation.x, StorageLocation.y, StorageLocation.z);
  printf("Mine_InitialCount.: %d,%d,%d\n", Mine_InitialCount.x, Mine_InitialCount.y, Mine_InitialCount.z);
  printf("Mine_Count........: %d,%d,%d\n", Mine_Count.x, Mine_Count.y, Mine_Count.z);
  printf("Dir...............: %d,%d,%d\n", Dir.x, Dir.y, Dir.z);
  printf("CycleSkip.........: %d\n", CycleSkip);
  printf("SkipCounter.......: %d\n", SkipCounter);
  printf("RobotLevel........: %d\n", RobotLevel);
}

bool ZVoxelExtension_MiningRobot_xr1::Save(ZStream_SpecialRamStream * Stream)
{
  ULong * ExtensionSize;
  ULong   StartLen;

  //DebugOut();

  ExtensionSize = Stream->GetPointer_ULong();
  Stream->Put(0u);       // The size of the extension (defered storage).
  StartLen = Stream->GetActualBufferLen();
  Stream->Put((UShort)2); // Extension Version;

  // Storage informations.

  Stream->PutZone(&VoxelType,sizeof(VoxelType));
  Stream->PutZone(&VoxelQuantity,sizeof(VoxelQuantity));

  Stream->Put(State);
  Stream->Put(StorageLocation);
  Stream->Put(Mine_InitialCount);
  Stream->Put(Mine_Count);
  Stream->Put(Dir);
  Stream->Put(CycleSkip);
  Stream->Put(StorePerCycle);
  Stream->Put(SkipCounter);
  Stream->Put(RobotLevel);

  *ExtensionSize = Stream->GetActualBufferLen() - StartLen;

  return(true);
}

bool ZVoxelExtension_MiningRobot_xr1::Load(ZStream_SpecialRamStream * Stream)
{
  bool Ok;
  ULong  ExtensionSize;
  UShort ExtensionVersion;
  UByte  Temp_Byte;



  Ok = Stream->Get(ExtensionSize);
  Ok&= Stream->Get(ExtensionVersion);     if(!Ok) return(false);

  // Check for supported extension version. If unsupported new version, throw content and continue with a blank extension.

  if (ExtensionVersion>2) { ExtensionSize-=2; for(ZMemSize i=0;i<ExtensionSize;i++) Ok = Stream->Get(Temp_Byte); if (Ok) return(true); else return(false);}

  //Stream->Get(Quantity_Carbon);
  Stream->GetZone(&VoxelType, sizeof(VoxelType));
  Stream->GetZone(&VoxelQuantity, sizeof(VoxelQuantity));

  Stream->Get(State);
  Stream->Get(StorageLocation);
  Stream->Get(Mine_InitialCount);
  Stream->Get(Mine_Count);
  Stream->Get(Dir);
  Stream->Get(CycleSkip);
  if (ExtensionVersion>1) Stream->Get(StorePerCycle);
  Stream->Get(SkipCounter);
  Stream->Get(RobotLevel);
  //DebugOut();

  return(Ok);
}

void ZVoxelExtension_MiningRobot_xr1::Robot_Init( ZVector3L * StorageLocation,
                 ULong       DirectionCode,
                 Long HoleWidth_x,
                 Long HoleWidth_z,
                 Long HoleDeep_y,
                 Long CycleSkip ,
                 ULong StorePerCycle,
                 ULong RobotLevel )
{
  this->StorageLocation = *StorageLocation;
  Dir = 0;
  switch (DirectionCode)
  {
    case 0: Dir.x = -1; Dir.z = -1; break;
    case 1: Dir.x = -1; Dir.z =  1; break;
    case 2: Dir.x =  1; Dir.z =  1; break;
    case 3: Dir.x =  1; Dir.z = -1; break;
  }
  Dir.y = -1;
  State = 1;
  Mine_InitialCount.x = HoleWidth_x;
  Mine_InitialCount.y = HoleDeep_y;
  Mine_InitialCount.z = HoleWidth_z;
  Mine_Count.y = Mine_InitialCount.y;
  this->CycleSkip = CycleSkip;
  this->SkipCounter = 0;
  this->StorePerCycle = StorePerCycle;
  this->RobotLevel = RobotLevel;
}

void ZVoxelExtension_MiningRobot_xr1::Robot_Move( ZVector3L * Pos, ZGame * GameEnv )
{
  ZVoxelWorld * World = GameEnv->World;
  UShort VoxelType, VoxelType2;
  ULong LoopCount, Loop;
  ZVector3L NewPos;
  bool Redo;


  if (State != 5)
  {
    if ( SkipCounter < CycleSkip ) { SkipCounter++; return; }
    SkipCounter = 0;
  }

  LoopCount = 1;
  if (CycleSkip<0) LoopCount = - CycleSkip;
  for (Loop = 0; Loop<LoopCount; Loop++)
  do
  {
    Redo = false;
    switch (State)
    {
      case 0: break;

      case 1: // Going Down
              if (Mine_Count.y <1) {State = 4; Mine_Count.y = Mine_InitialCount.y; Redo = true;  break; }
              NewPos = *Pos; NewPos.y -= 1;
              VoxelType = World->GetVoxel(NewPos.x, NewPos.y , NewPos.z );
              if (VoxelType == 655535) break;
              if (VoxelType!=0)
              {
                if (GameEnv->VoxelTypeManager.VoxelTable[VoxelType]->BvProp_XrRobotPickMinLevel > RobotLevel) break;
                if (!StoreBlocks(VoxelType, 1)) break;
              }
              World->MoveVoxel_Sm( Pos->x , Pos->y, Pos->z , NewPos.x, NewPos.y, NewPos.z, 0, ZVoxelSector::CHANGE_UNIMPORTANT);
              *Pos = NewPos;
              Mine_Count.x = Mine_InitialCount.x;
              Mine_Count.z = Mine_InitialCount.z;
              Mine_Count.y --;
              State = 2;
              break;
      case 2: // Going x+
              if (Mine_Count.x <= 1) {State = 3; Dir.x = -Dir.x; Redo = true; break;}
              NewPos = *Pos; NewPos.x += Dir.x;
              VoxelType = World->GetVoxel(NewPos.x, NewPos.y , NewPos.z );
              if (VoxelType == 655535) break;
              if (VoxelType!=0)
              {
                if (GameEnv->VoxelTypeManager.VoxelTable[VoxelType]->BvProp_XrRobotPickMinLevel > RobotLevel) break;
                if (!StoreBlocks(VoxelType, 1)) break;
              }
              World->MoveVoxel_Sm( Pos->x , Pos->y, Pos->z , NewPos.x, NewPos.y, NewPos.z, 0, ZVoxelSector::CHANGE_UNIMPORTANT);
              *Pos = NewPos;
              Mine_Count.x--;
              break;
      case 3: // Going z+
              if (Mine_Count.z <= 1) {State = 1; Dir.z = -Dir.z; Redo = true; break;}
              NewPos = *Pos; NewPos.z += Dir.z;
              VoxelType = World->GetVoxel(NewPos.x, NewPos.y , NewPos.z );
              if (VoxelType == 655535) break;
              if (VoxelType!=0)
              {
                if (GameEnv->VoxelTypeManager.VoxelTable[VoxelType]->BvProp_XrRobotPickMinLevel > RobotLevel) break;
                if (!StoreBlocks(VoxelType, 1)) break;
              }
              World->MoveVoxel_Sm( Pos->x , Pos->y, Pos->z , NewPos.x, NewPos.y, NewPos.z, 0, ZVoxelSector::CHANGE_UNIMPORTANT);
              *Pos = NewPos;
              Mine_Count.z--;
              Mine_Count.x = Mine_InitialCount.x;
              State = 2;
              break;
      case 4: // Going UP to the storage.
              if (Mine_Count.y < 1)
              {
                ZVoxelLocation Loc;
                World->GetVoxelLocation( &Loc, Pos->x, Pos->y,Pos->z);
                Loc.Sector->Data[Loc.Offset] = 159;
                Loc.Sector->Flag_Render_Dirty = true;
                //Loc.Sector->Flag_Void_Regular = false;
                //Loc.Sector->Flag_Void_Transparent = false;
                State = 5; break;
              }
              NewPos = *Pos; NewPos.y += 1;
              VoxelType = World->GetVoxel(NewPos.x, NewPos.y , NewPos.z );
              if (VoxelType == 655535) break;
              if (VoxelType!=0)
              {
                if (GameEnv->VoxelTypeManager.VoxelTable[VoxelType]->BvProp_XrRobotPickMinLevel > RobotLevel) break;
                if (!StoreBlocks(VoxelType, 1)) break;
              }
              World->MoveVoxel_Sm( Pos->x , Pos->y, Pos->z , NewPos.x, NewPos.y, NewPos.z, 0, ZVoxelSector::CHANGE_UNIMPORTANT);
              *Pos = NewPos;
              Mine_Count.y--;
              break;
      case 5: // World->SetVoxel_WithCullingUpdate(Pos->x, Pos->y,Pos->z, 0, true, true, 0); break;
              // Putting data into storage.
              {
                ZVoxelLocation Loc;
                ULong Slot;
                ULong StoreCount;
                if (!World->GetVoxelLocation(&Loc, StorageLocation.x, StorageLocation.y, StorageLocation.z)) break;
                VoxelType = Loc.Sector->Data[Loc.Offset];
                if (!GameEnv->VoxelTypeManager.VoxelTable[VoxelType]->Is_Interface_PushBlock) { State = 6; break; }

                if ( ((ULong)-1) != (Slot = FindFirstUsedBlock()))
                {
                  StoreCount = this->StorePerCycle;
                  if (VoxelQuantity[Slot]<StoreCount) StoreCount = VoxelQuantity[Slot];
                  if (!GameEnv->VoxelTypeManager.VoxelTable[VoxelType]->Interface_PushBlock_Push(&Loc, this->VoxelType[Slot], StoreCount)) break;
                  VoxelQuantity[Slot] -=StoreCount;
                  if ( ! VoxelQuantity[Slot] ) this->VoxelType[Slot]=0;
                }
                else
                {
                  switch (RobotLevel)
                  {
                    default:
                    case 0:
                    case 1: VoxelType2 = 153; break;
                    case 2: VoxelType2 = 154; break;
                    case 3: VoxelType2 = 155; break;
                    case 4: VoxelType2 = 156; break;
                    case 5: VoxelType2 = 157; break;
                    case 6: VoxelType2 = 158; break;
                  }
                  if (!GameEnv->VoxelTypeManager.VoxelTable[VoxelType]->Interface_PushBlock_Push(&Loc, VoxelType2, 1)) break;
                  World->SetVoxel_WithCullingUpdate(Pos->x, Pos->y,Pos->z, 0, ZVoxelSector::CHANGE_UNIMPORTANT, true, 0);
                  // § State = 32;
                  return;
                }
              }
              break;
      case 6:
              {
                ULong i, Slot;
                ZVector3L NewLocation;

                for (i=0;i<6;i++)
                {
                  NewLocation.x = Pos->x + ZVoxelReactor::xbp6_nc[i].x; NewLocation.y = Pos->y + ZVoxelReactor::xbp6_nc[i].y; NewLocation.z = Pos->z + ZVoxelReactor::xbp6_nc[i].z;

                  VoxelType = World->GetVoxel(&NewLocation);
                  if (GameEnv->VoxelTypeManager.VoxelTable[VoxelType]->Is_Interface_PushBlock)
                  {
                    // Found a new location to push our stuff
                    StorageLocation = NewLocation;
                    State = 5;
                    break;
                  }
                }
                // Nobody want our blocks ? So, wait someone to empty it then transform to the original state.
                if ( ((ULong)-1) == (Slot = FindFirstUsedBlock()))
                {
                  switch (RobotLevel)
                  {
                    default:
                    case 0:
                    case 1: VoxelType = 153; break;
                    case 2: VoxelType = 154; break;
                    case 3: VoxelType = 155; break;
                    case 4: VoxelType = 156; break;
                    case 5: VoxelType = 157; break;
                    case 6: VoxelType = 158; break;
                  }
                  World->SetVoxel_WithCullingUpdate(Pos->x, Pos->y,Pos->z, VoxelType, true, true, 0);
                }

              }
              break;

      default: break;
    }
  } while (Redo);

}



