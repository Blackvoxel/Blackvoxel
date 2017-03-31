/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2017 Laurent Thiebaut & Olivia Merle
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
 * ZGameWindow_RTFM.cpp
 *
 *  Created on: 16 janv. 2017
 *      Author: laurent
 */

#include "ZGameWindow_RTFM.h"
#include "ZGame.h"
#include "ZActorPhysics.h"
#include "SDL/SDL.h"

void ZGameWindow_RTFM::Show()
{
  ZVector2f Rp, Ip, Size;
  ZActor * Actor;
  ULong x,y;

  Actor = GameEnv->PhysicEngine->GetSelectedActor(); if (!Actor) return;

  // Running position computing

  Ip.x = 32.0f; Ip.y = 25.0f;
  Rp.x = Ip.x; Rp.y = Ip.y;

  // Main Window

  ZVector2f MainWindow_Pos,MainWindow_Size;
  MainWindow_Size.x = 758.0f; MainWindow_Size.y = 400.0f;
  MainWindow_Pos.x = ((float)GameEnv->ScreenResolution.x - MainWindow_Size.x) / 2.0f;
  MainWindow_Pos.y = ((float)GameEnv->ScreenResolution.y - MainWindow_Size.y) / 2.0f;

  MainWindow->SetPosition( MainWindow_Pos.x, MainWindow_Pos.y );
  MainWindow->SetSize(MainWindow_Size.x,MainWindow_Size.y);
  MainWindow->SetTexture(8);
  GameEnv->GuiManager.AddFrame(MainWindow);

  // Main Windows Title

  MainTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_3));
  MainTitle.SetDisplayText(Text_MainTitle.String);
  MainTitle.GetTextDisplaySize(&Size);
  MainTitle.SetPosition(MainWindow_Size.x / 2.0f - Size.x / 2.0f ,Rp.y);
  MainTitle.SetSize(Size.x,Size.y);
  MainTitle.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&MainTitle);
  Rp.y += Size.y + 110.0f;

  // Displayed Text

  MainText.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2));
  MainText.SetDisplayText(Text_MainText.String);
  MainText.GetTextDisplaySize(&Size);
  MainText.SetPosition(Rp.x ,Rp.y);
  MainText.SetSize(Size.x,Size.y);
  MainText.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&MainText);
  Rp.y += Size.y + 20.0f;


//  SDL_ShowCursor(SDL_ENABLE);
//  SDL_WM_GrabInput(SDL_GRAB_OFF);
//  SDL_WarpMouse((Uint16)(MainWindow_Pos.x + MainWindow_Size.x / 2.0f),(Uint16)(MainWindow_Pos.y + MainWindow_Size.y / 2.0f));
//  GameEnv->Game_Events->SetDisableMouseEvents();
  Flag_Shown = true;
}

void ZGameWindow_RTFM::Hide()
{
  GameEnv->GuiManager.RemoveFrame(MainWindow);
//  SDL_ShowCursor(SDL_DISABLE);
//  SDL_WM_GrabInput(SDL_GRAB_ON);
//  GameEnv->Game_Events->SetEnableMouseEvents();
  Flag_Shown = false;
}

Bool ZGameWindow_RTFM::MouseButtonClick(UShort nButton, Short Absolute_x, Short Absolute_y)
{
  Bool Res;
  Res = ZFrame::MouseButtonClick(nButton, Absolute_x, Absolute_y);



  return (Res);
}
