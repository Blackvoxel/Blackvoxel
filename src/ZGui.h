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
 * ZGui.h
 *
 *  Created on: 8 avr. 2011
 *      Author: laurent
 */

#ifndef Z_ZGUI_H
#define Z_ZGUI_H

//#ifndef Z_ZGUI_H
//#  include "ZGui.h"
//#endif

#ifndef Z_ZGUI_H
#  include "ZGui.h"
#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZOBJECT_H
#  include "z/ZObject.h"
#endif

#ifndef Z_ZOBJECT_LISTS_H
#  include "z/ZObject_Lists.h"
#endif

#ifndef Z_ZTEXTUREMANAGER_H
#  include "ZTextureManager.h"
#endif

#ifndef Z_EVENTDISPATCH_H
#  include "ZEventManager.h"
#endif

/*
struct Frame_Dimensions
{
  float Position_x, Position_y, Position_z;
  float Width, Height, Depth;
};
*/

#ifndef _STDIO_H
#  include <stdio.h>
#endif

typedef ZBox3f Frame_Dimensions;

class ZGraphicUserManager;
class ZGame;


class ZScreen : public ZObject
{
  public:
    ULong ResultCode;

     ZScreen() {ResultCode = 0;}
    virtual ~ZScreen() {}
    virtual ULong ProcessScreen(ZGame * Game) = 0;
};

class ZFrame : public ZObject
{
  public:
    ULong FrameType;
    ZList SubFrameList;
    ULong TextureNum;
    ZColor3f              DrawColor;
    ZGraphicUserManager * GuiManager;
    ZFrame              * ParentFrame;
    Bool   Flag_Show_Frame;
    Bool   Flag_Show_Childs;
    Bool   Flag_Show_Master;
    bool   Flag_Cap_Dragable;
    bool   Flag_Draging;

    Frame_Dimensions Dimensions;
    Frame_Dimensions EffectivePosition;
    ZVector2f        DragAbsolutePosition;

    // Event Flags

    bool  Event_MouseWentIn; // Mouse Went over.
    bool  Event_MouseWentOut;
    bool  Event_MouseIsActualyOver;  // Mouse is actualy over.
    bool  Event_MouseClick;
    bool  Event_MouseRelease;
    bool  Event_GetFocus;
    bool  Event_LostFocus;

    // Event Callback enable

    bool Flag_Enable_Proc_MouseIn;
    bool Flag_Enable_Proc_MouseOut;
    bool Flag_Enable_Proc_MouseOver;
    bool Flag_Enable_Proc_GetFocus;
    bool Flag_Enable_Proc_LostFocus;
    bool Flag_Enable_Proc_KeyDown;
    bool Flag_Enable_Proc_KeyUp;

    //bool  Flag
  // Event Query
    bool Is_MouseOver() { return( Event_MouseIsActualyOver);}
    bool Is_MouseIn (bool Reset = true)     { bool res; res = Event_MouseWentIn;  if (Reset) Event_MouseWentIn = false;  return(res); }
    bool Is_MouseOut(bool Reset = true)     { bool res; res = Event_MouseWentOut; if (Reset) Event_MouseWentOut = false; return(res); }
    bool Is_MouseClick(bool Reset = true)   { bool res; res = Event_MouseClick; if (Reset) Event_MouseClick = false; return(res); }
    bool Is_MouseRelease(bool Reset = true) { bool res; res = Event_MouseRelease; if (Reset) Event_MouseRelease = false; return(res); }


    ZFrame()
    {
      FrameType = MulticharConst('B','F','R','M'); // = BaseFRaMe
      Flag_Cap_Dragable = false;
      Flag_Draging = false;
      Dimensions.Position_x = 0.0f;
      Dimensions.Position_y = 0.0f;
      Dimensions.Position_z = 0.0f;
      Dimensions.Width      = 0.0f;
      Dimensions.Height     = 0.0f;
      Dimensions.Depth      = 0.0f;
      EffectivePosition.Position_x = 0.0f;
      EffectivePosition.Position_y = 0.0f;
      EffectivePosition.Position_z = 0.0f;
      EffectivePosition.Width      = 0.0f;
      EffectivePosition.Height     = 0.0f;
      EffectivePosition.Depth      = 0.0f;

      Flag_Show_Frame = true;
      Flag_Show_Childs= true;
      Flag_Show_Master= true;

      Event_MouseWentIn        = false;
      Event_MouseWentOut       = false;
      Event_MouseIsActualyOver = false;
      Event_MouseClick         = false;
      Event_MouseRelease       = false;
      Event_GetFocus           = false;
      Event_LostFocus          = false;

      Flag_Enable_Proc_MouseIn      = false;
      Flag_Enable_Proc_MouseOut     = false;
      Flag_Enable_Proc_MouseOver    = false;
      Flag_Enable_Proc_GetFocus     = false;
      Flag_Enable_Proc_LostFocus    = false;
      Flag_Enable_Proc_KeyDown      = false;
      Flag_Enable_Proc_KeyUp        = false;

      GuiManager  = 0;
      TextureNum  = 0;
      ParentFrame = 0;

      DrawColor.r = 1.0f; DrawColor.v = 1.0f; DrawColor.b = 1.0f;
    }

    virtual void AddedToFrameCallback(ZFrame * Frame) {};

    virtual void AddFrame( ZFrame * Frame )
    {
      SubFrameList.AddToHead(Frame);
      Frame->GuiManager = GuiManager;
      Frame->ParentFrame = this;
      Frame->Dimensions.Position_z = this->Dimensions.Position_z + 1.0f;
      Frame->AddedToFrameCallback(this);
    }

