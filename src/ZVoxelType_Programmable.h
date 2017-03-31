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
 * ZVoxelType_Programmable.h
 *
 *  Created on: 28 déc. 2012
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPE_PROGRAMMABLE_H
#define Z_ZVOXELTYPE_PROGRAMMABLE_H

//#ifndef Z_ZVOXELTYPE_PROGRAMMABLE_H
//#  include "ZVoxelType_Programmable.h"
//#endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

class ZVoxelType_Programmable : public ZVoxelType
{
  public:
    ZVoxelType_Programmable(ULong VoxelType) : ZVoxelType(VoxelType)
    {
      Is_VoxelExtension = true;
      Is_HasAllocatedMemoryExtension = true;
      Is_Interface_PushBlock = true;
      Is_Interface_StoreBlock = true;
      Is_Interface_PullBlock  = true;
      Is_HasHelpingMessage = false;
      HelpingMessage = "PROGRAMMABLE VOXEL : PRESS THE MIDDLE MOUSE BUTTON (SCROLLWHEEL) TO USE";
    }

    // Voxel Extensions

    virtual ZVoxelExtension * CreateVoxelExtension(bool IsLoadingPhase = true);
    virtual void  DeleteVoxelExtension(ZMemSize VoxelExtension, bool IsUnloadingPhase = false);


    // The user click the activation button(Middle) on the voxel to activate special functions.
    virtual void  UserAction_Activate(ZMemSize VoxelInfo, Long x, Long y, Long z);

    virtual ULong  Interface_PushBlock_Push    ( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count );
    virtual ULong  Interface_PushBlock_Pull    ( ZVoxelLocation * DestLocation,  UShort * VoxelType, ULong Count );
    virtual ULong  Interface_PushBlock_PullTest( ZVoxelLocation * DestLocation,  UShort * VoxelType, ULong Count );

};


#endif /* Z_ZVOXELTYPE_PROGRAMMABLE_H */
