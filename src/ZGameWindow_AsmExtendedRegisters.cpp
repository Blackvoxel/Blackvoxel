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
 * ZGameWindow_AsmExtendedRegisters.cpp
 *
 *  Created on: 1 févr. 2015
 *      Author: laurent
 */

#include "ZGameWindow_AsmExtendedRegisters.h"

#include "ZGame.h"
#include "ZActorPhysics.h"
#include "SDL/SDL.h"

void ZGameWindow_AsmExtendedRegisters::Show()
{
  ZVector2f Rp, Ip, Size;
  ZActor * Actor;

  Actor = GameEnv->PhysicEngine->GetSelectedActor(); if (!Actor) return;

  // Running position computing

  Ip.x = 2.0f; Ip.y = 5.0f;
  Rp.x = Ip.x; Rp.y = Ip.y;

  // Main Window

  ZVector2f MainWindow_Pos,MainWindow_Size;
  MainWindow_Size.x = 324.0f; MainWindow_Size.y = 600.0f;
  MainWindow_Pos.x = 255.0f + 8.0f;
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
  Title.SetPosition( (MainWindow_Size.x - Size.x) / 2.0 ,Rp.y);
  Title.SetSize(Size.x,Size.y);
  Title.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&Title);
  Rp.y += Size.y + 5.0f;

  // RegisterDisplay

  RegisterDisplay.SetGameEnv(GameEnv);
  RegisterDisplay.SetVMachine(&VoxelExtension->VirtualMachine);
  RegisterDisplay.SetPosition(Rp.x, Rp.y);
  RegisterDisplay.SetSize(320,350);
  RegisterDisplay.SetTexture(14);
  MainWindow->AddFrame(&RegisterDisplay);
  Rp.y += 350.0f + 5.0f;
  Rp.x = Ip.x;

  // Memory Monitor title

  Title2.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2));
  Title2.SetDisplayText(Text_Title2.String);
  Title2.GetTextDisplaySize(&Size);
  Title2.SetPosition( (MainWindow_Size.x - Size.x) / 2.0 ,Rp.y);
  Title2.SetSize(Size.x,Size.y);
  Title2.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&Title2);
  Rp.y += Size.y + 5.0f;

  // Memory monitor

  MemoryMonitor.SetGameEnv(GameEnv);
  MemoryMonitor.SetVMachine(&VoxelExtension->VirtualMachine);
  MemoryMonitor.SetPosition(Rp.x, Rp.y);
  MemoryMonitor.SetSize(320,188);
  MemoryMonitor.SetTexture(14);
  MainWindow->AddFrame(&MemoryMonitor);
  Rp.y += 188;
  Rp.x = Ip.x;



/*
  // Servo progress bar

  Servo_x_Position.SetTileSet(GameEnv->GuiTileset);
  Servo_x_Position.SetTexture(0);
  Servo_x_Position.SetPosition(Rp.x,Rp.y);
  Servo_x_Position.SetSize(100.0f, 2.0f );
  Servo_x_Position.SetCompletion(50.0f);
  MainWindow->AddFrame(&Servo_x_Position);
  Rp.y += Size.y + 30.0f;
  Rp.x += 100.0f + 5.0f;

  // Servo value
  Servo_x_Value.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  Servo_x_Value.SetDisplayText(Servo_x_Value_Text.String);
  Servo_x_Value.GetTextDisplaySize(&Size);
  Servo_x_Value.SetPosition( Rp.x ,Rp.y);
  Servo_x_Value.SetSize(Size.x,Size.y);
  Servo_x_Value.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&Servo_x_Value);
  Rp.y += Size.y + 5.0f;
  Rp.x = Ip.x;
*/

  SDL_ShowCursor(SDL_ENABLE);
  SDL_WM_GrabInput(SDL_GRAB_OFF);
  //SDL_WarpMouse((Uint16)(MainWindow_Pos.x + MainWindow_Size.x / 2.0f),(Uint16)(MainWindow_Pos.y + MainWindow_Size.y / 2.0f));
  GameEnv->Game_Events->SetDisableMouseEvents();
  Flag_Shown = true;
}

void ZGameWindow_AsmExtendedRegisters::Hide()
{
  GameEnv->GuiManager.RemoveFrame(MainWindow);
  //SDL_ShowCursor(SDL_DISABLE);
  //SDL_WM_GrabInput(SDL_GRAB_ON);
  //GameEnv->Game_Events->SetEnableMouseEvents();
  Flag_Shown = false;
}

Bool ZGameWindow_AsmExtendedRegisters::MouseButtonClick(UShort nButton, Short Absolute_x, Short Absolute_y)
{
  Bool Res;
  Res = ZFrame::MouseButtonClick(nButton, Absolute_x, Absolute_y);

  if (CloseBox.Is_MouseClick())
  {
    this->Hide();
  }

  return (Res);
}

void ZGameWindow_AsmExtendedRegisters::Render(Frame_Dimensions * ParentPosition)
{
  float Completion;
  Long ServoValue;

  ServoValue = VoxelExtension->VirtualMachine.Servo_MovePos.x;
  Completion = 50.0f + ((float)ServoValue) / 5000.0f * 50.0f;
  Servo_x_Position.SetCompletion(Completion);

  Servo_x_Value_Text = ServoValue;
  Servo_x_Value.SetDisplayText(Servo_x_Value_Text.String);

  ZFrame::Render(ParentPosition);
}



