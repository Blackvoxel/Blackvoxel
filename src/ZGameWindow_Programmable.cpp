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
 * ZGameWindow_Programmable.cpp
 *
 *  Created on: 4 janv. 2013
 *      Author: laurent
 */

#ifndef Z_ZGAMEWINDOW_PROGRAMMABLE_H
#  include "ZGameWindow_Programmable.h"
#endif

#include "ZGame.h"
#include "ZActorPhysics.h"
#include "SDL/SDL.h"

void ZGameWindow_Programmable::Show()
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



  // Inventory main title

  MainTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_3));
  MainTitle.SetDisplayText(Text_MainTitle.String);
  MainTitle.GetTextDisplaySize(&Size);
  MainTitle.SetPosition(MainWindow_Size.x / 2.0f - Size.x / 2.0f ,Rp.y);
  MainTitle.SetSize(Size.x,Size.y);
  MainTitle.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&MainTitle);
  Rp.y += Size.y + 10.0f;

  // Robot program title

  RobotProgramTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  RobotProgramTitle.SetDisplayText(Text_RobotZoneTitle.String);
  RobotProgramTitle.GetTextDisplaySize(&Size);
  RobotProgramTitle.SetPosition(Rp.x ,Rp.y);
  RobotProgramTitle.SetSize(Size.x,Size.y);
  RobotProgramTitle.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&RobotProgramTitle);
  Rp.y += Size.y + 10.0f;

  // Robot Program choice

  RobotProgramNum.SetFontTileStyle(GameEnv->TileSetStyles->GetStyle(2));
  RobotProgramNum.SetGUITileset(GameEnv->GuiTileset);
  RobotProgramNum.SetPosition(Rp.x , Rp.y);
  RobotProgramNum.SetMinDigits(3);
  RobotProgramNum.SetParameters(0.0,999.0, ProgramNum, 1.0, 0);
  RobotProgramNum.GetTextDisplaySize(&Size);
  RobotProgramNum.SetSize(Size.x + 1.0f, Size.y);
  RobotProgramNum.SetValue(VoxelExtension->ScriptNum);

  MainWindow->AddFrame(&RobotProgramNum);
  Rp.x += Size.x + 20.0f;
  Rp.y -= 15.0f;

  // Robot program Load/Compile

  RobotProgramLoad.SetDisplayText(Text_RobotProgramLoad.String);
  RobotProgramLoad.SetTextStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2));
  RobotProgramLoad.GetEffectiveDisplaySize(&Size);
  RobotProgramLoad.SetPosition(Rp.x,Rp.y + (32.0 - Size.y / 2.0f));
  RobotProgramLoad.SetSize(Size.x,Size.y);
  //GetButton.SetColor(1.0f,0.0f,0.0f);
  MainWindow->AddFrame(&RobotProgramLoad);
  Rp.y += Size.y + 30.0f;
  Rp.x = Ip.x;

  // Tools Title

  StorageTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  StorageTitle.SetDisplayText(Text_StorageTitle.String);
  StorageTitle.GetTextDisplaySize(&Size);
  StorageTitle.SetPosition(Rp.x ,Rp.y);
  StorageTitle.SetSize(Size.x,Size.y);
  StorageTitle.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&StorageTitle);
  Rp.y += Size.y + 5.0f;

  // Tools

  for (y=0;y<2;y++)
  {
    for (x=0;x<10;x++)
    {
      ULong Indice = 0 + x + y * 10;
      MainStorage[Indice].SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
      MainStorage[Indice].SetPosition(32.0f + x*70.0f, Rp.y );
      MainStorage[Indice].SetSize(64.0f,64.0f);
      MainStorage[Indice].SetTileStyle(GameEnv->TileSetStyles->GetStyle(3));
      MainStorage[Indice].SetVoxelType(&VoxelExtension->VoxelType[Indice]);
      MainStorage[Indice].SetQuantity(&VoxelExtension->VoxelQuantity[Indice]);
      MainStorage[Indice].SetColor(128.0f,128.0f,128.0f);
      MainWindow->AddFrame(&MainStorage[Indice]);
    }
    Rp.y += 64.0f + 6.0f;
  }
  Rp.y += 12.0f;

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
      /*
      ULong Indice = ZInventory::Inventory_StartSlot + x + y * 10;
      ZInventory::Entry * Entry = Actor->Inventory->GetSlotRef(Indice);
      MainStorage[Indice+40].SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
      MainStorage[Indice+40].SetPosition(32.0f + x*70.0f, Rp.y );
      MainStorage[Indice+40].SetSize(48.0f,48.0f);
      MainStorage[Indice+40].SetTileStyle(GameEnv->TileSetStyles->GetStyle(3));
      MainStorage[Indice+40].SetVoxelType(&Entry->VoxelType);
      MainStorage[Indice+40].SetQuantity(&Entry->Quantity);
      MainStorage[Indice+40].SetColor(128.0f,128.0f,128.0f);
      MainWindow->AddFrame(&MainStorage[Indice+40]);
      */

      ULong Indice = ZInventory::Inventory_StartSlot + x + y * 10;
      ZInventory::Entry * Entry = Actor->Inventory->GetSlotRef(Indice);
      MainStorage[Indice+20].SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
      MainStorage[Indice+20].SetPosition(32.0f + x*70.0f, Rp.y );
      MainStorage[Indice+20].SetSize(64.0f,64.0f);
      MainStorage[Indice+20].SetTileStyle(GameEnv->TileSetStyles->GetStyle(3));
      MainStorage[Indice+20].SetVoxelType(&Entry->VoxelType);
      MainStorage[Indice+20].SetQuantity(&Entry->Quantity);
      MainStorage[Indice+20].SetColor(128.0f,128.0f,128.0f);
      MainWindow->AddFrame(&MainStorage[Indice+20]);
    }
    Rp.y += 64.0f + 6.0f;
    //Rp.y += 48.0f + 6.0f;
  }
  Rp.y += 12.0f;


  SDL_ShowCursor(SDL_ENABLE);
  SDL_WM_GrabInput(SDL_GRAB_OFF);
  SDL_WarpMouse((Uint16)(MainWindow_Pos.x + MainWindow_Size.x / 2.0f),(Uint16)(MainWindow_Pos.y + MainWindow_Size.y / 2.0f));
  GameEnv->Game_Events->SetDisableMouseEvents();
  Flag_Shown = true;
}

void ZGameWindow_Programmable::Hide()
{
  GameEnv->GuiManager.RemoveFrame(MainWindow);
  SDL_ShowCursor(SDL_DISABLE);
  SDL_WM_GrabInput(SDL_GRAB_ON);
  GameEnv->Game_Events->SetEnableMouseEvents();
  Flag_Shown = false;
}

Bool ZGameWindow_Programmable::MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y)
{
  Bool Res;
  Res = ZFrame::MouseButtonClick(nButton, Absolute_x, Absolute_y);

  if (RobotProgramLoad.Is_MouseClick(true))
  {
    // VoxelExtension->SetScriptNum(floor(RobotProgramNum.GetValue()));
    VoxelExtension->CompileAndRunScript(ZVoxelExtension_Programmable::CONTEXT_PROGRAMCHANGE, floor(RobotProgramNum.GetValue()));
  }
  return (Res);
}

