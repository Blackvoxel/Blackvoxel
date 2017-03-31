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
 * ZScreen_Options_Keymap.cpp
 *
 *  Created on: 15 juin 2011
 *      Author: laurent
 */

#include "ZScreen_Options_Keymap.h"
#include <GL/glew.h>

ULong ZScreen_Options_Keymap::ProcessScreen(ZGame * GameEnv)
{
  ZVector2f Size,Size2;
  ZVector2f Pos;
  ZVector2f Start;
  ULong i;
  bool Ol;

  Ol = GameEnv->Settings_Hardware->Setting_OldLooked;

  Start.x = GameEnv->ScreenResolution.x * 0.05f;
  Start.y = GameEnv->ScreenResolution.y * 0.05f;


  Pos.x = Start.x; Pos.y = Start.y;


  // Clear Screen

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  GameEnv->GuiManager.RemoveAllFrames();

  // Frames on screen

  ZFrame Background;
    if(!Ol)
    {
      Background.SetPosition(0.0f,0.0f);
      Background.SetSize(GameEnv->ScreenResolution.x, GameEnv->ScreenResolution.y);
      Background.SetTexture(19);
      GameEnv->GuiManager.AddFrame(&Background);
    }

  ZFrame_FontFrame Frame_MainTitle;
    Frame_MainTitle.SetDisplayText((char *)"KEYMAP SETTINGS ");
    Frame_MainTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_MainTitle.GetTextDisplaySize(&Size);
    Frame_MainTitle.SetPosition(GameEnv->ScreenResolution.x / 2.0f - Size.x / 2.0f , Pos.y );
    Frame_MainTitle.SetSize(Size.x + 1.0,Size.y);
    Frame_MainTitle.TextureNum = 3;
    if (Ol) GameEnv->GuiManager.AddFrame(&Frame_MainTitle);
    else    Background.AddFrame(&Frame_MainTitle);
    Pos.y += Size.y + GameEnv->ScreenResolution.y * 0.10f;


  ULong NumLines = 8;
  ZFrame_KeyChooser KeyChooser[NumLines];
  ZFrame_FontFrame  KeyTitle[NumLines];
  ZVector2f         Position[NumLines];
  char const *            Title[NumLines];
  UShort * Value[NumLines];

  Title[0] = "MOVE FORWARD";   Value[0] = &GameEnv->Settings_Hardware->Setting_Key_MoveForward;
  Title[1] = "MOVE BACKWARD";  Value[1] = &GameEnv->Settings_Hardware->Setting_Key_MoveBackward;
  Title[2] = "MOVE LEFT";      Value[2] = &GameEnv->Settings_Hardware->Setting_Key_MoveLeft;
  Title[3] = "MOVE RIGHT";     Value[3] = &GameEnv->Settings_Hardware->Setting_Key_MoveRight;
  Title[4] = "MOVE UP";        Value[4] = &GameEnv->Settings_Hardware->Setting_Key_MoveUp;
  Title[5] = "MOVE DOWN";      Value[5] = &GameEnv->Settings_Hardware->Setting_Key_MoveDown;
  Title[6] = "JUMP";           Value[6] = &GameEnv->Settings_Hardware->Setting_Key_Jump;
  Title[7] = "INVENTORY";      Value[7] = &GameEnv->Settings_Hardware->Setting_Key_Inventory;

  Pos.y = GameEnv->ScreenResolution.y * 0.2f;

  float MaxTitleWidth = 0.0;
  for(i=0;i<NumLines;i++)
  {
    Pos.x = Start.x;
    if (GameEnv->ScreenResolution.y > 800.0f ) KeyTitle[i].SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    else                                      KeyTitle[i].SetStyle(GameEnv->TileSetStyles->GetStyle(3));
    KeyTitle[i].SetDisplayText(Title[i]);
    KeyTitle[i].GetTextDisplaySize(&Size);
    KeyTitle[i].SetPosition(Pos.x, Pos.y);
    KeyTitle[i].SetSize(Size.x,Size.y);
    if (Ol) GameEnv->GuiManager.AddFrame(&KeyTitle[i]);
    else    Background.AddFrame(&KeyTitle[i]);
    Position[i].x = Pos.x; Position[i].y = Pos.y;
    if (Size.x > MaxTitleWidth) MaxTitleWidth = Size.x;

    KeyChooser[i].SetFontTileStyle(GameEnv->TileSetStyles->GetStyle(3));
    KeyChooser[i].GetTextDisplaySize(&Size2);
    KeyChooser[i].SetPosition(800.0f,Pos.y + Size.y /2.0f - Size2.y/2.0f);
    KeyChooser[i].SetSize(Size2.x,Size2.y + 8.0f * ((float)GameEnv->ScreenResolution.y) / 1080.f );
    KeyChooser[i].SetTexture(5);
    KeyChooser[i].SetKey(*Value[i]);
    KeyChooser[i].Show_Frame(true);
    if (Ol) GameEnv->GuiManager.AddFrame(&KeyChooser[i]);
    else    Background.AddFrame(&KeyChooser[i]);
    Pos.y = Pos.y + Size.y + 8.0f  ;
  }


  for (i=0;i<NumLines;i++) { KeyChooser[i].SetPosition(Position[i].x + MaxTitleWidth + GameEnv->ScreenResolution.x / 15.0f,Position[i].y + Size.y /2.0f - Size2.y/2.0f); }


  ZFrame_FontFrame Frame_Save;
    Frame_Save.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_Save.SetDisplayText("SAVE");
    Frame_Save.GetTextDisplaySize(&Size);
    Frame_Save.SetPosition(GameEnv->ScreenResolution.x * 0.9f - Size.x -1.0f, GameEnv->ScreenResolution.y * 0.85f - Size.y);
    Frame_Save.SetSize(Size.x + 1.0f, Size.y);
    if (Ol) GameEnv->GuiManager.AddFrame(&Frame_Save);
    else    Background.AddFrame(&Frame_Save);


    for (bool Loop = true; Loop; )
    {
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      Loop = GameEnv->EventManager.ProcessEvents();

      // if (Frame_MainTitle.Is_MouseClick()) {Loop = false; GameEnv->Settings_Hardware->Save(); exit(0); }

      for (i=0;i<NumLines;i++) if ( KeyChooser[i].Is_KeyChanged() ) *Value[i] = KeyChooser[i].GetKey();



      if (Frame_Save.Is_MouseClick()) { Loop = false; }

      GameEnv->GuiManager.Render();
      SDL_GL_SwapBuffers( );
      SDL_Delay(10);

    }
    GameEnv->GuiManager.RemoveAllFrames();

    GameEnv->Settings_Hardware->Save();

  return(CHOICE_QUIT);
}


