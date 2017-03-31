/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2017 Laurent Thiebaut & Olivia Merle
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
 * ZGui_VoxelQuantityBox.cpp
 *
 *  Created on: 27 déc. 2016
 *      Author: laurent
 */

#include "ZGui_VoxelQuantityBox.h"
#include <GL/glew.h>
#include "SDL/SDL.h"

Bool ZVoxelQuantityBox::MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y)
{
  if (nButton==2)
  {
    *VoxelType = 0;
    *Quantity = 0;
  }

  return(ZInventoryBox::MouseButtonClick(nButton, Absolute_x, Absolute_y));
}

void ZVoxelQuantityBox::DropItem(ZFrame * Item, UShort nButton)
{
  if (   Item->FrameType == MulticharConst('I','B','O','X')
      || Item->FrameType == MulticharConst('Q','B','O','X'))
  {
    ZInventoryBox * IbItem;
    UShort * In_VoxelType, Temp_VoxelType;
    ULong  * In_Quantity,  Temp_Quantity;
    IbItem = (ZInventoryBox *) Item;

    In_VoxelType = IbItem->GetVoxelType();
    In_Quantity = IbItem->GetQuantity();

    if (*In_VoxelType != *VoxelType && *Quantity > 0)
    {
        if (nButton == 1)
        {
          Temp_VoxelType = *In_VoxelType;
          Temp_Quantity  = *In_Quantity;
          if (Item->FrameType == MulticharConst('Q','B','O','X'))
          {
            *In_VoxelType = *VoxelType;
            *In_Quantity  = *Quantity;
          }
          *VoxelType = Temp_VoxelType;
          *Quantity  = Temp_Quantity;
        }
    }

    else if (*In_VoxelType == *VoxelType || *Quantity == 0)
    {
      if (nButton == 1)
      {
        *Quantity += *In_Quantity;
        *VoxelType = *In_VoxelType;
        if (Item->FrameType == MulticharConst('Q','B','O','X'))
        {
          *In_Quantity = 0;
          *In_VoxelType = 0;
        }
      }
      if (nButton == 3)
      {
        ULong TransfertQuantity = 1;
        if (GuiManager->EventManager->Keyboard_Matrix[SDLK_LSHIFT] ||
            GuiManager->EventManager->Keyboard_Matrix[SDLK_RSHIFT]    )  TransfertQuantity = 10;
        if (GuiManager->EventManager->Keyboard_Matrix[SDLK_LCTRL]  ||
            GuiManager->EventManager->Keyboard_Matrix[SDLK_RCTRL]     ) TransfertQuantity = 100;
        if (GuiManager->EventManager->Keyboard_Matrix[SDLK_LALT]   ||
            GuiManager->EventManager->Keyboard_Matrix[SDLK_RALT]      ) TransfertQuantity = 1000;
        if (*In_Quantity>0)
        {
          (*Quantity)+= TransfertQuantity;
          *VoxelType = *In_VoxelType;
        }
      }
    }

  }
  // printf("Dropped : %lx in %lx\n",(ULong)Item, (ULong)this);
}



