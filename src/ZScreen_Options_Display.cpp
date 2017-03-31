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
 * ZScreen_Options.cpp
 *
 *  Created on: 30 mai 2011
 *      Author: laurent
 */

#include "ZScreen_Options_Display.h"
#include <GL/glew.h>
#include "z/ZObject_Array.h"



int ZScreen_Options_Display::ResolutionCompare(ZObject * Object_1, ZObject * Object_2)
{
  ZResolution * Ob1 = (ZResolution *) Object_1;
  ZResolution * Ob2 = (ZResolution *) Object_2;


  if      (Ob1->Resolution_x > Ob2->Resolution_x) return(1);
  else if (Ob1->Resolution_x < Ob2->Resolution_x) return(-1);
  else
  {
    if      (Ob1->Resolution_y > Ob2->Resolution_y) return(1);
    else if (Ob1->Resolution_y > Ob2->Resolution_y) return(-1);
    else    return(0);
  }
}

ULong ZScreen_Options_Display::ProcessScreen(ZGame * GameEnv)
{
  ZVector2f Size;
  ZVector2f Pos;
  ULong i,j;
  ULong Count;
  bool Found;

  ZResolution * Res;
  ZObjectArray Resolution_Array;

  // Add standard Resolutions.

  Res = new ZResolution; Res->Resolution_x = 0; Res->Resolution_y = 0; Res->Name = "Automatic"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 800; Res->Resolution_y = 600; Res->Name = "800*600"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1024; Res->Resolution_y = 600; Res->Name = "1024*600"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1024; Res->Resolution_y = 768; Res->Name = "1024*768"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1152; Res->Resolution_y = 864; Res->Name = "1152*864"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1280; Res->Resolution_y = 768; Res->Name = "1280*768"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1280; Res->Resolution_y = 800; Res->Name = "1280*800"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1280; Res->Resolution_y = 960; Res->Name = "1280*960"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1280; Res->Resolution_y = 1024; Res->Name = "1280*1024"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1360; Res->Resolution_y = 768; Res->Name = "1360*768"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1366; Res->Resolution_y = 768; Res->Name = "1366*768"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1440; Res->Resolution_y = 900; Res->Name = "1440*900"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1600; Res->Resolution_y = 900; Res->Name = "1600*900"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1600; Res->Resolution_y = 1200; Res->Name = "1600*1200"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1680; Res->Resolution_y = 1050; Res->Name = "1680*1050"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1800; Res->Resolution_y = 1440; Res->Name = "1800*1440"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1920; Res->Resolution_y = 1080; Res->Name = "1920*1080"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1920; Res->Resolution_y = 1200; Res->Name = "1920*1200"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 640; Res->Resolution_y = 480; Res->Name = "640*480"; Resolution_Array.Add(*Res);

  // Add the SDL resolutions.

  {
    SDL_Rect** modes;

    modes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_OPENGL ); // SDL_FULLSCREEN|SDL_OPENGL


    for(i=0;modes[i];i++)
    {
      Found = false;
      // printf("Resolution : %u x %u\n",modes[i]->w, modes[i]->h);
      Count = Resolution_Array.GetCount();
      for(j=0;j<Count;j++)
      {
        Res = (ZResolution *)Resolution_Array.GetEntry(j);
        if ((Res->Resolution_x == modes[i]->w) && (Res->Resolution_y == modes[i]->h)) Found = true;
      }
      if (!Found)
      {
        Res = new ZResolution;
        Res->Resolution_x = modes[i]->w; Res->Resolution_y = modes[i]->h;
        Res->Name.Append_ULong(Res->Resolution_x);
        Res->Name.Append_pchar("*");
        Res->Name.Append_ULong(Res->Resolution_y);
        Resolution_Array.Add(*Res);
      }
    }
    // printf("Resolution Bureau : %lu x %lu\n",(UNum)GameEnv->DesktopResolution.x, (UNum)GameEnv->DesktopResolution.y);
  }

  // Add the desktop resolution

  Found = false;
  Count = Resolution_Array.GetCount();
  for (i=0; i<Count; i++)
  {
    Res = (ZResolution *)Resolution_Array.GetEntry(i);
    if ((Res->Resolution_x == (ULong)GameEnv->DesktopResolution.x) && (Res->Resolution_y ==(ULong)GameEnv->DesktopResolution.y )) Found = true;
  }
  if (!Found)
  {
    Res = new ZResolution;
    Res->Resolution_x = GameEnv->DesktopResolution.x; Res->Resolution_y = GameEnv->DesktopResolution.y;
    Res->Name.Append_ULong(GameEnv->DesktopResolution.x);
    Res->Name.Append_pchar("*");
    Res->Name.Append_ULong(GameEnv->DesktopResolution.y);
    Resolution_Array.Add(*Res);
  }

  // Add the resolution used in the setting file

  Found = false;
  Count = Resolution_Array.GetCount();
  for (i=0; i<Count; i++)
  {
    Res = (ZResolution *)Resolution_Array.GetEntry(i);
    if ((Res->Resolution_x == GameEnv->Settings_Hardware->Setting_Resolution_h) && (Res->Resolution_y == GameEnv->Settings_Hardware->Setting_Resolution_v )) Found = true;
  }
  if (!Found)
  {
    Res = new ZResolution;
    Res->Resolution_x = GameEnv->Settings_Hardware->Setting_Resolution_h; Res->Resolution_y = GameEnv->Settings_Hardware->Setting_Resolution_v;
    Res->Name.Append_ULong(GameEnv->Settings_Hardware->Setting_Resolution_h);
    Res->Name.Append_pchar("*");
    Res->Name.Append_ULong(GameEnv->Settings_Hardware->Setting_Resolution_v);
    Resolution_Array.Add(*Res);
  }

  // Sort

  Resolution_Array.BubbleSort(ResolutionCompare);

  // Find the actual setting file choice

  ULong ActualResolutionChoice;

  ActualResolutionChoice = 0;
  Count = Resolution_Array.GetCount();
  for (i=0; i<Count; i++)
  {
    Res = (ZResolution *)Resolution_Array.GetEntry(i);
    if ((Res->Resolution_x == GameEnv->Settings_Hardware->Setting_Resolution_h) && (Res->Resolution_y == GameEnv->Settings_Hardware->Setting_Resolution_v )) ActualResolutionChoice = i;
  }




  // Make à string array to display

  ZSize NumElements;
  char ** StringArray;
  NumElements = Resolution_Array.GetCount();
  StringArray = new char *[NumElements];
  for (i=0;i<NumElements;i++)
  {
    Res = (ZResolution *)Resolution_Array.GetEntry(i);
    StringArray[i] = Res->Name.String;
  }


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
    Frame_MainTitle.SetDisplayText((char *)"DISPLAY SETTINGS");
    Frame_MainTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_MainTitle.GetTextDisplaySize(&Size);
    Frame_MainTitle.SetPosition(GameEnv->ScreenResolution.x / 2.0f - Size.x / 2.0f , Pos.y );
    Frame_MainTitle.SetSize(Size.x + 1.0,Size.y);
    Frame_MainTitle.TextureNum = 3;
    GameEnv->GuiManager.AddFrame(&Frame_MainTitle);
    Pos.y += Size.y + GameEnv->ScreenResolution.y * 0.10f;

    Pos.y += 32.0f + 16.0f;

  ZFrame_FontFrame Frame_TitleRes;
    Frame_TitleRes.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_TitleRes.SetDisplayText("RESOLUTION  ");
    Frame_TitleRes.GetTextDisplaySize(&Size);
    Frame_TitleRes.SetPosition(Pos.x, Pos.y);
    Frame_TitleRes.SetSize(Size.x + 1.0f, Size.y);
    GameEnv->GuiManager.AddFrame(&Frame_TitleRes);
    Pos.x += Size.x;

  ZFrame_CyclingChoiceBox Frame_ChooseRes;
    Frame_ChooseRes.SetFontTileStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_ChooseRes.SetGUITileset(GameEnv->GuiTileset);
    Frame_ChooseRes.SetChoiceList(StringArray, NumElements );
    Frame_ChooseRes.SetChoice(ActualResolutionChoice);
    Frame_ChooseRes.GetTextDisplaySize(&Size);
    Frame_ChooseRes.SetPosition(Pos.x,Pos.y);
    Frame_ChooseRes.SetSize(Size.x +1.0f,32.0f);
    Frame_ChooseRes.SetTexture(5);
    Frame_ChooseRes.Show_Frame(true);
    GameEnv->GuiManager.AddFrame(&Frame_ChooseRes);
    Pos.y += 32.0f + GameEnv->ScreenResolution.y * 0.05f;
    Pos.x = LeftMargin;


  ZFrame_FontFrame Frame_TitleFullScreen;
    Frame_TitleFullScreen.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_TitleFullScreen.SetDisplayText("FULL SCREEN ");
    Frame_TitleFullScreen.GetTextDisplaySize(&Size);
    Frame_TitleFullScreen.SetPosition(Pos.x, Pos.y);
    Frame_TitleFullScreen.SetSize(Size.x + 1.0f, Size.y);
    GameEnv->GuiManager.AddFrame(&Frame_TitleFullScreen);
    Pos.x += Size.x;

  ZFrame_CheckBox Frame_FullScreen;
    Frame_FullScreen.SetGUITileset(GameEnv->GuiTileset);
    Frame_FullScreen.SetPosition(Pos.x , Pos.y);
    Frame_FullScreen.SetSize(32.0f,32.0f);
    Frame_FullScreen.SetState(GameEnv->Settings_Hardware->Setting_FullScreen);
    GameEnv->GuiManager.AddFrame(&Frame_FullScreen);
    Pos.y += 32.0f + GameEnv->ScreenResolution.y * 0.15f;
    Pos.x = LeftMargin;

  ZFrame_FontFrame WarningText;
    ZString SWarningText;
    SWarningText = "WARNING : THESE SETTINGS NEEDS TO ";
    WarningText.SetStyle(GameEnv->TileSetStyles->GetStyle(3));
    WarningText.SetDisplayText(SWarningText.String);
    WarningText.GetTextDisplaySize(&Size);
    SWarningText << "\n\n";
    SWarningText<< "QUIT AND RESTART THE GAME TO APPLY";
    WarningText.SetPosition(GameEnv->ScreenResolution.x / 2.0f - Size.x / 2.0f, Pos.y);
    WarningText.SetSize(Size.x + 1.0f, Size.y);
    GameEnv->GuiManager.AddFrame(&WarningText);
    Pos.x += Size.x;

  ZFrame_FontFrame Frame_Save;
    Frame_Save.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_Save.SetDisplayText("SAVE");
    Frame_Save.GetTextDisplaySize(&Size);
    Frame_Save.SetPosition(GameEnv->ScreenResolution.x * 0.9f - Size.x -1.0f, GameEnv->ScreenResolution.y * 0.85f - Size.y);
    //Frame_Save.SetPosition(GameEnv->ScreenResolution.x * 0.9f - Size.x -1.0f, GameEnv->ScreenResolution.y * 0.9f - Size.y);

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
      if (Frame_ChooseRes.Is_ChoiceChanged())
      {
        ActualResolutionChoice = Frame_ChooseRes.GetChoice();
        Res = (ZResolution *)Resolution_Array.GetEntry(ActualResolutionChoice);
        GameEnv->Settings_Hardware->Setting_Resolution_h = Res->Resolution_x;
        GameEnv->Settings_Hardware->Setting_Resolution_v = Res->Resolution_y;
      }

      if (Frame_FullScreen.Is_StateChanged())
      {
        if (Frame_FullScreen.GetState())  { GameEnv->Settings_Hardware->Setting_FullScreen = true;  }
        else                              { GameEnv->Settings_Hardware->Setting_FullScreen = false; }
      }

      if (Frame_Save.Is_MouseClick()) { Loop = false; }

      GameEnv->GuiManager.Render();
      SDL_GL_SwapBuffers( );
      SDL_Delay(10);

    }
    GameEnv->GuiManager.RemoveAllFrames();

    GameEnv->Settings_Hardware->Save();

  return(0);
}

