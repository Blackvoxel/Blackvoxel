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
 * ZScreen_Main.cpp
 *
 *  Created on: 28 mai 2011
 *      Author: laurent
 */

#include "ZScreen_Main.h"
#include <GL/glew.h>

ULong ZScreen_Main::ProcessScreen(ZGame * GameEnv)
{

  bool Loop;

  GameEnv->GuiManager.RemoveAllFrames();


  ZFrame TitleBackground;
    TitleBackground.SetPosition(0,0);
    TitleBackground.SetSize( GameEnv->ScreenResolution.x, GameEnv->ScreenResolution.y );
    TitleBackground.SetTexture(0);
    TitleBackground.SetZPosition(50.0f);
    GameEnv->GuiManager.AddFrame(&TitleBackground);

  ZFrame Title;
    ZVector2f Title_Size;
    Title_Size.x = 1000.0f; Title_Size.y = 100.0f;
    Title.SetPosition(GameEnv->ScreenResolution.x / 2.0 - Title_Size.x / 2.0 , GameEnv->ScreenResolution.y / 8.0 );
    Title.SetSize(Title_Size.x,Title_Size.y);
    Title.SetTexture(1);
    Title.SetZPosition(49.0f);
    TitleBackground.AddFrame(&Title);

  ZFrame_FontFrame Frame_Version;
    ZVector2f Version_Size;
    Version_Size.x = 53.0*8.0; Version_Size.y = 8.0;
    Frame_Version.SetPosition(GameEnv->ScreenResolution.x - Version_Size.x , GameEnv->ScreenResolution.y - Version_Size.y );
    Frame_Version.SetSize(53.0*8.0+1.0,100.0);
    Frame_Version.SetZPosition(49.0f);
    Frame_Version.SetDisplayText((char *)COMPILEOPTION_VERSIONSTRING);
    Frame_Version.TextureNum = 3;
    Frame_Version.SetStyle(GameEnv->TileSetStyles->GetStyle(0));
    TitleBackground.AddFrame(&Frame_Version);

  ZFrame_FontFrame Frame_PlayGame;
    ZVector2f PlayGame_Size;
    PlayGame_Size.x = 9.0*32.0 + 1.0; PlayGame_Size.y = 8.0*4.0;
    Frame_PlayGame.SetPosition(GameEnv->ScreenResolution.x / 2.0 - PlayGame_Size.x / 2.0, GameEnv->ScreenResolution.y / 1.64 - 32.0 );
    Frame_PlayGame.SetSize(PlayGame_Size.x+128.0,PlayGame_Size.y);
    Frame_PlayGame.SetZPosition(49.0f);
    Frame_PlayGame.SetDisplayText((char *)"PLAY GAME");
    Frame_PlayGame.TextureNum = 3;
    Frame_PlayGame.SetStyle(GameEnv->TileSetStyles->GetStyle(1));
    TitleBackground.AddFrame(&Frame_PlayGame);
  ZFrame_FontFrame Frame_Options;
    ZVector2f Options_Size;
    Frame_Options.SetDisplayText((char *)"OPTIONS");
    Frame_Options.SetStyle(GameEnv->TileSetStyles->GetStyle(1));
    Frame_Options.GetTextDisplaySize(&Options_Size);
    Frame_Options.SetPosition(GameEnv->ScreenResolution.x / 2.0 - Options_Size.x / 2.0, GameEnv->ScreenResolution.y / 1.64 + 32.0 );
    Frame_Options.SetSize(Options_Size.x+1.0,Options_Size.y);
    Frame_Options.SetZPosition(49.0f);

    Frame_Options.TextureNum = 3;

    TitleBackground.AddFrame(&Frame_Options);

  ZFrame_FontFrame Frame_Quit;
    ZVector2f Frame_Size;
    Frame_Quit.SetDisplayText((char *)"QUIT");
    Frame_Quit.SetStyle(GameEnv->TileSetStyles->GetStyle(1));
    Frame_Quit.GetTextDisplaySize(&Frame_Size);
    Frame_Quit.SetPosition(GameEnv->ScreenResolution.x / 2.0 - Frame_Size.x / 2.0, GameEnv->ScreenResolution.y / 1.64 + 96.0  );
    Frame_Quit.SetSize(Frame_Size.x+1.0,Frame_Size.y);
    Frame_Quit.SetZPosition(49.0f);

    Frame_Quit.TextureNum = 3;

    TitleBackground.AddFrame(&Frame_Quit);
  //printf("FrameAdress : %lx\n",(unsigned int)&Frame_PlayGame);

  for (Loop = true; Loop; )
  {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glAlphaFunc(GL_GREATER, 0.2);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_TEXTURE_2D);
    Loop = GameEnv->EventManager.ProcessEvents();

    if (Frame_PlayGame.Is_MouseIn() )   { Frame_PlayGame.SetColor(0.5f,0.5f,1.0f); }
    if (Frame_PlayGame.Is_MouseOut())   { Frame_PlayGame.SetColor(1.0f,1.0f,1.0f); }
    if (Frame_Options.Is_MouseIn() )    { Frame_Options.SetColor(0.5f,0.5f,1.0f); }
    if (Frame_Options.Is_MouseOut())    { Frame_Options.SetColor(1.0f,1.0f,1.0f); }
    if (Frame_Quit.Is_MouseIn() )       { Frame_Quit.SetColor(0.5f,0.5f,1.0f); }
    if (Frame_Quit.Is_MouseOut())       { Frame_Quit.SetColor(1.0f,1.0f,1.0f); }
    if (Frame_PlayGame.Is_MouseClick()) {Loop = false; ResultCode = CHOICE_PLAYGAME;  }
    if (Frame_Options.Is_MouseClick())  {Loop = false; ResultCode = CHOICE_OPTIONS; }
    if (Frame_Quit.Is_MouseClick())     {Loop = false; ResultCode = CHOICE_QUIT; }
    GameEnv->GuiManager.Render();
    SDL_GL_SwapBuffers( );
    SDL_Delay(10);

  }
  GameEnv->GuiManager.RemoveAllFrames();
  return(ResultCode);
}
