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
 * ZGameWindow_UserTextureTransformer.cpp
 *
 *  Created on: 29 févr. 2012
 *      Author: laurent
 */

#include "ZGameWindow_UserTextureTransformer.h"
#include "ZGame.h"
#include "ZActorPhysics.h"
#include "SDL/SDL.h"

void ZGameWindow_UserTextureTransformer::Show()
{
  ZVector2f Rp, Ip, Size,Size2;
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

  this->SetPosition( MainWindow_Pos.x, MainWindow_Pos.y );
  this->SetSize(MainWindow_Size.x,MainWindow_Size.y);
  this->SetTexture(8);
  GameEnv->GuiManager.AddFrame(this);

  // CloseBox

  Size.x = 32.0f; Size.y = 32.0f;
  CloseBox.SetPosition(MainWindow_Size.x - Size.x - 5.0f, 5.0f);
  CloseBox.SetSize(Size.x, Size.y);
  CloseBox.SetTileSet(GameEnv->GuiTileset);
  CloseBox.SetTile(11);
  this->AddFrame(&CloseBox);

  // Main title

  MainTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_3));
  MainTitle.SetDisplayText(Text_MainTitle.String);
  MainTitle.GetTextDisplaySize(&Size);
  MainTitle.SetPosition(MainWindow_Size.x / 2.0f - Size.x / 2.0f ,Rp.y);
  MainTitle.SetSize(Size.x,Size.y);
  MainTitle.SetColor(255.0f,255.0f,255.0f);
  this->AddFrame(&MainTitle);
  Rp.y += Size.y + 20.0f;

  // Chooser Title

  ActionTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  ActionTitle.SetDisplayText(Text_ActionTitle.String);
  ActionTitle.GetTextDisplaySize(&Size);
  ActionTitle.SetPosition(Rp.x ,Rp.y);
  ActionTitle.SetSize(Size.x+1.0f,Size.y);
  ActionTitle.SetColor(255.0f,255.0f,255.0f);
  this->AddFrame(&ActionTitle);
  Rp.y += Size.y + 15.0f;

  // Chooser

  Rp.x = 32.0f;
  ArrowLeft.SetPosition(Rp.x,Rp.y+16.0f);
  ArrowLeft.SetTileSet(GameEnv->GuiTileset);
  ArrowLeft.SetSize(32.0f,32.0f);
  ArrowLeft.SetTile(0);
  this->AddFrame(&ArrowLeft);
  Rp.x += 36.0f;

  Size.x = 64; Size.y = 64;
  VoxelTypeView.SetPosition(Rp.x ,Rp.y);
  VoxelTypeView.SetSize(Size.x,Size.y);
  VoxelTypeView.SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
  VoxelTypeView.SetVoxelType(&VoxelExtension_UserTextureTransformer->ActualVoxel);
  this->AddFrame(&VoxelTypeView);
  Rp.x+=68.0f;

  ArrowRight.SetPosition(Rp.x,Rp.y+16.0f);
  ArrowRight.SetTileSet(GameEnv->GuiTileset);
  ArrowRight.SetSize(32.0f,32.0f);
  ArrowRight.SetTile(1);
  this->AddFrame(&ArrowRight);
  Rp.x += 40.0f+15.0f;


  GetButton.SetDisplayText(Text_Get.String);
  GetButton.SetTextStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2));
  GetButton.GetEffectiveDisplaySize(&Size2);
  GetButton.SetPosition(Rp.x,Rp.y + (32.0 - Size2.y / 2.0f));
  GetButton.SetSize(Size2.x,Size2.y);
  //GetButton.SetColor(1.0f,0.0f,0.0f);
  this->AddFrame(&GetButton);
  Rp.x = Ip.x;
  Rp.y += Size.y + 20.0f;

  // Tools Title

  StorageTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));
  StorageTitle.SetDisplayText(Text_StorageTitle.String);
  StorageTitle.GetTextDisplaySize(&Size);
  StorageTitle.SetPosition(Rp.x ,Rp.y);
  StorageTitle.SetSize(Size.x+1.0f,Size.y);
  StorageTitle.SetColor(255.0f,255.0f,255.0f);
  this->AddFrame(&StorageTitle);
  Rp.y += Size.y + 5.0f;

  // Tools

  for (y=0;y<1;y++)
  {
    for (x=0;x<10;x++)
    {
      ULong Indice = 0 + x + y * 10;
      MainStorage[Indice].SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
      MainStorage[Indice].SetPosition(32.0f + x*70.0f, Rp.y );
      MainStorage[Indice].SetSize(64.0f,64.0f);
      MainStorage[Indice].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2), ZInventoryBox::FONTTYPE_MAIN);
      MainStorage[Indice].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1), ZInventoryBox::FONTTYPE_REDUCED);
      MainStorage[Indice].SetVoxelType(&VoxelExtension_UserTextureTransformer->VoxelType[Indice]);
      MainStorage[Indice].SetQuantity(&VoxelExtension_UserTextureTransformer->VoxelQuantity[Indice]);
      MainStorage[Indice].SetColor(128.0f,128.0f,128.0f);
      this->AddFrame(&MainStorage[Indice]);
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
  this->AddFrame(&InventoryTitle);
  Rp.y += Size.y + 5.0f;


  // Inventory

  for (y=0;y<4;y++)
  {
    for (x=0;x<10;x++)
    {
      ULong Indice = ZInventory::Inventory_StartSlot + x + y * 10;
      ZInventory::Entry * Entry = Actor->Inventory->GetSlotRef(Indice);
      MainStorage[Indice+10].SetVoxelTypeManager(&GameEnv->VoxelTypeManager);
      MainStorage[Indice+10].SetPosition(32.0f + x*70.0f, Rp.y );
      MainStorage[Indice+10].SetSize(64.0f,64.0f);
      MainStorage[Indice+10].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2), ZInventoryBox::FONTTYPE_MAIN);
      MainStorage[Indice+10].SetTileStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1), ZInventoryBox::FONTTYPE_REDUCED);
      MainStorage[Indice+10].SetVoxelType(&Entry->VoxelType);
      MainStorage[Indice+10].SetQuantity(&Entry->Quantity);
      MainStorage[Indice+10].SetColor(128.0f,128.0f,128.0f);
      this->AddFrame(&MainStorage[Indice+10]);
    }
    Rp.y += 64.0f + 6.0f;
  }
  Rp.y += 12.0f;


  SDL_ShowCursor(SDL_ENABLE);
  SDL_WM_GrabInput(SDL_GRAB_OFF);
  SDL_WarpMouse((Uint16)(MainWindow_Pos.x + MainWindow_Size.x / 2.0f),(Uint16)(MainWindow_Pos.y + MainWindow_Size.y / 2.0f));
  GameEnv->Game_Events->SetDisableMouseEvents();
  Flag_Shown = true;
}

