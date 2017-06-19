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
    inline bool                MoveThis( ULong PositionCode, ZChangeImportance ChangeImportance);
    inline bool                MoveThis_Abs( ZVector3L * Destination, ZChangeImportance ChangeImportance );
    inline void                AnimateThis(UShort NewVoxelType, ZChangeImportance ChangeImportance);
    inline bool                MoveVoxel( ZVector3L * Source, ZVector3L * Destination, ZChangeImportance ChangeImportance = ZActiveVoxelInterface::CHANGE_CRITICAL );
    inline bool                ExchangeVoxels(ZVector3L * Voxel1, ZVector3L * Voxel2, ZChangeImportance ChangeImportance);
    inline bool                SetVoxel( ZVector3L * Coords, UShort VoxelType, ZChangeImportance ChangeImportance );
    inline bool                SetVoxelExt( ZVector3L * Coords, UShort VoxelType, ZChangeImportance ChangeImportance, ZVoxelLocation * VoxLocation );
    inline bool                SetVoxel_Main(UShort NewVoxelType, ZChangeImportance ChangeImportance );
    inline UShort              GetVoxel( ZVector3L * Coords );
    inline bool                GetVoxelExt_Abs( ZVector3L * Coords, UShort & VoxelType, ZVoxelLocation * VoxLocation );
    inline bool                GetVoxelExt_Rel( Long x, Long y, Long z , UShort & VoxelType, ZVoxelLocation * VoxLocation );
    inline bool                GetVoxelPointers( ZVector3L * Coords, ZVoxelLocation * VoxelLocation)                         { return(World->GetVoxelLocation(VoxelLocation, Coords)); }
    inline bool                GetNeighborVoxel(ULong PositionCode, ZVoxelLocation * VoxelLocation );
    inline UShort              GetNeighborVoxel(ULong PositionCode);
    inline bool                SetNeighborVoxel( ULong PositionCode, UShort VoxelType, ZChangeImportance ChangeImportance );
    inline bool                PushToNeighborVoxel_Try(ULong PositionCode, UShort VoxelType, ULong & VoxelQuantity );
    inline bool                PushToNeighborVoxel_Do(ULong PositionCode, UShort VoxelType, ULong & VoxelQuantity );
    inline bool                PullFromNeighborVoxel_Try(ULong PositionCode, UShort &VoxelType, ULong & VoxelQuantity );
    inline bool                PullFromNeighborVoxel_Do(ULong PositionCode, UShort &VoxelType, ULong & VoxelQuantity );
};

inline bool  ZActiveVoxelInterface::MoveThis_Abs( ZVector3L * Destination, ZChangeImportance ChangeImportance )
{
  return( World->MoveVoxel_Sm(&Coords, Destination, 0, (UByte) ChangeImportance) );
}

inline void ZActiveVoxelInterface::AnimateThis(UShort NewVoxelType, ZChangeImportance ChangeImportance)
{
  Location.Sector->Data[Location.Offset] = NewVoxelType;
  Location.Sector->Flag_Render_Dirty = true;
  Location.Sector->Flag_IsModified = ChangeImportance;
}

inline bool ZActiveVoxelInterface::MoveThis( int x, int y, int z, ZChangeImportance ChangeImportance)
{
  ZVector3L Destination;

  Destination = Coords;
  Destination.x += x; Destination.y += y; Destination.z += z;

  return( World->MoveVoxel_Sm(&Coords, &Destination, 0, (UByte) ChangeImportance ) );
}

