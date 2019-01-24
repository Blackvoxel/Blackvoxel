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
 * ZScreen_UnsupportedGameMode.h
 *
 *  Created on: 19 oct. 2018
 *      Author: laurent
 */

#ifndef Z_ZSCREEN_UNSUPPORTEDGAMEMODE_H
#define Z_ZSCREEN_UNSUPPORTEDGAMEMODE_H

//#ifndef Z_ZSCREEN_UNSUPPORTEDGAMEMODE_H
//#  include "ZScreen_UnsupportedGameMode.h"
//#endif

#ifndef Z_ZGUI_H
#  include "ZGui.h"
#endif

#ifndef Z_GUI_FONTFRAME_H
#  include "ZGui_FontFrame.h"
#endif

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

class ZScreen_UnsupportedGameMode : public ZScreen
{
  public:
    ZScreen_UnsupportedGameMode() { }
    virtual ULong ProcessScreen(ZGame * GameEnv);
};



#endif /* Z_ZSCREEN_UNSUPPORTEDGAMEMODE_H */
