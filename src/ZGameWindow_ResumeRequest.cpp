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
 * ZGameWindow_ResumeRequest.cpp
 *
 *  Created on: 13 mars 2015
 *      Author: laurent
 */


#include "ZGameWindow_ResumeRequest.h"

#include "ZGame.h"
#include "SDL/SDL.h"

void ZGameWindow_ResumeRequest::Show()
{
  ZVector2f Rp, Ip, Size;
  ZActor * Actor;

  Actor = GameEnv->PhysicEngine->GetSelectedActor(); if (!Actor) return;

  // Running position computing

  Ip.x = 2.0f+ 8.0f; Ip.y = 5.0f;
  Rp.x = Ip.x; Rp.y = Ip.y;

  // Main Window

  ZVector2f MainWindow_Pos,MainWindow_Size;
//  MainWindow_Size.x = 600.0f; MainWindow_Size.y = 100.0f;
  MainWindow_Size.x = 500.0f; MainWindow_Size.y = 200.0f;
  MainWindow_Pos.x = ((float)GameEnv->ScreenResolution.x - MainWindow_Size.x) / 2.0f;
  MainWindow_Pos.y = ((float)GameEnv->ScreenResolution.y - MainWindow_Size.y) / 2.0f;
  MainWindow->SetPosition( MainWindow_Pos.x, MainWindow_Pos.y );
  MainWindow->SetSize(MainWindow_Size.x,MainWindow_Size.y);
  MainWindow->SetTexture(15);
  GameEnv->GuiManager.AddFrame_ToTail(MainWindow);
  GameEnv->GuiManager.Frame_PushModal(MainWindow);
  // MainWindow->Dimensions.Position_z += 5.0f;

  MainWindow->Dimensions.Position_z = 10.0f;

  // CloseBox

  Size.x = 32.0f; Size.y = 32.0f;
  CloseBox.SetPosition(MainWindow_Size.x - Size.x - 5.0f, 5.0f);
  CloseBox.SetSize(Size.x, Size.y);
  CloseBox.SetTileSet(GameEnv->GuiTileset);
  CloseBox.SetTile(11);
  MainWindow->AddFrame(&CloseBox);

  Rp.y += 20.0f;

  // Main title

  Title.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2));
  Title.SetDisplayText(Text_Text.String);
  Title.GetTextDisplaySize(&Size);
  Title.SetPosition( (MainWindow_Size.x - Size.x) / 2.0f , (MainWindow_Size.y - Size.y) / 2.0f);
  Title.SetSize(Size.x,Size.y);
  Title.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&Title);
  Rp.y += Size.y + 30.0f;

  SDL_ShowCursor(SDL_ENABLE);
  SDL_WM_GrabInput(SDL_GRAB_OFF);
  //SDL_WarpMouse((Uint16)(MainWindow_Pos.x + MainWindow_Size.x / 2.0f),(Uint16)(MainWindow_Pos.y + MainWindow_Size.y / 2.0f));
  GameEnv->Game_Events->SetDisableMouseEvents();
  Flag_Shown = true;
}

void ZGameWindow_ResumeRequest::Hide()
{
  GameEnv->GuiManager.Frame_PopModal();
  GameEnv->GuiManager.RemoveFrame(MainWindow);
  //SDL_ShowCursor(SDL_DISABLE);
  //SDL_WM_GrabInput(SDL_GRAB_ON);
  //GameEnv->Game_Events->SetEnableMouseEvents();
  Flag_Shown = false;
}

Bool ZGameWindow_ResumeRequest::MouseButtonClick(UShort nButton, Short Absolute_x, Short Absolute_y)
{
  Bool Res;
  Res = ZFrame::MouseButtonClick(nButton, Absolute_x, Absolute_y);

  //if (CloseBox.Is_MouseClick())
  {
    this->Hide();
    SDL_ShowCursor(SDL_DISABLE);
    SDL_WM_GrabInput(SDL_GRAB_ON);
    GameEnv->Game_Events->SetEnableMouseEvents();
  }

  return (Res);
}

void ZGameWindow_ResumeRequest::Render(Frame_Dimensions * ParentPosition)
{
  ZFrame::Render(ParentPosition);
}

