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
 * ZGame_GUI.cpp
 *
 *  Created on: 2 juil. 2011
 *      Author: laurent
 */

#include "ZGameWindow_Inventory.h"
#include "ZGame.h"
#include "ZActorPhysics.h"
#include "SDL/SDL.h"

void ZGameWindow_Inventory::Show()
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
  MainWindow_Size.x = 758.0f; MainWindow_Size.y = 600.0f;
  MainWindow_Pos.x = ((float)GameEnv->ScreenResolution.x - MainWindow_Size.x) / 2.0f;
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

  // Inventory main title

  MainTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_3));
  MainTitle.SetDisplayText(Text_MainTitle.String);
  MainTitle.GetTextDisplaySize(&Size);
  MainTitle.SetPosition(MainWindow_Size.x / 2.0f - Size.x / 2.0f ,Rp.y);
  MainTitle.SetSize(Size.x,Size.y);
  MainTitle.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&MainTitle);
  Rp.y += Size.y + 20.0f;

  // Tools Title

  ToolTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  ToolTitle.SetDisplayText(Text_ToolTitle.String);
  ToolTitle.GetTextDisplaySize(&Size);
  ToolTitle.SetPosition(Rp.x ,Rp.y);
  ToolTitle.SetSize(Size.x,Size.y);
  ToolTitle.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&ToolTitle);
  Rp.y += Size.y + 5.0f;

  // Tools

  for (x=0;x<10;x++)
  {
    ULong Indice = ZInventory::Tools_StartSlot + x;
    ZInventory::Entry * Entry = Actor->Inventory->GetSlotRef(Indice);
    MainInventory[Indice].SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
    MainInventory[Indice].SetPosition(32.0f + x*70.0f, Rp.y );
    MainInventory[Indice].SetSize(64.0f,64.0f);
    MainInventory[Indice].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2), ZInventoryBox::FONTTYPE_MAIN);
    MainInventory[Indice].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1), ZInventoryBox::FONTTYPE_REDUCED);
    MainInventory[Indice].SetVoxelType(&Entry->VoxelType);
    MainInventory[Indice].SetQuantity(&Entry->Quantity);
    MainInventory[Indice].SetColor(255.0f,255.0f,255.0f);
    MainWindow->AddFrame(&MainInventory[Indice]);
  }
  Rp.y += 64.0f + 12.0f;

  // Inventory Zone Title

  InventoryTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  InventoryTitle.SetDisplayText(Text_InventoryTitle.String);
  InventoryTitle.GetTextDisplaySize(&Size);
  InventoryTitle.SetPosition(Rp.x ,Rp.y);
  InventoryTitle.SetSize(Size.x,Size.y);
  InventoryTitle.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&InventoryTitle);
  Rp.y += Size.y + 5.0f;


  // Inventory

  for (y=0;y<4;y++)
  {
    for (x=0;x<10;x++)
    {
      ULong Indice = ZInventory::Inventory_StartSlot + x + y * 10;
      ZInventory::Entry * Entry = Actor->Inventory->GetSlotRef(Indice);
      MainInventory[Indice].SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
      MainInventory[Indice].SetPosition(32.0f + x*70.0f, Rp.y );
      MainInventory[Indice].SetSize(64.0f,64.0f);
      MainInventory[Indice].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2), ZInventoryBox::FONTTYPE_MAIN);
      MainInventory[Indice].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1), ZInventoryBox::FONTTYPE_REDUCED);
      MainInventory[Indice].SetVoxelType(&Entry->VoxelType);
      MainInventory[Indice].SetQuantity(&Entry->Quantity);
      MainInventory[Indice].SetColor(128.0f,128.0f,128.0f);
      MainWindow->AddFrame(&MainInventory[Indice]);
    }
    Rp.y += 64.0f + 6.0f;
  }
  Rp.y += 12.0f;

  // Powers Zone Title

  PowerTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  PowerTitle.SetDisplayText(Text_PowerTitle.String);
  PowerTitle.GetTextDisplaySize(&Size);
  PowerTitle.SetPosition(Rp.x ,Rp.y);
  PowerTitle.SetSize(Size.x,Size.y);
  PowerTitle.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&PowerTitle);
  Rp.y += Size.y + 5.0f;

  // Powers

  for (x=0;x<10;x++)
  {
    ULong Indice = ZInventory::Powers_StartSlot + x;
    ZInventory::Entry * Entry = Actor->Inventory->GetSlotRef(Indice);
    MainInventory[Indice].SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
    MainInventory[Indice].SetPosition(32.0f + x*70.0f, Rp.y );
    MainInventory[Indice].SetSize(64.0f,64.0f);
    MainInventory[Indice].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2), ZInventoryBox::FONTTYPE_MAIN);
    MainInventory[Indice].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1), ZInventoryBox::FONTTYPE_REDUCED);
    MainInventory[Indice].SetVoxelType(&Entry->VoxelType);
    MainInventory[Indice].SetQuantity(&Entry->Quantity);
    MainInventory[Indice].SetColor(255.0f,255.0f,255.0f);
    MainWindow->AddFrame(&MainInventory[Indice]);
  }
  Rp.y += 64.0f + 12.0f;



  SDL_ShowCursor(SDL_ENABLE);
  SDL_WM_GrabInput(SDL_GRAB_OFF);
  SDL_WarpMouse((Uint16)(MainWindow_Pos.x + MainWindow_Size.x / 2.0f),(Uint16)(MainWindow_Pos.y + MainWindow_Size.y / 2.0f));
  GameEnv->Game_Events->SetDisableMouseEvents();
  Flag_Shown = true;
}



void ZGameWindow_Inventory::Hide()
{
  GameEnv->GuiManager.RemoveFrame(MainWindow);
  SDL_ShowCursor(SDL_DISABLE);
  SDL_WM_GrabInput(SDL_GRAB_ON);
  GameEnv->Game_Events->SetEnableMouseEvents();
  Flag_Shown = false;
}

Bool ZGameWindow_Inventory::MouseButtonClick(UShort nButton, Short Absolute_x, Short Absolute_y)
{
  Bool Res;
  Res = ZFrame::MouseButtonClick(nButton, Absolute_x, Absolute_y);

  if (CloseBox.Is_MouseClick(true))
  {
    Hide();
  }

  return (Res);
}

