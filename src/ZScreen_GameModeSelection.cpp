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
 * ZScreen_GameModeSelection.cpp
 *
 *  Created on: 25 sept. 2018
 *      Author: laurent
 */

#ifndef Z_ZSCREEN_GAMEMODESELECTION_H
#  include "ZScreen_GameModeSelection.h"
#endif

#include <GL/glew.h>

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

#ifndef Z_ZOS_SPECIFIC_VIEWDOC_H
#  include "ZOs_Specific_ViewDoc.h"
#endif


#include "SDL/SDL.h"

//define COMPILEOPTION_ONLYSCHOOLMODE
#define ZMODESELECTION_SLOTCOUNT 2

ULong ZScreen_ModeSelection::ProcessScreen(ZGame * GameEnv)
{
  ZVector2f Size;
  ZString As;
  ULong i;
  bool Loop, Redisplay;
  ULong ChoosedSlot=0;
  ULong SlotOffset=0;

// Clear the Screen

glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); SDL_GL_SwapBuffers( );



  do
  {
    Redisplay = false;

    // Enlever toutes les boites affichées

    GameEnv->GuiManager.RemoveAllFrames();

    // Définition et réglage de la boite du fond

    ZFrame Background;
    if (!GameEnv->Settings_Hardware->Setting_OldLooked)
    {
      Background.SetPosition(0.0f,0.0f);
      Background.SetSize(GameEnv->ScreenResolution.x, GameEnv->ScreenResolution.y);
      Background.SetTexture(20);
      GameEnv->GuiManager.AddFrame(&Background);
    }

    // Définition du titre

    ZFrame LogoBlackvoxel;
    if (GameEnv->Settings_Hardware->Setting_OldLooked)
    {
      Size.y = GameEnv->ScreenResolution.y * 0.1f;
      Size.x = Size.y * 10.0f;
      LogoBlackvoxel.SetPosition(GameEnv->ScreenResolution.x / 2.0f - Size.x/2.0f, GameEnv->ScreenResolution.y *0.01f);
      LogoBlackvoxel.SetSize(Size.x, Size.y);
      LogoBlackvoxel.SetTexture(1);
      GameEnv->GuiManager.AddFrame(&LogoBlackvoxel);
    }

    // Définition et réglage du sous-titre

    ZFrame_FontFrame Frame_MainTitle;
    Frame_MainTitle.SetDisplayText((char *)"GAME MODE SELECTION");
    Frame_MainTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_VAR4));
    Frame_MainTitle.GetTextDisplaySize(&Size);
    Frame_MainTitle.SetPosition(GameEnv->ScreenResolution.x * 0.05f , GameEnv->ScreenResolution.y *0.26f );
    Frame_MainTitle.SetSize(Size.x+1.0f,Size.y+1.0f);
    GameEnv->GuiManager.AddFrame(&Frame_MainTitle);

    // Définition des variables et objets utilisés pour la suite

    ZFrame Slot[ZMODESELECTION_SLOTCOUNT];
    ZFrame_FontFrame SlotTitle[ZMODESELECTION_SLOTCOUNT];
    ZString SlotString[ZMODESELECTION_SLOTCOUNT];
    ZString SlotTextString[ZMODESELECTION_SLOTCOUNT];
    ZString SlotClickString[ZMODESELECTION_SLOTCOUNT];
    ZFrame_FontFrame SlotText[ZMODESELECTION_SLOTCOUNT];
    ZFrame_FontFrame SlotClick[ZMODESELECTION_SLOTCOUNT];

    // Loop for settings of choice boxes for universe selection.

    for (i=0;i<2;i++)
    {

      // Set parameters of the universe slot box.

      ZVector2f SlotPos,SlotSize;

      //SlotPos.y = (i / 4 * (GameEnv->ScreenResolution.y / 4.3f)) + ( i/ 8 * (GameEnv->ScreenResolution.y * 0.05f) ) + (float)GameEnv->ScreenResolution.y * 0.025f;
      SlotSize.x = GameEnv->ScreenResolution.x / 2.8f; // 2.2f
      SlotSize.y = GameEnv->ScreenResolution.y / 2.3f; // 2.1f
      SlotSize.x = floor(SlotSize.x); // Help to make a better texture display
      SlotSize.y = floor(SlotSize.y);

      // SlotPos.x = (float)(i * ((float)GameEnv->ScreenResolution.x) * 0.5f) + ((float)GameEnv->ScreenResolution.x)*0.25f - SlotSize.x*0.5f;
      SlotPos.x = (((float)GameEnv->ScreenResolution.x) * 0.5f);
      if (i==0) SlotPos.x-= SlotSize.x + (((float)GameEnv->ScreenResolution.x) * 0.045f) ;
      else      SlotPos.x+=              (((float)GameEnv->ScreenResolution.x) * 0.045f);
      SlotPos.y = (float)GameEnv->ScreenResolution.y * 0.40f;
      // SlotPos.x = (float)(i * ((float)GameEnv->ScreenResolution.x) * 0.5) + ((float)GameEnv->ScreenResolution.x)*0.020f ;
      // SlotPos.y = (float)GameEnv->ScreenResolution.y * 0.35f;
      SlotPos.x = floor(SlotPos.x); // Help to make a better texture display
      SlotPos.y = floor(SlotPos.y);


      if (i==0) Slot[i].SetTexture(21);
      else      Slot[i].SetTexture(23);


      Slot[i].SetPosition( SlotPos.x , SlotPos.y );
      Slot[i].SetSize(SlotSize.x, SlotSize.y );
      GameEnv->GuiManager.AddFrame(&Slot[i]);
      //Background.AddFrame(&Slot[i]);

      // Régler les paramètres de la boite du texte slot univers

      switch(i)
      {
        case 0: SlotString[i] = "ADVENTURE"; break;
        case 1: SlotString[i] = "SCHOOL MODE";    break;
      }
      SlotTitle[i].SetDisplayText(SlotString[i].String);
      SlotTitle[i].SetTexture(3);
      SlotTitle[i].SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_VAR4));
      SlotTitle[i].GetTextDisplaySize(&Size);
      SlotTitle[i].SetPosition(SlotSize.x * 0.07f, SlotSize.y * 0.1f);
      // SlotTitle[i].SetPosition(SlotSize.x / 2 - Size.x/2, SlotSize.y * 0.2f);
      SlotTitle[i].SetSize(Size.x+1.0f, Size.y);
      #ifdef COMPILEOPTION_ONLYSCHOOLMODE
        if (i==1) SlotTitle[i].SetColor(1.0f,1.0f,1.0f);
        else      SlotTitle[i].SetColor(0.6f,0.6f,0.6f);
      #else
        SlotText[i].SetColor(1.0f,1.0f,1.0f);
      #endif
      Slot[i].AddFrame(&SlotTitle[i]);

      // Slot Text

      switch(i)
      {
        case 0: SlotTextString[i]  = "* FOR PLAYING THE GAME\n\n";
                SlotTextString[i] << "* FACTORY CREATION\n\n";
                SlotTextString[i] << "* BIG UNLIMITED WORLD\n\n";
                SlotTextString[i] << "* DIFFERENT LANDSCAPES\n\n";
                SlotTextString[i] << "* EQUIPMENTS MUST BE MANUFACTURED\n\n";
                SlotTextString[i] << "* INTENDED FOR GAMERS\n\n";
                #ifdef COMPILEOPTION_ONLYSCHOOLMODE
                SlotTextString[i] << "~1:FF0000:";
                #endif
                SlotTextString[i] << "* ONLY IN FULL VERSION\n\n";
                break;

        case 1: SlotTextString[i]  = "* FOR TEACHING PROGRAMMING\n\n";
                SlotTextString[i] << "* DIRECT ACCESS TO ALL AUTOMATION\n\n";
                SlotTextString[i] << "* ONLY BLUE GRID\n\n";
                SlotTextString[i] << "* LIMITED SIZE UNIVERSE\n\n";
                SlotTextString[i] << "* INTENDED FOR SCHOOLS AND TEATCHERS\n\n";
                break;
      }

      SlotText[i].SetDisplayText(SlotTextString[i].String);
      SlotText[i].SetStyle(GameEnv->TileSetStyles->GetStyle(1));
      SlotText[i].SetTexture(3);
      if      (GameEnv->ScreenResolution.x >  1920)  { SlotText[i].SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_VAR2));}
      else if (GameEnv->ScreenResolution.x >= 1800)  { SlotText[i].SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_2));}
      else if (GameEnv->ScreenResolution.x >  1024)  { SlotText[i].SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_VAR2));}
      else if (GameEnv->ScreenResolution.x == 1024)  { SlotText[i].SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_1));}
      else                                           { SlotText[i].SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_VAR2));}

      SlotText[i].GetTextDisplaySize(&Size);
      SlotText[i].SetPosition(SlotSize.x * 0.07f, SlotSize.y * 0.25f);
      SlotText[i].SetSize(Size.x+1.0f, Size.y);

      #ifdef COMPILEOPTION_ONLYSCHOOLMODE
        if (i==1) SlotText[i].SetColor(1.0f,1.0f,1.0f);
        else      SlotText[i].SetColor(0.6f,0.6f,0.6f);
      #else
        SlotText[i].SetColor(1.0f,1.0f,1.0f);
      #endif
      Slot[i].AddFrame(&SlotText[i]);

      #ifdef COMPILEOPTION_ONLYSCHOOLMODE
        SlotClickString[i] = "CLICK HERE TO BUY FULL VERSION";
        SlotClick[i].SetDisplayText(SlotClickString[i].String);
        SlotClick[i].SetTexture(3);
        SlotClick[i].SetStyle(GameEnv->TileSetStyles->GetStyle(ZGame::FONTSIZE_VAR2));
        SlotClick[i].GetTextDisplaySize(&Size);
        SlotClick[i].SetPosition( (SlotSize.x / 2.0f) - (Size.x / 2.0f) , SlotSize.y * 0.81f);
        printf("SlotSize.x = %f\n",SlotSize.x);
        SlotClick[i].SetSize(Size.x+1.0f, Size.y);
        SlotClick[i].SetColor(0.0f,1.0f,0.0f);
        if (i==0) Slot[i].AddFrame(&SlotClick[i]);
      #endif
    }

  // Display loop and event management

    for (Loop = true; Loop; )
    {
      // Effacer l'écran

      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      glAlphaFunc(GL_GREATER, 0.05);

      // Manage inbound events (clicks, mous move, key stroke)

      Loop = GameEnv->EventManager.ProcessEvents();

      // Detect user action on each choice box and take according  actions.

      for(i=0;i<2;i++)
      {
        #ifdef COMPILEOPTION_ONLYSCHOOLMODE
        //if (i!=0)
        #endif
        if (Slot[i].Is_MouseIn())
        {
          Slot[i].SetTexture( (!i) ? 22 : 24 );
          if (i==0) SlotClick[i].SetColor(1.0f,1.0f,1.0f);
        }
        if (Slot[i].Is_MouseOut())
        {
          Slot[i].SetTexture( (!i) ? 21 : 23 );
          if (i==0) SlotClick[i].SetColor(0.0f,1.0f,0.0f);
        }
        if (Slot[i].Is_MouseClick())
        {
          ChoosedSlot = i;
          Loop = false;
          #ifdef COMPILEOPTION_ONLYSCHOOLMODE
          if (i==0) { ZViewDoc::OpenURL((char *)"https://www.blackvoxel.com/view.php?node=1717"); Loop = true; }
          #endif
        }
      }

      // Ask the Gui manager to render all boxes.

      GameEnv->GuiManager.Render();

      // Show the work to the user.

      SDL_GL_SwapBuffers();

      // Wait for 10 milliseconds in order to avoid over refreshing.
      // Over refreshing in the menu screens would be a waste of power.

      SDL_Delay(10);
    }

    // Remove all boxes from the display.

    GameEnv->GuiManager.RemoveAllFrames();

  } while (Redisplay);

  // Return to sender with the choosed game mode.

  return(ChoosedSlot+SlotOffset);
}
