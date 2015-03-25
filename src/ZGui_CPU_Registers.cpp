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
 * ZGui_CPU_Registers.cpp
 *
 *  Created on: 21 déc. 2014
 *      Author: laurent
 */

#include "ZGui_CPU_Registers.h"

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

void ZCPURegisters::GetEffectiveHeight(float & Height)
{
  ULong LineCount;

  LineCount = (VMachine->GetCPURegisterCount(0) >> 1) + (VMachine->GetCPURegisterCount(0) & 1);
  LineCount += 2; // Add the line for flags and a blank line between.

  Height = (float)(LineHeight * LineCount);
}

void ZCPURegisters::SetSize(float Width, float Height)
{
  ULong i;

  ZFrame::SetSize(Width, Height);

  DisplaySlots = VMachine->GetCPURegisterCount(0);
  DisplaySlots ++;
  if (DisplaySlots >= (ZCPUREGISTERS_MAXLINECOUNT)) DisplaySlots = ZCPUREGISTERS_MAXLINECOUNT;
  for (i=0;i<DisplaySlots;i++)
  {
    Display[i].SetPosition(Margin,Margin + LineHeight * i);
    Display[i].SetSize(Width-(Margin*2.0f), LineHeight);
    Display[i].SetColor(1.0f,1.0f,1.0f);
    Display[i].SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  }
}

void ZCPURegisters::AddedToFrameCallback(ZFrame * Frame)
{
  ULong i;

  for (i=0;i<DisplaySlots;i++)
  {
    AddFrame(&Display[i]);
  }
}

void ZCPURegisters::Render(Frame_Dimensions * ParentPosition)
{
  ZString Text, Opcode, Args, RegisterName, RegisterValue;
  ULong i,j, Slot, RegisterNum, Pad;
  ULong RegisterCount;

  RegisterValue.Set_DisplayBase(16);
  RegisterValue.Set_DisplayTrailingZero(true);

  RegisterCount = VMachine->GetCPURegisterCount(0);
  for (i=0;i<RegisterCount;i++)
  {
    Slot = i >> 1;
    RegisterNum = Slot + ((i&1) ? 8 : 0);
    if (!(i&1)) {DisplayText[Slot].Clear(); Pad = 2;}
    else        {DisplayText[Slot] << "  "; Pad = 3;}
    if (VMachine->GetCPURegister_Alpha(0,RegisterNum,RegisterName, RegisterValue))
    {
      RegisterName.PadUptoLen(' ',Pad);
      DisplayText[Slot] << RegisterName << " : " << RegisterValue;
    }

    Display[i].SetColor(1.0f, 1.0f, 1.0f);
    Display[i].SetDisplayText(DisplayText[i].String);
  }

  // Program counter;
  Slot++;DisplayText[Slot].Clear();
  VMachine->GetCPURegister_Alpha(2,RegisterNum,RegisterName, RegisterValue);
  DisplayText[Slot]<<RegisterName<<" : "<<RegisterValue;
  Slot++;
  //

  RegisterCount = VMachine->GetCPURegisterCount(1);
  DisplayText[Slot].Clear();
  RegisterValue.Set_DisplayTrailingZero(false);
  for (j=0;j<RegisterCount;j++)
  {
    VMachine->GetCPURegister_Alpha(1,j,RegisterName, RegisterValue);
    DisplayText[Slot] << RegisterName << ":" << RegisterValue << " ";
  }
  Display[Slot].SetColor(1.0f, 1.0f, 1.0f);
  Display[Slot].SetDisplayText(DisplayText[Slot].String);

  ZFrame::Render(ParentPosition);
}
