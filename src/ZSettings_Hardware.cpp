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

bool ZSettings_Hardware::Load()
{
  ZString File, FileName, As, Line, Token;

  if (COMPILEOPTION_USEHOMEDIRSTORAGE)
  {
    FileName = ZStream_File::Get_Directory_UserData();
    FileName.AddToPath(COMPILEOPTION_SAVEFOLDERNAME);
  }
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

        if (Token=="Setting_ViewPort_Offset_x") Setting_ViewPort_Offset_x = Line.GetULong();
        if (Token=="Setting_ViewPort_Offset_y") Setting_ViewPort_Offset_y = Line.GetULong();
        if (Token=="Setting_ViewPort_Size_x")   Setting_ViewPort_Size_x   = Line.GetULong();
        if (Token=="Setting_ViewPort_Size_y")   Setting_ViewPort_Size_y   = Line.GetULong();
        if (Token=="Setting_Sound_Enabled")     Setting_SoundEnabled      = (Line.GetULong() > 0) ? true : false;
        if (Token=="Setting_Sound_Volume")      Setting_SoundVolume       = Line.GetDouble(); if (Setting_SoundVolume < 0.0 || Setting_SoundVolume > 100.0) Setting_SoundVolume = 100.0;
        if (Token=="Setting_Mouse_Factor")      Setting_MouseFactor       = Line.GetDouble();

        if (Token=="Setting_Key_MoveForward")   Setting_Key_MoveForward   = (UShort)Line.GetULong();
        if (Token=="Setting_Key_MoveBackward")  Setting_Key_MoveBackward  = (UShort)Line.GetULong();
        if (Token=="Setting_Key_MoveLeft")      Setting_Key_MoveLeft      = (UShort)Line.GetULong();
        if (Token=="Setting_Key_MoveRight")     Setting_Key_MoveRight     = (UShort)Line.GetULong();
        if (Token=="Setting_Key_MoveUp")        Setting_Key_MoveUp        = (UShort)Line.GetULong();
        if (Token=="Setting_Key_MoveDown")      Setting_Key_MoveDown      = (UShort)Line.GetULong();

        if (Token=="Setting_Key_Jump")          Setting_Key_Jump          = (UShort)Line.GetULong();
        if (Token=="Setting_Key_Inventory")     Setting_Key_Inventory     = (UShort)Line.GetULong();

        if (Token=="RenderingDistance_Horizontal")
        {
          RenderingDistance_Horizontal  = (ULong)Line.GetULong();
          if (RenderingDistance_Horizontal<1) RenderingDistance_Horizontal = 1;
          if (RenderingDistance_Horizontal>65535) RenderingDistance_Horizontal = 65535;
        }
        if (Token=="RenderingDistance_Vertical")
        {
          RenderingDistance_Vertical    = (ULong)Line.GetULong();
          if (RenderingDistance_Vertical<1) RenderingDistance_Vertical = 1;
          if (RenderingDistance_Vertical>65535) RenderingDistance_Vertical = 65535;
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
        if (Token=="Experimental_LearningMode") { Experimental_LearningMode = (Line.GetULong() > 0) ? true : false; }

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
    FileName.AddToPath(COMPILEOPTION_SAVEFOLDERNAME);
  }
  FileName.AddToPath("Settings_Hardware.dat");

  Out << "Setting_Resolution_h         = " << Setting_Resolution_h                   << Out.NewLine();
  Out << "Setting_Resolution_v         = " << Setting_Resolution_v                   << Out.NewLine();
  Out << "Setting_FullScreen           = " << (Setting_FullScreen ? 1u:0u )          << Out.NewLine();
  Out << "Setting_ViewPort_Offset_x    = " << Setting_ViewPort_Offset_x              << Out.NewLine();
  Out << "Setting_ViewPort_Offset_y    = " << Setting_ViewPort_Offset_y              << Out.NewLine();
  Out << "Setting_ViewPort_Size_x      = " << Setting_ViewPort_Size_x                << Out.NewLine();
  Out << "Setting_ViewPort_Size_y      = " << Setting_ViewPort_Size_y                << Out.NewLine();
  Out << "Setting_Sound_Enabled        = " << (Setting_SoundEnabled ? 1u:0u)         << Out.NewLine();
  Out << "Setting_Sound_Volume         = " << Setting_SoundVolume                    << Out.NewLine();
  Out << "Setting_Mouse_Factor         = " << Setting_MouseFactor                    << Out.NewLine();
  Out << "Setting_Key_MoveForward      = " << (ULong)Setting_Key_MoveForward         << Out.NewLine();
  Out << "Setting_Key_MoveBackward     = " << (ULong)Setting_Key_MoveBackward        << Out.NewLine();
  Out << "Setting_Key_MoveLeft         = " << (ULong)Setting_Key_MoveLeft            << Out.NewLine();
  Out << "Setting_Key_MoveRight        = " << (ULong)Setting_Key_MoveRight           << Out.NewLine();
  Out << "Setting_Key_MoveUp           = " << (ULong)Setting_Key_MoveUp              << Out.NewLine();
  Out << "Setting_Key_MoveDown         = " << (ULong)Setting_Key_MoveDown            << Out.NewLine();
  Out << "Setting_Key_Jump             = " << (ULong)Setting_Key_Jump                << Out.NewLine();
  Out << "Setting_Key_Inventory        = " << (ULong)Setting_Key_Inventory           << Out.NewLine();
  Out << "RenderingDistance_Horizontal = " << (ULong)RenderingDistance_Horizontal << Out.NewLine();
  Out << "RenderingDistance_Vertical   = " << (ULong)RenderingDistance_Vertical     << Out.NewLine();
  Out << "Opt_SectCFactor              = " << Opt_SectCFactor << Out.NewLine();
  Out << "PixelAspectRatio             = " << PixelAspectRatio << Out.NewLine();
  Out << "Experimental_LearningMode    = " << (Experimental_LearningMode ? 1u:0u)    << Out.NewLine();


  return( Out.SaveToFile(FileName.String) );
}
