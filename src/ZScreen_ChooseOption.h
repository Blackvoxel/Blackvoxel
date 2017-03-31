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
 * ZScreen_ChooseOption.h
 *
 *  Created on: 10 juin 2011
 *      Author: laurent
 */

#ifndef Z_ZSCREEN_CHOOSEOPTION_H
#define Z_ZSCREEN_CHOOSEOPTION_H

//#ifndef Z_ZSCREEN_CHOOSEOPTION_H
//#  include "ZScreen_ChooseOption.h"
//#endif

#ifndef Z_ZGUI_H
#  include "ZGui.h"
#endif

#ifndef Z_GUI_FONTFRAME_H
#  include "ZGui_FontFrame.h"
#endif

#ifndef Z_ZGUI_TILEFRAME_H
#  include "ZGui_TileFrame.h"
#endif

#ifndef Z_ZGUI_CYCLINGCHOICEBOX_H
#  include "ZGui_CyclingChoiceBox.h"
#endif

#ifndef ZGUI_ZNUMERICCHOICEBOX_H
#  include "ZGui_NumericChoiceBox.h"
#endif

#ifndef Z_ZGUI_CHECKBOX_H
#  include "ZGui_CheckBox.h"
#endif

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

class ZScreen_ChooseOption : public ZScreen
{
    virtual ULong ProcessScreen_OldLook(ZGame * GameEnv);
  public:
    enum {CHOICE_QUIT, CHOICE_DISPLAY, CHOICE_SOUND, CHOICE_MOUSE, CHOICE_KEYMAP};
    virtual ULong ProcessScreen(ZGame * GameEnv);
};

#endif /* Z_ZSCREEN_CHOOSEOPTION_H */
