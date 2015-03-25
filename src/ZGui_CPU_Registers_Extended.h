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
 * ZGui_CPU_Registers_Extended.h
 *
 *  Created on: 1 févr. 2015
 *      Author: laurent
 */

#ifndef Z_ZGUI_CPU_REGISTERS_EXTENDED_H
#define Z_ZGUI_CPU_REGISTERS_EXTENDED_H

//#ifndef Z_ZGUI_CPU_REGISTERS_EXTENDED_H
//#  include "ZGui_CPU_Registers_Extended.h"
//#endif

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

#ifndef Z_ZGUI_H
#  include "ZGui.h"
#endif

#ifndef Z_GUI_FONTFRAME_H
#  include "ZGui_FontFrame.h"
#endif

class ZVMachine;

#define ZCPUREGISTERSEXTENDED_MAXLINECOUNT 50

class ZCPURegisters_Extended : public ZFrame
{
    ZGame * GameEnv;
    ZVMachine * VMachine;
    ZFrame_FontFrame Display[ZCPUREGISTERSEXTENDED_MAXLINECOUNT];
    ZString          DisplayText[ZCPUREGISTERSEXTENDED_MAXLINECOUNT];
    float Margin;
    ULong LineHeight;
    ULong DisplaySlots;
  public:

    ZCPURegisters_Extended ()
    {
      FrameType = MulticharConst('C','R','E','X');
      VMachine = 0;
      Margin = 2.0f;
      LineHeight = 12.0;
      GameEnv = 0;
      DisplaySlots = 0;
    }

    virtual void AddedToFrameCallback(ZFrame * Frame);

    virtual void GetEffectiveHeight(float & Height);

    void SetVMachine(ZVMachine * VMachine) {this->VMachine = VMachine;}

    void SetGameEnv(ZGame * GameEnv) { this->GameEnv = GameEnv; }

    virtual void SetSize    (float Width, float Height);

    virtual void Render(Frame_Dimensions * ParentPosition);

};


#endif /* SRC_ZGUI_CPU_REGISTERS_EXTENDED_H_ */
