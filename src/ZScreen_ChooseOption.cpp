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
 * ZScreen_ChooseOption.cpp
 *
 *  Created on: 10 juin 2011
 *      Author: laurent
 */

#include "ZScreen_ChooseOption.h"

#include <GL/glew.h>

ULong ZScreen_ChooseOption::ProcessScreen(ZGame * GameEnv)
{
      ZVector2f FrameSize,Size, Pos;
      ZString As;
      bool Loop;

      // If old looked style, call the specific routine
      if (GameEnv->Settings_Hardware->Setting_OldLooked) return(ProcessScreen_OldLook(GameEnv));

      Pos.x = 0.0f; Pos.y = 0.0f;
      GameEnv->GuiManager.RemoveAllFrames();

      // Frames on screen

      // Définition et réglage de la boite du fond

      ZFrame Background;
        Background.SetPosition(0.0f,0.0f);
        Background.SetSize(GameEnv->ScreenResolution.x, GameEnv->ScreenResolution.y);
        Background.SetTexture(19);
        GameEnv->GuiManager.AddFrame(&Background);

      ZFrame Frame_Blackvoxel;
        FrameSize.x = GameEnv->ScreenResolution.x / 2.0f;
        FrameSize.y = GameEnv->ScreenResolution.y / 13.0f;
        Frame_Blackvoxel.SetTexture(1);
        Frame_Blackvoxel.SetPosition( GameEnv->ScreenResolution.x / 2.0f - FrameSize.x / 2.0f , GameEnv->ScreenResolution.y / 11.0f );
        Frame_Blackvoxel.SetSize(FrameSize.x, FrameSize.y );
        Background.AddFrame(&Frame_Blackvoxel);

      ZFrame_FontFrame Frame_MainTitle;
        Frame_MainTitle.SetDisplayText((char *)"SETTINGS");
        Frame_MainTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_VAR5));
        Frame_MainTitle.GetTextDisplaySize(&Size);
        Frame_MainTitle.SetPosition(GameEnv->ScreenResolution.x / 2.0f - Size.x / 2.0f , GameEnv->ScreenResolution.y / 4.2f );
        Frame_MainTitle.SetSize(Size.x + 1.0,Size.y);
        Frame_MainTitle.TextureNum = 3;
        Background.AddFrame(&Frame_MainTitle);
        Pos.y += Size.y*1.10f;
        //Pos.y += Size.y*1.10f;

      ZFrame Frame_1;
        FrameSize.x = GameEnv->ScreenResolution.x / 3.0f;
        FrameSize.y = GameEnv->ScreenResolution.y / 13.0f;
        Frame_1.SetTexture(5);
        Frame_1.SetPosition( GameEnv->ScreenResolution.x / 3.0f, (GameEnv->ScreenResolution.y / 2.0  ) - 2.0* (GameEnv->ScreenResolution.y/11.0f) );
        Frame_1.SetSize(FrameSize.x, FrameSize.y );
        Background.AddFrame(&Frame_1);

      ZFrame_FontFrame FontFrame_1;
        FontFrame_1.SetDisplayText("DISPLAY");
        FontFrame_1.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_VAR5));
        FontFrame_1.GetTextDisplaySize(&Size);
        FontFrame_1.SetPosition( FrameSize.x / 2.0f - Size.x /2.0f, FrameSize.y / 2.0f - Size.y / 2.0f );
        FontFrame_1.SetSize(Size.x + 1.0f, Size.y);
        FontFrame_1.SetColor(0.665f,0.665f,0.665f);
        Frame_1.AddFrame(&FontFrame_1);

      ZFrame Frame_2;
        FrameSize.x = GameEnv->ScreenResolution.x / 3.0f;
        FrameSize.y = GameEnv->ScreenResolution.y / 13.0f;
        Frame_2.SetTexture(5);
        Frame_2.SetPosition( GameEnv->ScreenResolution.x / 3.0f, (GameEnv->ScreenResolution.y / 2.0  ) - 1.0* (GameEnv->ScreenResolution.y/11.0f) );
        Frame_2.SetSize(FrameSize.x, FrameSize.y );
        Background.AddFrame(&Frame_2);

      ZFrame_FontFrame FontFrame_2;
        FontFrame_2.SetDisplayText("SOUND");
        FontFrame_2.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_VAR5));
        FontFrame_2.GetTextDisplaySize(&Size);
        FontFrame_2.SetPosition( FrameSize.x / 2.0f - Size.x /2.0f, FrameSize.y / 2.0f - Size.y / 2.0f );
        FontFrame_2.SetSize(Size.x + 1.0f, Size.y);
        FontFrame_2.SetColor(0.665f,0.665f,0.665f);
        Frame_2.AddFrame(&FontFrame_2);

      ZFrame Frame_3;
        FrameSize.x = GameEnv->ScreenResolution.x / 3.0f;
        FrameSize.y = GameEnv->ScreenResolution.y / 13.0f;
        Frame_3.SetTexture(5);
        Frame_3.SetPosition( GameEnv->ScreenResolution.x / 3.0f, (GameEnv->ScreenResolution.y / 2.0  ) - 0.0* (GameEnv->ScreenResolution.y/11.0f) );
        Frame_3.SetSize(FrameSize.x, FrameSize.y );
        Background.AddFrame(&Frame_3);

      ZFrame_FontFrame FontFrame_3;
        FontFrame_3.SetDisplayText("GAMEPLAY");
        FontFrame_3.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_VAR5));
        FontFrame_3.GetTextDisplaySize(&Size);
        FontFrame_3.SetPosition( FrameSize.x / 2.0f - Size.x /2.0f, FrameSize.y / 2.0f - Size.y / 2.0f );
        FontFrame_3.SetSize(Size.x + 1.0f, Size.y);
        FontFrame_3.SetColor(0.665f,0.665f,0.665f);
        Frame_3.AddFrame(&FontFrame_3);

      ZFrame Frame_4;
        FrameSize.x = GameEnv->ScreenResolution.x / 3.0f;
        FrameSize.y = GameEnv->ScreenResolution.y / 13.0f;
        Frame_4.SetTexture(5);
        Frame_4.SetPosition( GameEnv->ScreenResolution.x / 3.0f, (GameEnv->ScreenResolution.y / 2.0  ) + 1.0* (GameEnv->ScreenResolution.y/11.0f) );
        Frame_4.SetSize(FrameSize.x, FrameSize.y );
        Background.AddFrame(&Frame_4);

      ZFrame_FontFrame FontFrame_4;
        FontFrame_4.SetDisplayText("KEYMAP");
        FontFrame_4.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_VAR5));
        FontFrame_4.GetTextDisplaySize(&Size);
        FontFrame_4.SetPosition( FrameSize.x / 2.0f - Size.x / 2.0f, FrameSize.y / 2.0f - Size.y / 2.0f );
        FontFrame_4.SetSize(Size.x + 1.0f, Size.y);
        FontFrame_4.SetColor(0.665f,0.665f,0.665f);
        Frame_4.AddFrame(&FontFrame_4);

      ZTileFrame Quit;
        Quit.SetTileSet(GameEnv->GuiTileset);
        Quit.SetTile(4);
        Quit.SetPosition( GameEnv->ScreenResolution.x * 0.78f ,(GameEnv->ScreenResolution.y * 0.77f));
        Quit.SetSize(64.0f,64.0f);
        Background.AddFrame(&Quit);

        for (Loop = true; Loop; )
        {
          glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
          glAlphaFunc(GL_GREATER, 0.05);

          Loop = GameEnv->EventManager.ProcessEvents();

          if (FontFrame_1.Is_MouseIn() )    { FontFrame_1.SetColor(1.0f,1.0f,1.0f); }
          if (FontFrame_1.Is_MouseOut())    { FontFrame_1.SetColor(0.665f,0.665f,0.665f); }

          if (FontFrame_2.Is_MouseIn() )    { FontFrame_2.SetColor(1.0f,1.0f,1.0f); }
          if (FontFrame_2.Is_MouseOut())    { FontFrame_2.SetColor(0.665f,0.665f,0.665f); }

          if (FontFrame_3.Is_MouseIn() )    { FontFrame_3.SetColor(1.0f,1.0f,1.0f); }
          if (FontFrame_3.Is_MouseOut())    { FontFrame_3.SetColor(0.665f,0.665f,0.665f); }

          if (FontFrame_4.Is_MouseIn() )    { FontFrame_4.SetColor(1.0f,1.0f,1.0f); }
          if (FontFrame_4.Is_MouseOut())    { FontFrame_4.SetColor(0.665f,0.665f,0.665f); }

          if (Frame_1.Is_MouseClick()) {ResultCode = CHOICE_DISPLAY; Loop=false;}
          if (Frame_2.Is_MouseClick()) {ResultCode = CHOICE_SOUND;   Loop=false;}
          if (Frame_3.Is_MouseClick()) {ResultCode = CHOICE_MOUSE;   Loop=false;}
          if (Frame_4.Is_MouseClick()) {ResultCode = CHOICE_KEYMAP;  Loop=false;}
          if (Quit.Is_MouseClick())    {ResultCode = CHOICE_QUIT;    Loop=false;}

          GameEnv->GuiManager.Render();
          SDL_GL_SwapBuffers();
          SDL_Delay(10);
        }

        GameEnv->GuiManager.RemoveAllFrames();
        return(0);
}

