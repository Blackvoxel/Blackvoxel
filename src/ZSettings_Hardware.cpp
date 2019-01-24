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
 * ZSettings_Hardware.cpp
 *
 *  Created on: 2 juin 2011
 *      Author: laurent
 */

#include "ZSettings_Hardware.h"
#include <stdio.h>

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

#ifndef Z_ZSTREAMS_FILE_H
#  include "z/ZStream_File.h"
#endif

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

ZSettings_Hardware::ZSettings_Hardware()
{
  // Default Settings

  Setting_Resolution_h = 0;
  Setting_Resolution_v = 0;
  Setting_FullScreen = true;
  Setting_NoframeWindow = false;
  Setting_OldLooked = false;
  Setting_ViewPort_Offset_x = 0;
  Setting_ViewPort_Offset_y = 0;
  Setting_ViewPort_Size_x = 0;
  Setting_ViewPort_Size_y =0;
  Setting_SoundEnabled = true;
  Setting_SoundVolume  = 100.0;
  Setting_MouseFactor = 2.5;
  Setting_MouseFlipY = false;
  Setting_Key_MoveForward = 'w';
  Setting_Key_MoveBackward = 's';
  Setting_Key_MoveLeft = 'a';
  Setting_Key_MoveRight= 'd';
  Setting_Key_MoveUp   = 'q';
  Setting_Key_MoveDown = 'z';
  Setting_Key_Jump = ' ';
  Setting_Key_Inventory = 'i';
  Set_RenderDistance_Horiz = 0;
  Set_RenderDistance_Vert = 0;
  RenderingDistance_Horizontal = 8;
  RenderingDistance_Vertical = 3;
  Opt_SectCFactor = 1.0;
  PixelAspectRatio = 1.0;
  // Experimental_LearningMode = false;
  Setting_Version = 1;

  // Special default settings for Raspberry Pi

  if (COMPILEOPTION_PLATFORM_RASPBERRY_PI==1)
  {
    Setting_FullScreen = false;
    Setting_Favorite_Editor = "leafpad";
  }

  RemoteExtension_Enable = false;
  RemoteExtension_Port   = 0;
}


