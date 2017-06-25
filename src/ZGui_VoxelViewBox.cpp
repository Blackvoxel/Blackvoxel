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
 * ZGui_VoxelView.cpp
 *
 *  Created on: 29 févr. 2012
 *      Author: laurent
 */

#include "ZGui_VoxelViewBox.h"
#include <GL/glew.h>
#include "SDL/SDL.h"
#include "ZGui_InventoryBox.h"

void ZVoxelViewBox::Render(Frame_Dimensions * ParentPosition)
{

  ZVector3f TopLeft, BottomRight;
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

    if (this->Flag_Draging) { EffectivePosition.Position_x = DragAbsolutePosition.x; EffectivePosition.Position_y = DragAbsolutePosition.y; EffectivePosition.Position_z = ParentPosition->Position_z + Dimensions.Position_z +2.0f; }

    // Render this frame

    if (Flag_Show_Frame)
    {
      TopLeft.x = EffectivePosition.Position_x;
      TopLeft.y = EffectivePosition.Position_y;
      TopLeft.z = EffectivePosition.Position_z;
      BottomRight.x = EffectivePosition.Position_x + EffectivePosition.Width;
      BottomRight.y = EffectivePosition.Position_y + EffectivePosition.Height;
      BottomRight.z = EffectivePosition.Position_z;


      // if (TileSet) TileSet->RenderTile(&TopLeft, &BottomRight, Tile, &DrawColor);

      ZVector2f P1,P2,P3,P4;

      P1.x = TopLeft.x;     P1.y = TopLeft.y;
      P2.x = BottomRight.x; P2.y = TopLeft.y;
      P3.x = BottomRight.x; P3.y = BottomRight.y;
      P4.x = TopLeft.x;     P4.y = BottomRight.y;



      // Render
      if (*VoxelType!=0) glBindTexture(GL_TEXTURE_2D,VoxelTypeManager->VoxelTable[*VoxelType]->OpenGl_TextureRef);
      else               glBindTexture(GL_TEXTURE_2D,0);
      if ((*VoxelType)) glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      else              glColor4f(0.7f, 0.7f, 0.7f, 0.9f);

      if (!VoxelTypeManager->VoxelTable[*VoxelType]->BvProp_UseInventoryImage)
      {
        glBegin(GL_TRIANGLES);
          glTexCoord2f(0.25f , 0.25f ); glVertex3f( P1.x , P1.y , TopLeft.z);
          glTexCoord2f(0.50f , 0.25f ); glVertex3f( P2.x , P2.y , TopLeft.z);
          glTexCoord2f(0.50f , 0.50f ); glVertex3f( P3.x , P3.y , TopLeft.z);
          glTexCoord2f(0.50f , 0.50f ); glVertex3f( P3.x , P3.y , BottomRight.z);
          glTexCoord2f(0.25f , 0.50f ); glVertex3f( P4.x , P4.y , BottomRight.z);
          glTexCoord2f(0.25f , 0.25f ); glVertex3f( P1.x , P1.y , BottomRight.z);
        glEnd();
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      }
      else
      {
        glBegin(GL_TRIANGLES);
          glTexCoord2f(0.75f , 0.75f ); glVertex3f( P1.x , P1.y , TopLeft.z);
          glTexCoord2f(1.00f , 0.75f ); glVertex3f( P2.x , P2.y , TopLeft.z);
          glTexCoord2f(1.00f , 1.00f ); glVertex3f( P3.x , P3.y , TopLeft.z);
          glTexCoord2f(1.00f , 1.00f ); glVertex3f( P3.x , P3.y , BottomRight.z);
          glTexCoord2f(0.75f , 1.00f ); glVertex3f( P4.x , P4.y , BottomRight.z);
          glTexCoord2f(0.75f , 0.75f ); glVertex3f( P1.x , P1.y , BottomRight.z);
        glEnd();
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
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
        if (Frame) Frame->Render(&EffectivePosition);

        Item = SubFrameList.GetNext(Item);
      }
    }

  }

}


void ZVoxelViewBox::DropItem(ZFrame * Item, UShort nButton)
{
  if (Item->FrameType == MulticharConst('I','B','O','X'))
  {
    ZInventoryBox * IbItem;
    UShort * In_VoxelType;

    IbItem = (ZInventoryBox *) Item;
    In_VoxelType = IbItem->GetVoxelType();

    if (*In_VoxelType != *VoxelType && *VoxelType >= 32768)
    {
          *VoxelType = *In_VoxelType;;
    }
  }
  // printf("Dropped : %lx in %lx\n",(ULong)Item, (ULong)this);
}
