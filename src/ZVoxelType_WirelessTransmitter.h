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
 * ZVoxelType_WirelessTransmitter.h
 *
 *  Created on: 29 déc. 2016
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPE_WIRELESSTRANSMITTER_H
#define Z_ZVOXELTYPE_WIRELESSTRANSMITTER_H

//#ifndef Z_ZVOXELTYPE_WIRELESSTRANSMITTER_H
//#  include "ZVoxelType_WirelessTransmitter.h"
//#endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

#ifndef Z_ZGENERICTABLE_H
#  include "z/ZGenericTable.h"
#endif

class ZVoxelType_WirelessTransmitter : public ZVoxelType
{
    class  ZBlocPosN { public: Byte x;  Byte y;  Byte z; };
    static ZBlocPosN nbp6[6];

/*
    enum {

      MaxDestPoints = 128,
      MaxNodePoints = 128
    };

    class ZTransmitterContext
    {
      public:
        ZGenericTable<ZVector3L, MaxDestPoints > EndPointTable;
    };
*/

  public:
    ZVoxelType_WirelessTransmitter(ULong VoxelType) : ZVoxelType(VoxelType)
    {
      Is_VoxelExtension = true;
      Is_HasAllocatedMemoryExtension = true;
      Is_Interface_PushBlock = true;
      Is_Interface_SetLocation = true;
      Is_Interface_PushBlock = true;
    }
    virtual ZVoxelExtension * CreateVoxelExtension(bool IsLoadingPhase = true);
    virtual bool   Interface_SetPointingLocation(ZVoxelLocation * VLoc, ZToolsPointer * ToolsPointer, ULong Slot, ZVector3L * Location, ZString * OutMessage );
    virtual ULong  Interface_PushBlock_Push    ( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count );
    virtual ULong  Interface_PushBlock_PushTest( ZVoxelLocation * DestLocation, UShort VoxelType, ULong Count );
    virtual void   GetScanInformations(ZVoxelCoords * VoxelCoords, UShort VoxelType, ZMemSize VoxelInfo, ZString & Infos);

};



#endif /* Z_ZVOXELTYPE_WIRELESSTRANSMITTER_H */
