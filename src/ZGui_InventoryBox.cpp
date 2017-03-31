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
 * ZGui_InventoryBox.cpp
 *
 *  Created on: 4 juil. 2011
 *      Author: laurent
 */

#include "ZGui_InventoryBox.h"
#include <GL/glew.h>
#include "SDL/SDL.h"

void ZInventoryBox::Render(Frame_Dimensions * ParentPosition)
{

  ZVector3f TopLeft, BottomRight;
  ZListItem * Item;
  ZFrame * Frame;
  ZTileStyle * UsedTileStyle;

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
      else              glColor4f(0.4f, 0.4f, 0.4f, 0.6f); //  glColor4f(0.7f, 0.7f, 0.7f, 0.9f);
      glBegin(GL_TRIANGLES);
        glTexCoord2f(0.25f , 0.25f ); glVertex3f( P1.x , P1.y , TopLeft.z);
        glTexCoord2f(0.50f , 0.25f ); glVertex3f( P2.x , P2.y , TopLeft.z);
        glTexCoord2f(0.50f , 0.50f ); glVertex3f( P3.x , P3.y , TopLeft.z);
        glTexCoord2f(0.50f , 0.50f ); glVertex3f( P3.x , P3.y , BottomRight.z);
        glTexCoord2f(0.25f , 0.50f ); glVertex3f( P4.x , P4.y , BottomRight.z);
        glTexCoord2f(0.25f , 0.25f ); glVertex3f( P1.x , P1.y , BottomRight.z);
      glEnd();
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

      // Rendering Quantity


      if (*Quantity > 1)
      {
        ZString QuantityText;

        QuantityText = *Quantity;

        if (QuantityText.Len <=  ReducedFontThreshold) UsedTileStyle = TileStyle;
        else                                           UsedTileStyle = (TileStyle_Reduced) ? TileStyle_Reduced : TileStyle;

        ZBox3f FontBox;
        ZVector2f Dim;

        UsedTileStyle->TileSet->GetFontRenderSize(UsedTileStyle, QuantityText.String, &Dim );

        FontBox.Position_x = BottomRight.x - Dim.x - 1.0f;
        FontBox.Position_y = BottomRight.y - Dim.y - 1.0f;
        FontBox.Position_z = TopLeft.z + 2.0f;
        FontBox.Width  = Dim.x;
        FontBox.Height = Dim.y;
        FontBox.Depth  = 0.0f;
        ZColor3f Color;
        ZColor3f BkColor;
        Color.r = 255.0f; Color.v = 255.0f; Color.b = 255.0f;
        BkColor.r = 0.0f; BkColor.v = 0.0f; BkColor.b = 0.0f;
        UsedTileStyle->TileSet->RenderFont(UsedTileStyle, &FontBox, QuantityText.String, &Color );
        FontBox.Position_z -= 1.0f;
        FontBox.Position_x -= 1.0f;
        FontBox.Position_y -= 1.0f;

        UsedTileStyle->TileSet->RenderFont(UsedTileStyle, &FontBox, QuantityText.String, &BkColor );
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


void ZInventoryBox::DropItem(ZFrame * Item, UShort nButton)
{
  if (Item->FrameType == MulticharConst('I','B','O','X'))
  {
    ZInventoryBox * IbItem;
    UShort * In_VoxelType, Temp_VoxelType;
    ULong  * In_Quantity,  Temp_Quantity;

    IbItem = (ZInventoryBox *) Item;

    In_VoxelType = IbItem->GetVoxelType();
    In_Quantity = IbItem->GetQuantity();

    if (*In_VoxelType != *VoxelType && *Quantity > 0)
    {
        if (nButton == 1)
        {
          Temp_VoxelType = *In_VoxelType;
          Temp_Quantity  = *In_Quantity;
          *In_VoxelType = *VoxelType;
          *In_Quantity  = *Quantity;
          *VoxelType = Temp_VoxelType;
          *Quantity  = Temp_Quantity;
        }
    }

    else if (*In_VoxelType == *VoxelType || *Quantity == 0)
    {
      if (nButton == 1)
      {
        *Quantity += *In_Quantity;
        *VoxelType = *In_VoxelType;
        *In_Quantity = 0;
        *In_VoxelType = 0;
      }
      if (nButton == 3)
      {
        ULong TransfertQuantity = 1;
        if (GuiManager->EventManager->Keyboard_Matrix[SDLK_LSHIFT] ||
            GuiManager->EventManager->Keyboard_Matrix[SDLK_RSHIFT]    )  TransfertQuantity = 10;
        if (GuiManager->EventManager->Keyboard_Matrix[SDLK_LCTRL]  ||
            GuiManager->EventManager->Keyboard_Matrix[SDLK_RCTRL]     ) TransfertQuantity = 100;
        if (GuiManager->EventManager->Keyboard_Matrix[SDLK_LALT]   ||
            GuiManager->EventManager->Keyboard_Matrix[SDLK_RALT]      ) TransfertQuantity = 1000;
        if (TransfertQuantity>*In_Quantity) TransfertQuantity = *In_Quantity;
        if (*In_Quantity>1)
        {
          (*Quantity)+= TransfertQuantity;
          *VoxelType = *In_VoxelType;
          (*In_Quantity)-=TransfertQuantity;

        }
      }
    }

  }
  // printf("Dropped : %lx in %lx\n",(ULong)Item, (ULong)this);
}
