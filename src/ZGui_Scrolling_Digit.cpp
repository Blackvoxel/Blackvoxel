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
 * ZGui_Scrolling_Digit.cpp
 *
 *  Created on: 11 févr. 2015
 *      Author: laurent
 */

#include "ZGui_Scrolling_Digit.h"
#include "ZGame.h"

void ZFrame_ScrollingDigit::GetEffectiveSize(ZVector2f * OutSize)
{
  ZString As;

  OutSize->x=0;
  OutSize->y=0;

  ZVector3d Arrow_Up_Size, Arrow_Down_Size, Digit_Size;

  Arrow_Up_Size.x = GameEnv->Font_Symbols->GetTileWidth(1);
  Arrow_Up_Size.y = GameEnv->Font_Symbols->GetTileWidth(2);
  Arrow_Down_Size.x = GameEnv->Font_Symbols->GetTileWidth(1);
  Arrow_Down_Size.y = GameEnv->Font_Symbols->GetTileWidth(2);

  Digit_Size.x = GameEnv->Font_1->GetTileWidth('0');
  Digit_Size.y = GameEnv->Font_1->GetTileHeight('0');

  OutSize->y = Arrow_Up_Size.y + Arrow_Down_Size.y + Digit_Size.y;
  OutSize->x = (Arrow_Up_Size.x > Arrow_Down_Size.x) ? Arrow_Up_Size.x:Arrow_Down_Size.x;
  OutSize->x = (OutSize->x > Digit_Size.x) ? OutSize->x:Digit_Size.x;

}

void ZFrame_ScrollingDigit::SetSize    (float Width, float Height)
{
  ZVector2f Arrow_Up_Size, Arrow_Down_Size;

  Arrow_Up_Size.x = 8;
  Arrow_Up_Size.y = 8;
  Arrow_Down_Size.x = 8;
  Arrow_Down_Size.y = 8;

  Arrow_Up.SetPosition(0.0f,0.0f);
  Arrow_Up.SetSize(Arrow_Up_Size.x,Arrow_Up_Size.y);
  Digit.SetPosition(0.0f,Arrow_Up_Size.y);
  Digit.SetSize(Width+10.0f, Height- Arrow_Up_Size.y - Arrow_Down_Size.y+10.0f);
  Arrow_Down.SetPosition(0.0f,Height-Arrow_Down_Size.y);
  Arrow_Down.SetSize(Arrow_Down_Size.x,Arrow_Down_Size.y);
  Dimensions.Width = Width ; Dimensions.Height = Height;
}

void ZFrame_ScrollingDigit::AddedToFrameCallback(ZFrame * Frame)
{
  AddFrame(&Arrow_Up);
  AddFrame(&Arrow_Down);
  AddFrame(&Digit);
  Digit.SetDisplayText(Text_Digit.String);
  Digit.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  Arrow_Up.SetTileSet(GameEnv->Font_Symbols);
  Arrow_Down.SetTileSet(GameEnv->Font_Symbols);
  Arrow_Up.SetTile(1);
  Arrow_Down.SetTile(2);
};

void ZFrame_ScrollingDigit::Render(Frame_Dimensions * ParentPosition)
{
  ZMemSize Value;

  Value = *DigitAddress;
  Value = (Value>>(DigitPosition<<2)) & 0xf;
  Text_Digit.Clear().Append_HexNumber_Nibble(Value);
  Digit.SetDisplayText(Text_Digit.String);
  ZFrame::Render(ParentPosition);
}

Bool ZFrame_ScrollingDigit::MouseButtonClick(UShort nButton, Short Absolute_x, Short Absolute_y)
{
  Bool Res;
  Res = ZFrame::MouseButtonClick(nButton, Absolute_x, Absolute_y);

  if (Arrow_Up.Is_MouseClick())
  {
    ZMemSize Value, MaskPos, MaskNeg;

    MaskPos = 0xf << (DigitPosition << 2);
    MaskNeg = ~MaskPos;

    Value = *DigitAddress & MaskPos;
    Value = Value >> (DigitPosition << 2);
    Value += 1;
    Value &= 0xf;

    Value = Value << (DigitPosition << 2);
    *DigitAddress &= MaskNeg;
    *DigitAddress |= Value;
  }

  if (Arrow_Down.Is_MouseClick())
  {
    ZMemSize Value, MaskPos, MaskNeg;

    MaskPos = 0xf << (DigitPosition << 2);
    MaskNeg = ~MaskPos;

    Value = *DigitAddress & MaskPos;
    Value = Value >> (DigitPosition << 2);
    Value -= 1;
    Value &= 0xf;

    Value = Value << (DigitPosition << 2);
    *DigitAddress &= MaskNeg;
    *DigitAddress |= Value;
  }

  return (Res);
}
