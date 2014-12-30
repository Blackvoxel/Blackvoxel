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
 * ZGui_AsmRobotDisplay.cpp
 *
 *  Created on: 24 déc. 2014
 *      Author: laurent
 */


#include "ZGui_AsmRobotDisplay.h"

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif


void ZAsmRobotDisplay::SetSize(float Width, float Height)
{
  ULong i;

  ZFrame::SetSize(Width, Height);

  for (i=0;i<ZASMROBOTDISPLAY_MAXLINECOUNT;i++)
  {
    Display[i].SetPosition(0,LineHeight * i);
    Display[i].SetSize(320.0f, LineHeight);
    Display[i].SetColor(1.0f,1.0f,1.0f);
    Display[i].SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  }
}

void ZAsmRobotDisplay::AddedToFrameCallback(ZFrame * Frame)
{
  ULong i;

  for (i=0;i<ZASMROBOTDISPLAY_MAXLINECOUNT;i++)
  {
    AddFrame(&Display[i]);
  }
}

void ZAsmRobotDisplay::Render(Frame_Dimensions * ParentPosition)
{
  ULong i,j;
  ULong DisplayMode;
  UByte * RamBase;
  ZMemSize RamMask, VideoMemoryOffset;
  VMachine->GetVRAMInfo(DisplayMode, RamBase, RamMask, VideoMemoryOffset);

  switch(DisplayMode)
  {
    case 0: for (i=0;i<ZASMROBOTDISPLAY_MAXLINECOUNT;i++)
            {
              DisplayText[i].SetLen(40);
              DisplayText[i].Fill(' ');
              Display[i].SetDisplayText(DisplayText[i].String);
            }
            break;

    case 1: for (i=0;i<ZASMROBOTDISPLAY_MAXLINECOUNT;i++)
            {
              UByte c;
              DisplayText[i].SetLen(40);
              for (j=0;j<40;j++)
              {
                c = RamBase[VideoMemoryOffset++ & RamMask];
                if (c<32) c= 32;
                DisplayText[i].String[j]=c;
              }
              DisplayText[i].String[j]=0;
              Display[i].SetDisplayText(DisplayText[i].String);
            }
            break;
  }

  ZFrame::Render(ParentPosition);
}


