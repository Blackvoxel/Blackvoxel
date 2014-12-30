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
 * ZGameWindow_AsmDebug.h
 *
 *  Created on: 19 déc. 2014
 *      Author: laurent
 */

#ifndef Z_ZGAMEWINDOW_ASMDEBUG_H
#define Z_ZGAMEWINDOW_ASMDEBUG_H

//#ifndef Z_ZGAMEWINDOW_ASMDEBUG_H
//#  include "ZGameWindow_AsmDebug.h"
//#endif

#ifndef Z_ZGUI_H
#  include "ZGui.h"
#endif

#ifndef Z_ZGUI_DISASSEMBLER_H
#  include "ZGui_Disassembler.h"
#endif

#ifndef Z_ZGUI_CPU_REGISTERS_H
#  include "ZGui_CPU_Registers.h"
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

#ifndef Z_ZVOXELEXTENSION_PROGROBOT_ASM_H
#  include "ZVoxelExtension_ProgRobot_Asm.h"
#endif

class ZGame;

class ZGameWindow_AsmDebug : public ZFrame
{
  protected:
    ZFrame           * MainWindow;
    ZTileFrame         CloseBox;
    ZFrame_FontFrame   Title;
    ZPanelTextButton   Button_Step;
    ZPanelTextButton   Button_Run;
    ZPanelTextButton   Button_Stop;
    ZPanelTextButton   Button_Reset;
    ZPanelTextButton   Button_HardwarePannel;

    ZFrame_FontFrame   Title_CPURegisters;
    ZFrame_FontFrame   Title_Disassembly;

    ZString            Text_Title;
    ZString            Text_Button_Step;
    ZString            Text_Button_Run;
    ZString            Text_Button_Stop;
    ZString            Text_Button_Reset;
    ZString            Text_Button_HardwarePannel;
    ZString            Text_Title_CPURegisters;
    ZString            Text_Title_Disassembly;
    ZDisassemblerFrame DisassemblerFrame;
    ZCPURegisters      CPURegisters;
    ZGame              * GameEnv;
    ZVoxelExtension_ProgRobot_Asm * VoxelExtension;
    bool    Flag_Shown;
  public:

  ZGameWindow_AsmDebug()
  {
    MainWindow = this;
    GameEnv = 0;
    Flag_Shown = false;
    VoxelExtension = 0;
    Text_Title       = "ROBOT MONITOR";
    Text_Button_Step = "STEP";
    Text_Button_Run  = "RUN";
    Text_Button_Stop = "STOP";
    Text_Button_Reset= "RESET";
    Text_Button_HardwarePannel = "HARDWARE";
    Text_Title_CPURegisters = "CPU REGISTERS";
    Text_Title_Disassembly = "DISASSEMBLY";
  }

  ~ZGameWindow_AsmDebug()
  {

  }

  void SetGameEnv(ZGame * GameEnv) {this->GameEnv = GameEnv;}
  void SetVoxelExtension(ZVoxelExtension * Extension) { VoxelExtension = (ZVoxelExtension_ProgRobot_Asm *)Extension; }
  ZVoxelExtension * GetVoxelExtension() { return(VoxelExtension);}


  void Show();


  void Hide();


  bool Is_Shown() {return(Flag_Shown);}

  // Overloaded events

  virtual Bool MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y);


};


#endif /* Z_ZGAMEWINDOW_ASMDEBUG_H */