    virtual void AddFrame_ToTail( ZFrame * Frame )
    {
      SubFrameList.AddToTail(Frame);
      Frame->GuiManager = GuiManager;
      Frame->ParentFrame = this;
      Frame->Dimensions.Position_z = this->Dimensions.Position_z + 1.0f;
      Frame->AddedToFrameCallback(this);
    }

    virtual void RemoveFrame(ZFrame * Frame)
    {
      SubFrameList.RemoveObject(Frame);
      Frame->RemoveAllFrames();
    }

    virtual void RemoveAllFrames()
    {
      ZListItem * Item, * OldItem;
      Item = SubFrameList.GetFirst();
      while(Item)
      {
        OldItem = Item;
        Item = SubFrameList.GetNext(Item);
        SubFrameList.Remove(OldItem);
        ((ZFrame*)OldItem->GetObject())->RemoveAllFrames();
      }

      SubFrameList.ClearList();
    }


    virtual void Render(Frame_Dimensions * ParentPosition);
    virtual void SetTexture(ULong TextureNum) {this->TextureNum = TextureNum; }
    virtual void SetPosition(float x, float y) {Dimensions.Position_x = x; Dimensions.Position_y = y;}
    virtual void SetDragPosition(float x, float y) { DragAbsolutePosition.x = x; DragAbsolutePosition.y = y; }
    virtual void SetZPosition(float z) {Dimensions.Position_z = z;}
    virtual void SetSize    (float Width, float Height) {Dimensions.Width = Width ; Dimensions.Height = Height; }
    virtual void SetSize    (ZVector2f * Size)          {Dimensions.Width = Size->x; Dimensions.Height = Size->y; }
    virtual void GetSize    (float &Width, float &Height) {Width = Dimensions.Width; Height = Dimensions.Height; }
    virtual void SetDepth   (float Depth)               {Dimensions.Depth = Depth ;}
    virtual void SetColor   (float r,float v, float b)  {DrawColor.r = r; DrawColor.v = v; DrawColor.b = b; }

    virtual void Show (Bool ShowState) {Flag_Show_Master = ShowState;}
    virtual void Show_Frame(Bool ShowState = true)  { Flag_Show_Frame = ShowState; }
    virtual void Show_Childs(Bool ShowState = true) { Flag_Show_Childs= ShowState; }

    virtual Bool KeyDown( UShort KeySym ) { return true; }
    virtual Bool KeyUp( UShort KeySym )   { return true; }
    virtual Bool MouseMove ( Short Relative_x, Short Relative_y, UShort Absolute_x, UShort Absolute_y);

    virtual Bool MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y);

    virtual Bool MouseButtonRelease(UShort nButton, Short Absolute_x, Short Absolute_y);


    virtual void GetFocus() {}
    virtual void LostFocus() {}

    virtual void MouseOut()
    {

    }

    virtual void MouseIn()
    {

    }

    virtual void MouseOver(Short Inner_x, Short Inner_y)
    {

    }
    virtual void DropItem(ZFrame * Item, UShort nButton)
    {

    }

};

class ZGraphicUserManager : public ZEventConsumer
{
  friend class ZFrame;
  ZFrame * FirstFrame;
  public:


    ZTextureManager * TextureManager;
    ZEventManager   * EventManager;


    ZGraphicUserManager();
   ~ZGraphicUserManager();

    // Modal stack size

    ZFrame ** Modal_Stack;
    ZMemSize  Modal_StackSize;

    // Mouse over imbrication stack tables

    ZFrame ** Actual_PointedStack;
    ZFrame ** Previous_PointedStack;
    ZFrame ** Actual_FocusStack;
    ZFrame ** Previous_FocusStack;

    ZMemSize  Actual_StackSize;
    ZMemSize  Previous_StackSize;
    ZMemSize  Actual_FocusStackSize;
    ZMemSize  Previous_FocusStackSize;

    // Draging support

    ZFrame * DragingFrame;

    // Mouse over stack functions. These are not for public use.

    inline void MouseOverStack_AddToStack(ZFrame * Frame) { if (Actual_StackSize>=1024) return; Actual_PointedStack[Actual_StackSize++] = Frame; }
    void MouseOverStack_SwapAndReset();
    void FocusStack_SwapAndReset();
    inline void FocusStack_AddToStack(ZFrame * Frame) { if (Actual_FocusStackSize>=1024) return;  Actual_FocusStack[Actual_FocusStackSize++]=Frame;}


    void SetTextureManager( ZTextureManager * TextureManager );
    void SetEventManager( ZEventManager * EventManager );


    void SetScreenDimensions(float OriginX,float OriginY, float Width, float Height);

    void AddFrame(ZFrame * Frame);
    void AddFrame_ToTail(ZFrame * Frame);
    void RemoveFrame(ZFrame * Frame);
    void RemoveAllFrames(); // Clear Screen
    void Frame_PushModal(ZFrame * Frame);
    void Frame_PopModal();

    void Render();


    // Event Input

    virtual Bool KeyDown( UShort KeySym );
    virtual Bool KeyUp( UShort KeySym );
    virtual Bool MouseMove ( Short Relative_x, Short Relative_y, UShort Absolute_x, UShort Absolute_y);
    virtual Bool MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y);
    virtual Bool MouseButtonRelease(UShort nButton, Short Absolute_x, Short Absolute_y);

};


#endif
