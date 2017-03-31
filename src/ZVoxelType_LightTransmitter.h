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
 * ZVoxelType_LightTransmitter.h
 *
 *  Created on: 1 déc. 2012
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPE_LIGHTTRANSMITTER_H
#define Z_ZVOXELTYPE_LIGHTTRANSMITTER_H

//#ifndef Z_ZVOXELTYPE_LIGHTTRANSMITTER_H
//#  include "ZVoxelType_LightTransmitter.h"
//#endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

#ifndef Z_ZGENERICTABLE_H
#  include "z/ZGenericTable.h"
#endif

class ZVoxelType_LightTransmitter : public ZVoxelType
{
    class  ZBlocPosN { public: Byte x;  Byte y;  Byte z; };
    static ZBlocPosN nbp6[6];

    enum {

      MaxDestPoints = 128,
      MaxNodePoints = 128
    };

    class ZTransmitterContext
    {
      public:
        ZGenericTable<ZVector3L, MaxDestPoints > EndPointTable;
        ZGenericTable<ZVector3L, MaxNodePoints > NodeTable;
    };

  public:
    ZVoxelType_LightTransmitter(ULong VoxelType) : ZVoxelType(VoxelType)
    {
      Is_Interface_PushBlock = true;
    }

    void LightTransmitter_FindEndPoints(ZVector3L * Location    , ZVector3L * CommingDirection,   ZTransmitterContext * Context );
    void LightTransmitter_FollowTransmitter(ZVector3L * Location, ZVector3L * FollowingDirection, ZTransmitterContext * Context );

    virtual ULong  Interface_PushBlock_Push    ( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count );
    virtual ULong  Interface_PushBlock_PushTest( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count );

};

#endif /* ZVOXELTYPE_LIGHTTRANSMITTER_H_ */
