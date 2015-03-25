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
 * ZGameWindow_AsmExtendedRegisters.h
 *
 *  Created on: 1 févr. 2015
 *      Author: laurent
 */

#ifndef Z_ZGAMEWINDOW_ASMEXTENDEDREGISTERS_H
#define Z_ZGAMEWINDOW_ASMEXTENDEDREGISTERS_H

//#ifndef Z_ZGAMEWINDOW_ASMEXTENDEDREGISTERS_H
//#  include "ZGameWindow_AsmExtendedRegisters.h"
//#endif

#ifndef Z_ZGUI_H
#  include "ZGui.h"
#endif

#ifndef Z_ZGUI_DISASSEMBLER_H
#  include "ZGui_Disassembler.h"
#endif

#ifndef Z_ZGUI_CPU_REGISTERS_EXTENDED_H
#  include "ZGui_CPU_Registers_Extended.h"
#endif

#ifndef Z_ZGUI_ASMROBOTDISPLAY_H
#include "ZGui_AsmRobotDisplay.h"
#endif

#ifndef Z_ZGUI_PANELTEXTBUTTON_H
#  include "ZGui_PanelTextButton.h"
#endif

#ifndef Z_GUI_FONTFRAME_H
#  include "ZGui_FontFrame.h"
#endif

#ifndef Z_ZGUI_TILEFRAME_H
#  include "ZGui_TileFrame.h"
#endif

#ifndef Z_ZGUI_PROGRESSBAR_H
#  include "ZGui_ProgressBar.h"
#endif

#ifndef Z_ZVOXELEXTENSION_PROGROBOT_ASM_H
#  include "ZVoxelExtension_ProgRobot_Asm.h"
#endif

#ifndef Z_ZGUI_MEMORY_MONITOR_H
#  include "ZGui_Memory_Monitor.h"
#endif


class ZGame;

class ZGameWindow_AsmExtendedRegisters : public ZFrame
{
  protected:
    ZFrame           * MainWindow;
    ZTileFrame         CloseBox;
    ZFrame_FontFrame   Title;
    ZFrame_FontFrame   Title2;
    ZCPURegisters_Extended RegisterDisplay;
    ZString            Text_Title;
    ZString            Text_Title2;

    ZFrame_MemoryMonitor MemoryMonitor;

    ZFrame_FontFrame   Servo_x_Value;
    ZString            Servo_x_Value_Text;
    ZFrame_ProgressBar Servo_x_Position;

    ZGame              * GameEnv;
    ZVoxelExtension_ProgRobot_Asm * VoxelExtension;
    bool    Flag_Shown;
  public:

  ZGameWindow_AsmExtendedRegisters()
  {
    MainWindow = this;
    GameEnv = 0;
    Flag_Shown = false;
    VoxelExtension = 0;
    Text_Title       = "REGISTERS";
    Text_Title2      = "MEMORY MONITOR";
    Servo_x_Value_Text = "10000";
  }

  ~ZGameWindow_AsmExtendedRegisters()
  {

  }

  void SetGameEnv(ZGame * GameEnv) {this->GameEnv = GameEnv;}
  void SetVoxelExtension(ZVoxelExtension * Extension) { VoxelExtension = (ZVoxelExtension_ProgRobot_Asm *)Extension; }
  ZVoxelExtension * GetVoxelExtension() { return(VoxelExtension);}

  void Show();
  void Hide();
  bool Is_Shown() {return(Flag_Shown);}

  virtual void Render(Frame_Dimensions * ParentPosition);


  // Overloaded events

  virtual Bool MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y);



};




#endif /* Z_ZGAMEWINDOW_ASMEXTENDEDREGISTERS_H */
