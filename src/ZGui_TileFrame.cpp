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
 * ZGui_TileFrame.cpp
 *
 *  Created on: 1 juin 2011
 *      Author: laurent
 */

#include "ZGui_TileFrame.h"


void ZTileFrame::Render(Frame_Dimensions * ParentPosition)
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

    // Render this frame

    if (Flag_Show_Frame)
    {
      TopLeft.x = EffectivePosition.Position_x;
      TopLeft.y = EffectivePosition.Position_y;
      TopLeft.z = EffectivePosition.Position_z;
      BottomRight.x = EffectivePosition.Position_x + EffectivePosition.Width;
      BottomRight.y = EffectivePosition.Position_y + EffectivePosition.Height;
      BottomRight.z = EffectivePosition.Position_z;
      if (TileSet) TileSet->RenderTile(&TopLeft, &BottomRight, Tile, &DrawColor);
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
