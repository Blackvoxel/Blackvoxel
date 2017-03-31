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
 * ZVoxelType_WirelessReceiver.h
 *
 *  Created on: 31 déc. 2016
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPE_WIRELESSRECEIVER_H
#define Z_ZVOXELTYPE_WIRELESSRECEIVER_H

//#ifndef Z_ZVOXELTYPE_WIRELESSRECEIVER_H
//#  include "ZVoxelType_WirelessReceiver.h"
//#endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

#ifndef Z_ZGENERICTABLE_H
#  include "z/ZGenericTable.h"
#endif

class ZVoxelType_WirelessReceiver : public ZVoxelType
{


  public:
    ZVoxelType_WirelessReceiver(ULong VoxelType) : ZVoxelType(VoxelType)
    {

    }

    virtual bool   Interface_SetPointingLocation(ZVoxelLocation * VLoc, ZToolsPointer * ToolsPointer, ULong Slot, ZVector3L * Location, ZString * OutMessage );
    virtual ULong  Interface_PushBlock_Push    ( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count );
    virtual ULong  Interface_PushBlock_PushTest( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count );

};





#endif /* Z_ZVOXELTYPE_WIRELESSRECEIVER_H */
