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
 * ZGame_GUI.h
 *
 *  Created on: 2 juil. 2011
 *      Author: laurent
 */

#ifndef Z_ZGAMEWINDOW_INVENTORY_H
#define Z_ZGAMEWINDOW_INVENTORY_H

//#ifndef Z_ZGAMEWINDOW_INVENTORY_H
//#  include "ZGameWindow_Inventory.h"
//#endif

#ifndef Z_ZGUI_H
#  include "ZGui.h"
#endif

#ifndef Z_ZGUI_INVENTORYBOX_H
#  include "ZGui_InventoryBox.h"
#endif

#ifndef Z_GUI_FONTFRAME_H
#  include "ZGui_FontFrame.h"
#endif

class ZGame;

class ZGameWindow_Inventory
{
  protected:
    ZFrame        MainWindow;
    ZInventoryBox IBox;
    ZInventoryBox IBox2;
    ZInventoryBox IBox3;
    ZGame * GameEnv;
    bool Flag_Shown;

    ZFrame_FontFrame MainTitle;
    ZFrame_FontFrame ToolTitle;
    ZFrame_FontFrame InventoryTitle;
    ZFrame_FontFrame PowerTitle;
    ZInventoryBox * MainInventory;

    ZString Text_MainTitle;
    ZString Text_ToolTitle;
    ZString Text_InventoryTitle;
    ZString Text_PowerTitle;


    UShort i1,i2,i3;
    ULong Q1,Q2,Q3;
  public:

  ZGameWindow_Inventory()
  {
    MainInventory = new ZInventoryBox[128];
    Flag_Shown = false;
    Text_MainTitle = "INVENTORY";
    Text_ToolTitle = "TOOLS";
    Text_InventoryTitle = "INVENTORY";
    Text_PowerTitle = "POWERS";
  }

  ~ZGameWindow_Inventory()
  {
    if (MainInventory) delete [] MainInventory;
  }

  void SetGameEnv(ZGame * GameEnv) {this->GameEnv = GameEnv;}

  void Show();


  void Hide();


  bool Is_Shown() {return(Flag_Shown);}

};


#endif /* Z_ZGAMEWINDOW_INVENTORY_H */
