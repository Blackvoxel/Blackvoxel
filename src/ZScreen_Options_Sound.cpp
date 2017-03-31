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
 * ZScreen_Options_Sound.cpp
 *
 *  Created on: 14 juin 2011
 *      Author: laurent
 */

/*
 * ZScreen_Options.cpp
 *
 *  Created on: 30 mai 2011
 *      Author: laurent
 */

#include "ZScreen_Options_Sound.h"
#include <GL/glew.h>

ULong ZScreen_Options_Sound::ProcessScreen(ZGame * GameEnv)
{
  ZVector2f Size;
  ZVector2f Pos;

  float LeftMargin = GameEnv->ScreenResolution.x * 0.05f;
  Pos.x = LeftMargin; Pos.y = GameEnv->ScreenResolution.y * 0.05f;

  // Clear Screen

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  GameEnv->GuiManager.RemoveAllFrames();

  // Frames on screen

  // Définition et réglage de la boite du fond

  ZFrame Background;
    if(!GameEnv->Settings_Hardware->Setting_OldLooked)
    {
      Background.SetPosition(0.0f,0.0f);
      Background.SetSize(GameEnv->ScreenResolution.x, GameEnv->ScreenResolution.y);
      Background.SetTexture(19);
      GameEnv->GuiManager.AddFrame(&Background);
    }

  ZFrame_FontFrame Frame_MainTitle;
    Frame_MainTitle.SetDisplayText((char *)"SOUND SETTINGS");
    Frame_MainTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_MainTitle.GetTextDisplaySize(&Size);
    Frame_MainTitle.SetPosition(GameEnv->ScreenResolution.x / 2.0f - Size.x / 2.0f , Pos.y );
    Frame_MainTitle.SetSize(Size.x + 1.0,Size.y);
    Frame_MainTitle.TextureNum = 3;
    GameEnv->GuiManager.AddFrame(&Frame_MainTitle);
    Pos.y += Size.y + GameEnv->ScreenResolution.y * 0.10f;

    Pos.y += 32.0f + 16.0f;

    Pos.x = LeftMargin;

  ZFrame_FontFrame Frame_TitleSoundEnable;
    Frame_TitleSoundEnable.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_TitleSoundEnable.SetDisplayText("ENABLE SOUND  ");
    Frame_TitleSoundEnable.GetTextDisplaySize(&Size);
    Frame_TitleSoundEnable.SetPosition(Pos.x, Pos.y);
    Frame_TitleSoundEnable.SetSize(Size.x + 1.0f, Size.y);
    GameEnv->GuiManager.AddFrame(&Frame_TitleSoundEnable);
    Pos.x += Size.x;

  ZFrame_CheckBox Frame_SoundEnable;
    Frame_SoundEnable.SetGUITileset(GameEnv->GuiTileset);
    Frame_SoundEnable.SetPosition(Pos.x , Pos.y);
    Frame_SoundEnable.SetSize(32.0f,32.0f);
    Frame_SoundEnable.SetState(GameEnv->Settings_Hardware->Setting_SoundEnabled);
    GameEnv->GuiManager.AddFrame(&Frame_SoundEnable);
    Pos.y += 32.0f + GameEnv->ScreenResolution.y * 0.05f;


    Pos.x = LeftMargin;

  ZFrame_FontFrame Frame_TitleSoundVolume;
    Frame_TitleSoundVolume.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_TitleSoundVolume.SetDisplayText("SOUND VOLUME  ");
    Frame_TitleSoundVolume.GetTextDisplaySize(&Size);
    Frame_TitleSoundVolume.SetPosition(Pos.x, Pos.y);
    Frame_TitleSoundVolume.SetSize(Size.x + 1.0f, Size.y);
    GameEnv->GuiManager.AddFrame(&Frame_TitleSoundVolume);
    Pos.x += Size.x;

  ZFrame_NumericChoiceBox Frame_SoundVolume;
    Frame_SoundVolume.SetFontTileStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_SoundVolume.SetGUITileset(GameEnv->GuiTileset);
    Frame_SoundVolume.SetPosition(Pos.x , Pos.y);
    Frame_SoundVolume.SetParameters(0.0,100.0, GameEnv->Settings_Hardware->Setting_SoundVolume,5.0, 0);
    Frame_SoundVolume.GetTextDisplaySize(&Size);
    Frame_SoundVolume.SetSize(Size.x + 1.0f, Size.y);
    GameEnv->GuiManager.AddFrame(&Frame_SoundVolume);
    Pos.y += 32.0f + GameEnv->ScreenResolution.y * 0.20f;

  ZFrame_FontFrame Frame_Save;
    Frame_Save.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_Save.SetDisplayText("SAVE");
    Frame_Save.GetTextDisplaySize(&Size);
    Frame_Save.SetPosition(GameEnv->ScreenResolution.x * 0.9f - Size.x -1.0f, GameEnv->ScreenResolution.y * 0.85f - Size.y);
    Frame_Save.SetSize(Size.x + 1.0f, Size.y);
    GameEnv->GuiManager.AddFrame(&Frame_Save);


/*
  ZFrame_NumericChoiceBox Frame_NBox;
    Frame_NBox.SetFontTileStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_NBox.SetGUITileset(GameEnv->GuiTileset);
    Frame_NBox.SetPosition(0.0f,Pos.y);
    Frame_NBox.SetParameters(0.0, 1000.0 ,50.0,50.0);
    Frame_NBox.GetTextDisplaySize(&Size);
    Frame_NBox.SetSize(Size.x +1.0f,32.0f);
    GameEnv->GuiManager.AddFrame(&Frame_NBox);
    Pos.y += 32.0f + 16.0f;
*/



    for (bool Loop = true; Loop; )
    {
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      Loop = GameEnv->EventManager.ProcessEvents();

      // if (Frame_MainTitle.Is_MouseClick()) {Loop = false; GameEnv->Settings_Hardware->Save(); exit(0); }


      if (Frame_SoundEnable.Is_StateChanged())
      {
        if (Frame_SoundEnable.GetState())  { GameEnv->Settings_Hardware->Setting_SoundEnabled = true;}
        else                               { GameEnv->Settings_Hardware->Setting_SoundEnabled = false; }
      }

      if (Frame_SoundVolume.Is_ChoiceChanged())
      {
        GameEnv->Settings_Hardware->Setting_SoundVolume = Frame_SoundVolume.GetValue();
      }

      if (Frame_Save.Is_MouseClick()) { Loop = false; }

      GameEnv->GuiManager.Render();
      SDL_GL_SwapBuffers( );
      SDL_Delay(10);

    }
    GameEnv->GuiManager.RemoveAllFrames();

    // Reset Sound server.
    GameEnv->Cleanup_Sound(GameEnv->InitLog.Sec(2130));
    GameEnv->Init_Sound(GameEnv->InitLog.Sec(1130));

    GameEnv->Settings_Hardware->Save();

  return(CHOICE_QUIT);
}


