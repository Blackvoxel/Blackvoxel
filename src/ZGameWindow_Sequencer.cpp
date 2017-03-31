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
 * ZGameWindow_Sequencer.cpp
 *
 *  Created on: 1 mai 2013
 *      Author: laurent
 */

#include "ZGameWindow_Sequencer.h"
#include "ZGame.h"
#include "ZActorPhysics.h"
#include "SDL/SDL.h"

void ZGameWindow_Sequencer::Show()
{
  ZVector2f Rp, Ip, Size, SavePos;
  ZActor * Actor;
  ULong x,y;
  ULong Io;

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

  // Window Main Title

  MainTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_3));
  MainTitle.SetDisplayText(Text_MainTitle.String);
  MainTitle.GetTextDisplaySize(&Size);
  MainTitle.SetPosition(MainWindow_Size.x / 2.0f - Size.x / 2.0f ,Rp.y);
  MainTitle.SetSize(Size.x,Size.y);
  MainTitle.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&MainTitle);
  Rp.y += Size.y + 10.0f;


  SavePos = Rp;
  Rp.x = 600.0f;
  Rp.y += 32.0f;

  // Output Zone Title

  OutputTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  OutputTitle.SetDisplayText(Text_OutputTitle.String);
  OutputTitle.GetTextDisplaySize(&Size);
  OutputTitle.SetPosition(Rp.x ,Rp.y);
  OutputTitle.SetSize(Size.x,Size.y);
  OutputTitle.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&OutputTitle);
  Rp.y += Size.y + 5.0f;

  // Output choice
  Rp.x -= 10.0f;
  OutputNum.SetFontTileStyle(GameEnv->TileSetStyles->GetStyle(2));
  OutputNum.SetGUITileset(GameEnv->GuiTileset);
  OutputNum.SetPosition(Rp.x , Rp.y);
  OutputNum.SetMinDigits(1);
  OutputNum.SetParameters(0.0,5.0, VoxelExtension_Sequencer->OutputLocation , 1.0, 0);
  OutputNum.GetTextDisplaySize(&Size);
  OutputNum.SetSize(Size.x + 1.0f, Size.y);
  OutputNum.SetValue((double)VoxelExtension_Sequencer->OutputLocation);
  MainWindow->AddFrame(&OutputNum);
  // Rp.x += Size.x + 20.0f;
  // Rp.y -= 15.0f;

  Rp = SavePos;

  // Input Zone Title

  InputTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  InputTitle.SetDisplayText(Text_InputTitle.String);
  InputTitle.GetTextDisplaySize(&Size);
  InputTitle.SetPosition(Rp.x ,Rp.y);
  InputTitle.SetSize(Size.x,Size.y);
  InputTitle.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&InputTitle);
  Rp.y += Size.y + 5.0f;


  // Input Zone

  for (y=0;y<2;y++)
  {
    for (x=0;x<10;x++)
    {
      ULong Indice = ZInventory::Inventory_StartSlot + x + y * 10;
      // ZInventory::Entry * Entry = Actor->Inventory->GetSlotRef(Indice);
      MainStorage[Indice].SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
      MainStorage[Indice].SetPosition(32.0f + x*52.0f, Rp.y );
      MainStorage[Indice].SetSize(48.0f,48.0f);
      MainStorage[Indice].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
      MainStorage[Indice].SetVoxelType(&VoxelExtension_Sequencer->VoxelType[Indice + ZVoxelExtension_Sequencer::Start_InputSlots]);
      MainStorage[Indice].SetQuantity(&VoxelExtension_Sequencer->VoxelQuantity[Indice + ZVoxelExtension_Sequencer::Start_InputSlots]);
      MainStorage[Indice].SetColor(128.0f,128.0f,128.0f);
      MainWindow->AddFrame(&MainStorage[Indice]);
    }
    Rp.y += 48.0f + 4.0f;
  }
  Rp.y += 12.0f;

  // Sequence Title

  SequenceTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  SequenceTitle.SetDisplayText(Text_SequenceTitle.String);
  SequenceTitle.GetTextDisplaySize(&Size);
  SequenceTitle.SetPosition(Rp.x ,Rp.y);
  SequenceTitle.SetSize(Size.x,Size.y);
  SequenceTitle.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&SequenceTitle);
  Rp.y += Size.y + 4.0f;

  // Sequence Zone

  //Io = ZVoxelExtension_Sequencer::Input_NumSlots;
  for (y=0;y<4;y++)
  {
    for (x=0;x<20;x++)
    {
      ULong Indice = 0 + x + y * 20;
      SequencerStorage[Indice].SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
      SequencerStorage[Indice].SetPosition(32.0f + x*34.0f, Rp.y );
      SequencerStorage[Indice].SetSize(32.0f,32.0f);
      SequencerStorage[Indice].SetTileStyle(GameEnv->TileSetStyles->GetStyle(0));
      SequencerStorage[Indice].SetVoxelType(&VoxelExtension_Sequencer->VoxelType[Indice+ZVoxelExtension_Sequencer::Start_SequenceSlots]);
      SequencerStorage[Indice].SetQuantity(&VoxelExtension_Sequencer->VoxelQuantity[Indice+ZVoxelExtension_Sequencer::Start_SequenceSlots]);
      SequencerStorage[Indice].SetColor(128.0f,128.0f,128.0f);
      MainWindow->AddFrame(&SequencerStorage[Indice]);
    }
    Rp.y += 32.0f + 3.0f;
  }
  Rp.y += 10.0f;

  // Inventory Zone Title

  InventoryTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  InventoryTitle.SetDisplayText(Text_InventoryTitle.String);
  InventoryTitle.GetTextDisplaySize(&Size);
  InventoryTitle.SetPosition(Rp.x ,Rp.y);
  InventoryTitle.SetSize(Size.x,Size.y);
  InventoryTitle.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&InventoryTitle);
  Rp.y += Size.y + 4.0f;


  // Inventory

  Io = ZVoxelExtension_Sequencer::Input_NumSlots;
  for (y=0;y<4;y++)
  {
    for (x=0;x<10;x++)
    {
      ULong Indice = ZInventory::Inventory_StartSlot + x + y * 10;
      ZInventory::Entry * Entry = Actor->Inventory->GetSlotRef(Indice);
      MainStorage[Indice+Io].SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
      MainStorage[Indice+Io].SetPosition(32.0f + x*70.0f, Rp.y );
      MainStorage[Indice+Io].SetSize(48.0f,48.0f);
      MainStorage[Indice+Io].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
      MainStorage[Indice+Io].SetVoxelType(&Entry->VoxelType);
      MainStorage[Indice+Io].SetQuantity(&Entry->Quantity);
      MainStorage[Indice+Io].SetColor(128.0f,128.0f,128.0f);
      MainWindow->AddFrame(&MainStorage[Indice+Io]);
    }
    Rp.y += 48.0f + 4.0f;
  }
  Rp.y += 12.0f;


  SDL_ShowCursor(SDL_ENABLE);
  SDL_WM_GrabInput(SDL_GRAB_OFF);
  SDL_WarpMouse((Uint16)(MainWindow_Pos.x + MainWindow_Size.x / 2.0f),(Uint16)(MainWindow_Pos.y + MainWindow_Size.y / 2.0f));
  GameEnv->Game_Events->SetDisableMouseEvents();
  Flag_Shown = true;
}

void ZGameWindow_Sequencer::Hide()
{
  GameEnv->GuiManager.RemoveFrame(MainWindow);
  SDL_ShowCursor(SDL_DISABLE);
  SDL_WM_GrabInput(SDL_GRAB_ON);
  GameEnv->Game_Events->SetEnableMouseEvents();
  Flag_Shown = false;
}

Bool ZGameWindow_Sequencer::MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y)
{
  Bool Res;
  Res = ZFrame::MouseButtonClick(nButton, Absolute_x, Absolute_y);

  if (OutputNum.Is_ChoiceChanged(true))
  {
    VoxelExtension_Sequencer->OutputLocation = floor(OutputNum.GetValue());
    if (VoxelExtension_Sequencer->OutputLocation > 5) VoxelExtension_Sequencer->OutputLocation =5;
  }

  if (CloseBox.Is_MouseClick(true))
  {
    Hide();
  }

  return (Res);
}


