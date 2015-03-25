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
 * ZGui_Memory_Monitor.cpp
 *
 *  Created on: 17 févr. 2015
 *      Author: laurent
 */

#include "ZGui_Memory_Monitor.h"

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

void ZFrame_MemoryMonitor::GetEffectiveHeight(float & Height)
{
  Height = 300.0f;
}

void ZFrame_MemoryMonitor::SetSize(float Width, float Height)
{
  ULong i;
  ZVector2f Size, Pos;

  ZFrame::SetSize(Width, Height);

  Pos.x = 8.0f;
  Pos.y = 4.0f;

  for (i=0;i<8;i++)
  {

    Digits[i].SetPosition(Pos.x,Pos.y);
    Digits[i].GetEffectiveSize(&Size);
    Digits[i].SetSize(Size.x,Size.y);
    Digits[i].SetDigitAddress(&MemoryAddress, 7-i);
    Pos.x += Size.x;
  }
  Pos.x += 8.0f;
  Pos.y += 2.0f;
  for (i=0;i<3;i++)
  {
    DispSize[i].SetDisplayText(Text_DispSize[i].String);
    DispSize[i].SetTextStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
    DispSize[i].SetButtonNumber(i);
    DispSize[i].SetMargins(6.0f,6.0f,6.0f,6.0f);
    DispSize[i].GetEffectiveDisplaySize(&Size);
    DispSize[i].SetColor(1.0f,1.0f,1.0f);
    DispSize[i].SetPosition(Pos.x,Pos.y);
    DispSize[i].SetSize(Size.x, Size.y);
    DispSize[i].SetDataRef(&DisplaySize);
    DispSize[i].SetButtonList(DispSize_List);
    Pos.x += Size.x + 1.0f;
  }
  Pos.y += Size.y+15.0f;

  for (i=0;i<ZFRAME_MEMORYMONITOR_DISPLAYLINES;i++)
  {
    MemLines[i].SetPosition(0.0f,Pos.y);
    MemLines[i].SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
    MemLines[i].SetColor(1.0f,1.0f,1.0f);
    MemLines[i].SetSize(Width,8);
    MemLines[i].SetDisplayText(Text_MemLines[i].String);
    Pos.y+=8.0f;
  }



}

void ZFrame_MemoryMonitor::AddedToFrameCallback(ZFrame * Frame)
{
  ULong i;

  for (i=0;i<8;i++) AddFrame(&Digits[i]);
  for (i=0;i<ZFRAME_MEMORYMONITOR_DISPLAYLINES;i++) AddFrame(&MemLines[i]);
  for (i=0;i<3;i++)  AddFrame(&DispSize[i]);

}

void ZFrame_MemoryMonitor::Render(Frame_Dimensions * ParentPosition)
{
  ZMemSize i,j, Address,SAddress;
  ULong Slot;

  Slot = 0;
  Address = MemoryAddress;

  switch(DisplaySize)
  {
    case 0: for(j=0;j<ZFRAME_MEMORYMONITOR_DISPLAYLINES;j++)
            {
              Text_MemLines[Slot].Clear();
              Text_MemLines[Slot].Append_HexNumber_ULong(Address);
              SAddress = Address;
              for(i=0;i<8;i++) (Text_MemLines[Slot] << " ").Append_HexNumber_UByte(VMachine->Debug_Read_Byte(Address++));
              Address = SAddress;
              for (i=0;i<8;i++) Text_MemLines[Slot].Append_char(CharFilter.Convert(VMachine->Debug_Read_Byte(Address++)));
              MemLines[Slot].SetDisplayText(Text_MemLines[Slot].String);
              Slot++;
            }
            break;
    case 1: for(j=0;j<ZFRAME_MEMORYMONITOR_DISPLAYLINES;j++)
            {
              Address &= ~1;
              Text_MemLines[Slot].Clear();
              Text_MemLines[Slot].Append_HexNumber_ULong(Address);
              for(i=0;i<6;i++)
              {
                (Text_MemLines[Slot] << " ").Append_HexNumber_UShort(VMachine->Debug_Read_Short(Address));
                Address += 2;
              }
              MemLines[Slot].SetDisplayText(Text_MemLines[Slot].String);
              Slot++;
            }
            break;
    case 2: for(j=0;j<ZFRAME_MEMORYMONITOR_DISPLAYLINES;j++)
            {
              Address &= ~3;
              Text_MemLines[Slot].Clear();
              Text_MemLines[Slot].Append_HexNumber_ULong(Address);
              for(i=0;i<3;i++)
              {
                (Text_MemLines[Slot] << " ").Append_HexNumber_ULong(VMachine->Debug_Read_Long(Address));
                Address += 4;
              }
              MemLines[Slot].SetDisplayText(Text_MemLines[Slot].String);
              Slot++;
            }
            break;

  }

  ZFrame::Render(ParentPosition);
}
