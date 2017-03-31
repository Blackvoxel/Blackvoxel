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
 * ZTileSets.cpp
 *
 *  Created on: 7 mai 2011
 *      Author: laurent
 */

#include "ZTileSets.h"
#include <GL/glew.h>
#include <math.h>
#include <stdio.h>
#include "SDL/SDL.h"

    ZTileSet::ZTileSet()
    {
      CoordTable = new TileCoord[256];
      TextureNum = 0;

      Texture_Width  = 128;
      Texture_Height = 128;
      TileSlot_Width = 8;
      TileSlot_Height= 8;
      Tile_Width     = 8;
      Tile_Height    = 8;
      TileOffset_x   = 0;
      TileOffset_y   = 0;
      TilesPerLine   = 16;
      DefaultDrawColor.r = 1.0f;
      DefaultDrawColor.v = 1.0f;
      DefaultDrawColor.b = 1.0f;
    }

    ZTileSet::~ZTileSet()
    {
      if (CoordTable) delete [] CoordTable;
    }

    void ZTileSet::SetTextureSize(ULong Width, ULong Height)  {Texture_Width = Width; Texture_Height = Height;}

    void ZTileSet::SetTileSlotSize(ULong Width, ULong Height) {TileSlot_Width = Width; TileSlot_Height = Height;}

    void ZTileSet::SetTileSize(ULong Width, ULong Height)     {Tile_Width = Width, Tile_Height = Height;}

    void ZTileSet::SetTilesPerLine(ULong TilesPerLine)        {this->TilesPerLine = TilesPerLine;}

    void  ZTileSet::SetTileOffset(ULong x, ULong y)           { TileOffset_x = x; TileOffset_y = y; }


    void ZTileSet::ComputeTileCoords()
    {
      ULong i, x1,y1,x2,y2;

      for (i=0;i<256;i++)
      {
        x1 = (i % TilesPerLine) * TileSlot_Width  + TileOffset_x;
        y1 = (i / TilesPerLine) * TileSlot_Height + TileOffset_y;
        x2 = x1 + Tile_Width;
        y2 = y1 + Tile_Height;

        CoordTable[i].TopLeft_x     = (float)x1 / (float)Texture_Width;
        CoordTable[i].TopLeft_y     = (float)y1 / (float)Texture_Height;
        CoordTable[i].BottomRight_x = (float)x2 / (float)Texture_Width;
        CoordTable[i].BottomRight_y = (float)y2 / (float)Texture_Height;
        CoordTable[i].Tile_Width = (float)Tile_Width;
        CoordTable[i].Tile_Height= (float)Tile_Height;
      }
    }

    ULong ZTileSet::GetTilePixel(UByte TileNum, ULong x, ULong y)
    {
      ZBitmapImage * Image;
      ULong Image_x, Image_y;

      Image = TextureManager->GetTextureEntry(TextureNum)->Texture;

      Image_x = x + (TileNum % TilesPerLine) * TileSlot_Width;
      Image_y = y + (TileNum / TilesPerLine) * TileSlot_Height;

      return(Image->GetPixel(Image_x,Image_y));
    }

    void ZTileSet::RenderTile( ZVector3f * TopLeft, ZVector3f * BottomRight, UByte TileNum, ZColor3f * Color)
    {
      TileCoord * Coord;

      Coord = &CoordTable[TileNum];

      if(!Color) Color = &DefaultDrawColor;

      glBindTexture(GL_TEXTURE_2D,TextureManager->GetTextureEntry(TextureNum)->OpenGl_TextureRef );
      glColor3f(Color->r, Color->v, Color->b);
      glBegin(GL_TRIANGLES);
       glTexCoord2f(Coord->TopLeft_x      , Coord->TopLeft_y     ); glVertex3f(TopLeft->x    , TopLeft->y , TopLeft->z);
        glTexCoord2f(Coord->BottomRight_x , Coord->TopLeft_y    ); glVertex3f(BottomRight->x, TopLeft->y , TopLeft->z);
        glTexCoord2f(Coord->BottomRight_x , Coord->BottomRight_y); glVertex3f(BottomRight->x, BottomRight->y , TopLeft->z);
        glTexCoord2f(Coord->BottomRight_x , Coord->BottomRight_y); glVertex3f(BottomRight->x, BottomRight->y , BottomRight->z);
        glTexCoord2f(Coord->TopLeft_x     , Coord->BottomRight_y); glVertex3f(TopLeft->x    , BottomRight->y , BottomRight->z);
        glTexCoord2f(Coord->TopLeft_x     , Coord->TopLeft_y    ); glVertex3f(TopLeft->x    , TopLeft->y     , BottomRight->z);
      glEnd();
    }

    bool _Buggy_FastParseHex(char const * Text, ULong &Offset, ULong & Result)
    {

      char c;

      for (;;)
      {
        c = Text[Offset]; if (!c) return(false);
        if      (c >= '0' && c <= '9' ) c-=48;
        else if (c >= 'A' && c <= 'F' ) c-='A'+10;
        else return(true);
        Result <<= 4;
        Result += c;
        Offset++;
      }
      return(false);
    }

    bool _FastParseHex(char const * Text, ULong &Offset, ULong & Result)
    {

      char c;

      for (;;)
      {
        c = Text[Offset]; if (!c) return(false);
        if      (c >= '0' && c <= '9' ) c-=48;
        else if (c >= 'A' && c <= 'F' ) c-=55;
        else return(true);
        Result <<= 4;
        Result += c;
        Offset++;
      }
      return(false);
    }

    void ZTileSet::RenderFont(ZTileStyle const * TileStyle , ZBox3f const * DrawBox, char const * TextToRender, ZColor3f * Color=0)
    {
      float x,y, xp,yp, DimX, DimY, LimitX;// LimitY;
      ZColor3f DrawColor;

      if (Color == 0) { Color = & DrawColor; DrawColor.r = 1.0; DrawColor.v = 1.0; DrawColor.b = 1.0; }

      ULong i;
      ZTileSet * TileSet;
      TileCoord * Coord;
      UByte c;

      TileSet = TileStyle->TileSet;
      x = DrawBox->Position_x;
      y = DrawBox->Position_y;
      LimitX = x + DrawBox->Width ;
      //LimitY = y + DrawBox->Height;

      glBindTexture(GL_TEXTURE_2D,TextureManager->GetTextureEntry(TileSet->TextureNum)->OpenGl_TextureRef );
      glColor3f(Color->r, Color->v, Color->b);
      for (i=0; (c = (UByte)(TextToRender[i])) ;i++)
      {
        // Instructions

        if (c == '~')
        {
          i++;
          ULong Cmd, Value;
          Cmd = 0; Value = 0;
          if (!_FastParseHex(TextToRender, i, Cmd)) return;
          c=(UByte)(TextToRender[i++]);
          if (c!=':') return;
          switch(Cmd)
          {
            /*
            // buggy
            case 1: if (!_Buggy_FastParseHex(TextToRender, i, Value)) return;
                    glColor3d((Value>>16) & 0xff, (Value >> 8) & 0xff, Value & 0xff );
                    break;
            */

            // Change color
            case 1: if (!_FastParseHex(TextToRender, i, Value)) return;
                    glColor3d( ((double)((Value>>16) & 0xff))/255.0,
                               ((double)((Value >> 8) & 0xff))/255.0,
                               ((double)(Value & 0xff ))/255.0       );
                    break;

            default: return;
          }
          c=(UByte)(TextToRender[i]);
          if (c!=':') return;
          continue;
        }
        //


        Coord = &TileSet->CoordTable[c];
        DimX = Coord->Tile_Width * TileStyle->HSizeFactor;
        DimY = Coord->Tile_Height* TileStyle->VSizeFactor;
        xp = x + DimX;
        yp = y + DimY;
        if (xp > LimitX || c==0x0A)
        {
          x = DrawBox->Position_x;
          y+= DimY + TileStyle->Interligne_sup;
          xp = x + DimX;
          yp = y + DimY;
          if (c==0x0A) continue;
        }
        glBegin(GL_TRIANGLES);
          glTexCoord2f(Coord->TopLeft_x     , Coord->TopLeft_y    ); glVertex3f(x , y , DrawBox->Position_z);
          glTexCoord2f(Coord->BottomRight_x , Coord->TopLeft_y    ); glVertex3f(xp, y , DrawBox->Position_z);
          glTexCoord2f(Coord->BottomRight_x , Coord->BottomRight_y); glVertex3f(xp, yp, DrawBox->Position_z);
          glTexCoord2f(Coord->BottomRight_x , Coord->BottomRight_y); glVertex3f(xp, yp, DrawBox->Position_z);
          glTexCoord2f(Coord->TopLeft_x     , Coord->BottomRight_y); glVertex3f(x , yp, DrawBox->Position_z);
          glTexCoord2f(Coord->TopLeft_x     , Coord->TopLeft_y    ); glVertex3f(x , y , DrawBox->Position_z);
        glEnd();

        x += DimX + TileStyle->CharSpacing_Sup;

      }
      glColor3f(1.0,1.0,1.0);
    }



    void  ZTileSet::GetFontRenderSize(ZTileStyle const * TileStyle , char const * TextToRender, ZVector2f * OutSize)
    {
      float x,y, DimX, DimY;

      ULong i;
      ZTileSet * TileSet;
      TileCoord * Coord;
      UByte c;

      TileSet = TileStyle->TileSet;
      x = 0;
      y = 0;

      for (i=0; (c = (UByte)(TextToRender[i])) ;i++)
      {
        Coord = &TileSet->CoordTable[c];
        DimX = Coord->Tile_Width * TileStyle->HSizeFactor;
        DimY = Coord->Tile_Height* TileStyle->VSizeFactor;
        x += DimX + TileStyle->CharSpacing_Sup;
        if (DimY>y) y = DimY;
      }
      OutSize->x = x;
      OutSize->y = y;
    }

