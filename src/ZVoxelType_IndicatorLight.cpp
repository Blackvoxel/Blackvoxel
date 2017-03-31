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
 * ZVoxelType_IndicatorLight.cpp
 *
 *  Created on: 7 nov. 2013
 *      Author: laurent
 */

#ifndef Z_ZVOXELTYPE_INDICATORLIGHT_H
#  include "ZVoxelType_IndicatorLight.h"
#endif

#ifndef Z_ZGAME_H
#  include "ZGame.h"
#endif

#ifndef Z_ZWORLD_H
#  include "ZWorld.h"
#endif


bool ZVoxelType_IndicatorLight::Interface_GetInfo(ZVoxelLocation * VLoc, ULong InfoNum, ZVar * Out)
{
  switch(InfoNum)
  {
    case 0: Out->Set_Int(1); break;
    case 1: if (VoxelType >= 216 && VoxelType<=231) Out->Set_Int(VoxelType-216);
            break;
    default: return(false);
  }
  return(true);
}

bool  ZVoxelType_IndicatorLight::Interface_GetInfoDoc(ULong InfoNum, ULong DocType, ZVar * Out)
{
  if (InfoNum == 0) Out->Set_Int(1); return(true);
  switch(InfoNum)
  {
    case 1: switch(DocType) // Name
            {
              case 0: Out->Set_String("Light Indicator State and Color"); break; // Setting Name
              case 1: Out->Set_String("rw");                              break; // rw,r,w
              case 2: Out->Set_String("[int]");                           break; // Read returned type
              case 3: Out->Set_String("[bool][int]");                     break; // Write accepted type
            }
            break;
    default:return(false);
  }
  return(true);
}

bool ZVoxelType_IndicatorLight::Interface_SetInfo(ZVoxelLocation * VLoc, ULong InfoNum, ZVar * In)
{
  switch (InfoNum)
  {
    case 1:  {
               ZVector3L Coords;
               UShort NewVoxelValue = 216;
               ULong Info;
               switch (In->VarType)
               {
                 case ZVar::TYPE_BOOL: Info = In->Scalar.Bool ? 8 : 0; break;
                 case ZVar::TYPE_INT:  Info = In->Scalar.Int & 15; break;
                 default: return(false);
               }
               NewVoxelValue+=Info;
               ZVoxelWorld::Convert_Location_ToCoords(VLoc, &Coords); // Get the coordinates.
               GameEnv->World->SetVoxel_WithCullingUpdate(Coords.x, Coords.y, Coords.z, NewVoxelValue, ZVoxelSector::CHANGE_CRITICAL, true, 0 );
             }
             break;
    default: return(false);break;
  }
  return(true);
}

