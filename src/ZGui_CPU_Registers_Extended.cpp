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
 * ZGui_CPU_Registers_Extended.cpp
 *
 *  Created on: 1 févr. 2015
 *      Author: laurent
 */


#include "ZGui_CPU_Registers_Extended.h"

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

void ZCPURegisters_Extended::GetEffectiveHeight(float & Height)
{
  ULong LineCount;

  LineCount = (VMachine->GetCPURegisterCount(0) >> 1) + (VMachine->GetCPURegisterCount(0) & 1);
  LineCount += 2; // Add the line for flags and a blank line between.

  Height = (float)(LineHeight * LineCount);
}

void ZCPURegisters_Extended::SetSize(float Width, float Height)
{
  ULong i;

  ZFrame::SetSize(Width, Height);

  DisplaySlots = VMachine->GetCPURegisterCount(0);
  DisplaySlots +=30;
  if (DisplaySlots >= (ZCPUREGISTERSEXTENDED_MAXLINECOUNT)) DisplaySlots = ZCPUREGISTERSEXTENDED_MAXLINECOUNT;
  for (i=0;i<DisplaySlots;i++)
  {
    Display[i].SetPosition(Margin,Margin + LineHeight * i);
    Display[i].SetSize(Width-(Margin*2.0f), LineHeight);
    Display[i].SetColor(1.0f,1.0f,1.0f);
    Display[i].SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  }
}

void ZCPURegisters_Extended::AddedToFrameCallback(ZFrame * Frame)
{
  ULong i;

  for (i=0;i<DisplaySlots;i++)
  {
    AddFrame(&Display[i]);
  }
}

