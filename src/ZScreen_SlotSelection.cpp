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
 * ZScreen_SlotSelection.cpp
 *
 *  Created on: 30 mai 2011
 *      Author: laurent
 */

#include "ZScreen_SlotSelection.h"
#include <GL/glew.h>

#ifndef Z_ZSTREAMS_FILE_H
#  include "z/ZStream_File.h"
#endif

ULong ZScreen_SlotSelection::ProcessScreen(ZGame * GameEnv)
{
      ZVector2f Size;
      ZString As;
      ULong i;
      bool Loop;
      ULong ChoosedSlot=0;

      // Effacer l'écran

      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); SDL_GL_SwapBuffers( );

      // Régler la transparence

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glAlphaFunc(GL_GREATER, 0.2);
      glEnable(GL_ALPHA_TEST);

      // Enlever toutes les boites affichées

      GameEnv->GuiManager.RemoveAllFrames();

      // Définition et réglage de la boite du fond

      /*ZFrame Background;
      Background.SetPosition(0.0f,0.0f);
      Background.SetSize(GameEnv->ScreenResolution.x, GameEnv->ScreenResolution.y);
      Background.SetTexture(11);
      GameEnv->GuiManager.AddFrame(&Background);
      */

      // Définition du titre

      ZFrame LogoBlackvoxel;
      Size.y = GameEnv->ScreenResolution.y * 0.1f;
      Size.x = Size.y * 10.0f;
      LogoBlackvoxel.SetPosition(GameEnv->ScreenResolution.x / 2.0f - Size.x/2.0f, GameEnv->ScreenResolution.y *0.01f);
      LogoBlackvoxel.SetSize(Size.x, Size.y);
      LogoBlackvoxel.SetTexture(1);
      GameEnv->GuiManager.AddFrame(&LogoBlackvoxel);

      // Définition et réglage du sous-titre

      ZFrame_FontFrame Frame_MainTitle;
      Frame_MainTitle.SetDisplayText((char *)"GAME UNIVERSE SELECTION");
      Frame_MainTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(1));
      Frame_MainTitle.GetTextDisplaySize(&Size);
      Frame_MainTitle.SetPosition(GameEnv->ScreenResolution.x * 0.05f , GameEnv->ScreenResolution.y *0.2f );
      //Frame_MainTitle.SetPosition(GameEnv->ScreenResolution.x / 2.0f - Size.x/2.0f, GameEnv->ScreenResolution.y / 2.0f - Size.y / 2.0f);
      Frame_MainTitle.SetSize(Size.x+1.0f,Size.y+1.0f);
      GameEnv->GuiManager.AddFrame(&Frame_MainTitle);
      //Background.AddFrame(&Frame_MainTitle);

      // Définition des variables et objets utilisés pour la suite

      ZFrame Slot[16];
      ZFrame_FontFrame SlotTitle[16];
      ZFrame_FontFrame SlotStatus[16];
      ZString SlotString[16];
      ZString StatusString[16];
      bool    SlotUsed[16];
      ZString FileName;
      FILE * fh;
      ZString Directory;

      // Récupération du chemin du répertoire de stockage des univers

      if (COMPILEOPTION_USEHOMEDIRSTORAGE)
      {
        Directory = ZStream_File::Get_Directory_UserData();
        Directory.AddToPath(COMPILEOPTION_SAVEFOLDERNAME);
      }
      Directory.AddToPath("Universes");

      // Boucle de réglages des boites de choix d'univers

      for (i=0;i<16;i++)
      {
        // Tester si le slot est utilisé / Test if GameSlot is used

        FileName = Directory;
        FileName.AddToPath(i+1).AddToPath("PlayerInfo.dat");
        if ((fh = fopen(FileName.String, "rb"))) { SlotUsed[i] = true; fclose(fh); }
        else                                     { SlotUsed[i] = false; }

        // Régler les paramètres de la boite du slot univers

        ZVector2f SlotPos,SlotSize;
        SlotPos.x = ((float)(i % 4 )) * (((float)GameEnv->ScreenResolution.x) / 4.25f) + (float)GameEnv->ScreenResolution.x * 0.045f;
        SlotPos.y = (i / 4 * (GameEnv->ScreenResolution.y / 6.5f)) + (float)GameEnv->ScreenResolution.y * 0.3f;
        //SlotPos.y = (i / 4 * (GameEnv->ScreenResolution.y / 4.3f)) + ( i/ 8 * (GameEnv->ScreenResolution.y * 0.05f) ) + (float)GameEnv->ScreenResolution.y * 0.025f;
        SlotSize.x = GameEnv->ScreenResolution.x / 5.0f;
        SlotSize.y = GameEnv->ScreenResolution.y / 10.0f;
        Slot[i].SetTexture(11);

        if (SlotUsed[i]) Slot[i].SetColor(1.0f,1.0f,1.0f);
        else             Slot[i].SetColor(1.0f,1.0f,1.0f);

        Slot[i].SetPosition( SlotPos.x , SlotPos.y );
        Slot[i].SetSize(SlotSize.x, SlotSize.y );
        GameEnv->GuiManager.AddFrame(&Slot[i]);
        //Background.AddFrame(&Slot[i]);

        // Régler les paramètres de la boite du texte slot univers

        SlotString[i] = "UNIVERSE "; SlotString[i] << i+1;
        SlotTitle[i].SetDisplayText(SlotString[i].String);
        SlotTitle[i].SetStyle(GameEnv->TileSetStyles->GetStyle(3));
        SlotTitle[i].SetTexture(3);
        SlotTitle[i].GetTextDisplaySize(&Size);
        SlotTitle[i].SetPosition(SlotSize.x * 0.07f, SlotSize.y * 0.2f);
        // SlotTitle[i].SetPosition(SlotSize.x / 2 - Size.x/2, SlotSize.y * 0.2f);
        SlotTitle[i].SetSize(Size.x+1.0f, Size.y);
        SlotTitle[i].SetColor(1.0f,1.0f,1.0f);
        Slot[i].AddFrame(&SlotTitle[i]);

        // Régler les paramètres de la boite du texte Used/Free

        if (SlotUsed[i]) StatusString[i] = "USED";
        else             StatusString[i] = "FREE";
        SlotStatus[i].SetDisplayText(StatusString[i].String);
        SlotStatus[i].SetStyle(GameEnv->TileSetStyles->GetStyle(3));
        SlotStatus[i].SetTexture(3);
        SlotStatus[i].GetTextDisplaySize(&Size);
        SlotStatus[i].SetPosition(SlotSize.x * 0.65f, SlotSize.y * 0.65f);
        // SlotStatus[i].SetPosition(SlotSize.x / 2 - Size.x/2, SlotSize.y * 0.45f);
        SlotStatus[i].SetSize(Size.x+1.0f, Size.y);
        if (SlotUsed[i])SlotStatus[i].SetColor(1.0f,0.0f,0.0f);
        else            SlotStatus[i].SetColor(0.0f,1.0f,0.0f);
        // else            SlotStatus[i].SetColor(0.5f,0.5f,0.5f);
        Slot[i].AddFrame(&SlotStatus[i]);
      }

      // Boucle d'affichage et de gestion des évènements

        for (Loop = true; Loop; )
        {
          // Effacer l'écran

          glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

          // Gérer les évènements entrants (clics, déplacemets souris, frappe clavier)

          Loop = GameEnv->EventManager.ProcessEvents();

          // Détecter les actions de l'utilisateur sur chaque boite slot univers et prendre les actions appropriées

          for(i=0;i<16;i++)
          {
            if (Slot[i].Is_MouseIn()) Slot[i].SetTexture(12);
            if (Slot[i].Is_MouseOut()) Slot[i].SetTexture(11);
            if (Slot[i].Is_MouseClick()) { ChoosedSlot = i+1;Loop = false; }
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

        // Retourner à l'appelant et lui donner le numéro de l'univers choisis

        return(ChoosedSlot);
}
