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
 * ZGameWindow_Sequencer.h
 *
 *  Created on: 1 mai 2013
 *      Author: laurent
 */

#ifndef Z_GAMEWINDOW_SEQUENCER_H
#define Z_GAMEWINDOW_SEQUENCER_H


//#ifndef Z_GAMEWINDOW_SEQUENCER_H
//#  include "ZGameWindow_Sequencer.h"
//#endif

#ifndef Z_ZGUI_H
#  include "ZGui.h"
#endif

#ifndef Z_ZGUI_INVENTORYBOX_H
#  include "ZGui_InventoryBox.h"
#endif

#ifndef Z_ZGUI_VOXELQUANTITYBOX_H
#  include "ZGui_VoxelQuantityBox.h"
#endif

#ifndef Z_GUI_FONTFRAME_H
#  include "ZGui_FontFrame.h"
#endif

#ifndef ZGUI_ZNUMERICCHOICEBOX_H
#  include "ZGui_NumericChoiceBox.h"
#endif

#ifndef Z_ZVOXELTYPEMANAGER_H
#  include "ZVoxelTypeManager.h"
#endif

#ifndef Z_ZVOXELEXTENSION_SEQUENCER_H
#  include "ZVoxelExtension_Sequencer.h"
#endif

class ZGame;

class ZGameWindow_Sequencer : public ZFrame
{
  protected:
    ZFrame *      MainWindow;
    ZTileFrame    CloseBox;
    ZInventoryBox IBox;
    ZInventoryBox IBox2;
    ZInventoryBox IBox3;
    ZGame * GameEnv;
    bool Flag_Shown;

    ZFrame_FontFrame MainTitle;
    ZFrame_FontFrame InputTitle;
    ZFrame_FontFrame OutputTitle;
    ZFrame_NumericChoiceBox OutputNum;
    ZFrame_FontFrame SequenceTitle;
    ZFrame_FontFrame InventoryTitle;

    ZInventoryBox     * MainStorage;
    ZVoxelQuantityBox * SequencerStorage;

    ZString Text_MainTitle;
    ZString Text_InputTitle;
    ZString Text_OutputTitle;
    ZString Text_SequenceTitle;
    ZString Text_InventoryTitle;

    ZVoxelExtension_Sequencer * VoxelExtension_Sequencer;

    UShort i1,i2,i3;
    ULong Q1,Q2,Q3;
  public:

  ZGameWindow_Sequencer()
  {
    GameEnv = 0;
    MainWindow = this;
    MainStorage = new ZInventoryBox[60];
    SequencerStorage = new ZVoxelQuantityBox[80];
    Flag_Shown = false;
    Text_MainTitle = "SEQUENCER";
    Text_InputTitle = "INPUT";
    Text_OutputTitle = "OUTPUT SIDE";
    Text_SequenceTitle = "SEQUENCE";
    Text_InventoryTitle = "INVENTORY";
    VoxelExtension_Sequencer = 0;
    i1 = i2 = i3 = 0;
    Q1 = Q2 = Q3 = 0;
  }

  ~ZGameWindow_Sequencer()
  {
    if (MainStorage) delete [] MainStorage;
    if (SequencerStorage) delete [] SequencerStorage;
  }

  void SetGameEnv(ZGame * GameEnv) {this->GameEnv = GameEnv;}

  void SetVoxelExtension(ZVoxelExtension * Extension) { VoxelExtension_Sequencer = (ZVoxelExtension_Sequencer *)Extension; }

  void Show();
  void Hide();


  bool Is_Shown() {return(Flag_Shown);}

  // Overloaded events

  virtual Bool MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y);

};


#endif /* Z_ZGAMEWINDOW_SEQUENCER_H */
