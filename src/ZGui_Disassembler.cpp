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
 * ZGui_Disassembler.cpp
 *
 *  Created on: 20 déc. 2014
 *      Author: laurent
 */

#include "ZGui_Disassembler.h"

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

void ZDisassemblerFrame::SetSize(float Width, float Height)
{
  ULong i;

  LineCount = (ULong)floor (Height / LineHeight);
  if (LineCount > ZDISASSEMBLERFRAME_MAXLINECOUNT) LineCount = ZDISASSEMBLERFRAME_MAXLINECOUNT;

  ZFrame::SetSize(Width, Height);

  for (i=0;i<LineCount;i++)
  {
    Display[i].SetPosition(Margin,Margin+ LineHeight * i);
    Display[i].SetSize(Width-(Margin*2.0f), LineHeight);
    Display[i].SetColor(1.0f,1.0f,1.0f);

    Display[i].SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  }
}

void ZDisassemblerFrame::AddedToFrameCallback(ZFrame * Frame)
{
  ULong i;

  for (i=0;i<LineCount;i++)
  {
    AddFrame(&Display[i]);
  }

}

void ZDisassemblerFrame::Render(Frame_Dimensions * ParentPosition)
{
  ZString Text, Opcode, Args;
  ZMemSize Pc,StartPc, i;
  ULong RewindLines;

  StartPc = Pc = VMachine->GetProgramCounter();

  // Try to rewind some instructions backward to show global program view.

  // Fast divide by 3.
  RewindLines = LineCount >> 3;
  RewindLines = RewindLines + (RewindLines << 1);

  while(RewindLines--) if (!VMachine->GetPreviousInstructionAddress(Pc)) break;

  // Disassemble

  for (i=0;i<LineCount;i++)
  {
    DisplayText[i].Set_DisplayBase(16);
    DisplayText[i].Set_DisplayTrailingZero(true);
    DisplayText[i].Clear();
    DisplayText[i] << (ULong)Pc;
    if (Pc == StartPc) Display[i].SetColor(1.0f, 0.4f, 0.4f);
    else                         Display[i].SetColor(1.0f, 1.0f, 1.0f);
    VMachine->GetDisassembly(Pc, Opcode, Args);
    DisplayText[i] << " " << Opcode << " " << Args << "\n";
    Display[i].SetDisplayText(DisplayText[i].String);
  }

  ZFrame::Render(ParentPosition);
}

