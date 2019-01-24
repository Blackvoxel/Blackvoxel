/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2018 Laurent Thiebaut & Olivia Merle
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
 * ZScreen_UnsupportedGameMode.cpp
 *
 *  Created on: 19 oct. 2018
 *      Author: laurent
 */

#ifndef Z_ZSCREEN_UNSUPPORTEDGAMEMODE_H
#  include "ZScreen_UnsupportedGameMode.h"
#endif

#include <GL/glew.h>

#ifndef Z_GUI_FONTFRAME_H
#  include "ZGui_FontFrame.h"
#endif

ULong ZScreen_UnsupportedGameMode::ProcessScreen(ZGame * GameEnv)
{
  ZVector2f Size, Pos;


  // Clear Screen

  GameEnv->GuiManager.RemoveAllFrames();

  // Text


  ZFrame_FontFrame Frame_MainTitle;
  Frame_MainTitle.SetDisplayText((char *)"UNSUPPORTED GAMEMODE");
  Frame_MainTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_VAR7));
  Frame_MainTitle.GetTextDisplaySize(&Size);
  Pos.y = (GameEnv->ScreenResolution.y / 2.0f) - (Size.y / 1.0f);
  Frame_MainTitle.SetPosition(GameEnv->ScreenResolution.x / 2.0f - Size.x / 2.0f , Pos.y );
  Frame_MainTitle.SetSize(Size.x + 1.0,Size.y);
  Frame_MainTitle.TextureNum = 3;
  GameEnv->GuiManager.AddFrame(&Frame_MainTitle);
  Pos.y += Size.y + GameEnv->ScreenResolution.y * 0.05f;

  ZFrame_FontFrame Frame_SubTitle;
  Frame_SubTitle.SetDisplayText((char *)"ADVENTURE GAMEMODE IS NOT SUPPORTED IN THE SCHOOL VERSION");
  Frame_SubTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_VAR2));
  Frame_SubTitle.GetTextDisplaySize(&Size);
  Frame_SubTitle.SetPosition(GameEnv->ScreenResolution.x / 2.0f - Size.x / 2.0f , Pos.y );
  Frame_SubTitle.SetSize(Size.x + 1.0,Size.y);
  Frame_SubTitle.TextureNum = 3;
  GameEnv->GuiManager.AddFrame(&Frame_SubTitle);
  Pos.y += Size.y + GameEnv->ScreenResolution.y * 0.10f;

  ZFrame_FontFrame Frame_Save;
  Frame_Save.SetStyle(GameEnv->TileSetStyles->GetStyle(4));
  Frame_Save.SetDisplayText("PROCEED");
  Frame_Save.GetTextDisplaySize(&Size);
  Frame_Save.SetPosition(GameEnv->ScreenResolution.x * 0.9f - Size.x -1.0f, GameEnv->ScreenResolution.y * 0.85f - Size.y);
  Frame_Save.SetSize(Size.x + 1.0f, Size.y);
  GameEnv->GuiManager.AddFrame(&Frame_Save);


  // Boucle d'affichage et de gestion des évènements

  for (bool Loop = true; Loop; )
  {
    // Effacer l'écran

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glAlphaFunc(GL_GREATER, 0.05);

    // Gérer les évènements entrants (clics, déplacemets souris, frappe clavier)

    Loop = GameEnv->EventManager.ProcessEvents();

    // Détecter les actions de l'utilisateur sur chaque boite slot univers et prendre les actions appropriées


    if (Frame_Save.Is_MouseClick())
    {
      Loop = false;
    }

    // Demander au gestionnaire de boite d'effectuer le rendu graphique

    GameEnv->GuiManager.Render();

    // On montre à l'utilisateur ce qui a été rendu

    SDL_GL_SwapBuffers();

    // On met le programme en pause pendant 10 millièmes de seconde

    SDL_Delay(10);
  }

  // Enlever toutes les boites de l'affichage

  GameEnv->GuiManager.RemoveAllFrames();

  return(true);
}
