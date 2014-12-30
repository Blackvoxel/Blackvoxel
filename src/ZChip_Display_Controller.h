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
 * ZChip_Display_Controller.h
 *
 *  Created on: 24 déc. 2014
 *      Author: laurent
 */

#ifndef Z_ZCHIP_DISPLAY_CONTROLLER_H
#define Z_ZCHIP_DISPLAY_CONTROLLER_H

//#ifndef Z_ZCHIP_DISPLAY_CONTROLLER_H
//#  include "ZChip_Display_Controller.h"
//#endif

#ifndef Z_ZCIRCUIT_H
#  include "ZChip.h"
#endif

#define Z_ZCHIP_DISPLAY_CONTROLLER_INTERRUPT_ENABLE 1
#define Z_ZCHIP_DISPLAY_CONTROLLER_INTERRUPT_TRIGGERED 2
#define Z_ZCHIP_DISPLAY_CONTROLLER_VRAMSIZE (320*256)

class ZChip_Display_Controller : public ZChip
{
    ULong DisplayMode;
    ULong VideoPointer;
    ULong Interrupts;

    //ZMemSize LastCycleCount;
  public:
    ZChip_Display_Controller() {Reset();}

    void  SetRegister(ULong Register, ULong Value);
    ULong GetRegister(ULong Register);
    void  DoChipManagement();
    void  DoChipManagement_FrameCycle();
    void  Reset();

    void  TriggerInterrupt();

    virtual bool Save(ZStream_SpecialRamStream * Stream);
    virtual bool Load(ZStream_SpecialRamStream * Stream);

    // External "screen DMA" need to get these pointers to access memory.

    inline ULong GetDisplayMode()  {return(DisplayMode); }
    inline ULong GetVideoPointer() {return(VideoPointer);}



};



#endif /* Z_ZCHIP_DISPLAY_CONTROLLER_H */
