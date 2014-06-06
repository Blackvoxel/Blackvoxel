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
 * ZGui_CheckBox.h
 *
 *  Created on: 3 juin 2011
 *      Author: laurent
 */

#ifndef Z_ZGUI_CHECKBOX_H
#define Z_ZGUI_CHECKBOX_H

//#ifndef Z_ZGUI_CHECKBOX_H
//#  include "ZGui_CheckBox.h"
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

class ZFrame_CheckBox : public ZFrame
{
  protected:
    ZTileSet   * GUITileSet;


  public:

  // Events
    bool Event_CheckBoxChange;
    bool CheckBoxState;

  // SubGadgets

    ZTileFrame CheckBox;

    ZFrame_CheckBox()
    {
      FrameType = MulticharConst('C','H','C','K');
      GUITileSet = 0;
      Flag_Show_Frame = false;
      Event_CheckBoxChange = false;
      CheckBoxState = false;
      CheckBox.SetTile(2);
    }

    virtual void SetSize    (float Width, float Height)
    {
      CheckBox.SetPosition(0.0f,0.0f);
      CheckBox.SetSize(Width,Height);
      Dimensions.Width = Width ; Dimensions.Height = Height;
    }

    virtual void AddedToFrameCallback(ZFrame * Frame)
    {
      AddFrame(&CheckBox);
    };

    virtual void SetState(bool state)
    {
      CheckBoxState = state;
      if (CheckBoxState) CheckBox.SetTile(3);
      else               CheckBox.SetTile(2);
    }

    virtual bool GetState() { return(CheckBoxState); }

    virtual bool Is_StateChanged( bool Reset = true )  { bool res; res = Event_CheckBoxChange;  if (Reset) Event_CheckBoxChange = false;  return(res); }
    virtual bool Is_GotChecked( bool Reset = true )
    {
      if (!Event_CheckBoxChange) return(false);
      if (Reset) Event_CheckBoxChange = false;
      return (CheckBoxState);
    }

    virtual bool Is_GotUnchecked( bool Reset = true )
    {
      if (!Event_CheckBoxChange) return(false);
      if (Reset) Event_CheckBoxChange = false;
      return (!CheckBoxState);
    }

    virtual void SetGUITileset( ZTileSet   * Tileset   ) { GUITileSet = Tileset; CheckBox.SetTileSet(Tileset);  }

    virtual Bool MouseButtonClick (UShort nButton, Short Absolute_x, Short Absolute_y)
    {
      Bool Res;
      Res = ZFrame::MouseButtonClick(nButton, Absolute_x, Absolute_y);

      if (CheckBox.Is_MouseClick(true))
      {
        Event_CheckBoxChange = true; SetState(!CheckBoxState);
      }
      return (Res);
    }

    virtual void CheckBoxChange()
    {

    }

};


#endif /* Z_ZGUI_CHECKBOX_H */
