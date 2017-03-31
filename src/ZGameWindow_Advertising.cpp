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
 * ZGameWindow_Advertising.cpp
 *
 *  Created on: 26 oct. 2011
 *      Author: laurent
 */

#include "ZGameWindow_Advertising.h"


#include "ZGame.h"
#include "ZActorPhysics.h"
#include "SDL/SDL.h"

void ZGameWindow_Advertising::Show()
{

  // Running position computing



  Flag_Shown = true;
}

void ZGameWindow_Advertising::Hide()
{
    Flag_Shown = false;
}

void ZGameWindow_Advertising::Advertising_Actions(double FrameTime)
{
  ZVector2f Size;
  ZVector2f ScreenSize;

  ScreenSize.x = (float)GameEnv->ScreenResolution.x;
  ScreenSize.y = (float)GameEnv->ScreenResolution.y;
  Bool Stop;

  if (!ActualyDisplayedEntry)
  {
    if (!Flag_Shown) return;
    if((ActualyDisplayedEntry = (DisplayEntry *)DisplayFile.PullFromFile()))
    {
      if (ActualyDisplayedEntry->SoundToPlay != 0) this->GameEnv->Sound->PlaySound(ActualyDisplayedEntry->SoundToPlay);
      switch (ActualyDisplayedEntry->Visibility)
      {

        default: break;
        case VISIBILITY_HIGH:
          ActualText   = ActualyDisplayedEntry->Message;
          Frame_Text.SetDisplayText(ActualText.String);
#if COMPILEOPTION_PLATFORM_RASPBERRY_PI ==1
          Frame_Text.SetStyle(GameEnv->TileSetStyles->GetStyle(3)); //2
#else
          Frame_Text.SetStyle(GameEnv->TileSetStyles->GetStyle(2)); //2
#endif
          Frame_Text.GetTextDisplaySize(&Size);
          if (Size.x > ScreenSize.x-128.0f) Size.x = ScreenSize.x -128.0f;
          Frame_Text.SetPosition( (ScreenSize.x - Size.x) / 2.0f , (ScreenSize.y / 4.0f) - (Size.y / 2.0f) );
          // Frame_Text.SetPosition( (ScreenSize.x - Size.x) / 2.0f , (ScreenSize.y / 2.5f) - (Size.y / 2.0f) );
          Frame_Text.SetSize(Size.x,Size.y);
          GameEnv->GuiManager.AddFrame(&Frame_Text);
          break;
        case VISIBILITY_MEDIUM:
          ActualText   = ActualyDisplayedEntry->Message;
          Frame_Text.SetDisplayText(ActualText.String);
          Frame_Text.SetStyle(GameEnv->TileSetStyles->GetStyle(4));
          Frame_Text.GetTextDisplaySize(&Size);
          if (Size.x > ScreenSize.x-128.0f) Size.x = ScreenSize.x -128.0f;
          Frame_Text.SetPosition( (ScreenSize.x - Size.x) / 2.0f , (ScreenSize.y / 4.0f) - (Size.y / 2.0f) );
          Frame_Text.SetSize(Size.x,Size.y);
          GameEnv->GuiManager.AddFrame(&Frame_Text);
          break;
        case VISIBILITY_MEDLOW:
          ActualText   = ActualyDisplayedEntry->Message;
          Frame_Text.SetDisplayText(ActualText.String);
          Frame_Text.SetStyle(GameEnv->TileSetStyles->GetStyle(4));
          Frame_Text.GetTextDisplaySize(&Size);
          if (Size.x > ScreenSize.x-128.0f) Size.x = ScreenSize.x -128.0f;
          Frame_Text.SetPosition( (ScreenSize.x - Size.x) / 2.0f , (ScreenSize.y * 0.75f) - (Size.y / 2.0f) );
          Frame_Text.SetSize(Size.x,Size.y);
          GameEnv->GuiManager.AddFrame(&Frame_Text);
          break;
        case VISIBILITY_LOW:
          ActualText   = ActualyDisplayedEntry->Message;
          Frame_Text.SetDisplayText(ActualText.String);
          Frame_Text.SetStyle(GameEnv->TileSetStyles->GetStyle(3));
          Frame_Text.GetTextDisplaySize(&Size);
          if (Size.x > ScreenSize.x-128.0f) Size.x = ScreenSize.x -128.0f;
          Frame_Text.SetPosition( (ScreenSize.x - Size.x) / 2.0f , (ScreenSize.y * 0.75f) - (Size.y / 2.0f) );
          Frame_Text.SetSize(Size.x,Size.y);
          GameEnv->GuiManager.AddFrame(&Frame_Text);
          break;
        case VISIBILITY_VERYLOW:
          ActualText   = ActualyDisplayedEntry->Message;
          Frame_Text.SetDisplayText(ActualText.String);
          Frame_Text.SetStyle(GameEnv->TileSetStyles->GetStyle(3));
          Frame_Text.GetTextDisplaySize(&Size);
          if (Size.x > ScreenSize.x-128.0f) Size.x = ScreenSize.x -128.0f;
          Frame_Text.SetPosition( (ScreenSize.x - Size.x) / 2.0f , (ScreenSize.y * 0.95f) - (Size.y / 2.0f) );
          Frame_Text.SetSize(Size.x,Size.y);
          GameEnv->GuiManager.AddFrame(&Frame_Text);
          break;
        case VISIBILITY_VERYHARDTOREAD:
          ActualText   = ActualyDisplayedEntry->Message;
          Frame_Text.SetDisplayText(ActualText.String);
          Frame_Text.SetStyle(GameEnv->TileSetStyles->GetStyle(0));
          Frame_Text.GetTextDisplaySize(&Size);
          if (Size.x > ScreenSize.x-128.0f) Size.x = ScreenSize.x -128.0f;
          Frame_Text.SetPosition( (ScreenSize.x - Size.x) / 2.0f , (ScreenSize.y * 0.95f) - (Size.y / 2.0f) );
          Frame_Text.SetSize(Size.x,Size.y);
          GameEnv->GuiManager.AddFrame(&Frame_Text);
          break;
      }
    }
  }
  else
  {
    ActualyDisplayedEntry->DisplayTime -= FrameTime;
    ActualyDisplayedEntry->MinimumDisplayTime -= FrameTime;

    Stop = false;
    if (!Flag_Shown)                                     Stop = true;
    else if   (ActualyDisplayedEntry->DisplayTime < 0.0) Stop = true;
    else if   ((ActualyDisplayedEntry->MinimumDisplayTime < 0.0) && DisplayFile.GetCount() > 2) Stop = true;

    if ( Stop )
    {
      GameEnv->GuiManager.RemoveFrame(&Frame_Text);
      ActualText.Clear();
      delete ActualyDisplayedEntry;
      ActualyDisplayedEntry = 0;
    }


  }
}

void ZGameWindow_Advertising::Clear()
{
  DisplayEntry * Entry;

  if (ActualyDisplayedEntry)
  {
    GameEnv->GuiManager.RemoveFrame(&Frame_Text);
    ActualText.Clear();
    delete ActualyDisplayedEntry;
    ActualyDisplayedEntry = 0;
  }
  while ((Entry = (DisplayEntry *)DisplayFile.PullFromFile())) delete Entry;
}