/*
ULong ZScreen_Options_Keymap::ProcessScreen(ZGame * GameEnv)
{
  ZVector2f Size,Size2;
  ZVector2f Pos;
  ZVector2f Start;
  ULong i;

  Start.x = GameEnv->ScreenResolution.x * 0.05f;
  Start.y = GameEnv->ScreenResolution.y * 0.05f;

  Pos.x = Start.x; Pos.y = Start.y;

  // Clear Screen

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  GameEnv->GuiManager.RemoveAllFrames();

  // Frames on screen

  ZFrame_FontFrame Frame_MainTitle;
    Frame_MainTitle.SetDisplayText((char *)"KEYMAP SETTINGS ");
    Frame_MainTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_MainTitle.GetTextDisplaySize(&Size);
    Frame_MainTitle.SetPosition(GameEnv->ScreenResolution.x / 2.0f - Size.x / 2.0f , Pos.y );
    Frame_MainTitle.SetSize(Size.x + 1.0,Size.y);
    Frame_MainTitle.TextureNum = 3;
    GameEnv->GuiManager.AddFrame(&Frame_MainTitle);
    Pos.y += Size.y + GameEnv->ScreenResolution.y * 0.10f;
    Pos.y += 32.0f + 16.0f;

  ULong NumLines = 8;
  ZFrame_KeyChooser KeyChooser[NumLines];
  ZFrame_FontFrame  KeyTitle[NumLines];
  ZVector2f         Position[NumLines];
  char const *            Title[NumLines];
  UShort * Value[NumLines];

  Title[0] = "MOVE FORWARD";   Value[0] = &GameEnv->Settings_Hardware->Setting_Key_MoveForward;
  Title[1] = "MOVE BACKWARD";  Value[1] = &GameEnv->Settings_Hardware->Setting_Key_MoveBackward;
  Title[2] = "MOVE LEFT";      Value[2] = &GameEnv->Settings_Hardware->Setting_Key_MoveLeft;
  Title[3] = "MOVE RIGHT";     Value[3] = &GameEnv->Settings_Hardware->Setting_Key_MoveRight;
  Title[4] = "MOVE UP";        Value[4] = &GameEnv->Settings_Hardware->Setting_Key_MoveUp;
  Title[5] = "MOVE DOWN";      Value[5] = &GameEnv->Settings_Hardware->Setting_Key_MoveDown;
  Title[6] = "JUMP";           Value[6] = &GameEnv->Settings_Hardware->Setting_Key_Jump;
  Title[7] = "INVENTORY";      Value[7] = &GameEnv->Settings_Hardware->Setting_Key_Inventory;

  float MaxTitleWidth = 0.0;
  for(i=0;i<NumLines;i++)
  {
    Pos.x = Start.x;
    if (GameEnv->ScreenResolution.y > 800.0f ) KeyTitle[i].SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    else                                      KeyTitle[i].SetStyle(GameEnv->TileSetStyles->GetStyle(3));
    KeyTitle[i].SetDisplayText(Title[i]);
    KeyTitle[i].GetTextDisplaySize(&Size);
    KeyTitle[i].SetPosition(Pos.x, Pos.y);
    KeyTitle[i].SetSize(Size.x,Size.y);
    GameEnv->GuiManager.AddFrame(&KeyTitle[i]);
    Position[i].x = Pos.x; Position[i].y = Pos.y;
    if (Size.x > MaxTitleWidth) MaxTitleWidth = Size.x;

    KeyChooser[i].SetFontTileStyle(GameEnv->TileSetStyles->GetStyle(3));
    KeyChooser[i].GetTextDisplaySize(&Size2);
    KeyChooser[i].SetPosition(800.0f,Pos.y + Size.y /2.0f - Size2.y/2.0f);
    KeyChooser[i].SetSize(Size2.x,Size2.y);
    KeyChooser[i].SetTexture(5);
    KeyChooser[i].SetKey(*Value[i]);
    KeyChooser[i].Show_Frame(true);
    GameEnv->GuiManager.AddFrame(&KeyChooser[i]);
    Pos.y = Pos.y + Size.y;
  }


  for (i=0;i<NumLines;i++) { KeyChooser[i].SetPosition(Position[i].x + MaxTitleWidth + GameEnv->ScreenResolution.x / 15.0f,Position[i].y + Size.y /2.0f - Size2.y/2.0f); }


  ZFrame_FontFrame Frame_Save;
    Frame_Save.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_Save.SetDisplayText("SAVE");
    Frame_Save.GetTextDisplaySize(&Size);
    Frame_Save.SetPosition(GameEnv->ScreenResolution.x * 0.9f - Size.x -1.0f, GameEnv->ScreenResolution.y * 0.9f - Size.y);
    Frame_Save.SetSize(Size.x + 1.0f, Size.y);
    GameEnv->GuiManager.AddFrame(&Frame_Save);


    for (bool Loop = true; Loop; )
    {
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      Loop = GameEnv->EventManager.ProcessEvents();

      // if (Frame_MainTitle.Is_MouseClick()) {Loop = false; GameEnv->Settings_Hardware->Save(); exit(0); }

      for (i=0;i<NumLines;i++) if ( KeyChooser[i].Is_KeyChanged() ) *Value[i] = KeyChooser[i].GetKey();



      if (Frame_Save.Is_MouseClick()) { Loop = false; }

      GameEnv->GuiManager.Render();
      SDL_GL_SwapBuffers( );
      SDL_Delay(10);

    }
    GameEnv->GuiManager.RemoveAllFrames();

    GameEnv->Settings_Hardware->Save();

  return(CHOICE_QUIT);
}
*/
