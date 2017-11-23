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
 * ZGameWindow_VoxelTypeBar.cpp
 *
 *  Created on: 12 juil. 2011
 *      Author: laurent
 */

#include "ZGameWindow_VoxelTypeBar.h"

#include "ZGame.h"
#include "ZActorPhysics.h"
#include "SDL/SDL.h"

void ZGameWindow_VoxelTypeBar::Show()
{
  ZVector2f Rp, Ip;
  ZActor * Actor;
  ULong y;

  Actor = GameEnv->PhysicEngine->GetSelectedActor(); if (!Actor) return;

  // Running position computing

  Ip.x = 18.0f; Ip.y = 25.0f;
  Rp.x = Ip.x; Rp.y = Ip.y;

  // Main Window

  ZVector2f MainWindow_Pos,MainWindow_Size;
  MainWindow_Size.x = 48.0f; MainWindow_Size.y = 600.0f;
  MainWindow_Pos.x = ((float)GameEnv->ScreenResolution.x - MainWindow_Size.x);
  MainWindow_Pos.y = ((float)GameEnv->ScreenResolution.y - MainWindow_Size.y) / 2.0f;

  MainWindow.SetPosition( MainWindow_Pos.x, MainWindow_Pos.y);
  MainWindow.SetSize(MainWindow_Size.x,MainWindow_Size.y);
  MainWindow.SetTexture(9);

  GameEnv->GuiManager.AddFrame(&MainWindow);



  // Inventory main title



  // Textures

  for (y=0;y<14;y++)
  {
    ULong Indice = y;
    ZInventory::Entry * Entry = Actor->Inventory->GetSlotRef(Indice);
    MainInventory[Indice].SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
    if (y!=6)
    {
      MainInventory[Indice].SetPosition(8.0f, Rp.y );
      MainInventory[Indice].SetSize(32.0f,32.0f);
      Rp.y += 32.0f + 6.0f;
    }
    else
    {
      MainInventory[Indice].SetPosition(-24.0f, Rp.y );
      MainInventory[Indice].SetSize(64.0f,64.0f);
      Rp.y += 64.0f + 6.0f;
    }
    MainInventory[Indice].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
    MainInventory[Indice].SetVoxelType(&Entry->VoxelType);
    MainInventory[Indice].SetQuantity(&Entry->Quantity);
    MainInventory[Indice].SetColor(255.0f,255.0f,255.0f);
    MainInventory[Indice].SetDragCapability(false);
    MainWindow.AddFrame(&MainInventory[Indice]);

  }

  //SDL_ShowCursor(SDL_ENABLE);
  //SDL_WM_GrabInput(SDL_GRAB_OFF);
  //SDL_WarpMouse((Uint16)(Pos.x + Size.x / 2.0f),(Uint16)(Pos.y + Size.y / 2.0f));
  //GameEnv->Game_Events->SetDisableMouseEvents();
  Flag_Shown = true;
}

void ZGameWindow_VoxelTypeBar::Hide()
{
  GameEnv->GuiManager.RemoveFrame(&MainWindow);

  //SDL_ShowCursor(SDL_DISABLE);
  // SDL_WM_GrabInput(SDL_GRAB_ON);
  //GameEnv->Game_Events->SetEnableMouseEvents();
  Flag_Shown = false;
}
