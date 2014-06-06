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
 * ZGui_FontFrame.cpp
 *
 *  Created on: 6 mai 2011
 *      Author: laurent
 */

#include "ZGui_FontFrame.h"

#include <GL/glew.h>
#include <math.h>
#include <stdio.h>
#include "SDL/SDL.h"



void ZFrame_FontFrame::Render2(Frame_Dimensions * ParentPosition)
{
  Frame_Dimensions Position;

  ZListItem * Item;
  ZFrame * Frame;
  ULong i,TextureRef;
  char c;

  // Frame Position Computing
  if (Flag_Show_Master)
  {
    Position.Position_x = ParentPosition->Position_x + Dimensions.Position_x;
    Position.Position_y = ParentPosition->Position_y + Dimensions.Position_y;
    Position.Position_z = ParentPosition->Position_z + Dimensions.Position_z;
    Position.Width = Dimensions.Width;
    Position.Height= Dimensions.Height;
    Position.Depth = Dimensions.Depth;

    // Render this frame

    float FontSize_h = 128.0;
    float FontSize_v = 128.0;
    if (Flag_Show_Frame)
    {
      float x = Position.Position_x;
      float y = Position.Position_y;
      float xLim = Position.Position_x + Position.Width - FontSize_h;
      float yLim = Position.Position_y + Position.Height - FontSize_v;

      TextureRef = GuiManager->TextureManager->GetTextureEntry(this->TextureNum)->OpenGl_TextureRef;
      glBindTexture(GL_TEXTURE_2D,TextureRef );
      for (i=0;(c=TextToDisplay[i]);i++)
      {
        float Tx_x = (float)(c % 16) * 0.0625;
        float Tx_y = (float)(c / 16) * 0.0625;

        float Tx_xp = Tx_x + 0.0625;
        float Tx_yp = Tx_y + 0.0625;

        float xp = x+FontSize_h;
        float yp = y+FontSize_v;

        glBegin(GL_TRIANGLES);
          glColor3f(1.0,1.0,1.0);
          glTexCoord2f(Tx_x ,Tx_y ); glVertex3f(x , y , Position.Position_z);
          glTexCoord2f(Tx_xp,Tx_y ); glVertex3f(xp, y , Position.Position_z);
          glTexCoord2f(Tx_xp,Tx_yp); glVertex3f(xp, yp, Position.Position_z);
          glTexCoord2f(Tx_xp,Tx_yp); glVertex3f(xp, yp, Position.Position_z);
          glTexCoord2f(Tx_x ,Tx_yp); glVertex3f(x , yp, Position.Position_z);
          glTexCoord2f(Tx_x ,Tx_y ); glVertex3f(x , y , Position.Position_z);
        glEnd();

        x += FontSize_h; if (x > xLim) {x = Position.Position_x; y += FontSize_v; if (y>yLim) break;}

      }
    }



    // Render child frames

    if (Flag_Show_Childs)
    {
      //SubFrameList.Dump();
      Item = SubFrameList.GetFirst();
      while (Item)
      {
        Frame = (ZFrame *)Item->GetObject();
        if (Frame) Frame->Render(&Position);

        Item = SubFrameList.GetNext(Item);
      }
    }
    Position.Position_x = ParentPosition->Position_x + Dimensions.Position_x;
    Position.Position_y = ParentPosition->Position_y + Dimensions.Position_y;
    Position.Position_z = ParentPosition->Position_z + Dimensions.Position_z;
    Position.Width = Dimensions.Width;
    Position.Height= Dimensions.Height;
    Position.Depth = Dimensions.Depth;

  }

}


void ZFrame_FontFrame::GetTextDisplaySize(ZVector2f * OutSize)
{
  if (TileStyle)
  {
    TileStyle->TileSet->GetFontRenderSize(TileStyle,TextToDisplay, OutSize);
  }
}





void ZFrame_FontFrame::Render(Frame_Dimensions * ParentPosition)
{

  ZListItem * Item;
  ZFrame * Frame;

  if (Flag_Show_Master)
  {
    EffectivePosition.Position_x = ParentPosition->Position_x + Dimensions.Position_x;
    EffectivePosition.Position_y = ParentPosition->Position_y + Dimensions.Position_y;
    EffectivePosition.Position_z = ParentPosition->Position_z + Dimensions.Position_z;
    EffectivePosition.Width = Dimensions.Width;
    EffectivePosition.Height= Dimensions.Height;
    EffectivePosition.Depth = Dimensions.Depth;
    if (Flag_Show_Frame)
    {
      if (TileStyle)
        if (TileStyle->TileSet)
          TileStyle->TileSet->RenderFont(TileStyle, &EffectivePosition, this->TextToDisplay, &DrawColor );
    }



    // Render child frames

    if (Flag_Show_Childs)
    {
      //SubFrameList.Dump();
      Item = SubFrameList.GetFirst();
      while (Item)
      {
        Frame = (ZFrame *)Item->GetObject();
        if (Frame) Frame->Render(&EffectivePosition);

        Item = SubFrameList.GetNext(Item);
      }
    }

  }

}
