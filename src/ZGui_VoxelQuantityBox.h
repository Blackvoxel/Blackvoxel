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
 * ZGui_QuantityBox.h
 *
 *  Created on: 27 déc. 2016
 *      Author: laurent
 */

#ifndef Z_ZGUI_VOXELQUANTITYBOX_H
#define Z_ZGUI_VOXELQUANTITYBOX_H

//#ifndef Z_ZGUI_VOXELQUANTITYBOX_H
//#  include "ZGui_VoxelQuantityBox.h"
//#endif

#ifndef Z_ZGUI_INVENTORYBOX_H
#  include "ZGui_InventoryBox.h"
#endif

class ZVoxelQuantityBox : public ZInventoryBox
{

  public:
    ZVoxelQuantityBox ()
    {
      FrameType = MulticharConst('Q','B','O','X'); // = VoxelQuantityBox;
    }

    virtual Bool MouseButtonClick  (UShort nButton, Short Absolute_x, Short Absolute_y);
    virtual void DropItem(ZFrame * Item, UShort nButton);

};


#endif /* SRC_ZGUI_VOXELQUANTITYBOX_H_ */
