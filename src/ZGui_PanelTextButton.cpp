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
 * ZGui_PanelTextButton.cpp
 *
 *  Created on: 1 mars 2012
 *      Author: laurent
 */

#include "ZGui_PanelTextButton.h"
#include <GL/glew.h>

void ZPanelTextButton::Render(Frame_Dimensions * ParentPosition)
{

  ZVector3f P1,P2,P3,P4;
  ZVector3f P1m,P2m,P3m,P4m;
  float SliceWidth;

  ZListItem * Item;
  ZFrame * Frame;

  // Frame Position Computing
  if (Flag_Show_Master)
  {
    EffectivePosition.Position_x = ParentPosition->Position_x + Dimensions.Position_x;
    EffectivePosition.Position_y = ParentPosition->Position_y + Dimensions.Position_y;
    EffectivePosition.Position_z = ParentPosition->Position_z + Dimensions.Position_z;
    EffectivePosition.Width = Dimensions.Width;
    EffectivePosition.Height= Dimensions.Height;
    EffectivePosition.Depth = Dimensions.Depth;

    if (this->Flag_Draging) { EffectivePosition.Position_x = DragAbsolutePosition.x; EffectivePosition.Position_y = DragAbsolutePosition.y; }

    // Render this frame

    if (Flag_Show_Frame)
    {
/*
      P1.x = EffectivePosition.Position_x; P1.y = EffectivePosition.Position_y; P1.z = EffectivePosition.Position_z;
      P2.x = EffectivePosition.Position_x + EffectivePosition.Width ; P2.y = EffectivePosition.Position_y ; P2.z = EffectivePosition.Position_z + EffectivePosition.Depth;
      P3.x = EffectivePosition.Position_x + EffectivePosition.Width ; P3.y = EffectivePosition.Position_y + EffectivePosition.Height; P3.z = EffectivePosition.Position_z + EffectivePosition.Depth;
      P4.x = EffectivePosition.Position_x; P4.y = EffectivePosition.Position_y + EffectivePosition.Height; P4.z = EffectivePosition.Position_z + EffectivePosition.Depth;
*/

      P1.x = EffectivePosition.Position_x; P1.y = EffectivePosition.Position_y; P1.z = EffectivePosition.Position_z;
      P2.x = EffectivePosition.Position_x + EffectivePosition.Width ; P2.y = EffectivePosition.Position_y ; P2.z = EffectivePosition.Position_z + EffectivePosition.Depth;
      P3.x = EffectivePosition.Position_x + EffectivePosition.Width ; P3.y = EffectivePosition.Position_y + EffectivePosition.Height; P3.z = EffectivePosition.Position_z + EffectivePosition.Depth;
      P4.x = EffectivePosition.Position_x; P4.y = EffectivePosition.Position_y + EffectivePosition.Height; P4.z = EffectivePosition.Position_z + EffectivePosition.Depth;

      ULong TextureRef = GuiManager->TextureManager->GetTextureEntry(this->TextureNum)->OpenGl_TextureRef;
      glBindTexture(GL_TEXTURE_2D,TextureRef );

      SliceWidth = EffectivePosition.Height * 0.25f;

      // The left slice

      P1m.x = P1.x; P1m.y = P1.y; P1m.z = P1.z;
      P2m.x = P1.x + SliceWidth ; P2m.y = P2.y; P2m.z = P2.z;
      P3m.x = P1.x + SliceWidth ; P3m.y = P3.y; P3m.z = P3.z;
      P4m.x = P4.x; P4m.y = P4.y; P4m.z = P4.z;

      glBegin(GL_TRIANGLES);
        glColor3f(DrawColor.r, DrawColor.v, DrawColor.b);
        glTexCoord2f(0.0,0.0);  glVertex3f(P1m.x, P1m.y , P1m.z);
        glTexCoord2f(0.25,0.0); glVertex3f(P2m.x, P2m.y , P2m.z);
        glTexCoord2f(0.25,1.0); glVertex3f(P3m.x, P3m.y , P3m.z);
        glTexCoord2f(0.25,1.0); glVertex3f(P3m.x, P3m.y , P3m.z);
        glTexCoord2f(0.0,1.0);  glVertex3f(P4m.x, P4m.y , P4m.z);
        glTexCoord2f(0.0,0.0);  glVertex3f(P1m.x, P1m.y , P1m.z);
      glEnd();

      // Right slice

      P1m.x = P2.x-SliceWidth; P1m.y = P1.y; P1m.z = P1.z;
      P2m.x = P2.x; P2m.y = P2.y; P2m.z = P2.z;
      P3m.x = P3.x; P3m.y = P3.y; P3m.z = P3.z;
      P4m.x = P3.x-SliceWidth; P4m.y = P4.y; P4m.z = P4.z;
      glBegin(GL_TRIANGLES);
        //glColor3f(DrawColor.r, DrawColor.v, DrawColor.b);
        glTexCoord2f(0.75,0.0);  glVertex3f(P1m.x, P1m.y , P1m.z);
        glTexCoord2f(1.0,0.0);   glVertex3f(P2m.x, P2m.y , P2m.z);
        glTexCoord2f(1.0,1.0);   glVertex3f(P3m.x, P3m.y , P3m.z);
        glTexCoord2f(1.0,1.0);   glVertex3f(P3m.x, P3m.y , P3m.z);
        glTexCoord2f(0.75,1.0);  glVertex3f(P4m.x, P4m.y , P4m.z);
        glTexCoord2f(0.75,0.0);  glVertex3f(P1m.x, P1m.y , P1m.z);
      glEnd();

      // Middle slice

      P1m.x = P1.x+SliceWidth; P1m.y = P1.y; P1m.z = P1.z;
      P2m.x = P2.x-SliceWidth; P2m.y = P2.y; P2m.z = P2.z;
      P3m.x = P3.x-SliceWidth; P3m.y = P3.y; P3m.z = P3.z;
      P4m.x = P4.x+SliceWidth; P4m.y = P4.y; P4m.z = P4.z;
      glBegin(GL_TRIANGLES);
        //glColor3f(DrawColor.r, DrawColor.v, DrawColor.b);
        glTexCoord2f(0.25,0.0);  glVertex3f(P1m.x, P1m.y , P1m.z);
        glTexCoord2f(0.75,0.0);  glVertex3f(P2m.x, P2m.y , P2m.z);
        glTexCoord2f(0.75,1.0);  glVertex3f(P3m.x, P3m.y , P3m.z);
        glTexCoord2f(0.75,1.0);  glVertex3f(P3m.x, P3m.y , P3m.z);
        glTexCoord2f(0.25,1.0);  glVertex3f(P4m.x, P4m.y , P4m.z);
        glTexCoord2f(0.25,0.0);  glVertex3f(P1m.x, P1m.y , P1m.z);
      glEnd();

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
