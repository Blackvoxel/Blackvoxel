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
 * ZDestructionProgress.h
 *
 *  Created on: 18 nov. 2017
 *      Author: laurent
 */

#ifndef Z_ZDESTRUCTIONPROGRESS_H
#define Z_ZDESTRUCTIONPROGRESS_H

//#ifndef Z_ZDESTRUCTIONPROGRESS_H
//#  include "ZDestructionProgress.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

class ZGame;

class ZDestructionProgress
{
  protected:
    ZGame *      GameEnv;
    bool         MiningInProgress;
    bool         MiningNoTarget;
    ZVoxelCoords MinedVoxel;
    double       Mining_Hardness;
    double       Mining_MaterialResistanceCounter;
    void *       SoundHandle;
  public:
    ZDestructionProgress();
    void SetGameEnv(ZGame * GameEnv) {this->GameEnv = GameEnv;}
    void Start(double MiningHardness, ZVoxelCoords & PointedVoxel);
    bool DoMine(double Amount);
    void Stop();
    void SetNewtarget(double MiningHardness, ZVoxelCoords & PointedVoxel);
    void SetNoTarget();
    ZVoxelCoords & GetMinedVoxel() {return(MinedVoxel);}
    bool Is_InProgress() {return(MiningInProgress);}
};



#endif /* Z_ZDESTRUCTIONPROGRESS_H */
