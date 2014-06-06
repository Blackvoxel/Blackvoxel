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
 * ZGameWindow_VoxelTypeBar.h
 *
 *  Created on: 12 juil. 2011
 *      Author: laurent
 */

#ifndef Z_ZGAMEWINDOW_VOXELTYPEBAR_H
#define Z_ZGAMEWINDOW_VOXELTYPEBAR_H

//#ifndef Z_ZGAMEWINDOW_VOXELTYPEBAR_H
//#  include "ZGameWindow_VoxelTypeBar.h"
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

#ifndef Z_ZVOXELTYPEMANAGER_H
#  include "ZVoxelTypeManager.h"
#endif

class ZGame;

class ZGameWindow_VoxelTypeBar
{
  protected:
    ZGame * GameEnv;
    ZFrame        MainWindow;
    bool Flag_Shown;


    ZInventoryBox * MainInventory;
  public:

  ZGameWindow_VoxelTypeBar()
  {
    MainInventory = new ZInventoryBox[128];
    Flag_Shown = false;
  }

  ~ZGameWindow_VoxelTypeBar()
  {
    if (MainInventory) delete [] MainInventory;
  }

  void SetGameEnv(ZGame * GameEnv) {this->GameEnv = GameEnv;}
  inline ZInventoryBox * GetInventoryBox(ULong BoxNum) { return(&MainInventory[BoxNum]);}

  void Show();

  void Hide();

  void Show_Slot(ULong SlotNum) { MainInventory[SlotNum].Show(true); }
  void Hide_Slot(ULong SlotNum) { MainInventory[SlotNum].Show(false); }

  bool Is_Shown() {return(Flag_Shown);}

};

#endif /* Z_ZGAMEWINDOW_VOXELTYPEBAR_H */
