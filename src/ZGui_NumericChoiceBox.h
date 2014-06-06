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
 * ZGui_NumericChoiceBox.h
 *
 *  Created on: 7 juin 2011
 *      Author: laurent
 */

#ifndef ZGUI_ZNUMERICCHOICEBOX_H
#define ZGUI_ZNUMERICCHOICEBOX_H

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

//#ifndef ZGUI_ZNUMERICCHOICEBOX_H
//#  include "ZGui_NumericChoiceBox.h"
//#endif

#ifndef Z_ZGUI_H
#  include "ZGui.h"
#endif

#ifndef Z_GUI_FONTFRAME_H
#  include "ZGui_FontFrame.h"
#endif

#ifndef Z_ZGUI_TILEFRAME_H
#  include "ZGui_TileFrame.h"
#endif

class ZFrame_NumericChoiceBox : public ZFrame
{
  protected:
    ZTileStyle * FontTileStyle;
    ZTileSet   * GUITileSet;
    double       ActualChoice;
    double       Step;
    double       LowerLimit;
    double       UpperLimit;
    ULong        FracDecimals;
    ULong        MinDigits;

    ZString      DisplayString;
    ZString      ConvertValue(double Value);

  public:

  // Events
    bool Event_ChoiceChange;

  // SubGadgets

    ZTileFrame Arrow_Left;
    ZTileFrame Arrow_Right;
    ZFrame_FontFrame Text;
    ZVector2f  Arrow_Left_Size;
    ZVector2f  Arrow_Right_Size;

    ZFrame_NumericChoiceBox();
    virtual void GetTextDisplaySize(ZVector2f * OutSize);
    virtual void SetSize    (float Width, float Height);
    virtual void SetFontTileStyle( ZTileStyle * TileStyle );
    virtual void SetGUITileset( ZTileSet   * Tileset   );

    virtual void SetParameters(double LowerLimit, double UpperLimit, double InitialValue, double Step, ULong Decimals = 0);
    virtual void SetValue(double Value);
    virtual void SetMinDigits(ULong Digits) { MinDigits = Digits; }
    virtual double GetValue() { return(ActualChoice); }

    virtual bool Is_ChoiceChanged(bool Reset = true) { bool Res = Event_ChoiceChange; if (Reset) Event_ChoiceChange = false; return(Res);}

    // Events

    virtual Bool MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y);

    // Callbacks

    virtual void AddedToFrameCallback(ZFrame * Frame);

};




#endif /* ZGUI_ZNUMERICCHOICEBOX_H */