ULong ZScreen_ChooseOption::ProcessScreen_OldLook(ZGame * GameEnv)
{
      ZVector2f FrameSize,Size, Pos;
      ZString As;
      bool Loop;

      Pos.x = 0.0f; Pos.y = 0.0f;
      GameEnv->GuiManager.RemoveAllFrames();

      ZFrame_FontFrame Frame_MainTitle;
        Frame_MainTitle.SetDisplayText((char *)"SETTINGS");
        Frame_MainTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
        Frame_MainTitle.GetTextDisplaySize(&Size);
        Frame_MainTitle.SetPosition(GameEnv->ScreenResolution.x / 2.0f - Size.x / 2.0f , GameEnv->ScreenResolution.y / 10.0f );
        Frame_MainTitle.SetSize(Size.x + 1.0,Size.y);
        Frame_MainTitle.TextureNum = 3;
        GameEnv->GuiManager.AddFrame(&Frame_MainTitle);
        Pos.y += Size.y*1.10f;

      ZFrame Frame_1;
        FrameSize.x = GameEnv->ScreenResolution.x / 2.0f;
        FrameSize.y = GameEnv->ScreenResolution.y / 10.0f;
        Frame_1.SetTexture(5);
        Frame_1.SetZPosition(50.0f);
        //Frame_1.SetDepth(50.0f);
        Frame_1.SetPosition( GameEnv->ScreenResolution.x / 4.0f, (GameEnv->ScreenResolution.y / 2.0  ) - 2.0* (GameEnv->ScreenResolution.y/10.0f) );
        Frame_1.SetSize(FrameSize.x, FrameSize.y );
        GameEnv->GuiManager.AddFrame(&Frame_1);

      ZFrame_FontFrame FontFrame_1;
        FontFrame_1.SetDisplayText("DISPLAY");
        FontFrame_1.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
        FontFrame_1.GetTextDisplaySize(&Size);
        FontFrame_1.SetPosition( FrameSize.x / 2.0f - Size.x /2.0f, FrameSize.y / 2.0f - Size.y / 2.0f );
        FontFrame_1.SetSize(Size.x + 1.0f, Size.y);
        Frame_1.AddFrame(&FontFrame_1);

      ZFrame Frame_2;
        FrameSize.x = GameEnv->ScreenResolution.x / 2.0f;
        FrameSize.y = GameEnv->ScreenResolution.y / 10.0f;
        Frame_2.SetTexture(5);
        Frame_2.SetPosition( GameEnv->ScreenResolution.x / 4.0f, (GameEnv->ScreenResolution.y / 2.0  ) - 1.0* (GameEnv->ScreenResolution.y/10.0f) );
        Frame_2.SetSize(FrameSize.x, FrameSize.y );
        GameEnv->GuiManager.AddFrame(&Frame_2);

      ZFrame_FontFrame FontFrame_2;
        FontFrame_2.SetDisplayText("SOUND");
        FontFrame_2.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
        FontFrame_2.GetTextDisplaySize(&Size);
        FontFrame_2.SetPosition( FrameSize.x / 2.0f - Size.x /2.0f, FrameSize.y / 2.0f - Size.y / 2.0f );
        FontFrame_2.SetSize(Size.x + 1.0f, Size.y);
        Frame_2.AddFrame(&FontFrame_2);

        ZFrame Frame_3;
          FrameSize.x = GameEnv->ScreenResolution.x / 2.0f;
          FrameSize.y = GameEnv->ScreenResolution.y / 10.0f;
          Frame_3.SetTexture(5);
          Frame_3.SetPosition( GameEnv->ScreenResolution.x / 4.0f, (GameEnv->ScreenResolution.y / 2.0  ) - 0.0* (GameEnv->ScreenResolution.y/10.0f) );
          Frame_3.SetSize(FrameSize.x, FrameSize.y );
          GameEnv->GuiManager.AddFrame(&Frame_3);

        ZFrame_FontFrame FontFrame_3;
          FontFrame_3.SetDisplayText("GAMEPLAY");
          FontFrame_3.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
          FontFrame_3.GetTextDisplaySize(&Size);
          FontFrame_3.SetPosition( FrameSize.x / 2.0f - Size.x /2.0f, FrameSize.y / 2.0f - Size.y / 2.0f );
          FontFrame_3.SetSize(Size.x + 1.0f, Size.y);
          Frame_3.AddFrame(&FontFrame_3);

        ZFrame Frame_4;
          FrameSize.x = GameEnv->ScreenResolution.x / 2.0f;
          FrameSize.y = GameEnv->ScreenResolution.y / 10.0f;
          Frame_4.SetTexture(5);
          Frame_4.SetPosition( GameEnv->ScreenResolution.x / 4.0f, (GameEnv->ScreenResolution.y / 2.0  ) + 1.0* (GameEnv->ScreenResolution.y/10.0f) );
          Frame_4.SetSize(FrameSize.x, FrameSize.y );
          GameEnv->GuiManager.AddFrame(&Frame_4);

        ZFrame_FontFrame FontFrame_4;
          FontFrame_4.SetDisplayText("KEYMAP");
          FontFrame_4.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
          FontFrame_4.GetTextDisplaySize(&Size);
          FontFrame_4.SetPosition( FrameSize.x / 2.0f - Size.x / 2.0f, FrameSize.y / 2.0f - Size.y / 2.0f );
          FontFrame_4.SetSize(Size.x + 1.0f, Size.y);
          Frame_4.AddFrame(&FontFrame_4);

         ZTileFrame Quit;

           Quit.SetTileSet(GameEnv->GuiTileset);
           Quit.SetTile(4);
           Quit.SetPosition( GameEnv->ScreenResolution.x * 0.75  ,(GameEnv->ScreenResolution.y / 2.0  ) + 1.5* (GameEnv->ScreenResolution.y/10.0f));
           Quit.SetSize(32.0f,32.0f);
           GameEnv->GuiManager.AddFrame(&Quit);



        for (Loop = true; Loop; )
        {
          glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

          Loop = GameEnv->EventManager.ProcessEvents();

          if (Frame_MainTitle.Is_MouseClick()) exit(0);

          if (Frame_4.Is_MouseOut()) Frame_4.SetTexture(5);
          if (Frame_3.Is_MouseOut()) Frame_3.SetTexture(5);
          if (Frame_2.Is_MouseOut()) Frame_2.SetTexture(5);
          if (Frame_1.Is_MouseOut()) Frame_1.SetTexture(5);

          if (Frame_4.Is_MouseIn()) Frame_4.SetTexture(6);
          if (Frame_3.Is_MouseIn()) Frame_3.SetTexture(6);
          if (Frame_2.Is_MouseIn()) Frame_2.SetTexture(6);
          if (Frame_1.Is_MouseIn()) Frame_1.SetTexture(6);

          if (Frame_1.Is_MouseClick()) {ResultCode = CHOICE_DISPLAY; Loop=false;}
          if (Frame_2.Is_MouseClick()) {ResultCode = CHOICE_SOUND;   Loop=false;}
          if (Frame_3.Is_MouseClick()) {ResultCode = CHOICE_MOUSE;   Loop=false;}
          if (Frame_4.Is_MouseClick()) {ResultCode = CHOICE_KEYMAP;  Loop=false;}
          if (Quit.Is_MouseClick())    {ResultCode = CHOICE_QUIT;    Loop=false;}

          GameEnv->GuiManager.Render();
          SDL_GL_SwapBuffers();
          SDL_Delay(10);
        }

        GameEnv->GuiManager.RemoveAllFrames();
        return(0);
}
