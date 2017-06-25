/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2017 Laurent Thiebaut & Olivia Merle
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
 * ZGameWindow_ZProgRobot_Remote.h
 *
 *  Created on: 3 juin 2017
 *      Author: laurent
 */

#ifndef Z_ZGAMEWINDOW_ZPROGROBOT_REMOTE_H
#define Z_ZGAMEWINDOW_ZPROGROBOT_REMOTE_H

//#ifndef Z_ZGAMEWINDOW_ZPROGROBOT_REMOTE_H
//#  include "ZGameWindow_ZProgRobot_Remote.h"
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

#ifndef ZGUI_ZNUMERICCHOICEBOX_H
#  include "ZGui_NumericChoiceBox.h"
#endif

#ifndef Z_ZGUI_PANELTEXTBUTTON_H
#  include "ZGui_PanelTextButton.h"
#endif

#ifndef Z_ZVOXELTYPEMANAGER_H
#  include "ZVoxelTypeManager.h"
#endif

#ifndef Z_ZVOXELEXTENSION_PROGROBOT_REMOTE_H
#  include "ZVoxelExtension_ProgRobot_Remote.h"
#endif

class ZGame;

class ZGameWindow_ProgRobot_Remote : public ZFrame
{
  protected:
    ZFrame         * MainWindow;
    ZTileFrame       CloseBox;

    ZInventoryBox IBox;
    ZInventoryBox IBox2;
    ZInventoryBox IBox3;
    ZGame * GameEnv;
    bool Flag_Shown;

    ZFrame_FontFrame MainTitle;
    ZFrame_FontFrame RobotProgramTitle;
    ZFrame_NumericChoiceBox RobotProgramNum;
    ZPanelTextButton RobotProgramEdit;
    ZPanelTextButton RobotProgramLoad;
    ZPanelTextButton RobotProgramDebug;
    ZPanelTextButton RobotProgramDebugShow;
    ZPanelTextButton Manual;
    ZFrame_FontFrame ProgramName;

    ZFrame_FontFrame StorageTitle;
    ZFrame_FontFrame InventoryTitle;

    ZInventoryBox * MainStorage;

    ZString Text_MainTitle;
    ZString Text_RobotZoneTitle;
    ZString Text_StorageTitle;
    ZString Text_InventoryTitle;
    ZString Text_RobotProgramEdit;
    ZString Text_RobotProgramLoad;
    ZString Text_RobotProgramDebug;
    ZString Text_RobotProgramDebugShow;
    ZString Text_Manual;
    ZString Text_ProgramName;
    double  ProgramNum;

    ZVoxelExtension_ProgRobot_Remote * VoxelExtension;

    UShort i1,i2,i3;
    ULong Q1,Q2,Q3;
  public:

    ZGameWindow_ProgRobot_Remote()
  {
    MainWindow = this;
    GameEnv = 0;
    VoxelExtension = 0;
    i1=i2=i3 = 0;
    Q1=Q2=Q3 = 0;
    MainStorage = new ZInventoryBox[128];
    Flag_Shown = false;
    Text_MainTitle = "REMOTE ROBOT";
    Text_RobotZoneTitle = "ROBOT PROGRAM";
    Text_StorageTitle = "ROBOT STORAGE";
    Text_InventoryTitle = "INVENTORY";
    Text_RobotProgramEdit = "EDIT";
    Text_RobotProgramLoad = "LOAD-RUN";
    Text_RobotProgramDebug = "LOAD-DEBUG";
    Text_RobotProgramDebugShow = "DEBUG";
    Text_Manual = "MANUAL";
    ProgramNum = 0;
  }

  ~ZGameWindow_ProgRobot_Remote()
  {
    if (MainStorage) delete [] MainStorage;
  }

  void SetGameEnv(ZGame * GameEnv) {this->GameEnv = GameEnv;}

  void SetVoxelExtension(ZVoxelExtension * Extension) { VoxelExtension = (ZVoxelExtension_ProgRobot_Remote *)Extension; }
  ZVoxelExtension * GetVoxelExtension() { return(VoxelExtension);}
  void Show();
  void Hide();


  bool Is_Shown() {return(Flag_Shown);}

  // Overloaded events

  virtual Bool MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y);


  protected:

    void DisplayProgramName();

};



#endif /* Z_ZGAMEWINDOW_ZPROGROBOT_REMOTE_H */
