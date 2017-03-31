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
 * ZVoxelType_UniverseBorder.h
 *
 *  Created on: 18 déc. 2016
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPE_UNIVERSEBORDER_H
#define Z_ZVOXELTYPE_UNIVERSEBORDER_H

//#ifndef Z_ZVOXELTYPE_UNIVERSEBORDER_H
//#  include "ZVoxelType_UniverseBorder.h"
//#endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

class ZVoxelType_UniverseBorder : public ZVoxelType
{
  public:
    ZVoxelType_UniverseBorder(ULong VoxelType) : ZVoxelType(VoxelType)
    {
      this->VoxelType = VoxelType;
      Is_PlayerCanPassThrough = false;
      Draw_TransparentRendering = false;
      Draw_FullVoxelOpacity = false;
      DrawInfo = ZVOXEL_DRAWINFO_VOID;
      Is_Harming = false;
      FrictionCoef = 0.0001;
      Grip_Vertical = 0.0;
      Grip_Horizontal = 0.0;
      Is_SpaceGripType = false;
      Is_KeepControlOnJumping = true;
      Is_Active = false;
      Is_CanBeReplacedBy_Water = false;
      Is_CombinableWith_GreenAcid = false;
      Is_CanBeReplacedBy_GreenAcid = false;
      Is_CanBeReplacedBy_MustardGaz = false;
      BvProp_CanBePickedUpByRobot = false;
      BvProp_XrRobotPickMinLevel = 255;
      BvProp_PrRobotReplaceMinLevel = 0;
      BvProp_PrRobotPickMinLevel = 255;
      BvProp_PrRobotMoveMinLevel = 0;
      BvProp_AtomicFireResistant = true;
      Is_Liquid = false;
      Is_Gaz = false;
      Is_Loadable_ByLoader_L1 = false;
      BvProp_MoveableByTreadmill = false;
      BvProp_EgmyT1Resistant = false;
      LiquidDensity = 0.0;
      VoxelTypeName = "Universe Border";



      GameEnv = 0;
      VoxelTypeManager = 0;
      VoxelTypeName = "Universe Border";
      Draw_TransparentRendering = false;
      Draw_FullVoxelOpacity     = false;
      DrawInfo = ZVOXEL_DRAWINFO_DRAWFULLVOXELOPACITY;
      this->VoxelType = VoxelType;
      MainTexture = 0;
      OpenGl_TextureRef = 0;

      Is_NoType = false;
      Is_UserTypeTransformable = false;
      Is_PlayerCanPassThrough = false;
      Is_CanBeReplacedBy_Water = false;
      Is_CanBeReplacedBy_GreenAcid = false;
      Is_CanBeReplacedBy_MustardGaz = false;
      Is_CanTriggerBomb = false;
      Is_Pumpable_ByPump_T1 = false;
      Is_Pumpable_ByPump_T2 = false;
      Is_Loadable_ByLoader_L1 = false;
      BvProp_MoveableByTreadmill = false;
      BvProp_CanBePickedUpByRobot = false;
      BvProp_AtomicFireResistant  = true;
      BvProp_EgmyT1Resistant = true;
      BvProp_AccelerateOnFall = false;
      BvProp_XrRobotPickMinLevel = 255;
      BvProp_PrRobotReplaceMinLevel = 255;
      BvProp_PrRobotPickMinLevel = 255;
      BvProp_PrRobotMoveMinLevel = 255;
      BlastResistance = 0x7FFFFFFF;
      Is_SpaceGripType = false;
      Is_KeepControlOnJumping = false;

      Is_Harming = false;
      Is_Active  = false;
      Is_CombinableWith_GreenAcid = false;
      ExtensionType = 0;
      Is_VoxelExtension = false;
      Is_HasAllocatedMemoryExtension = false;

      Is_Interface_StoreBlock = false;
      Is_Interface_PushBlock = false;
      Is_Interface_PullBlock = false;
      Is_Interface_GetInfo = false;
      Is_Interface_SetInfo = false;

      MiningHardness = 1000.0;
      MiningType = 2;
      FrictionCoef = 1.0;
      HarmingLifePointsPerSecond = 0.0;
      LiquidDensity = 0.0;
      Grip_Horizontal = 0.9;
      Grip_Vertical = 0.0;
      Is_Liquid = false;
      Is_Gaz = false;


      Is_Power = false;
      Is_Rideable = false;
      Is_HasHelpingMessage = false;
      BvProp_FastMoving = false;
      FabInfos = 0;
      Documentation_PageNum = false;
    }



};



#endif /* Z_ZVOXELTYPE_UNIVERSEBORDER_H */
