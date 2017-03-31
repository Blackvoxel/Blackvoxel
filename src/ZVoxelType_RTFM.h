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
 * ZVoxelType_RTFM.h
 *
 *  Created on: 16 nov. 2013
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPE_RTFM_H
#define Z_ZVOXELTYPE_RTFM_H

// #ifndef Z_ZVOXELTYPE_RTFM_H
// #  include "ZVoxelType_RTFM.h"
// #endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

class ZVoxelType_RTFM : public ZVoxelType
{
  public:
    ZVoxelType_RTFM(ULong VoxelType = 0) : ZVoxelType(VoxelType)
    {
      Is_HasHelpingMessage = true;
      HelpingMessage = "RTFM : YOU SHOULD READ THE GAME MANUAL ON THE WEBSITE";
    }

    virtual void Event_Start_Selected(ZVoxelLocation * Loc, ZVector3L * Coords);
    virtual void Event_Is_Selected(ZVoxelLocation * Loc, ZVector3L * Coords);
    virtual void Event_End_Selected(ZVoxelLocation * Loc, ZVector3L * Coords);
};



#endif /* Z_ZVOXELTYPE_RTFM_H */
