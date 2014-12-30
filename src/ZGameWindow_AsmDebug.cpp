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
 * ZGameWindow_AsmDebug.cpp
 *
 *  Created on: 19 déc. 2014
 *      Author: laurent
 */

#include "ZGameWindow_AsmDebug.h"


#include "ZGame.h"
#include "ZActorPhysics.h"
#include "SDL/SDL.h"

void ZGameWindow_AsmDebug::Show()
{
  ZVector2f Rp, Ip, Size;
  ZActor * Actor;

  Actor = GameEnv->PhysicEngine->GetSelectedActor(); if (!Actor) return;

  // Running position computing

  Ip.x = 2.0f; Ip.y = 5.0f;
  Rp.x = Ip.x; Rp.y = Ip.y;

  // Main Window

  ZVector2f MainWindow_Pos,MainWindow_Size;
  MainWindow_Size.x = 250.0f; MainWindow_Size.y = 600.0f;
  MainWindow_Pos.x = 0.0f;
  MainWindow_Pos.y = ((float)GameEnv->ScreenResolution.y - MainWindow_Size.y) / 2.0f;
  MainWindow->SetPosition( MainWindow_Pos.x, MainWindow_Pos.y );
  MainWindow->SetSize(MainWindow_Size.x,MainWindow_Size.y);
  MainWindow->SetTexture(8);
  GameEnv->GuiManager.AddFrame(MainWindow);

  // CloseBox

  Size.x = 32.0f; Size.y = 32.0f;
  CloseBox.SetPosition(MainWindow_Size.x - Size.x - 5.0f, 5.0f);
  CloseBox.SetSize(Size.x, Size.y);
  CloseBox.SetTileSet(GameEnv->GuiTileset);
  CloseBox.SetTile(11);
  MainWindow->AddFrame(&CloseBox);

  // Main title

  Title.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2));
  Title.SetDisplayText(Text_Title.String);
  Title.GetTextDisplaySize(&Size);
  Title.SetPosition( Rp.x + 2.0f/*(MainWindow_Size.x - Size.x) / 2.0*/ ,Rp.y);
  Title.SetSize(Size.x,Size.y);
  Title.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&Title);
  Rp.y += Size.y + 5.0f;

  // Step Button
  Button_Step.SetDisplayText(Text_Button_Step.String);
  Button_Step.SetTextStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  Button_Step.GetEffectiveDisplaySize(&Size);
  Button_Step.SetPosition(Rp.x,Rp.y);
  Button_Step.SetSize(Size.x,Size.y);
  MainWindow->AddFrame(&Button_Step);
  Rp.x += Size.x + 2.0f;

  // Run Button
  Button_Run.SetDisplayText(Text_Button_Run.String);
  Button_Run.SetTextStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  Button_Run.GetEffectiveDisplaySize(&Size);
  Button_Run.SetPosition(Rp.x,Rp.y);
  Button_Run.SetSize(Size.x,Size.y);
  MainWindow->AddFrame(&Button_Run);
  Rp.x += Size.x + 2.0f;

  // Stop Button
  Button_Stop.SetDisplayText(Text_Button_Stop.String);
  Button_Stop.SetTextStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  Button_Stop.GetEffectiveDisplaySize(&Size);
  Button_Stop.SetPosition(Rp.x,Rp.y);
  Button_Stop.SetSize(Size.x,Size.y);
  MainWindow->AddFrame(&Button_Stop);
  Rp.x += Size.x + 2.0f;

  // Reset Button
  Button_Reset.SetDisplayText(Text_Button_Reset.String);
  Button_Reset.SetTextStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  Button_Reset.GetEffectiveDisplaySize(&Size);
  Button_Reset.SetPosition(Rp.x,Rp.y);
  Button_Reset.SetSize(Size.x,Size.y);
  MainWindow->AddFrame(&Button_Reset);
  Rp.y += Size.y + 5.0f;
  Rp.x = Ip.x;

  // Hardware pannel Button
  Button_HardwarePannel.SetDisplayText(Text_Button_HardwarePannel.String);
  Button_HardwarePannel.SetTextStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  Button_HardwarePannel.GetEffectiveDisplaySize(&Size);
  Button_HardwarePannel.SetPosition(Rp.x,Rp.y);
  Button_HardwarePannel.SetSize(Size.x,Size.y);
  MainWindow->AddFrame(&Button_HardwarePannel);
  Rp.y += Size.y + 5.0f;
  Rp.x = Ip.x;

  // CPU Registers title

  Title_CPURegisters.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  Title_CPURegisters.SetDisplayText(Text_Title_CPURegisters.String);
  Title_CPURegisters.GetTextDisplaySize(&Size);
  Title_CPURegisters.SetPosition(Rp.x ,Rp.y);
  Title_CPURegisters.SetSize(Size.x,Size.y);
  Title_CPURegisters.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&Title_CPURegisters);
  Rp.y += Size.y + 3.0f;

  // CPU Registers

  Size.x = MainWindow_Size.x - 4.0f;
  CPURegisters.SetGameEnv(GameEnv);
  CPURegisters.SetVMachine(&VoxelExtension->VirtualMachine);
  CPURegisters.SetPosition(Rp.x, Rp.y);
  CPURegisters.GetEffectiveHeight(Size.y);
  CPURegisters.SetSize(Size.x, Size.y);
  CPURegisters.SetTexture(4);
  MainWindow->AddFrame(&CPURegisters);
  Rp.y += Size.y + 10.0f;
  Rp.x = Ip.x;

  // Disassembly title

  Title_Disassembly.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  Title_Disassembly.SetDisplayText(Text_Title_Disassembly.String);
  Title_Disassembly.GetTextDisplaySize(&Size);
  Title_Disassembly.SetPosition(Rp.x ,Rp.y);
  Title_Disassembly.SetSize(Size.x,Size.y);
  Title_Disassembly.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&Title_Disassembly);
  Rp.y += Size.y + 3.0f;

  // Disassembler Frame

  Size.x = MainWindow_Size.x - 4.0f;
  Size.y = MainWindow_Size.y - 300.0f;
  DisassemblerFrame.SetGameEnv(GameEnv);
  DisassemblerFrame.SetVMachine(&VoxelExtension->VirtualMachine);
  DisassemblerFrame.SetPosition(Rp.x, Rp.y);
  DisassemblerFrame.SetSize(Size.x, MainWindow_Size.y - Rp.y - 10.0f );
  DisassemblerFrame.SetTexture(4);
  MainWindow->AddFrame(&DisassemblerFrame);
  Rp.y += Size.y + 30.0f;
  Rp.x = Ip.x;

  SDL_ShowCursor(SDL_ENABLE);
  SDL_WM_GrabInput(SDL_GRAB_OFF);
  SDL_WarpMouse((Uint16)(MainWindow_Pos.x + MainWindow_Size.x / 2.0f),(Uint16)(MainWindow_Pos.y + MainWindow_Size.y / 2.0f));
  GameEnv->Game_Events->SetDisableMouseEvents();
  Flag_Shown = true;
}

