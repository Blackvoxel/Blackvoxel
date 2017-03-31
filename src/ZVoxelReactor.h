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
#ifndef Z_ZVOXELREACTOR_H
#define Z_ZVOXELREACTOR_H

//#ifndef Z_ZVOXELREACTOR_H
//#  include "ZVoxelReactor.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZVOXELSECTOR_H
#  include "ZVoxelSector.h"
#endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

#ifndef Z_ZWORLD_H
#  include "ZWorld.h"
#endif

#ifndef Z_ZVOXELTYPEMANAGER_H
#  include "ZVoxelTypeManager.h"
#endif

#ifndef Z_ZFASTBIT_ARRAY_H
#  include "z/ZFastBit_Array.h"
#endif

#ifndef Z_ZFASTRANDOM_H
#  include "z/ZFastRandom.h"
#endif

#ifndef Z_ZVOXELREACTION_H
#  include "ZVoxelReaction.h"
#endif

#ifndef Z_VOXELEXTENSION_FABMACHINE_H
#  include "ZVoxelExtension_FabMachine.h"
#endif

#ifndef Z_ZFABMACHINEINFOS_H
#  include "ZFabMachineInfos.h"
#endif

#ifndef Z_ZVOXELEXTENSION_PROGRAMMABLE_H
#  include "ZVoxelExtension_Programmable.h"
#endif

#ifndef Z_ZEGMYTARGETMANAGER_H
#  include "ZEgmyTargetManager.h"
#endif

#ifndef Z_ZINCLUSIONPROBABILIZER_H
#  include "ZInclusionProbabilizer.h"
#endif


class ZGame;

struct ZonePressure;

class ZVoxelReactor
{
  protected:
    ZLightSpeedRandom Random;
    ZVector3d PlayerPosition;
    ZGame * GameEnv;
    ZVoxelWorld * World;
    ZVoxelTypeManager * VoxelTypeManager;
    ZEgmyTargetManager    EgmyWaveManager;
    ZInclusionProbabilizer RobotSC_Proba;
    ULong               CycleNum;

    ZVoxelSector * DummySector;

    ZVoxelReaction ** ReactionTable;

  public:
    class ZBlocPos { public: UByte x; UByte y; UByte z; };
    class ZBlocPosN{ public: Byte x;  Byte y;  Byte z; };

    static ZBlocPos  bp6[6];   // Bloc positions with 6 slots around main cube.
    static ZBlocPos  bft[8];   // Bloc fall test positions with 4 slots around and 4 slots under;
    static ZBlocPos  bft6[10]; // Bloc fall test positions with 6 slots around main cube and 4 slots under (Special case for acid).
    static UByte     BlocOpposite[6];
    static ZBlocPosN nbp6[6];
    static ZBlocPos  xbp6[6];  // Bloc positions with 6 slots around main cube. ( New standardised robot order.).
    static ZBlocPosN xbp6_nc[6];// same as xbp6 with -1,+1 range

  protected:

    // Fast computing offsets;

    UByte Of_x[ZVOXELBLOCSIZE_X+2];
    UByte Of_y[ZVOXELBLOCSIZE_Y+2];
    UByte Of_z[ZVOXELBLOCSIZE_Z+2];
    ULong If_x[ZVOXELBLOCSIZE_X+2];
    ULong If_y[ZVOXELBLOCSIZE_Y+2];
    ULong If_z[ZVOXELBLOCSIZE_Z+2];

    // DirCodes

    static UByte DirCodeTable[16];


    ULong * ActiveTable;


// Time remaining on FireMine action

    ULong FireMineTime;


  public:
    void Init(ZGame * GameEnv);


    ZVoxelReactor();
    ~ZVoxelReactor();


    void UpdatePlayerPosition(ZVector3d * PlayerPosition)
    {
      this->PlayerPosition = *PlayerPosition;
    }



    void LightTransmitter_FindEndPoints(ZVector3L * Location, ZVector3L * NewCommingDirection);
    void LightTransmitter_FollowTransmitter(ZVector3L * Location, ZVector3L * FollowingDirection);
    bool VoxelFluid_ComputeVolumePressure(ZVector3L * Location, UShort VoxelType, bool EvenCycle);
    void VoxelFluid_ComputeVolumePressure_Recurse(ZVector3L * Location, ZonePressure * Pr  );
    void VoxelFluid_SetVolumePressure_Recurse(ZVector3L * Location, ZonePressure * Pr  );
    void ProcessSectors( double LastLoopTime );


};


#endif /* Z_ZVOXELREACTOR_H */
