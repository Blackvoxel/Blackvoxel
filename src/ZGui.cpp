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
 * ZGui.cpp
 *
 *  Created on: 13 avr. 2011
 *      Author: laurent
 */

#include "ZGui.h"
#include <GL/glew.h>
#include <math.h>
#include <stdio.h>
#include "SDL/SDL.h"

ZGraphicUserManager::ZGraphicUserManager()
{
  TextureManager = 0;
  FirstFrame = new ZFrame();
  FirstFrame->GuiManager = this;
  FirstFrame->ParentFrame = 0;
  FirstFrame->Show_Frame(false);

  // Init MouseOver stack imbrication tables
  Actual_PointedStack   = new ZFrame *[1024];
  Previous_PointedStack = new ZFrame *[1024];
  Actual_FocusStack     = new ZFrame *[1024];
  Previous_FocusStack    = new ZFrame *[1024];
  Actual_StackSize = 0;
  Previous_StackSize = 0;
  Actual_FocusStackSize = 0;
  Previous_FocusStackSize = 0;
  DragingFrame = 0;
}

ZGraphicUserManager::~ZGraphicUserManager()
{
  if (Actual_PointedStack)  delete [] Actual_PointedStack;
  if (Previous_PointedStack)delete [] Previous_PointedStack;
  if (Actual_FocusStack)    delete [] Actual_FocusStack;
  if (Previous_FocusStack)  delete [] Previous_FocusStack;
  Actual_StackSize = 0;
  Previous_StackSize = 0;
  Actual_FocusStackSize = 0;
  Previous_FocusStackSize = 0;
}

void ZGraphicUserManager::MouseOverStack_SwapAndReset()
{
  ZFrame ** tmp;
  tmp                   = Actual_PointedStack;
  Actual_PointedStack   = Previous_PointedStack;
  Previous_PointedStack = tmp;
  Previous_StackSize    = Actual_StackSize;
  Actual_StackSize      = 0;
}

void ZGraphicUserManager::FocusStack_SwapAndReset()
{
  ZFrame ** tmp;
  tmp                      = Actual_FocusStack;
  Actual_FocusStack        = Previous_FocusStack;
  Previous_FocusStack      = tmp;
  Previous_FocusStackSize  = Actual_FocusStackSize;
  Actual_FocusStackSize    = 0;
}





void ZGraphicUserManager::SetTextureManager( ZTextureManager * TextureManager )
{
  this->TextureManager = TextureManager;
}

void ZGraphicUserManager::SetEventManager( ZEventManager * EventManager )
{
  this->EventManager = EventManager;
}

void ZGraphicUserManager::SetScreenDimensions(float OriginX,float OriginY, float Width, float Height)
{
  FirstFrame->SetPosition(OriginX,OriginY);
  FirstFrame->SetSize(Width,Height);
}

void ZGraphicUserManager::AddFrame(ZFrame * Frame)
{
  FirstFrame->AddFrame(Frame);
  Frame->SetZPosition(0.0);
  // Frame->SetTexture(1);
}

void ZGraphicUserManager::RemoveFrame(ZFrame * Frame)
{
  FirstFrame->RemoveFrame(Frame);
}

void ZGraphicUserManager::RemoveAllFrames()
{
  FirstFrame->RemoveAllFrames();
  Actual_StackSize = 0;
  Previous_StackSize = 0;
  Actual_FocusStackSize = 0;
  Previous_FocusStackSize = 0;
}

void RemoveFrame(ZFrame * Frame)
{

}



Bool ZGraphicUserManager::KeyDown( UShort KeySym )
{
  ZMemSize i;

  for (i=0;i<this->Actual_FocusStackSize;i++)
  {
    if (Actual_FocusStack[i]->Flag_Enable_Proc_KeyDown) Actual_FocusStack[i]->KeyDown(KeySym);
  }

  return(true);
}

Bool ZGraphicUserManager::KeyUp( UShort KeySym )
{
  ZMemSize i;

  for (i=0;i<this->Actual_FocusStackSize;i++)
  {
    if (Actual_FocusStack[i]->Flag_Enable_Proc_KeyDown) Actual_FocusStack[i]->KeyUp(KeySym);
  }

  return(true);
}

Bool ZGraphicUserManager::MouseMove ( Short Relative_x, Short Relative_y, UShort Absolute_x, UShort Absolute_y)
{
  Bool res;
  ZMemSize i;

  // Frame Draging

  if (DragingFrame)
  {
    ZVector2f Dim;
    DragingFrame->GetSize(Dim.x,Dim.y);
    DragingFrame->SetDragPosition(Absolute_x - Dim.x / 2.0f,Absolute_y - Dim.y / 2.0f);
  }

  //

  MouseOverStack_SwapAndReset();
  res = FirstFrame->MouseMove(Relative_x, Relative_y, Absolute_x, Absolute_y );

  // MouseOut Event.

  //printf("As:"); for (z=0;z<Actual_StackSize;z++) printf(" %lx",(long unsigned int)Actual_PointedStack[z]); printf(" ");
  //printf("Pr:"); for (z=0;z<Previous_StackSize;z++) printf(" %lx",(long unsigned int)Previous_PointedStack[z]); printf("\n");

  for (i=0;i<Previous_StackSize;i++)
  {
    if ((Actual_StackSize <= i) || (Actual_PointedStack[i]!= Previous_PointedStack[i]))
    {
      Previous_PointedStack[i]->Event_MouseIsActualyOver = false;
      Previous_PointedStack[i]->Event_MouseWentOut = true;
      if (Previous_PointedStack[i]->Flag_Enable_Proc_MouseOut) Previous_PointedStack[i]->MouseOut();

      //printf("ActualStack:"); for (z=0;z<Actual_StackSize;z++) printf(" %lx",(long unsigned int)Actual_PointedStack[z]); printf("\n");
      //printf("PrevioStack:");         for (z=0;z<Previous_StackSize;z++) printf(" %lx",(long unsigned int)Previous_PointedStack[z]); printf("\n");

    }
  }

  return(res);
}

