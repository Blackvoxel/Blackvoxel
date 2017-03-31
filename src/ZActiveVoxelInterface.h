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
 * ZVoxelModInterface.h
 *
 *  Created on: 9 nov. 2014
 *      Author: laurent
 */

#ifndef Z_ZACTIVEVOXELINTERFACE_H
#define Z_ZACTIVEVOXELINTERFACE_H

//#ifndef Z_ZACTIVEVOXELINTERFACE_H
//#  include "ZActiveVoxelInterface.h"
//#endif

#ifndef Z_ZWORLD_H
#  include "ZWorld.h"
#endif

#ifndef Z_ZVOXELTYPEMANAGER_H
#  include "ZVoxelTypeManager.h"
#endif

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif


class ZActiveVoxelInterface
{
  public:

    enum ZChangeImportance
    {
      CHANGE_DISCARDABLE = 3, // Changes aren't important at all and MUST be discarded at saving.
      CHANGE_UNIMPORTANT = 5, // Changes aren't important BUT MUST be saved anyway. Can be discarded if a new game version change zone disposition.
      CHANGE_IMPORTANT   = 9, // Changes are important and bust be preserved.
      CHANGE_CRITICAL    = 17 // Critical changes : work done directly by the player. Must be preserved at all cost.
    };
    struct ZBlocPosN{ Byte x,y,z; } * _xbp;

    ZVector3L         Coords;
    ZVoxelLocation     Location;
    ZGame             * GameEnv;
    ZVoxelWorld       * World;
    ZVoxelTypeManager * VoxelTypeManager;

    inline UShort              GetVoxelType_Main();
    inline UShort              GetVoxelType(ZVoxelLocation * VoxelLocation);
    inline ZVoxelExtension *   GetVoxelExtension_Main();
    inline ZVoxelExtension *   GetVoxelExtension( ZVoxelLocation * VoxLocation ) { return((ZVoxelExtension *)VoxLocation->Sector->OtherInfos[VoxLocation->Offset]);};
    inline bool                MoveThis( int x, int y, int z, ZChangeImportance ChangeImportance);
    inline bool                MoveThis_Abs( ZVector3L * Destination, ZChangeImportance ChangeImportance );
    inline bool                MoveVoxel( ZVector3L * Source, ZVector3L * Destination, ZChangeImportance ChangeImportance = ZActiveVoxelInterface::CHANGE_CRITICAL );
    inline bool                ExchangeVoxels(ZVector3L * Voxel1, ZVector3L * Voxel2, ZChangeImportance ChangeImportance);
    inline bool                SetVoxel( ZVector3L * Coords, UShort VoxelType, ZChangeImportance ChangeImportance );
    inline bool                SetVoxelExt( ZVector3L * Coords, UShort VoxelType, ZChangeImportance ChangeImportance, ZVoxelLocation * VoxLocation );
    inline UShort              GetVoxel( ZVector3L * Coords );
    inline bool                GetVoxelExt_Abs( ZVector3L * Coords, UShort & VoxelType, ZVoxelLocation * VoxLocation );
    inline bool                GetVoxelExt( Long x, Long y, Long z , UShort & VoxelType, ZVoxelLocation * VoxLocation );
    inline bool                GetVoxelPointers( ZVector3L * Coords, ZVoxelLocation * VoxelLocation)                         { return(World->GetVoxelLocation(VoxelLocation, Coords)); }
    inline bool                GetNeighborVoxel(ULong PositionCode, ZVoxelLocation * VoxelLocation );
};

inline bool  ZActiveVoxelInterface::MoveThis_Abs( ZVector3L * Destination, ZChangeImportance ChangeImportance )
{
  return( World->MoveVoxel_Sm(&Coords, Destination, 0, (UByte) ChangeImportance) );
}

