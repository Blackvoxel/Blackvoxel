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
 * ZSettings_Hardware.h
 *
 *  Created on: 2 juin 2011
 *      Author: laurent
 */

#ifndef Z_ZSETTINGS_HARDWARE_H
#define Z_ZSETTINGS_HARDWARE_H

//#ifndef Z_ZSETTINGS_HARDWARE_H
//#  include "ZSettings_Hardware.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

class ZSettings_Hardware
{
  public:
    // Resolution and screen
    ULong Setting_Resolution_h;
    ULong Setting_Resolution_v;
    ULong Setting_ViewPort_Offset_x;
    ULong Setting_ViewPort_Offset_y;
    ULong Setting_ViewPort_Size_x;
    ULong Setting_ViewPort_Size_y;
    bool  Setting_FullScreen;


    // Sound
    bool   Setting_SoundEnabled;
    double Setting_SoundVolume;

    // Mouse
    double Setting_MouseFactor;

    // Keyboard
    UShort Setting_Key_MoveForward;
    UShort Setting_Key_MoveBackward;
    UShort Setting_Key_MoveLeft;
    UShort Setting_Key_MoveRight;
    UShort Setting_Key_MoveUp;
    UShort Setting_Key_MoveDown;
    UShort Setting_Key_Jump;
    UShort Setting_Key_Inventory;

    // Graphic quality;

    ULong RenderingDistance_Horizontal;
    ULong RenderingDistance_Vertical;
    double Opt_SectCFactor; // Sector Rendering Culling Factor.
    double PixelAspectRatio;

    // Options for special gaming modes

    bool Experimental_LearningMode;

    ZSettings_Hardware()
    {
      Setting_Resolution_h = 0;
      Setting_Resolution_v = 0;
      Setting_FullScreen = true;
      Setting_ViewPort_Offset_x = 0;
      Setting_ViewPort_Offset_y = 0;
      Setting_ViewPort_Size_x = 0;
      Setting_ViewPort_Size_y =0;
      Setting_SoundEnabled = true;
      Setting_SoundVolume  = 100.0;
      Setting_MouseFactor = 2.5;
      Setting_Key_MoveForward = 'w';
      Setting_Key_MoveBackward = 's';
      Setting_Key_MoveLeft = 'a';
      Setting_Key_MoveRight= 'd';
      Setting_Key_MoveUp   = 'a';
      Setting_Key_MoveDown = 'w';
      Setting_Key_Jump = ' ';
      Setting_Key_Inventory = 'i';

      RenderingDistance_Horizontal = 8;
      RenderingDistance_Vertical = 3;
      Opt_SectCFactor = 1.0;
      PixelAspectRatio = 1.0;
      Experimental_LearningMode = false;
    }

    bool Load();
    bool Save();

};

#endif /* Z_ZSETTINGS_HARDWARE_H */
