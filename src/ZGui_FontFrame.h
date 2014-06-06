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
 * ZGui_FontFrame.h
 *
 *  Created on: 6 mai 2011
 *      Author: laurent
 */

#ifndef Z_GUI_FONTFRAME_H
#define Z_GUI_FONTFRAME_H

//#ifndef Z_GUI_FONTFRAME_H
//#  include "ZGui_FontFrame.h"
//#endif

#ifndef Z_ZGUI_H
#  include "ZGui.h"
#endif

#ifndef Z_ZTILESETS_H
#  include "ZTileSets.h"
#endif

class ZFrame_FontFrame : public ZFrame
{
  protected:
    char * TextToDisplay;
    ZTileStyle * TileStyle;
  public:

    ZFrame_FontFrame()         {FrameType = MulticharConst('T','E','X','T');TileStyle = 0; TextToDisplay = (char *)""; }
    virtual void  SetDisplayText(char const * TextToDisplay)   { this->TextToDisplay = (char *)TextToDisplay; }
    virtual void  SetStyle      (ZTileStyle * TileStyle) { this->TileStyle = TileStyle;         }
    virtual void GetTextDisplaySize(ZVector2f * OutSize);

    virtual void Render2(Frame_Dimensions * ParentPosition);
    virtual void Render(Frame_Dimensions * ParentPosition);
};


#endif /* Z_GUI_FONTFRAME_H */
