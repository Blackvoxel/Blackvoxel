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
 * ZScreen_Message.cpp
 *
 *  Created on: 24 févr. 2014
 *      Author: laurent
 */

#include "ZScreen_Message.h"
#include <GL/glew.h>

ULong ZScreen_Message::ProcessScreen(ZGame * GameEnv)
{

  bool Loop;

  GameEnv->GuiManager.RemoveAllFrames();

  ProceedString = "Press space bar to proceed";

  ZFrame Background;
    Background.SetPosition(0,0);
    Background.SetSize( GameEnv->ScreenResolution.x, GameEnv->ScreenResolution.y );
    Background.SetTexture(13);
    Background.SetZPosition(50.0f);
    GameEnv->GuiManager.AddFrame(&Background);

  ZFrame_FontFrame Frame_Proceed;
    ZVector2f Frame_Size;
    Frame_Proceed.SetDisplayText(ProceedString.String);
    Frame_Proceed.SetStyle(GameEnv->TileSetStyles->GetStyle(1));
    Frame_Proceed.GetTextDisplaySize(&Frame_Size);
    Frame_Proceed.SetPosition(GameEnv->ScreenResolution.x / 2.0 - Frame_Size.x / 2.0, GameEnv->ScreenResolution.y * 0.95 - Frame_Size.y );
    Frame_Proceed.SetSize(Frame_Size.x+1.0,Frame_Size.y);
    Frame_Proceed.SetZPosition(49.0f);
    Frame_Proceed.TextureNum = 3;
    Background.AddFrame(&Frame_Proceed);
    Frame_Proceed.Show(true);

  ULong Timer = 400;
  for (Loop = true; Loop; )
  {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //glAlphaFunc(GL_GREATER, 0.2);
    //glEnable(GL_ALPHA_TEST);
    //glEnable(GL_TEXTURE_2D);
    Loop = GameEnv->EventManager.ProcessEvents();

    if (Timer) Timer-=1;
    else Frame_Proceed.Show(true);

    if (GameEnv->EventManager.Is_KeyPressed(SDLK_SPACE,1)) Loop = false;

    GameEnv->GuiManager.Render();
    SDL_GL_SwapBuffers();
    SDL_Delay(10);
  }
  GameEnv->GuiManager.RemoveAllFrames();
  return(ResultCode);
}



