/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2016 Laurent Thiebaut & Olivia Merle
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
 * ZGameWindow_Scan.h
 *
 *  Created on: 14 juil. 2016
 *      Author: laurent
 */

#ifndef Z_ZGAMEWINDOW_SCAN_H
#define Z_ZGAMEWINDOW_SCAN_H

//#ifndef Z_ZGAMEWINDOW_SCAN_H
//#  include "ZGameWindow_Scan.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

#ifndef Z_ZGUI_H
#  include "ZGui.h"
#endif

#ifndef Z_GUI_FONTFRAME_H
#  include "ZGui_FontFrame.h"
#endif

class ZGame;

class ZGameWindow_Scan
{
  protected:
    ZFrame  MainWindow;
    ZGame * GameEnv;
    bool    Flag_Shown;

    ZFrame_FontFrame Infos;

    ZString Text_Infos;

  public:

  ZGameWindow_Scan()
  {
    Flag_Shown = false;
    GameEnv = 0;
  }

  ~ZGameWindow_Scan()
  {
  }

  void SetGameEnv(ZGame * GameEnv) {this->GameEnv = GameEnv;}

  void Show();

  void SetText(ZString * TextToDisplay)
  {
    this->Text_Infos = *TextToDisplay;
    Infos.SetDisplayText(Text_Infos.String);
  }

  void Hide();


  bool Is_Shown() {return(Flag_Shown);}

};



#endif /* Z_ZGAMEWINDOW_SCAN_H */
