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
 * ZGameWindow_Scan.cpp
 *
 *  Created on: 14 juil. 2016
 *      Author: laurent
 */

#ifndef Z_ZGAMEWINDOW_SCAN_H
#  include "ZGameWindow_Scan.h"
#endif

#include "ZGame.h"

void ZGameWindow_Scan::Show()
{
  ZActor * Actor;

  Actor = GameEnv->PhysicEngine->GetSelectedActor(); if (!Actor) return;

  // Main Window

  ZVector2f MainWindow_Pos,MainWindow_Size;
  MainWindow_Size.x = 250.0f; MainWindow_Size.y = 600.0f;
  MainWindow_Pos.x = (float)GameEnv->ScreenResolution.x - MainWindow_Size.x;
  MainWindow_Pos.y = (GameEnv->ScreenResolution.y - MainWindow_Size.y) / 2.0f;

  MainWindow.SetPosition( MainWindow_Pos.x, MainWindow_Pos.y );
  MainWindow.SetSize( MainWindow_Size.x,MainWindow_Size.y);
  MainWindow.SetTexture(8);
  GameEnv->GuiManager.AddFrame(&MainWindow);

  // Inventory main title

  Infos.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  Infos.SetDisplayText(Text_Infos.String);
  Infos.SetPosition(10.0f,10.0f);
  Infos.SetSize(MainWindow_Size.x, MainWindow_Size.y);
  Infos.SetColor(255.0f,255.0f,255.0f);
  MainWindow.AddFrame(&Infos);

  Flag_Shown = true;
}

void ZGameWindow_Scan::Hide()
{
  GameEnv->GuiManager.RemoveFrame(&MainWindow);
  Flag_Shown = false;
}