/*
ULong ZScreen_Options_Sound::ProcessScreen(ZGame * GameEnv)
{
  ZVector2f Size;
  ZVector2f Pos;

  Pos.x = 0.0; Pos.y = GameEnv->ScreenResolution.y * 0.05f;

  // Clear Screen

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  GameEnv->GuiManager.RemoveAllFrames();

  // Frames on screen

  ZFrame_FontFrame Frame_MainTitle;
    Frame_MainTitle.SetDisplayText((char *)"SOUND SETTINGS");
    Frame_MainTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_MainTitle.GetTextDisplaySize(&Size);
    Frame_MainTitle.SetPosition(GameEnv->ScreenResolution.x / 2.0f - Size.x / 2.0f , Pos.y );
    Frame_MainTitle.SetSize(Size.x + 1.0,Size.y);
    Frame_MainTitle.TextureNum = 3;
    GameEnv->GuiManager.AddFrame(&Frame_MainTitle);
    Pos.y += Size.y + GameEnv->ScreenResolution.y * 0.10f;

    Pos.y += 32.0f + 16.0f;

  Pos.x = 0.0;
  ZFrame_FontFrame Frame_TitleSoundEnable;
    Frame_TitleSoundEnable.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_TitleSoundEnable.SetDisplayText("ENABLE SOUND  ");
    Frame_TitleSoundEnable.GetTextDisplaySize(&Size);
    Frame_TitleSoundEnable.SetPosition(Pos.x, Pos.y);
    Frame_TitleSoundEnable.SetSize(Size.x + 1.0f, Size.y);
    GameEnv->GuiManager.AddFrame(&Frame_TitleSoundEnable);
    Pos.x += Size.x;

  ZFrame_CheckBox Frame_SoundEnable;
    Frame_SoundEnable.SetGUITileset(GameEnv->GuiTileset);
    Frame_SoundEnable.SetPosition(Pos.x , Pos.y);
    Frame_SoundEnable.SetSize(32.0f,32.0f);
    Frame_SoundEnable.SetState(GameEnv->Settings_Hardware->Setting_SoundEnabled);
    GameEnv->GuiManager.AddFrame(&Frame_SoundEnable);
    Pos.y += 32.0f + GameEnv->ScreenResolution.y * 0.05f;


  Pos.x = 0.0;
  ZFrame_FontFrame Frame_TitleSoundVolume;
    Frame_TitleSoundVolume.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_TitleSoundVolume.SetDisplayText("SOUND VOLUME  ");
    Frame_TitleSoundVolume.GetTextDisplaySize(&Size);
    Frame_TitleSoundVolume.SetPosition(Pos.x, Pos.y);
    Frame_TitleSoundVolume.SetSize(Size.x + 1.0f, Size.y);
    GameEnv->GuiManager.AddFrame(&Frame_TitleSoundVolume);
    Pos.x += Size.x;

  ZFrame_NumericChoiceBox Frame_SoundVolume;
    Frame_SoundVolume.SetFontTileStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_SoundVolume.SetGUITileset(GameEnv->GuiTileset);
    Frame_SoundVolume.SetPosition(Pos.x , Pos.y);
    Frame_SoundVolume.SetParameters(0.0,100.0, GameEnv->Settings_Hardware->Setting_SoundVolume,5.0, 0);
    Frame_SoundVolume.GetTextDisplaySize(&Size);
    Frame_SoundVolume.SetSize(Size.x + 1.0f, Size.y);
    GameEnv->GuiManager.AddFrame(&Frame_SoundVolume);
    Pos.y += 32.0f + GameEnv->ScreenResolution.y * 0.20f;

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


      if (Frame_SoundEnable.Is_StateChanged())
      {
        if (Frame_SoundEnable.GetState())  { GameEnv->Settings_Hardware->Setting_SoundEnabled = true;}
        else                               { GameEnv->Settings_Hardware->Setting_SoundEnabled = false; }
      }

      if (Frame_SoundVolume.Is_ChoiceChanged())
      {
        GameEnv->Settings_Hardware->Setting_SoundVolume = Frame_SoundVolume.GetValue();
      }

      if (Frame_Save.Is_MouseClick()) { Loop = false; }

      GameEnv->GuiManager.Render();
      SDL_GL_SwapBuffers( );
      SDL_Delay(10);

    }
    GameEnv->GuiManager.RemoveAllFrames();

    // Reset Sound server.
    GameEnv->Cleanup_Sound(GameEnv->InitLog.Sec(2130));
    GameEnv->Init_Sound(GameEnv->InitLog.Sec(1130));

    GameEnv->Settings_Hardware->Save();

  return(CHOICE_QUIT);
}
*/