void ZGameWindow_UserTextureTransformer::Hide()
{
  GameEnv->GuiManager.RemoveFrame(this);
  SDL_ShowCursor(SDL_DISABLE);
  SDL_WM_GrabInput(SDL_GRAB_ON);

  GameEnv->Game_Events->SetEnableMouseEvents();
  Flag_Shown = false;
}

Bool ZGameWindow_UserTextureTransformer::MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y)
{
  Bool Res;
  Res = ZFrame::MouseButtonClick(nButton, Absolute_x, Absolute_y);

  if (ArrowLeft.Is_MouseClick(true))
  {
    ZVoxelType * VoxelType;
    UShort NewVoxelType;

    NewVoxelType = VoxelExtension_UserTextureTransformer->ActualVoxel - 1;
    if (NewVoxelType<32768) NewVoxelType = 32768;
    if ((VoxelType = GameEnv->VoxelTypeManager.GetVoxelType(NewVoxelType)))
    {
      if (VoxelType->Is_NoType) NewVoxelType = 32768;
      VoxelExtension_UserTextureTransformer->ActualVoxel = NewVoxelType;
      // printf("ActualVoxelType %d :%s\n",VoxelExtension_UserTextureTransformer->ActualVoxel, VoxelType->VoxelTypeName.String);
    }
  }
  if (ArrowRight.Is_MouseClick(true))
  {
    ZVoxelType * VoxelType;
    UShort NewVoxelType;

    NewVoxelType = VoxelExtension_UserTextureTransformer->ActualVoxel + 1;
    if (NewVoxelType<32768) NewVoxelType = 32768;
    if ((VoxelType = GameEnv->VoxelTypeManager.GetVoxelType(NewVoxelType)))
    {
      if (!VoxelType->Is_NoType)
      {
        VoxelExtension_UserTextureTransformer->ActualVoxel = NewVoxelType;
        // printf("ActualVoxelType %d :%s\n",VoxelExtension_UserTextureTransformer->ActualVoxel, VoxelType->VoxelTypeName.String);
      }
    }
  }

  if (GetButton.Is_MouseClick(true))
  {
    ULong i;
    UShort * DataVoxelType;
    ZVoxelType * VoxelType, * ModelVoxelType;

    if (VoxelExtension_UserTextureTransformer->ActualVoxel >= 32768)
    {
      ModelVoxelType = GameEnv->VoxelTypeManager.GetVoxelType(VoxelExtension_UserTextureTransformer->ActualVoxel);
      if (ModelVoxelType != 0)
        if (!ModelVoxelType->Is_NoType)
          for (i=0;i<10;i++)
          {
            DataVoxelType = &VoxelExtension_UserTextureTransformer->VoxelType[i];
            if (VoxelExtension_UserTextureTransformer->VoxelQuantity[i] > 0)
              if ( (VoxelType = GameEnv->VoxelTypeManager.GetVoxelType(*DataVoxelType)))
              {
                if (VoxelType->Is_UserTypeTransformable) *DataVoxelType = VoxelExtension_UserTextureTransformer->ActualVoxel;
              }
          }
    }
  }

  if (CloseBox.Is_MouseClick(true))
  {
    Hide();
  }
  return (Res);
}