void ZCPURegisters_Extended::Render(Frame_Dimensions * ParentPosition)
{
  ZString Text, Opcode, Args, RegisterName, RegisterValue;
  ULong i,Slot;
  ULong RegisterCount;

  RegisterValue.Set_DisplayBase(16);
  RegisterValue.Set_DisplayTrailingZero(true);

  RegisterCount = VMachine->GetCPURegisterCount(0);
  Slot = 0;
  for (i=0;i<RegisterCount;i++)
  {
    DisplayText[Slot].Clear();
    if (VMachine->GetCPURegister_Alpha(16,i,RegisterName, RegisterValue))
    {
      RegisterName.PadUptoLen(' ',3);
      DisplayText[Slot] << RegisterName << " : " << RegisterValue;
    }
    Display[Slot].SetColor(1.0f, 1.0f, 1.0f);
    Display[Slot].SetDisplayText(DisplayText[i].String);
    Slot++;
  }

  // A Blank line

    DisplayText[Slot].Clear(); Display[Slot].SetColor(1.0f, 1.0f, 1.0f); Display[Slot].SetDisplayText(DisplayText[Slot].String);
    Slot++;

  // Status Register

  if (VMachine->GetCPURegister_Alpha(17,i,RegisterName, RegisterValue))
  {
    DisplayText[Slot].Clear();
    DisplayText[Slot]<< "      " << RegisterName; Display[Slot].SetColor(1.0f, 1.0f, 1.0f); Display[Slot].SetDisplayText(DisplayText[Slot].String);
    Slot++;
    DisplayText[Slot].Clear();
    DisplayText[Slot]<< "SR  : " <<RegisterValue;Display[Slot].SetColor(1.0f, 1.0f, 1.0f); Display[Slot].SetDisplayText(DisplayText[Slot].String);
    Slot++;
    // A Blank line
    DisplayText[Slot].Clear(); Display[Slot].SetColor(1.0f, 1.0f, 1.0f); Display[Slot].SetDisplayText(DisplayText[Slot].String);
    Slot++;

  }

  // PC
  if (VMachine->GetCPURegister_Alpha(18,i,RegisterName, RegisterValue))
  {
    RegisterName.PadUptoLen(' ',3);
    DisplayText[Slot].Clear();
    DisplayText[Slot]<< RegisterName << " : " << RegisterValue; Display[Slot].SetColor(1.0f, 1.0f, 1.0f); Display[Slot].SetDisplayText(DisplayText[Slot].String);
    Slot++;
    // A Blank line
    DisplayText[Slot].Clear(); Display[Slot].SetColor(1.0f, 1.0f, 1.0f); Display[Slot].SetDisplayText(DisplayText[Slot].String);
    Slot++;

  }



  // Next Instruction Statement
  DisplayText[Slot] = "-----------NEXT INSTRUCTION------------"; Display[Slot].SetColor(1.0f, 1.0f, 1.0f); Display[Slot].SetDisplayText(DisplayText[Slot].String);
  Slot++;
  // A Blank line
  DisplayText[Slot].Clear(); Display[Slot].SetColor(1.0f, 1.0f, 1.0f); Display[Slot].SetDisplayText(DisplayText[Slot].String);
  Slot++;

  // Next Instruction
  ZMemSize Pc,OldPc,ByteCount;
  OldPc = Pc = VMachine->GetProgramCounter();
  VMachine->GetDisassembly(Pc, Opcode, Args);
  DisplayText[Slot].Clear();
  DisplayText[Slot] << "INSTR : " << Opcode << " " << Args;
  Display[Slot].SetColor(1.0f, 1.0f, 1.0f);
  Display[Slot].SetDisplayText(DisplayText[Slot].String);
  Slot++;
  ByteCount = Pc - OldPc;

  // Instruction in hexadecimal format

  DisplayText[Slot].Clear();
  //DisplayText[Slot]= "~1:FF0000:";
  DisplayText[Slot]<<"CODE  : ";
  for (i=0;i<ByteCount;i++)
  {
   // if (i>ByteCount) DisplayText[Slot]<< "~1:FFFFFF:";
    // Instruction bytes
    DisplayText[Slot].Append_HexNumber_UByte( VMachine->Debug_Read_Byte(OldPc+i) ) << " ";
  }

  Display[Slot].SetColor(1.0f, 1.0f, 1.0f);
  Display[Slot].SetDisplayText(DisplayText[Slot].String);
  Slot++;

  // Instruction in binary format

  DisplayText[Slot].Clear();
  DisplayText[Slot]<<"BINARY:";
  for (i=0;i<10;i++)
  {
    if (i<ByteCount)
    {
      if ( i && (!(i %3))) {Slot++;  DisplayText[Slot].Clear(); DisplayText[Slot]<<"       "; }
      (DisplayText[Slot]<<" ").Append_BinaryNumber_UByte( VMachine->Debug_Read_Byte(OldPc+i) );
      Display[Slot].SetColor(1.0f, 1.0f, 1.0f);
      Display[Slot].SetDisplayText(DisplayText[Slot].String);
    }
  }
  Slot++;

  // A Blank line
  DisplayText[Slot].Clear(); Display[Slot].SetColor(1.0f, 1.0f, 1.0f); Display[Slot].SetDisplayText(DisplayText[Slot].String);
  Slot++;

/*
  // Program counter;
  Slot++;DisplayText[i].Clear();
  VMachine->GetCPURegister_Alpha(2,i,RegisterName, RegisterValue);
  DisplayText[Slot]<<RegisterName<<" : "<<RegisterValue;
  i++;
  */
  //
/*
  RegisterCount = VMachine->GetCPURegisterCount(1);
  DisplayText[i].Clear();
  RegisterValue.Set_DisplayTrailingZero(false);
  for (j=0;j<RegisterCount;j++)
  {
    VMachine->GetCPURegister_Alpha(1,j,RegisterName, RegisterValue);
    DisplayText[Slot] << RegisterName << ":" << RegisterValue << " ";
  }
  Display[Slot].SetColor(1.0f, 1.0f, 1.0f);
  Display[Slot].SetDisplayText(DisplayText[Slot].String);
*/
  ZFrame::Render(ParentPosition);
}

