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
 * ZGui_CyclingChoiceBox.cpp
 *
 *  Created on: 1 juin 2011
 *      Author: laurent
 */

#  include "ZGui_CyclingChoiceBox.h"


ZFrame_CyclingChoiceBox::ZFrame_CyclingChoiceBox()
{
  FrameType = MulticharConst('C','Y','C','L');
  FontTileStyle = 0;
  GUITileSet = 0;
  Flag_Show_Frame = false;
  Event_ChoiceChange = false;
  ChoiceList = 0;
  ChoiceCount = 0;
  ActualChoice = 0;
}

void ZFrame_CyclingChoiceBox::GetTextDisplaySize(ZVector2f * OutSize)
{
  ZVector2f Out;
  ULong i;

  OutSize->x=0;
  OutSize->y=0;
  if (FontTileStyle)
  {
    Arrow_Left_Size.x = GUITileSet->GetTileWidth(2);
    Arrow_Left_Size.y = GUITileSet->GetTileHeight(2);
    Arrow_Right_Size.x = GUITileSet->GetTileWidth(3);
    Arrow_Right_Size.y = GUITileSet->GetTileHeight(3);

    for (i=0;i<ChoiceCount;i++)
    {
      FontTileStyle->TileSet->GetFontRenderSize(FontTileStyle,ChoiceList[i], &Out);
      if (Out.x > OutSize->x) OutSize->x = Out.x;
      if (Out.y > OutSize->y) OutSize->y = Out.y;
    }
  }
  OutSize->x += Arrow_Left_Size.x + Arrow_Right_Size.x;
  if (OutSize->y < Arrow_Left_Size.y ) OutSize->y = Arrow_Left_Size.y;
  if (OutSize->y < Arrow_Right_Size.y ) OutSize->y = Arrow_Right_Size.y;

}

void ZFrame_CyclingChoiceBox::SetChoiceList(char ** ChoiceList, ULong ChoiceCount)
{
  this->ChoiceList = ChoiceList; this->ChoiceCount = ChoiceCount;
}

void ZFrame_CyclingChoiceBox::SetChoice(ULong Choice)
{
  ActualChoice = Choice;
}

void ZFrame_CyclingChoiceBox::SetSize    (float Width, float Height)
{
  Arrow_Left_Size.x = GUITileSet->GetTileWidth(2);
  Arrow_Left_Size.y = GUITileSet->GetTileHeight(2);
  Arrow_Right_Size.x = GUITileSet->GetTileWidth(3);
  Arrow_Right_Size.y = GUITileSet->GetTileHeight(3);

  Arrow_Left.SetPosition(0.0f,0.0f);
  Arrow_Left.SetSize(Arrow_Left_Size.x,Arrow_Left_Size.y);
  Text.SetPosition(Arrow_Left_Size.x,0.0f);
  Text.SetSize(Width - Arrow_Left_Size.x - Arrow_Right_Size.x, Height);
  Arrow_Right.SetPosition(Width-Arrow_Right_Size.x,0.0f);
  Arrow_Right.SetSize(Arrow_Right_Size.x,Arrow_Right_Size.y);
  Dimensions.Width = Width ; Dimensions.Height = Height;
}

void ZFrame_CyclingChoiceBox::AddedToFrameCallback(ZFrame * Frame)
{
  AddFrame(&Arrow_Left);
  AddFrame(&Arrow_Right);
  AddFrame(&Text);
  Text.SetDisplayText(ChoiceList[ActualChoice]);
  Arrow_Left.SetTile(0);
  Arrow_Right.SetTile(1);
};

void ZFrame_CyclingChoiceBox::SetFontTileStyle( ZTileStyle * TileStyle )
{
  FontTileStyle = TileStyle;
  Text.SetStyle(TileStyle);
}

void ZFrame_CyclingChoiceBox::SetGUITileset( ZTileSet   * Tileset   )
{
  GUITileSet = Tileset;
  Arrow_Left.SetTileSet(Tileset);
  Arrow_Right.SetTileSet(Tileset);
}

Bool ZFrame_CyclingChoiceBox::MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y)
{
  Bool Res;
  Res = ZFrame::MouseButtonClick(nButton, Absolute_x, Absolute_y);

  if (Arrow_Right.Is_MouseClick(true))
  {
    if (ActualChoice < (ChoiceCount-1) )
    {
      ActualChoice++;
      Text.SetDisplayText(ChoiceList[ActualChoice]);
      Event_ChoiceChange = true;
    }
  }
  if (Arrow_Left.Is_MouseClick(true))
  {

    if (ActualChoice > 0 )
    {
      ActualChoice--;
      Text.SetDisplayText(ChoiceList[ActualChoice]);
      Event_ChoiceChange = true;
    }
  }

  if (Text.Is_MouseClick(true))
  {
    if (ActualChoice < (ChoiceCount-1) )
    {
      ActualChoice++;
      Text.SetDisplayText(ChoiceList[ActualChoice]);
      Event_ChoiceChange = true;
    }
  }

  return (Res);
}
