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
 * ZGameWindow_ZProgRobot_Remote.cpp
 *
 *  Created on: 3 juin 2017
 *      Author: laurent
 */

#ifndef Z_ZGAMEWINDOW_ZPROGROBOT_REMOTE_H
#  include "ZGameWindow_ZProgRobot_Remote.h"
#endif

#ifndef Z_ZOS_SPECIFIC_VIEWDOC_H
#  include "ZOs_Specific_ViewDoc.h"
#endif

#ifndef Z_ZOS_SPECIFIC_EDITCODE_H
#  include "ZOs_Specific_EditCode.h"
#endif

#include "ZGame.h"
#include "ZActorPhysics.h"
#include "SDL/SDL.h"


void ZGameWindow_ProgRobot_Remote::DisplayProgramName()
{
  ZStream_File Stream;
  ZString      FileName,FileSpec,File, Keyword, KeywordValue;
  FileName << (ULong)floor(RobotProgramNum.GetValue()) << ".asm";
  (FileSpec = GameEnv->Path_UserScripts_Asm_1).AddToPath(FileName);
  Stream.SetFileName(FileSpec.String);
  if (Stream.GetFileContent(File))
  {
    Keyword = ";";
    File.GetKeywordValue(&Keyword,KeywordValue);
    Text_ProgramName = KeywordValue;
    Text_ProgramName.StripLeading(' ');
    if (Text_ProgramName.Len > 60) Text_ProgramName.SetLen(60);
  }
  else Text_ProgramName.Clear();
  ProgramName.SetDisplayText(Text_ProgramName.String);
}

void ZGameWindow_ProgRobot_Remote::Show()
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
  Rp.y += Size.y;

  float Alignment = Rp.x;


  // Next button line

  Rp.y += Size.y - 67.0;
  Rp.x = Ip.x;
  Rp.x = Alignment;

   // Manual

  Manual.SetDisplayText(Text_Manual.String);
  Manual.SetTextStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2));
  Manual.GetEffectiveDisplaySize(&Size);
  Manual.SetPosition(Rp.x,Rp.y + (32.0 - Size.y / 2.0f));
  Manual.SetSize(Size.x,Size.y);
  MainWindow->AddFrame(&Manual);
  Rp.x += Size.x + 10.0f;
    Rp.y += Size.y + Size.y + 32.0;

  // Robot program DebugShow
/*
  RobotProgramDebugShow.SetDisplayText(Text_RobotProgramDebugShow.String);
  RobotProgramDebugShow.SetTextStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2));
  RobotProgramDebugShow.GetEffectiveDisplaySize(&Size);
  RobotProgramDebugShow.SetPosition(Rp.x,Rp.y + (32.0 - Size.y / 2.0f));
  RobotProgramDebugShow.SetSize(Size.x,Size.y);
  //GetButton.SetColor(1.0f,0.0f,0.0f);
  MainWindow->AddFrame(&RobotProgramDebugShow);
  Rp.y += Size.y + 24.0f;
  Rp.x = Alignment;
*/
  // Program Name

  DisplayProgramName();

  ProgramName.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  ProgramName.SetDisplayText(Text_ProgramName.String);
  ProgramName.GetTextDisplaySize(&Size);
  Size.y = ProgramName.GetStandardLineHeight();  // In case of null string, force line height to something.
  ProgramName.SetPosition(Rp.x ,Rp.y);
  Size.x = 480;
  ProgramName.SetSize(Size.x,Size.y);
  ProgramName.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&ProgramName);
  Rp.y += Size.y - 8.0f;
  Rp.x = Ip.x;



  // Tools Title

  StorageTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  StorageTitle.SetDisplayText(Text_StorageTitle.String);
  StorageTitle.GetTextDisplaySize(&Size);
  StorageTitle.SetPosition(Rp.x ,Rp.y);
  StorageTitle.SetSize(Size.x,Size.y);
  StorageTitle.SetColor(255.0f,255.0f,255.0f);
  MainWindow->AddFrame(&StorageTitle);
  Rp.y += Size.y + 7.0f;

  // Tools

  for (y=0;y<2;y++)
  {
    for (x=0;x<10;x++)
    {
      ULong Indice = 0 + x + y * 10;
      MainStorage[Indice].SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
      MainStorage[Indice].SetPosition(32.0f + x*70.0f, Rp.y );
      MainStorage[Indice].SetSize(64.0f,64.0f);
      MainStorage[Indice].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2), ZInventoryBox::FONTTYPE_MAIN);
      MainStorage[Indice].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1), ZInventoryBox::FONTTYPE_REDUCED);
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
      MainStorage[Indice+20].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2), ZInventoryBox::FONTTYPE_MAIN);
      MainStorage[Indice+20].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1), ZInventoryBox::FONTTYPE_REDUCED);
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

void ZGameWindow_ProgRobot_Remote::Hide()
{
  GameEnv->GuiManager.RemoveFrame(MainWindow);
  SDL_ShowCursor(SDL_DISABLE);
  SDL_WM_GrabInput(SDL_GRAB_ON);
  GameEnv->Game_Events->SetEnableMouseEvents();
  Flag_Shown = false;
}

Bool ZGameWindow_ProgRobot_Remote::MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y)
{
  Bool Res;
  Res = ZFrame::MouseButtonClick(nButton, Absolute_x, Absolute_y);


  if (CloseBox.Is_MouseClick(true))
  {
    Hide();
  }

  if (Manual.Is_MouseClick(true))
  {
    ZViewDoc::ViewDocPage(1705,false);
  }

  return (Res);
}