inline bool ZActiveVoxelInterface::MoveThis( ULong PositionCode, ZChangeImportance ChangeImportance)
{
  ZVector3L Destination;

  Destination = Coords;
  Destination.x += _xbp[PositionCode].x; Destination.y += _xbp[PositionCode].y; Destination.z += _xbp[PositionCode].z;

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

inline bool ZActiveVoxelInterface::SetVoxel_Main(UShort NewVoxelType, ZChangeImportance ChangeImportance )
{
  ZVoxelLocation     VoxLocation;
  if (!World->SetVoxel_WithCullingUpdate( Coords.x, Coords.y, Coords.z, NewVoxelType, (UByte) ChangeImportance, true, &VoxLocation ) ) return(false);
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

inline bool ZActiveVoxelInterface::GetVoxelExt_Rel( Long x, Long y, Long z , UShort & VoxelType, ZVoxelLocation * VoxLocation )
{
  if (!World->GetVoxelLocation(VoxLocation , Coords.x + x, Coords.y + y, Coords.z + z)) { VoxelType = 65535; return(false); }
  VoxelType = VoxLocation->Sector->Data[ VoxLocation->Offset ];
  return(true);
}

inline bool ZActiveVoxelInterface::GetNeighborVoxel(ULong PositionCode, ZVoxelLocation * VoxelLocation )
{
  ZVector3L NewCoords;

  NewCoords = Coords;
  NewCoords.x += _xbp[PositionCode].x; NewCoords.y += _xbp[PositionCode].y; NewCoords.z += _xbp[PositionCode].z;

  return(World->GetVoxelLocation(VoxelLocation, &NewCoords));
}

inline UShort ZActiveVoxelInterface::GetNeighborVoxel(ULong PositionCode)
{
  ZVector3L NewCoords;

  NewCoords = Coords;
  NewCoords.x += _xbp[PositionCode].x; NewCoords.y += _xbp[PositionCode].y; NewCoords.z += _xbp[PositionCode].z;

  return(World->GetVoxel(&NewCoords));
}

inline bool ZActiveVoxelInterface::SetNeighborVoxel( ULong PositionCode, UShort VoxelType, ZChangeImportance ChangeImportance )
{
  ZVoxelLocation     VoxLocation;
  ZVector3L NewCoords;

  NewCoords = Coords;
  NewCoords.x += _xbp[PositionCode].x; NewCoords.y += _xbp[PositionCode].y; NewCoords.z += _xbp[PositionCode].z;

  if (!World->SetVoxel_WithCullingUpdate( NewCoords.x, NewCoords.y, NewCoords.z, VoxelType, (UByte) ChangeImportance, true, &VoxLocation ) ) return(false);
  VoxLocation.Sector->ModifTracker.Set(VoxLocation.Offset);

  return(true);
}

inline bool ZActiveVoxelInterface::ExchangeVoxels(ZVector3L * Voxel1, ZVector3L * Voxel2, ZChangeImportance ChangeImportance)
{
  return(World->ExchangeVoxels(Voxel1,Voxel2, ChangeImportance, true));
}

inline bool ZActiveVoxelInterface::PushToNeighborVoxel_Try(ULong PositionCode, UShort VoxelType, ULong & VoxelQuantity )
{
  ZVoxelLocation VoxLocation;
  ZVector3L NewCoords;
  UShort DestVoxelType;

  NewCoords = Coords;
  NewCoords.x += _xbp[PositionCode].x; NewCoords.y += _xbp[PositionCode].y; NewCoords.z += _xbp[PositionCode].z;

  if (!World->GetVoxelLocation(&VoxLocation, &NewCoords)) { VoxelQuantity = 0; return(false); }

  DestVoxelType = VoxLocation.Sector->Data[VoxLocation.Offset];

  if (!VoxelTypeManager->VoxelTable[DestVoxelType]->Is_Interface_PushBlock) {VoxelQuantity = 0; return(false);}

  VoxelQuantity = VoxelTypeManager->VoxelTable[DestVoxelType]->Interface_PushBlock_PushTest(&VoxLocation,VoxelType, VoxelQuantity);

  return(VoxelQuantity!=0);
}

inline bool ZActiveVoxelInterface::PushToNeighborVoxel_Do(ULong PositionCode, UShort VoxelType, ULong & VoxelQuantity )
{
  ZVoxelLocation VoxLocation;
  ZVector3L NewCoords;
  UShort DestVoxelType;

  NewCoords = Coords;
  NewCoords.x += _xbp[PositionCode].x; NewCoords.y += _xbp[PositionCode].y; NewCoords.z += _xbp[PositionCode].z;

  if (!World->GetVoxelLocation(&VoxLocation, &NewCoords)) { VoxelQuantity = 0; return(false); }

  DestVoxelType = VoxLocation.Sector->Data[VoxLocation.Offset];

  if (!VoxelTypeManager->VoxelTable[DestVoxelType]->Is_Interface_PushBlock) {VoxelQuantity = 0; return(false);}

  VoxelQuantity = VoxelTypeManager->VoxelTable[DestVoxelType]->Interface_PushBlock_Push(&VoxLocation,VoxelType, VoxelQuantity);

  return(VoxelQuantity!=0);
}

inline bool ZActiveVoxelInterface::PullFromNeighborVoxel_Try(ULong PositionCode, UShort &VoxelType, ULong & VoxelQuantity )
{
  ZVoxelLocation VoxLocation;
  ZVector3L NewCoords;
  UShort SourceVoxelType;

  NewCoords = Coords;
  NewCoords.x += _xbp[PositionCode].x; NewCoords.y += _xbp[PositionCode].y; NewCoords.z += _xbp[PositionCode].z;

  if (!World->GetVoxelLocation(&VoxLocation, &NewCoords)) { VoxelQuantity = 0; return(false); }

  SourceVoxelType = VoxLocation.Sector->Data[VoxLocation.Offset];

  if (!VoxelTypeManager->VoxelTable[SourceVoxelType]->Is_Interface_PullBlock) {VoxelQuantity = 0; return(false);}

  VoxelQuantity = VoxelTypeManager->VoxelTable[SourceVoxelType]->Interface_PushBlock_PullTest(&VoxLocation, &VoxelType, VoxelQuantity );

  if (VoxelQuantity == 0) return(false);

  return(true);
}

inline bool ZActiveVoxelInterface::PullFromNeighborVoxel_Do(ULong PositionCode, UShort &VoxelType, ULong & VoxelQuantity )
{
  ZVoxelLocation VoxLocation;
  ZVector3L NewCoords;
  UShort SourceVoxelType;

  NewCoords = Coords;
  NewCoords.x += _xbp[PositionCode].x; NewCoords.y += _xbp[PositionCode].y; NewCoords.z += _xbp[PositionCode].z;

  if (!World->GetVoxelLocation(&VoxLocation, &NewCoords)) { VoxelQuantity = 0; return(false); }

  SourceVoxelType = VoxLocation.Sector->Data[VoxLocation.Offset];

  if (!VoxelTypeManager->VoxelTable[SourceVoxelType]->Is_Interface_PullBlock) {VoxelQuantity = 0; return(false);}

  VoxelQuantity = VoxelTypeManager->VoxelTable[SourceVoxelType]->Interface_PushBlock_Pull(&VoxLocation, &VoxelType, VoxelQuantity );

  if (VoxelQuantity == 0) return(false);

  return(true);
}

#endif /* Z_ZACTIVEVOXELINTERFACE_H */
