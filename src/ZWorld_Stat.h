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
 * ZWorld_Stat.h
 *
 *  Created on: 11 févr. 2013
 *      Author: laurent
 */

#ifndef Z_ZWORLD_STAT_H
#define Z_ZWORLD_STAT_H

//#ifndef Z_ZWORLD_STAT_H
//#  include "ZWorld_Stat.h"
//#endif

#ifndef Z_ZWORLD_H
#  include "ZWorld.h"
#endif

class ZWorld_Stat
{
  protected:
    ZVoxelWorld * World;

  public:
    ZWorld_Stat()
    {
      World = 0;
    }

    void SetWorld( ZVoxelWorld * World ) { this->World = World; }

    void Get_Stat_DistanceToMineForVoxelType(UShort VoxelType);


};





#endif /* Z_ZWORLD_STAT_H */