Bool ZGraphicUserManager::MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y)
{
  ULong i;

  FocusStack_SwapAndReset();
  FirstFrame->MouseButtonClick( nButton, Absolute_x, Absolute_y );

  for (i=0;i<this->Previous_FocusStackSize;i++)
  {
    if ((Actual_FocusStackSize <= i) || (Actual_FocusStack[i]!= Previous_FocusStack[i]))
    {
      Previous_FocusStack[i]->Event_LostFocus = true;
      if (Previous_FocusStack[i]->Flag_Enable_Proc_LostFocus) Previous_FocusStack[i]->LostFocus();

      //printf("ActualStack:"); for (z=0;z<Actual_StackSize;z++) printf(" %lx",(long unsigned int)Actual_PointedStack[z]); printf("\n");
      //printf("PrevioStack:");         for (z=0;z<Previous_StackSize;z++) printf(" %lx",(long unsigned int)Previous_PointedStack[z]); printf("\n");

    }
  }

  return(true);
}

Bool ZGraphicUserManager::MouseButtonRelease(UShort nButton, Short Absolute_x, Short Absolute_y)
{

  FirstFrame->MouseButtonRelease(nButton, Absolute_x, Absolute_y);

  if (nButton == 1)
  {
    if (DragingFrame)
    {
      DragingFrame->Flag_Draging = false;
      DragingFrame = 0;
    }

  }
  return true;
}

void ZGraphicUserManager::Render()
{
  Frame_Dimensions Dimensions;

  Dimensions.Position_x = 0;
  Dimensions.Position_y = 0;
  Dimensions.Position_z = 0;
  Dimensions.Width = 0;
  Dimensions.Height = 0;
  Dimensions.Depth = 0;

  // OpenGL Setup for gui

  glEnable(GL_TEXTURE_2D);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  //glOrtho(FirstFrame->Dimensions.Position_x, FirstFrame->Dimensions.Width, FirstFrame->Dimensions.Height , FirstFrame->Dimensions.Position_y,50.0, 1000000.0);
  glOrtho(FirstFrame->Dimensions.Position_x, FirstFrame->Dimensions.Width, FirstFrame->Dimensions.Height , FirstFrame->Dimensions.Position_y,-100.0, 500.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_SRC_ALPHA);
  //glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  FirstFrame->Render(&Dimensions);
}

void ZFrame::Render(Frame_Dimensions * ParentPosition)
{

  ZVector3f P1,P2,P3,P4;
  ZListItem * Item;
  ZFrame * Frame;

  // Frame Position Computing
  if (Flag_Show_Master)
  {
    EffectivePosition.Position_x = ParentPosition->Position_x + Dimensions.Position_x;
    EffectivePosition.Position_y = ParentPosition->Position_y + Dimensions.Position_y;
    EffectivePosition.Position_z = ParentPosition->Position_z + Dimensions.Position_z;
    EffectivePosition.Width = Dimensions.Width;
    EffectivePosition.Height= Dimensions.Height;
    EffectivePosition.Depth = Dimensions.Depth;

    if (this->Flag_Draging) { EffectivePosition.Position_x = DragAbsolutePosition.x; EffectivePosition.Position_y = DragAbsolutePosition.y; }

    // Render this frame

    if (Flag_Show_Frame)
    {

      P1.x = EffectivePosition.Position_x; P1.y = EffectivePosition.Position_y; P1.z = EffectivePosition.Position_z;
      P2.x = EffectivePosition.Position_x + EffectivePosition.Width ; P2.y = EffectivePosition.Position_y ; P2.z = EffectivePosition.Position_z + EffectivePosition.Depth;
      P3.x = EffectivePosition.Position_x + EffectivePosition.Width ; P3.y = EffectivePosition.Position_y + EffectivePosition.Height; P3.z = EffectivePosition.Position_z + EffectivePosition.Depth;
      P4.x = EffectivePosition.Position_x; P4.y = EffectivePosition.Position_y + EffectivePosition.Height; P4.z = EffectivePosition.Position_z + EffectivePosition.Depth;

      ULong TextureRef = GuiManager->TextureManager->GetTextureEntry(this->TextureNum)->OpenGl_TextureRef;
      glBindTexture(GL_TEXTURE_2D,TextureRef );
      glBegin(GL_TRIANGLES);
        glColor3f(DrawColor.r, DrawColor.v, DrawColor.b);
        glTexCoord2f(0.0,0.0); glVertex3f(P1.x, P1.y , P1.z);
        glTexCoord2f(1.0,0.0); glVertex3f(P2.x, P2.y , P2.z);
        glTexCoord2f(1.0,1.0); glVertex3f(P3.x, P3.y , P3.z);
        glTexCoord2f(1.0,1.0);glVertex3f(P3.x, P3.y , P3.z);
        glTexCoord2f(0.0,1.0);glVertex3f(P4.x, P4.y , P4.z);
        glTexCoord2f(0.0,0.0);glVertex3f(P1.x, P1.y , P1.z);
      glEnd();
    }


    // Render child frames

    if (Flag_Show_Childs)
    {
      //SubFrameList.Dump();
      Item = SubFrameList.GetFirst();
      while (Item)
      {
        Frame = (ZFrame *)Item->GetObject();
        if (Frame) Frame->Render(&EffectivePosition);

        Item = SubFrameList.GetNext(Item);
      }
    }

  }

}

