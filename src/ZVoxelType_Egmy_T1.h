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
 * ZVoxelType_Egmy_T1.h
 *
 *  Created on: 23 sept. 2013
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPE_EGMY_T1_H
#define Z_ZVOXELTYPE_EGMY_T1_H

//#ifndef Z_ZVOXELTYPE_EGMY_T1_H
//#  include "ZVoxelType_Egmy_T1.h"
//#endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

class ZVoxelType_Egmy_T1 : public ZVoxelType
{
  public:
    ZVoxelType_Egmy_T1(ULong VoxelType) : ZVoxelType(VoxelType)
    {
      Is_VoxelExtension = true;
      Is_HasAllocatedMemoryExtension = true;
      Is_Interface_PushBlock = false;
      Is_Interface_StoreBlock = false;
      Is_Interface_PullBlock  = false;
      Is_HasHelpingMessage = true;
      HelpingMessage = "IT'S BETTER TO RUN AWAY";
    }

    // Voxel Extensions

    virtual ZVoxelExtension * CreateVoxelExtension(bool IsLoadingPhase = true);
    virtual void  GetBlockInformations(ZVoxelLocation * DestLocation, ZString & Infos);
};


#endif /* Z_ZVOXELTYPE_EGMY_T1_H */
