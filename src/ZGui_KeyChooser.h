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
 * ZGui_KeyChooser.h
 *
 *  Created on: 15 juin 2011
 *      Author: laurent
 */

#ifndef Z_ZGUI_KEYCHOOSER_H
#define Z_ZGUI_KEYCHOOSER_H

//#ifndef Z_ZGUI_KEYCHOOSER_H
//#  include "ZGui_KeyChooser.h"
//#endif

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif


#ifndef Z_ZGUI_H
#  include "ZGui.h"
#endif

#ifndef Z_ZTILESETS_H
#  include "ZTileSets.h"
#endif

#ifndef Z_GUI_FONTFRAME_H
#  include "ZGui_FontFrame.h"
#endif

#ifndef Z_ZGUI_TILEFRAME_H
#  include "ZGui_TileFrame.h"
#endif

class ZFrame_KeyChooser : public ZFrame
{
  protected:
    ZTileStyle * FontTileStyle;
    ZTileSet   * GUITileSet;
    UShort     Actual_KeySet;
    ZString    DisplayText;
  public:
    // ZTileFrame       BackTile;
    ZFrame_FontFrame Text;


    // Events

    bool Event_KeyChanged;

    ZFrame_KeyChooser()
    {
      FrameType = MulticharConst('K','C','H','O');
      FontTileStyle = 0;
      GUITileSet = 0;
      //BackTile.SetTile(6);
      Actual_KeySet = 65;
      Text.SetDisplayText("A");
      Flag_Show_Frame = false;
      Flag_Enable_Proc_GetFocus = true;
      Flag_Enable_Proc_KeyDown = true;
      Event_KeyChanged = 0;
    }

    virtual void SetFontTileStyle( ZTileStyle * TileStyle )
    {
      FontTileStyle = TileStyle;
      Text.SetStyle(TileStyle);
    }

    virtual void SetGUITileset( ZTileSet   * TileSet   )
    {
      GUITileSet = TileSet;
      // BackTile.SetTileSet(TileSet);
    }

    virtual void AddedToFrameCallback(ZFrame * Frame)
    {
      // AddFrame(&BackTile);
      AddFrame(&Text);
    };

    virtual void GetTextDisplaySize(ZVector2f * OutSize)
    {
      Text.SetDisplayText("(HIT NEW KEY) ");
      Text.GetTextDisplaySize(OutSize);
      Text.SetDisplayText(DisplayText.String);
    }

    virtual void SetSize    (float Width, float Height)
    {
      // BackTile.SetPosition(0.0f,0.0f);
      Text.SetPosition(0.0f,0.0f);
      // BackTile.SetSize(Width,Height);
      Text.SetSize(Width+100.0f,Height);
      Dimensions.Width = Width ; Dimensions.Height = Height;
    }

    virtual void SetKey(UShort Key);
    virtual UShort GetKey() { return(Actual_KeySet); }


    virtual void GetFocus();
    virtual Bool KeyDown( UShort KeySym );
    virtual void KeyChanged() {}
    bool Is_KeyChanged (bool Reset = true)     { bool res; res = Event_KeyChanged;  if (Reset) Event_KeyChanged = false;  return(res); }

};

#endif /* Z_ZGUI_KEYCHOOSER_H */