void ZGameWindow_AsmDebug::Hide()
{
  GameEnv->GuiManager.RemoveFrame(MainWindow);
  SDL_ShowCursor(SDL_DISABLE);
  SDL_WM_GrabInput(SDL_GRAB_ON);
  GameEnv->Game_Events->SetEnableMouseEvents();
  Flag_Shown = false;
}

Bool ZGameWindow_AsmDebug::MouseButtonClick(UShort nButton, Short Absolute_x, Short Absolute_y)
{
  Bool Res;
  Res = ZFrame::MouseButtonClick(nButton, Absolute_x, Absolute_y);

  if (Button_Step.Is_MouseClick(true))
  {
    VoxelExtension->VirtualMachine.EnableRunningStepCount(1);
  }

  if (Button_Run.Is_MouseClick(true))
  {
    VoxelExtension->VirtualMachine.SetStepModeOn(false);
  }

  if (Button_Stop.Is_MouseClick(true))
  {
    VoxelExtension->VirtualMachine.EnableRunningStepCount(0);
    VoxelExtension->VirtualMachine.SetStepModeOn(true);
  }

  if (Button_Reset.Is_MouseClick(true))
  {
    VoxelExtension->VirtualMachine.EnableRunningStepCount(0);
    VoxelExtension->VirtualMachine.SetStepModeOn(true);
    VoxelExtension->CompileAndRunScript(ZVoxelExtension_Programmable::CONTEXT_PROGRAMCHANGE, VoxelExtension->ScriptNum , true );
  }

  if (Button_HardwarePannel.Is_MouseClick(true))
  {
    GameEnv->GameWindow_AsmHardware->SetVoxelExtension(VoxelExtension);
    if (GameEnv->GameWindow_AsmHardware->Is_Shown()) { GameEnv->GameWindow_AsmHardware->Hide(); }
    else                                             { GameEnv->GameWindow_AsmHardware->Show(); }
  }

  if (CloseBox.Is_MouseClick(true))
  {
    Hide();
    if (GameEnv->GameWindow_AsmHardware->Is_Shown()) { GameEnv->GameWindow_AsmHardware->Hide(); }
  }

  return (Res);
}
