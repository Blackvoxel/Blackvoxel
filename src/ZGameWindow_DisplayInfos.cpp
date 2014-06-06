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
 * ZGameWindows_DisplayInfos.cpp
 *
 *  Created on: 15 avr. 2013
 *      Author: laurent
 */

#ifndef Z_GAMEWINDOW_DISPLAYINFOS_H
#  include "ZGameWindow_DisplayInfos.h"
#endif

#include "ZGame.h"

void ZGameWindow_DisplayInfos::Show()
{
  ZActor * Actor;

  Actor = GameEnv->PhysicEngine->GetSelectedActor(); if (!Actor) return;

  // Main Window

  ZVector2f MainWindow_Pos,MainWindow_Size;
  MainWindow_Size.x = 500.0f; MainWindow_Size.y = 80.0f;
  MainWindow_Pos.x = 10.0f;
  MainWindow_Pos.y = 10.0f;

  MainWindow.SetPosition( MainWindow_Pos.x, MainWindow_Pos.y );
  MainWindow.SetSize(MainWindow_Size.x,MainWindow_Size.y);
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

void ZGameWindow_DisplayInfos::Hide()
{
  GameEnv->GuiManager.RemoveFrame(&MainWindow);
  Flag_Shown = false;
}
