/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2015 Laurent Thiebaut & Olivia Merle
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
 * ZGui_PanelTextMultiButton.h
 *
 *  Created on: 19 févr. 2015
 *      Author: laurent
 */

#ifndef Z_ZGUI_PANELTEXTMULTIBUTTON_H
#define Z_ZGUI_PANELTEXTMULTIBUTTON_H

//#ifndef Z_ZGUI_PANELTEXTMULTIBUTTON_H
//#  include "ZGui_PanelTextMultiButton.h"
//#endif

#ifndef Z_ZGUI_H
#  include "ZGui.h"
#endif

#ifndef Z_GUI_FONTFRAME_H
#  include "ZGui_FontFrame.h"
#endif

class ZPanelTextMultiButton : public ZFrame
{
    ZFrame_FontFrame ButtonText;
    float Margin_Top, Margin_Bottom, Margin_Left, Margin_Right;
    ZPanelTextMultiButton ** ButtonList;
    ULong AlternateTextureNum;
    ULong ButtonNumber;
    ULong * DataRef;
    bool  Pushed;
  public:

  ZPanelTextMultiButton()
  {
    FrameType = MulticharConst('M','B','T','N'); // = MultiButton
    AlternateTextureNum = 0;
    Margin_Top    = 5.0f;
    Margin_Bottom = 5.0f;
    Margin_Left   = 10.0f;
    Margin_Right  = 10.0f;
    ButtonText.SetPosition(Margin_Left,Margin_Top);
    ButtonNumber = 0;
    ButtonList = 0;
    DataRef = 0;
    this->SetTexture(10);
    this->SetAlternateTexture(17);
  }

  virtual void SetAlternateTexture(ULong AlternateTextureNum) {this->AlternateTextureNum = AlternateTextureNum; }
  virtual void SetButtonNumber(ULong ButtonNumber) {this->ButtonNumber = ButtonNumber; }
  virtual void SetDataRef(ULong * DataRef) {this->DataRef = DataRef; }
  virtual void SetButtonList(ZPanelTextMultiButton ** ButtonList) {this->ButtonList = ButtonList;}

  virtual void SetDisplayText( char * Text ) { ButtonText.SetDisplayText(Text); }
  virtual void SetTextStyle(ZTileStyle * TileStyle) { ButtonText.SetStyle(TileStyle); }
  virtual void SetMargins( float top, float right, float bottom, float left) { Margin_Top = top; Margin_Right = right; Margin_Bottom = bottom; Margin_Left = left; }
  virtual void GetEffectiveDisplaySize(ZVector2f * OutSize)
  {
    ButtonText.GetTextDisplaySize(OutSize);
    OutSize->x += Margin_Left + Margin_Right;
    OutSize->y += Margin_Top  + Margin_Bottom;
  }

  virtual void SetSize    (float Width, float Height)
  {
    ZFrame::SetSize(Width, Height);

    ButtonText.SetSize(Width - Margin_Left - Margin_Right, Height - Margin_Top - Margin_Bottom );
    ButtonText.SetPosition(Margin_Left,Margin_Top);
  }

  virtual void SetTextColor(float Red, float Green, float Blue)
  {
    ButtonText.SetColor(Red,Green,Blue);
  }

  virtual void AddedToFrameCallback(ZFrame * Frame)
  {
    AddFrame(&ButtonText);
  };

  virtual void Render(Frame_Dimensions * ParentPosition);

  Bool MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y);

};



#endif /* SRC_ZGUI_PANELTEXTMULTIBUTTON_H_ */
