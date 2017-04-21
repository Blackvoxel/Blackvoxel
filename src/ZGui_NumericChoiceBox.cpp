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
 * ZGui_NumericChoiceBox.cpp
 *
 *  Created on: 7 juin 2011
 *      Author: laurent
 */

#include "ZGui_NumericChoiceBox.h"
#include <math.h>
#include "z/ZString.h"


ZFrame_NumericChoiceBox::ZFrame_NumericChoiceBox()
{
  FrameType = MulticharConst('N','C','B','X');
  FontTileStyle = 0;
  GUITileSet = 0;
  Flag_Show_Frame = false;
  Event_ChoiceChange = false;

  ActualChoice = 0.0;
  Step = 1.0;
  LowerLimit = 0.05;
  UpperLimit = 100.0;
  FracDecimals = 0;
  MinDigits = 0;
  DisplayString.SetLen(64);
  DisplayString.String[0]=0;
}

void ZFrame_NumericChoiceBox::GetTextDisplaySize(ZVector2f * OutSize)
{
  ZVector2f Out;
  ZString As;

  OutSize->x=0;
  OutSize->y=0;
  if (FontTileStyle)
  {
    Arrow_Left_Size.x = GUITileSet->GetTileWidth(2);
    Arrow_Left_Size.y = GUITileSet->GetTileHeight(2);
    Arrow_Right_Size.x = GUITileSet->GetTileWidth(3);
    Arrow_Right_Size.y = GUITileSet->GetTileHeight(3);

    As << ConvertValue(UpperLimit);
    FontTileStyle->TileSet->GetFontRenderSize(FontTileStyle, As.String , &Out);
    OutSize->x += Out.x;
    OutSize->y += Out.y;

    OutSize->x += Arrow_Left_Size.x + Arrow_Right_Size.x;
    if (OutSize->y < Arrow_Left_Size.y ) OutSize->y = Arrow_Left_Size.y;
    if (OutSize->y < Arrow_Right_Size.y ) OutSize->y = Arrow_Right_Size.y;
  }

}

void ZFrame_NumericChoiceBox::SetParameters(double LowerLimit, double UpperLimit, double InitialValue, double Step, ULong Decimals)
{
  this->Step = Step;
  this->FracDecimals = Decimals;
  this->LowerLimit = LowerLimit;
  this->UpperLimit = UpperLimit;
  SetValue(InitialValue);
}

void ZFrame_NumericChoiceBox::SetValue(double Value)
{
  ActualChoice = Value;
  DisplayString = ConvertValue(ActualChoice);


}

ZString ZFrame_NumericChoiceBox::ConvertValue(double Value)
{
  double Frac,Int;

  Value+=0.00001; // Fix a rounding error problem.

  Frac = modf( Value, &Int );

  ZString As,Bs,Cs,Ds;

  As = Frac;
  As.Split ('.',Cs);
  Bs << (ULong)Int;
  if (MinDigits && (Bs.Len < MinDigits) ) { Cs.SetLen(MinDigits-Bs.Len); Cs.Fill('0'); Ds = Cs; Ds << Bs; Bs = Ds; }
  if (FracDecimals>0)
  {
    Bs << "." << As.Lefts(FracDecimals);
  }

  return(Bs);
}

void ZFrame_NumericChoiceBox::SetSize    (float Width, float Height)
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

void ZFrame_NumericChoiceBox::AddedToFrameCallback(ZFrame * Frame)
{
  AddFrame(&Arrow_Left);
  AddFrame(&Arrow_Right);
  AddFrame(&Text);
  Text.SetDisplayText(DisplayString.String);
  Arrow_Left.SetTile(0);
  Arrow_Right.SetTile(1);
};

void ZFrame_NumericChoiceBox::SetFontTileStyle( ZTileStyle * TileStyle )
{
  FontTileStyle = TileStyle;
  Text.SetStyle(TileStyle);
}

void ZFrame_NumericChoiceBox::SetGUITileset( ZTileSet   * Tileset   )
{
  GUITileSet = Tileset;
  Arrow_Left.SetTileSet(Tileset);
  Arrow_Right.SetTileSet(Tileset);
}

Bool ZFrame_NumericChoiceBox::MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y)
{
  Bool Res;
  Res = ZFrame::MouseButtonClick(nButton, Absolute_x, Absolute_y);

  if (Arrow_Right.Is_MouseClick(true))
  {
    if (ActualChoice <= (UpperLimit - Step) )
    {
      SetValue ( ActualChoice + Step );
      Event_ChoiceChange = true;
    }
  }
  if (Arrow_Left.Is_MouseClick(true))
  {
    if (ActualChoice >= (LowerLimit + Step) )
    {
      SetValue( ActualChoice - Step );
      Event_ChoiceChange = true;
    }
  }

  if (Text.Is_MouseClick(true))
  {
    if (ActualChoice <= (LowerLimit - Step) )
    {
      SetValue ( ActualChoice + Step );
      Event_ChoiceChange = true;
    }
  }

  return (Res);
}