inline bool ZActiveVoxelInterface::MoveThis( int x, int y, int z, ZChangeImportance ChangeImportance)
{
  ZVector3L Destination;

  Destination = Coords;
  Destination.x += x; Destination.y += y; Destination.z += z;

  return( World->MoveVoxel_Sm(&Coords, &Destination, 0, (UByte) ChangeImportance ) );
}

inline bool ZActiveVoxelInterface::MoveVoxel( ZVector3L * Source, ZVector3L * Destination, ZChangeImportance ChangeImportance )
{
  return(World->MoveVoxel_Sm(Source, Destination, 0, (UByte) ChangeImportance  ));
}

inline UShort ZActiveVoxelInterface::GetVoxelType_Main()
{
  return(Location.Sector->Data[Location.Offset]);
}

inline UShort ZActiveVoxelInterface::GetVoxelType(ZVoxelLocation * VoxelLocation)
{
  return(VoxelLocation->Sector->Data[VoxelLocation->Offset]);
}

inline ZVoxelExtension * ZActiveVoxelInterface::GetVoxelExtension_Main()
{
  return((ZVoxelExtension *)Location.Sector->OtherInfos[Location.Offset]);
}

inline bool ZActiveVoxelInterface::SetVoxel( ZVector3L * Coords, UShort VoxelType, ZChangeImportance ChangeImportance )
{
  ZVoxelLocation     VoxLocation;
  if (!World->SetVoxel_WithCullingUpdate( Coords->x, Coords->y, Coords->z, VoxelType, (UByte) ChangeImportance, true, &VoxLocation ) ) return(false);
  VoxLocation.Sector->ModifTracker.Set(VoxLocation.Offset);
  return(true);
}

inline bool ZActiveVoxelInterface::SetVoxelExt( ZVector3L * Coords, UShort VoxelType, ZChangeImportance ChangeImportance, ZVoxelLocation * VoxLocation )
{
  if (!World->SetVoxel_WithCullingUpdate( Coords->x, Coords->y, Coords->z, VoxelType, (UByte) ChangeImportance, true, VoxLocation ) ) return(false);
  VoxLocation->Sector->ModifTracker.Set(VoxLocation->Offset);
  return(true);
}

inline UShort ZActiveVoxelInterface::GetVoxel( ZVector3L * Coords )
{
  return(World->GetVoxel(Coords));
}


inline bool  ZActiveVoxelInterface::GetVoxelExt_Abs( ZVector3L * Coords, UShort & VoxelType, ZVoxelLocation * VoxLocation )
{
  if (!World->GetVoxelLocation(VoxLocation, Coords)) { VoxelType = 65535; return(false); }
  VoxelType = VoxLocation->Sector->Data[ VoxLocation->Offset ];
  return(true);
}

inline bool ZActiveVoxelInterface::GetVoxelExt( Long x, Long y, Long z , UShort & VoxelType, ZVoxelLocation * VoxLocation )
{
  if (!World->GetVoxelLocation(VoxLocation , Coords.x + x, Coords.y + y, Coords.z + z)) { VoxelType = 65535; return(false); }
  VoxelType = VoxLocation->Sector->Data[ VoxLocation->Offset ];
  return(true);
}

inline bool ZActiveVoxelInterface::GetNeighborVoxel(ULong PositionCode, ZVoxelLocation * VoxelLocation )
{
  ZVector3L NewCoords;

  NewCoords = Coords;
  NewCoords.x += _xbp[PositionCode].x; NewCoords.x += _xbp[PositionCode].y; NewCoords.x += _xbp[PositionCode].z;

  return(World->GetVoxelLocation(VoxelLocation, &NewCoords));
}

inline bool ZActiveVoxelInterface::ExchangeVoxels(ZVector3L * Voxel1, ZVector3L * Voxel2, ZChangeImportance ChangeImportance)
{
  return(World->ExchangeVoxels(Voxel1,Voxel2, ChangeImportance, true));
}


#endif /* Z_ZACTIVEVOXELINTERFACE_H */