Bool ZFrame::MouseMove ( Short Relative_x, Short Relative_y, UShort Absolute_x, UShort Absolute_y)
{
  ZListItem * Item;
  ZFrame * Frame;

  GuiManager->MouseOverStack_AddToStack(this);
  if ((Item = SubFrameList.GetFirst()))
  do
  {
    Frame = (ZFrame *)Item->GetObject();
    if (Absolute_x >= Frame->EffectivePosition.Position_x)
      if (Absolute_y >= Frame->EffectivePosition.Position_y)
        if (Absolute_x <= (Frame->EffectivePosition.Position_x + Frame->EffectivePosition.Width))
          if (Absolute_y <= (Frame->EffectivePosition.Position_y + Frame->EffectivePosition.Height))
          {
            if (!Frame->Event_MouseIsActualyOver)
            {
              if (Frame->Flag_Enable_Proc_MouseIn) Frame->MouseIn();
              Frame->Event_MouseWentIn = true;
            }
            Frame->Event_MouseIsActualyOver = true;
            if (Frame->Flag_Enable_Proc_MouseOver) Frame->MouseOver( ((UShort)Frame->EffectivePosition.Position_x) - Absolute_x, ((UShort)Frame->EffectivePosition.Position_y) - Absolute_y );

            Frame->MouseMove( Relative_x, Relative_y , Absolute_x, Absolute_y);
          }
  } while ((Item = SubFrameList.GetNext(Item)));
  return true;
}

Bool ZFrame::MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y)
{
  ZListItem * Item;
  ZFrame * Frame;


  GuiManager->FocusStack_AddToStack(this);

  // Drag and drop

  if (Flag_Cap_Dragable && nButton == 1 )
  {
    //ZVector2f Dim;
    //GetSize(Dim.x,Dim.y);
    Flag_Draging = true;
    GuiManager->DragingFrame = this;
    DragAbsolutePosition.x = EffectivePosition.Position_x;
    DragAbsolutePosition.y = EffectivePosition.Position_y;
  }

  // Focus

  if (this->Flag_Enable_Proc_GetFocus) GetFocus();

  // Subframe events propagation

  if ((Item = SubFrameList.GetFirst()))
  do
  {
    Frame = (ZFrame *)Item->GetObject();
    if (Absolute_x >= Frame->EffectivePosition.Position_x)
     if (Absolute_y >= Frame->EffectivePosition.Position_y)
       if (Absolute_x <= (Frame->EffectivePosition.Position_x + Frame->EffectivePosition.Width))
         if (Absolute_y <= (Frame->EffectivePosition.Position_y + Frame->EffectivePosition.Height))
         {
           Frame->Event_MouseClick = true;
           Frame->MouseButtonClick( nButton, Absolute_x, Absolute_y);
         }

  } while ((Item = SubFrameList.GetNext(Item)));
  return true;
}

Bool ZFrame::MouseButtonRelease(UShort nButton, Short Absolute_x, Short Absolute_y)
{
  ZListItem * Item;
  ZFrame * Frame;

  // Drag and drop

  if (GuiManager->DragingFrame)
  {
    if (GuiManager->DragingFrame == this) {return(true);}
    DropItem(GuiManager->DragingFrame, nButton);
  }

  //

  if ((Item = SubFrameList.GetFirst()))
  do
  {
    Frame = (ZFrame *)Item->GetObject();
    if (Absolute_x >= Frame->EffectivePosition.Position_x)
     if (Absolute_y >= Frame->EffectivePosition.Position_y)
       if (Absolute_x <= (Frame->EffectivePosition.Position_x + Frame->EffectivePosition.Width))
         if (Absolute_y <= (Frame->EffectivePosition.Position_y + Frame->EffectivePosition.Height))
         {
           Frame->Event_MouseRelease = true;
           Frame->MouseButtonRelease( nButton, Absolute_x, Absolute_y);
         }

  } while ((Item = SubFrameList.GetNext(Item)));
  return true;
}