bool ZSettings_Hardware::Load()
{
  ZString File, FileName, As, Line, Token;

  if (COMPILEOPTION_USEHOMEDIRSTORAGE)
  {
    FileName = ZStream_File::Get_Directory_UserData();
  }
  FileName.AddToPath(COMPILEOPTION_SAVEFOLDERNAME);
  FileName.AddToPath("Settings_Hardware.dat");

  if (File.LoadFromFile(FileName.String))
  {
    while ( File.Split(0x0a, Line) )
    {
      Line.StripAll(0x0d);
      if (Line.Split('=', Token))
      {
        Token.StripAll(' ');

        if (Token=="Setting_Resolution_h") Setting_Resolution_h = Line.GetULong();
        if (Token=="Setting_Resolution_v") Setting_Resolution_v = Line.GetULong();
        if (Token=="Setting_FullScreen")   Setting_FullScreen = (Line.GetULong() > 0) ? true : false;
        if (Token=="Setting_NoframeWindow")Setting_NoframeWindow = (Line.GetULong() > 0) ? true : false;
        if (Token=="Setting_OldLooked")    Setting_OldLooked = (Line.GetULong() > 0) ? true : false;

        if (Token=="Setting_ViewPort_Offset_x") Setting_ViewPort_Offset_x = Line.GetULong();
        if (Token=="Setting_ViewPort_Offset_y") Setting_ViewPort_Offset_y = Line.GetULong();
        if (Token=="Setting_ViewPort_Size_x")   Setting_ViewPort_Size_x   = Line.GetULong();
        if (Token=="Setting_ViewPort_Size_y")   Setting_ViewPort_Size_y   = Line.GetULong();
        if (Token=="Setting_Sound_Enabled")     Setting_SoundEnabled      = (Line.GetULong() > 0) ? true : false;
        if (Token=="Setting_Sound_Volume")      Setting_SoundVolume       = Line.GetDouble(); if (Setting_SoundVolume < 0.0 || Setting_SoundVolume > 100.0) Setting_SoundVolume = 100.0;
        if (Token=="Setting_Mouse_Factor")      Setting_MouseFactor       = Line.GetDouble();
        if (Token=="Setting_MouseFlipY")        Setting_MouseFlipY       = (Line.GetULong() > 0) ? true : false;

        if (Token=="Setting_Key_MoveForward")   Setting_Key_MoveForward   = (UShort)Line.GetULong();
        if (Token=="Setting_Key_MoveBackward")  Setting_Key_MoveBackward  = (UShort)Line.GetULong();
        if (Token=="Setting_Key_MoveLeft")      Setting_Key_MoveLeft      = (UShort)Line.GetULong();
        if (Token=="Setting_Key_MoveRight")     Setting_Key_MoveRight     = (UShort)Line.GetULong();
        if (Token=="Setting_Key_MoveUp")        Setting_Key_MoveUp        = (UShort)Line.GetULong();
        if (Token=="Setting_Key_MoveDown")      Setting_Key_MoveDown      = (UShort)Line.GetULong();

        if (Token=="Setting_Key_Jump")          Setting_Key_Jump          = (UShort)Line.GetULong();
        if (Token=="Setting_Key_Inventory")     Setting_Key_Inventory     = (UShort)Line.GetULong();

        // These values are now computed automaticaly
        // Use Set_RenderDistance_Horiz & Set_RenderDistance_Vert to force values.

        if (Token=="RenderingDistance_Horizontal")
        {
          ULong Val = (ULong)Line.GetULong();
          if (Val == 8) Set_RenderDistance_Horiz = 0;
          else          Set_RenderDistance_Horiz = Val;
        }
        if (Token=="RenderingDistance_Vertical")
        {
          ULong Val = (ULong)Line.GetULong();
          if (Val == 3) Set_RenderDistance_Vert = 0;
          else          Set_RenderDistance_Vert = Val;
        }

        //

        if (Token=="RenderingDistance_Horizontal")
        {
          ULong Val = (ULong)Line.GetULong();
          if (Val == 8) Set_RenderDistance_Horiz = 0;
          else          Set_RenderDistance_Horiz = Val;
        }

        if (Token=="Set_RenderDistance_Horiz")
        {
          Set_RenderDistance_Horiz  = (ULong)Line.GetULong();
          if (Set_RenderDistance_Horiz>65535) Set_RenderDistance_Horiz = 65535;
        }
        if (Token=="Set_RenderDistance_Vert")
        {
          Set_RenderDistance_Vert    = (ULong)Line.GetULong();
          if (Set_RenderDistance_Vert>65535) Set_RenderDistance_Vert = 65535;
        }
        if (Token=="Opt_SectCFactor")
        {
          Opt_SectCFactor = Line.GetDouble();
          if (Opt_SectCFactor < 0.5) Opt_SectCFactor = 0.5;
          if (Opt_SectCFactor > 10.0) Opt_SectCFactor = 10.0;
        }

        if (Token=="PixelAspectRatio")
        {
          PixelAspectRatio = Line.GetDouble();
          if (PixelAspectRatio < 0.5) PixelAspectRatio = 0.5;
          if (PixelAspectRatio > 3.0) PixelAspectRatio = 3.0;
        }
        //if (Token=="Experimental_LearningMode") { Experimental_LearningMode = (Line.GetULong() > 0) ? true : false; }

        if (Token=="WebExtension_Enable") { RemoteExtension_Enable = (Line.GetULong() > 0) ? true : false; }
        if (Token=="WebExtension_Port")   { RemoteExtension_Port   = (UShort)Line.GetULong(); }

        if (Token=="Setting_Favorite_Editor")
        {
          Line.StripLeading(' ');
          Setting_Favorite_Editor = Line;
        }

        if (Token=="Setting_Version")
        {
          //ULong FileSettingVersion;

          File_Setting_Version = Line.GetULong();
        }


      }
    }
  } else return(false);
  return(true);
}