/*
ULong ZScreen_Options_Display::ProcessScreen(ZGame * GameEnv)
{
  ZVector2f Size;
  ZVector2f Pos;
  ULong i,j;
  ULong Count;
  bool Found;

  ZResolution * Res;
  ZObjectArray Resolution_Array;

  // Add standard Resolutions.

  Res = new ZResolution; Res->Resolution_x = 0; Res->Resolution_y = 0; Res->Name = "Automatic"; Resolution_Array.Add(*Res);
  //Res = new ZResolution; Res->Resolution_x = 800; Res->Resolution_y = 600; Res->Name = "800*600"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1024; Res->Resolution_y = 600; Res->Name = "1024*600"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1024; Res->Resolution_y = 768; Res->Name = "1024*768"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1152; Res->Resolution_y = 864; Res->Name = "1152*864"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1280; Res->Resolution_y = 768; Res->Name = "1280*768"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1280; Res->Resolution_y = 800; Res->Name = "1280*800"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1280; Res->Resolution_y = 960; Res->Name = "1280*960"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1280; Res->Resolution_y = 1024; Res->Name = "1280*1024"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1360; Res->Resolution_y = 768; Res->Name = "1360*768"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1366; Res->Resolution_y = 768; Res->Name = "1366*768"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1440; Res->Resolution_y = 900; Res->Name = "1440*900"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1600; Res->Resolution_y = 900; Res->Name = "1600*900"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1600; Res->Resolution_y = 1200; Res->Name = "1600*1200"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1680; Res->Resolution_y = 1050; Res->Name = "1680*1050"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1800; Res->Resolution_y = 1440; Res->Name = "1800*1440"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1920; Res->Resolution_y = 1080; Res->Name = "1920*1080"; Resolution_Array.Add(*Res);
  Res = new ZResolution; Res->Resolution_x = 1920; Res->Resolution_y = 1200; Res->Name = "1920*1200"; Resolution_Array.Add(*Res);
  //Res = new ZResolution; Res->Resolution_x = 640; Res->Resolution_y = 480; Res->Name = "640*480"; Resolution_Array.Add(*Res);

  // Add the SDL resolutions.

  {
    SDL_Rect** modes;

    modes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_OPENGL ); // SDL_FULLSCREEN|SDL_OPENGL


    for(i=0;modes[i];i++)
    {
      Found = false;
      // printf("Resolution : %u x %u\n",modes[i]->w, modes[i]->h);
      Count = Resolution_Array.GetCount();
      for(j=0;j<Count;j++)
      {
        Res = (ZResolution *)Resolution_Array.GetEntry(j);
        if ((Res->Resolution_x == modes[i]->w) && (Res->Resolution_y == modes[i]->h)) Found = true;
      }
      if (!Found)
      {
        if (modes[i]->w < 1024 || modes[i]->h < 600) continue;
        Res = new ZResolution;
        Res->Resolution_x = modes[i]->w; Res->Resolution_y = modes[i]->h;
        Res->Name.Append_ULong(Res->Resolution_x);
        Res->Name.Append_pchar("*");
        Res->Name.Append_ULong(Res->Resolution_y);
        Resolution_Array.Add(*Res);
      }
    }
    // printf("Resolution Bureau : %lu x %lu\n",(UNum)GameEnv->DesktopResolution.x, (UNum)GameEnv->DesktopResolution.y);
  }

  // Add the desktop resolution

  Found = false;
  Count = Resolution_Array.GetCount();
  for (i=0; i<Count; i++)
  {
    Res = (ZResolution *)Resolution_Array.GetEntry(i);
    if ((Res->Resolution_x == (ULong)GameEnv->DesktopResolution.x) && (Res->Resolution_y ==(ULong)GameEnv->DesktopResolution.y )) Found = true;
  }
  if (!Found)
  {
    Res = new ZResolution;
    Res->Resolution_x = GameEnv->DesktopResolution.x; Res->Resolution_y = GameEnv->DesktopResolution.y;
    Res->Name.Append_ULong(GameEnv->DesktopResolution.x);
    Res->Name.Append_pchar("*");
    Res->Name.Append_ULong(GameEnv->DesktopResolution.y);
    Resolution_Array.Add(*Res);
  }

  // Add the resolution used in the setting file

  Found = false;
  Count = Resolution_Array.GetCount();
  for (i=0; i<Count; i++)
  {
    Res = (ZResolution *)Resolution_Array.GetEntry(i);
    if ((Res->Resolution_x == GameEnv->Settings_Hardware->Setting_Resolution_h) && (Res->Resolution_y == GameEnv->Settings_Hardware->Setting_Resolution_v )) Found = true;
  }
  if (!Found)
  {
    Res = new ZResolution;
    Res->Resolution_x = GameEnv->Settings_Hardware->Setting_Resolution_h; Res->Resolution_y = GameEnv->Settings_Hardware->Setting_Resolution_v;
    Res->Name.Append_ULong(GameEnv->Settings_Hardware->Setting_Resolution_h);
    Res->Name.Append_pchar("*");
    Res->Name.Append_ULong(GameEnv->Settings_Hardware->Setting_Resolution_v);
    Resolution_Array.Add(*Res);
  }

  // Sort

  Resolution_Array.BubbleSort(ResolutionCompare);

  // Find the actual setting file choice

  ULong ActualResolutionChoice;

  ActualResolutionChoice = 0;
  Count = Resolution_Array.GetCount();
  for (i=0; i<Count; i++)
  {
    Res = (ZResolution *)Resolution_Array.GetEntry(i);
    if ((Res->Resolution_x == GameEnv->Settings_Hardware->Setting_Resolution_h) && (Res->Resolution_y == GameEnv->Settings_Hardware->Setting_Resolution_v )) ActualResolutionChoice = i;
  }




  // Make à string array to display

  ZSize NumElements;
  char ** StringArray;
  NumElements = Resolution_Array.GetCount();
  StringArray = new char *[NumElements];
  for (i=0;i<NumElements;i++)
  {
    Res = (ZResolution *)Resolution_Array.GetEntry(i);
    StringArray[i] = Res->Name.String;
  }


  Pos.x = 0.0; Pos.y = GameEnv->ScreenResolution.y * 0.05f;

  // Clear Screen

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  GameEnv->GuiManager.RemoveAllFrames();

  // Frames on screen

  ZFrame_FontFrame Frame_MainTitle;
    Frame_MainTitle.SetDisplayText((char *)"DISPLAY SETTINGS");
    Frame_MainTitle.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_MainTitle.GetTextDisplaySize(&Size);
    Frame_MainTitle.SetPosition(GameEnv->ScreenResolution.x / 2.0f - Size.x / 2.0f , Pos.y );
    Frame_MainTitle.SetSize(Size.x + 1.0,Size.y);
    Frame_MainTitle.TextureNum = 3;
    GameEnv->GuiManager.AddFrame(&Frame_MainTitle);
    Pos.y += Size.y + GameEnv->ScreenResolution.y * 0.10f;

    Pos.y += 32.0f + 16.0f;

  ZFrame_FontFrame Frame_TitleRes;
    Frame_TitleRes.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_TitleRes.SetDisplayText("RESOLUTION  ");
    Frame_TitleRes.GetTextDisplaySize(&Size);
    Frame_TitleRes.SetPosition(Pos.x, Pos.y);
    Frame_TitleRes.SetSize(Size.x + 1.0f, Size.y);
    GameEnv->GuiManager.AddFrame(&Frame_TitleRes);
    Pos.x += Size.x;

  ZFrame_CyclingChoiceBox Frame_ChooseRes;
    Frame_ChooseRes.SetFontTileStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_ChooseRes.SetGUITileset(GameEnv->GuiTileset);
    Frame_ChooseRes.SetChoiceList(StringArray, NumElements );
    Frame_ChooseRes.SetChoice(ActualResolutionChoice);
    Frame_ChooseRes.GetTextDisplaySize(&Size);
    Frame_ChooseRes.SetPosition(Pos.x,Pos.y);
    Frame_ChooseRes.SetSize(Size.x +1.0f,32.0f);
    Frame_ChooseRes.SetTexture(5);
    Frame_ChooseRes.Show_Frame(true);
    GameEnv->GuiManager.AddFrame(&Frame_ChooseRes);
    Pos.y += 32.0f + GameEnv->ScreenResolution.y * 0.05f;
    Pos.x = 0.0f;


  ZFrame_FontFrame Frame_TitleFullScreen;
    Frame_TitleFullScreen.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_TitleFullScreen.SetDisplayText("FULL SCREEN ");
    Frame_TitleFullScreen.GetTextDisplaySize(&Size);
    Frame_TitleFullScreen.SetPosition(Pos.x, Pos.y);
    Frame_TitleFullScreen.SetSize(Size.x + 1.0f, Size.y);
    GameEnv->GuiManager.AddFrame(&Frame_TitleFullScreen);
    Pos.x += Size.x;

  ZFrame_CheckBox Frame_FullScreen;
    Frame_FullScreen.SetGUITileset(GameEnv->GuiTileset);
    Frame_FullScreen.SetPosition(Pos.x , Pos.y);
    Frame_FullScreen.SetSize(32.0f,32.0f);
    Frame_FullScreen.SetState(GameEnv->Settings_Hardware->Setting_FullScreen);
    GameEnv->GuiManager.AddFrame(&Frame_FullScreen);
    Pos.y += 32.0f + GameEnv->ScreenResolution.y * 0.20f;
    Pos.x = 0.0f;

  ZFrame_FontFrame WarningText;
    WarningText.SetStyle(GameEnv->TileSetStyles->GetStyle(3));
    WarningText.SetDisplayText("WARNING : THESE SETTINGS NEEDS TO QUIT AND RESTART THE GAME TO APPLY");
    WarningText.GetTextDisplaySize(&Size);
    WarningText.SetPosition(GameEnv->ScreenResolution.x / 2.0f - Size.x / 2.0f, Pos.y);
    WarningText.SetSize(Size.x + 1.0f, Size.y);
    GameEnv->GuiManager.AddFrame(&WarningText);
    Pos.x += Size.x;

  ZFrame_FontFrame Frame_Save;
    Frame_Save.SetStyle(GameEnv->TileSetStyles->GetStyle(2));
    Frame_Save.SetDisplayText("SAVE");
    Frame_Save.GetTextDisplaySize(&Size);
    //Frame_Save.SetPosition(GameEnv->ScreenResolution.x * 0.9f - Size.x -1.0f, GameEnv->ScreenResolution.y * 0.9f - Size.y);
    Frame_Save.SetPosition(GameEnv->ScreenResolution.x * 0.05, GameEnv->ScreenResolution.y * 0.9f - Size.y);

    Frame_Save.SetSize(Size.x + 1.0f, Size.y);
    GameEnv->GuiManager.AddFrame(&Frame_Save);

    for (bool Loop = true; Loop; )
    {
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      Loop = GameEnv->EventManager.ProcessEvents();

      // if (Frame_MainTitle.Is_MouseClick()) {Loop = false; GameEnv->Settings_Hardware->Save(); exit(0); }
      if (Frame_ChooseRes.Is_ChoiceChanged())
      {
        ActualResolutionChoice = Frame_ChooseRes.GetChoice();
        Res = (ZResolution *)Resolution_Array.GetEntry(ActualResolutionChoice);
        GameEnv->Settings_Hardware->Setting_Resolution_h = Res->Resolution_x;
        GameEnv->Settings_Hardware->Setting_Resolution_v = Res->Resolution_y;
      }

      if (Frame_FullScreen.Is_StateChanged())
      {
        if (Frame_FullScreen.GetState())  { GameEnv->Settings_Hardware->Setting_FullScreen = true;  }
        else                              { GameEnv->Settings_Hardware->Setting_FullScreen = false; }
      }

      if (Frame_Save.Is_MouseClick()) { Loop = false; }

      GameEnv->GuiManager.Render();
      SDL_GL_SwapBuffers( );
      SDL_Delay(10);

    }
    GameEnv->GuiManager.RemoveAllFrames();

    GameEnv->Settings_Hardware->Save();

  return(0);
}
*/
