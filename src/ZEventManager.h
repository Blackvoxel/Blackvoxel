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
 * ZEventDispatch.h
 *
 *  Created on: 8 avr. 2011
 *      Author: laurent
 */

#ifndef Z_EVENTDISPATCH_H
#define Z_EVENTDISPATCH_H

// #ifndef Z_EVENTDISPATCH_H
// #  include "ZEventManager.h"
// #endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZOBJECT_H
#  include "z/ZObject.h"
#endif

#ifndef Z_ZOBJECT_LISTS_H
#  include "z/ZObject_Lists.h"
#endif


class ZEventTable
{


};


class ZEventConsumer : public ZObject
{
  public:
    virtual Bool KeyDown( UShort KeySym )  = 0;
    virtual Bool KeyUp( UShort KeySym )    = 0;
    virtual Bool MouseMove ( Short Relative_x, Short Relative_y, UShort Absolute_x, UShort Absolute_y) = 0;
    virtual Bool MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y) = 0;
    virtual Bool MouseButtonRelease(UShort nButton, Short Absolute_x, Short Absolute_y) = 0;

};

class ZEventManager
{
  protected:
    ZList ConsumerList;
  public:
    // Actual state

    UByte * Keyboard_Matrix;
    UShort MouseX,MouseY;
    bool  MouseButton[64];

    ZEventManager()
    {
      ULong i;

      Keyboard_Matrix = new UByte[4096];
      for (i = 0; i<64;i++) MouseButton[i]=false;
      for (i=0;i<4096;i++) Keyboard_Matrix[i]=0;
      MouseX = 0;
      MouseY = 0;
    }
   ~ZEventManager() { if (Keyboard_Matrix) delete [] Keyboard_Matrix; }

    void AddConsumer_ToTail ( ZEventConsumer * EventConsumer )
    {
      ConsumerList.AddToTail(EventConsumer);
    }

    void RemoveConsumer( ZEventConsumer * EventConsumer )
    {
      ZListItem * Item;
      Item = ConsumerList.GetFirst();
      while (Item)
      {
        Item = ConsumerList.GetNext(Item);
        if (EventConsumer == (ZEventConsumer *)Item->GetObject()) { ConsumerList.Remove(Item); return; }
      }
    }

    bool ProcessEvents();

    bool Is_KeyPressed(UShort KeyCode, bool Reset)
    {
      bool IsPressed;

      IsPressed = Keyboard_Matrix[KeyCode] ? true : false;
      if (Reset) Keyboard_Matrix[KeyCode] = 0;
      return IsPressed;
    }

    bool Is_MouseButtonPressed(UShort ButtonCode, bool Reset)
    {
      bool IsPressed;

      IsPressed = (MouseButton[ButtonCode] ? true : false);
      if (Reset) MouseButton[ButtonCode] = 0;
      return IsPressed;
    }

    // Manual Triggering mouse button events in order to simulate a mouse button click

    void ManualCall_MouseButtonClick(UShort ButtonCode);
    void ManualCall_MouseButtonRelease(UShort ButtonCode);
    void ManualCall_KeyPress(UShort Keycode);
    void ManualCall_KeyRelease(UShort Keycode);
};

#endif
