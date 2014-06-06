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
 * ZVoxelType_Void.h
 *
 *  Created on: 17 nov. 2013
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPE_VOID_H
#define Z_ZVOXELTYPE_VOID_H

//#ifndef Z_ZVOXELTYPE_VOID_H
//#  include "ZVoxelType_Void.h"
//#endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

class ZVoxelType_Void : public ZVoxelType
{
  public:
    ZVoxelType_Void(ULong VoxelType) : ZVoxelType(VoxelType)
    {
      VoxelType = 0;
      Is_PlayerCanPassThrough = true;
      Draw_TransparentRendering = false;
      Draw_FullVoxelOpacity = false;
      DrawInfo = ZVOXEL_DRAWINFO_VOID;
      Is_Harming = false;
      FrictionCoef = 0.0001;
      Grip_Vertical = 0.0;
      Grip_Horizontal = 0.8;
      Is_SpaceGripType = false;
      Is_KeepControlOnJumping = true;
      Is_Active = false;
      Is_CanBeReplacedBy_Water = true;
      Is_CombinableWith_GreenAcid = false;
      Is_CanBeReplacedBy_GreenAcid = true;
      Is_CanBeReplacedBy_MustardGaz = true;
      BvProp_CanBePickedUpByRobot = false;
      BvProp_XrRobotPickMinLevel = 255;
      BvProp_PrRobotReplaceMinLevel = 0;
      BvProp_PrRobotPickMinLevel = 255;
      BvProp_PrRobotMoveMinLevel = 0;
      BvProp_AtomicFireResistant = true;
      Is_Liquid = false;
      Is_Gaz = true;
      Is_Loadable_ByLoader_L1 = false;
      BvProp_MoveableByTreadmill = false;
      BvProp_EgmyT1Resistant = false;
      LiquidDensity = 0.0;
      VoxelTypeName = "VOID";
    }



};


#endif /* Z_ZVOXELTYPE_VOID_H */
