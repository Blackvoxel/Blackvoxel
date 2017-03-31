/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2016 Laurent Thiebaut & Olivia Merle
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
 * ZGameWindow_SPS.cpp
 *
 *  Created on: 1 juil. 2016
 *      Author: laurent
 */

#ifndef Z_ZGAMEWINDOW_SPS_H
#  include "ZGameWindow_SPS.h"
#endif

#include "ZGame.h"

void ZGameWindow_SPS::Show()
{
  ZActor * Actor;

  Actor = GameEnv->PhysicEngine->GetSelectedActor(); if (!Actor) return;

  // Main Window

  ZVector2f MainWindow_Pos,MainWindow_Size;
  MainWindow_Size.x = 500.0f; MainWindow_Size.y = 25.0f;
  MainWindow_Pos.x = 5.0f;
  MainWindow_Pos.y = GameEnv->ScreenResolution.y - MainWindow_Size.y;



  MainWindow.SetPosition( MainWindow_Pos.x, MainWindow_Pos.y );
  MainWindow.SetSize(MainWindow_Size.x,MainWindow_Size.y);
  MainWindow.Show_Frame(false);
  //MainWindow.SetTexture(8);
  GameEnv->GuiManager.AddFrame(&MainWindow);

  // Inventory main title

  Infos.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2));
  Infos.SetDisplayText(Text_Infos.String);
  Infos.SetPosition(10.0f,0.0f);
  Infos.SetSize(MainWindow_Size.x, MainWindow_Size.y);
  Infos.SetColor(255.0f,255.0f,255.0f);
  MainWindow.AddFrame(&Infos);

  Flag_Shown = true;
}

void ZGameWindow_SPS::Hide()
{
  GameEnv->GuiManager.RemoveFrame(&MainWindow);
  Flag_Shown = false;
}