bool ZSettings_Hardware::Save()
{
  ZString Out, FileName, NewLine;

  if (COMPILEOPTION_USEHOMEDIRSTORAGE)
  {
    FileName = ZStream_File::Get_Directory_UserData();
  }
  FileName.AddToPath(COMPILEOPTION_SAVEFOLDERNAME);
  FileName.AddToPath("Settings_Hardware.dat");

  Out << "Setting_Resolution_h          = " << Setting_Resolution_h                   << Out.NewLine();
  Out << "Setting_Resolution_v          = " << Setting_Resolution_v                   << Out.NewLine();
  Out << "Setting_FullScreen            = " << (Setting_FullScreen ? 1u:0u )          << Out.NewLine();
  Out << "Setting_NoframeWindow         = " << (Setting_NoframeWindow ? 1u:0u)        << Out.NewLine();
  Out << "Setting_OldLooked             = " << (Setting_OldLooked ? 1u:0u)            << Out.NewLine();
  Out << "Setting_ViewPort_Offset_x     = " << Setting_ViewPort_Offset_x              << Out.NewLine();
  Out << "Setting_ViewPort_Offset_y     = " << Setting_ViewPort_Offset_y              << Out.NewLine();
  Out << "Setting_ViewPort_Size_x       = " << Setting_ViewPort_Size_x                << Out.NewLine();
  Out << "Setting_ViewPort_Size_y       = " << Setting_ViewPort_Size_y                << Out.NewLine();
  Out << "Setting_Sound_Enabled         = " << (Setting_SoundEnabled ? 1u:0u)         << Out.NewLine();
  Out << "Setting_Sound_Volume          = " << Setting_SoundVolume                    << Out.NewLine();
  Out << "Setting_Mouse_Factor          = " << Setting_MouseFactor                    << Out.NewLine();
  Out << "Setting_MouseFlipY            = " << (Setting_MouseFlipY ? 1u:0u)           << Out.NewLine();
  Out << "Setting_Key_MoveForward       = " << (ULong)Setting_Key_MoveForward         << Out.NewLine();
  Out << "Setting_Key_MoveBackward      = " << (ULong)Setting_Key_MoveBackward        << Out.NewLine();
  Out << "Setting_Key_MoveLeft          = " << (ULong)Setting_Key_MoveLeft            << Out.NewLine();
  Out << "Setting_Key_MoveRight         = " << (ULong)Setting_Key_MoveRight           << Out.NewLine();
  Out << "Setting_Key_MoveUp            = " << (ULong)Setting_Key_MoveUp              << Out.NewLine();
  Out << "Setting_Key_MoveDown          = " << (ULong)Setting_Key_MoveDown            << Out.NewLine();
  Out << "Setting_Key_Jump              = " << (ULong)Setting_Key_Jump                << Out.NewLine();
  Out << "Setting_Key_Inventory         = " << (ULong)Setting_Key_Inventory           << Out.NewLine();
  Out << "Set_RenderDistance_Horiz      = " << (ULong)Set_RenderDistance_Horiz << Out.NewLine();
  Out << "Set_RenderDistance_Vert       = " << (ULong)Set_RenderDistance_Vert   << Out.NewLine();
  Out << "Opt_SectCFactor               = " << Opt_SectCFactor << Out.NewLine();
  Out << "PixelAspectRatio              = " << PixelAspectRatio << Out.NewLine();
  //Out << "Experimental_LearningMode     = " << (Experimental_LearningMode ? 1u:0u)    << Out.NewLine();
  Out << "WebExtension_Enable           = " << (RemoteExtension_Enable ? 1u:0u)    << Out.NewLine();
  Out << "WebExtension_Port             = " << RemoteExtension_Port << Out.NewLine();
  Out << "Setting_Favorite_Editor       = " << Setting_Favorite_Editor << Out.NewLine();
  Out << "Setting_Version               = " << (ULong)Setting_Version                 << Out.NewLine();

  return( Out.SaveToFile(FileName.String) );
}

void ZSettings_Hardware:: AdjustForRealHardware(ZHardwareInfo * HardwareInfo)
{
  // Adjust depends on hardware profile

  switch(HardwareInfo->HardwareProfile)
  {
    case ZHardwareInfo::X86_GENERAL_PROFILE:    RenderingDistance_Horizontal = 8;
                                                RenderingDistance_Vertical   = 5;
                                                break;
    case ZHardwareInfo::X86_NETBOOK_PROFILE:    RenderingDistance_Horizontal = 5;
                                                RenderingDistance_Vertical   = 1;
                                                break;
    case ZHardwareInfo::ARM_GENERAL_PROFILE:    RenderingDistance_Horizontal = 3;
                                                RenderingDistance_Vertical   = 1;
                                                break;
    case ZHardwareInfo::ARM_RASPBERRY_PI_1:     RenderingDistance_Horizontal = 1;
                                                RenderingDistance_Vertical   = 1;
                                                break;
    case ZHardwareInfo::ARM_RASPBERRY_PI_2:     RenderingDistance_Horizontal = 1;
                                                RenderingDistance_Vertical   = 1;
                                                break;
    case ZHardwareInfo::ARM_RASPBERRY_PI_3:     RenderingDistance_Horizontal = 2;
                                                RenderingDistance_Vertical   = 1;
                                                break;
  }

  // If settings are manually fixed, use these settings instead.

  if (Set_RenderDistance_Horiz) RenderingDistance_Horizontal = Set_RenderDistance_Horiz;
  if (Set_RenderDistance_Vert)  RenderingDistance_Vertical   = Set_RenderDistance_Vert;


}
