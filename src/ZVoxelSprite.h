/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2020 Laurent Thiebaut & Olivia Merle
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
 * ZVoxelSprite.h
 *
 *  Created on: 1 juil. 2020
 *      Author: laurent
 */

#ifndef Z_ZVOXELSPRITE_H
#define Z_ZVOXELSPRITE_H

//#ifndef Z_ZVOXELSPRITE_H
//#  include "ZVoxelSprite.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZWORLD_H
#  include "ZWorld.h"
#endif


class ZVoxelSector;
class ZVoxelWorld;

class ZVoxelSprite
{
  public:
    ZVoxelSector Sprite;

    ZVoxelSprite() {Sprite.Fill(0);}
    ZVoxelSprite(Long sx, Long sy, Long sz) : Sprite(sx,sy,sz) { }

    void Resize(ZVector3L * NewSize) { Sprite.ChangeSize(NewSize->x, NewSize->y, NewSize->z); }
    void SetHandle(Long x, Long y, Long z)
    {
      Sprite.Handle_x = x;
      Sprite.Handle_y = y;
      Sprite.Handle_z = z;
    }


    bool ExtractFromWorld(ZVoxelWorld * World, ZVector3L * Position, int Direction)
    {
      return(World->WorldToSprite(&Sprite, Position, Direction));
    }

    bool Old_ExtractFromWorld(ZVoxelWorld * World, ZVector3L * Position, int Direction)
    {
      Long x,y,z;

      //ZMemSize OtherInfos;
      //UShort VoxelType;
      ZVoxelLocation Loc;

      ZVector3L Size;
      int xf1, zf1, xf2, zf2;
      Size.x = Sprite.Size_x; Size.y = Sprite.Size_y; Size.z = Sprite.Size_z;

      switch (Direction)
      {
        default:
        case 0: xf1 = 1 ; zf1 = 0; xf2 = 0; zf2 = 1; break;
        case 1: xf1 = 0 ; zf1 = 1; xf2 = 1; zf2 = 0; break;
        case 2: xf1 = -1; zf1 = 0; xf2 = 0; zf2 = -1;break;
        case 3: xf1 = 0 ; zf1 = -1; xf2 = -1; zf2 = 0; break;
      }


      if (Size.x <= 0 || Size.y <= 0 || Size.z <= 0) return(false);
      Resize(&Size);

      for (z = 0 ; z<Size.z ; z++)
        for (x = 0 ; x<Size.x ; x++)
          for (y = 0 ; y<Size.y ; y++)
          {
            printf("(%d)",Position->y - Sprite.Handle_y + y);
            World->GetVoxelLocation(&Loc, Position->x - Sprite.Handle_x + (x * xf1) + (z *zf1) , Position->y - Sprite.Handle_y + y  , Position->z - Sprite.Handle_z + (x * xf2) + (z*zf2));
            // VoxelType = Loc.Sector->Data[Loc.Offset];
            Sprite.SetCube_Transfert_NSS(x,y,z, &Loc);

          }
      return(true);
    }


    bool PutIntoWorld(ZVoxelWorld * World, ZVector3L * Position, int Direction)
    {
      return(World->SpriteToWorld(&Sprite,Position, Direction));
    }

    bool Old_PutIntoWorld(ZVoxelWorld * World, ZVector3L * Position, int Direction)
    {
      Long x,y,z;

      //ZMemSize OtherInfos;
      //UShort VoxelType;
      ZVoxelLocation Loc;

      ZVector3L Size;
      int xf1, zf1, xf2, zf2;
      Size.x = Sprite.Size_x; Size.y = Sprite.Size_y; Size.z = Sprite.Size_z;

      switch (Direction)
      {
        default:
        case 0: xf1 = 1 ; zf1 = 0; xf2 = 0; zf2 = 1; break;
        case 1: xf1 = 0 ; zf1 = 1; xf2 = 1; zf2 = 0; break;
        case 2: xf1 = -1; zf1 = 0; xf2 = 0; zf2 = -1;break;
        case 3: xf1 = 0 ; zf1 = -1; xf2 = -1; zf2 = 0; break;
      }


      if (Size.x <= 0 || Size.y <= 0 || Size.z <= 0) return(false);
      Resize(&Size);

      for (z = 0 ; z<Size.z ; z++)
        for (x = 0 ; x<Size.x ; x++)
          for (y = 0 ; y<Size.y ; y++)
          {
            World->GetVoxelLocation(&Loc, Position->x - Sprite.Handle_x + (x * xf1) + (z *zf1) , Position->y - Sprite.Handle_y +y  , Position->z - Sprite.Handle_z + (x * xf2) + (z*zf2));
            // VoxelType = Loc.Sector->Data[Loc.Offset];
            Sprite.SetCube_TransfertTo_NSS(x,y,z, &Loc);

          }
      return(true);
    }


};


#endif /* Z_ZVOXELSPRITE_H */
