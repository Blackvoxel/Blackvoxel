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
 * ZTools.cpp
 *
 *  Created on: 12 juil. 2011
 *      Author: laurent
 */

#include "ZTools.h"
#include "ZActorPhysics.h"
#include "ZGame.h"

void ZToolManager::ProcessAndDisplay()
{
  ZActor * Actor;

  ZInventory::Entry * ToolSlot;

  ZTool * Tool;
  UShort NewToolType;

  Actor = GameEnv->PhysicEngine->GetSelectedActor();

  ToolSlot = Actor->Inventory->GetActualToolSlot();
  NewToolType = ToolSlot->VoxelType;

  if ((ULong)NewToolType != this->ActualTool_VoxelType)
  {
    // Stop the actual tool service.
    if (ActualTool_VoxelType != (ULong)-1)
    {
      Tool = GameEnv->ToolManager->GetTool(ActualTool_VoxelType);
      if (Tool) Tool->End_Tool();
    }

    // Start the new tool service.

    Tool = GameEnv->ToolManager->GetTool(NewToolType);
    if (Tool) Tool->Start_Tool();

    // Make the tool the actual tool

    ActualTool_VoxelType = NewToolType;
  }

  Tool = GameEnv->ToolManager->GetTool(NewToolType);
  if (Tool) Tool->Display();


}
