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
 * ZTileSets.h
 *
 *  Created on: 7 mai 2011
 *      Author: laurent
 */

#ifndef Z_ZTILESETS_H
#define Z_ZTILESETS_H

//#ifndef Z_ZTILESETS_H
//#  include "ZTileSets.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZOBJECT_H
#  include "z/ZObject.h"
#endif

#ifndef Z_ZOBJECT_ARRAY_H
#  include "z/ZObject_Array.h"
#endif

#ifndef Z_ZTEXTUREMANAGER_H
#  include "ZTextureManager.h"
#endif

class ZTileSet;

class ZTileStyle : public ZObject
{
  public:
    ZTileSet * TileSet;
    float HSizeFactor;
    float VSizeFactor;
    float Interligne_sup;
    float CharSpacing_Sup;
};

class ZTileSetStyles : public ZObject
{
  public:
    ZObjectArray StyleArray;

    void CreateStyle(ULong StyleNum, ZTileSet * TileSet, float HSizeFactor, float VSizeFactor, float Interligne_sup = 0.0f, float CharSpacing_Sup=0.0f)
    {
      ZTileStyle * Style;

      Style = new ZTileStyle;

      Style->TileSet           = TileSet;
      Style->HSizeFactor       = HSizeFactor;
      Style->VSizeFactor       = VSizeFactor;
      Style->Interligne_sup    = Interligne_sup;
      Style->CharSpacing_Sup   = CharSpacing_Sup;
      StyleArray.SetEntry(StyleNum, *Style);
    }

    inline ZTileStyle * GetStyle(ULong StyleNum) {return (ZTileStyle *)StyleArray.GetEntry(StyleNum);}

};

class ZTileSet : public ZObject
{
    ZColor3f DefaultDrawColor;
  public:
    struct TileCoord
    {
      float TopLeft_x;
      float TopLeft_y;
      float BottomRight_x;
      float BottomRight_y;
      float Tile_Width;
      float Tile_Height;
    };

    ZTextureManager * TextureManager;
    TileCoord * CoordTable;
    ULong       TextureNum;

    //
    ULong Texture_Width, Texture_Height;
    ULong TileSlot_Width, TileSlot_Height;
    ULong Tile_Width,Tile_Height;
    ULong TileOffset_x, TileOffset_y;
    ULong TilesPerLine;

    ZTileSet();
    ~ZTileSet();

    void SetTextureManager( ZTextureManager * TextureManager ) { this->TextureManager = TextureManager; }
    void SetTextureNum( ULong TextureNum ) { this->TextureNum = TextureNum; }


    // Prepare tiling
    void SetTextureSize(ULong Width, ULong Height);
    void SetTileSlotSize(ULong Width, ULong Height);
    void SetTileSize(ULong Width, ULong Height);
    void SetTilesPerLine(ULong TilesPerLine);
    void SetTileOffset(ULong x, ULong y);
    void ComputeTileCoords();

    inline TileCoord * GetTileCoord(ULong TileNum) { return( &CoordTable[TileNum] );}

    float GetTileWidth (UByte TileNum) { return( CoordTable[TileNum].Tile_Width ); }
    float GetTileHeight(UByte TileNum) { return( CoordTable[TileNum].Tile_Height); }

    void  RenderTile( ZVector3f * TopLeft, ZVector3f * BottomRight, UByte TileNum, ZColor3f * Color = 0);
    void  RenderFont(ZTileStyle const * TileStyle , ZBox3f const * DrawBox, char const * TextToRender, ZColor3f * Color );
    void  GetFontRenderSize(ZTileStyle const * TileStyle , char const * TextToRender, ZVector2f * OutSize);
    ULong GetTilePixel(UByte TileNum, ULong x, ULong y);
};



#endif /* Z_ZTILESETS_H */
