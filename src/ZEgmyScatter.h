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
 * ZEgmyScatter.h
 *
 *  Created on: 27 sept. 2013
 *      Author: laurent
 */

#ifndef Z_ZEGMYSCATTER_H
#define Z_ZEGMYSCATTER_H

#ifndef Z_ZEGMYSCATTER_H
#  include "ZEgmyScatter.h"
#endif

#ifndef Z_ZWORLD_H
#  include "ZWorld.h"
#endif

#ifndef Z_ZFASTRANDOM_H
#  include "ZFastRandom.h"
#endif

#ifndef Z_ZVOXELEXTENSION_EGMY_T1_H
#  include "ZVoxelExtension_Egmy_T1.h"
#endif

class ZEgmyScatter
{
  protected:
    ZVoxelWorld * World;
    ZLightSpeedRandom Random;

    // Egmys per wave
    ULong Count_T1;

  public:

    ZEgmyScatter()
    {
      World = 0;
      Count_T1 = 100;
      ResetWave();
    }
    void ScatterEgmys_T1(ZVoxelSector * Sector)
    {
      ZVoxelExtension_Egmy_T1 * Ext;

      if (!Count_T1) return;
      if (Sector->Flag_Void_Regular && Random.GetNumber() < (429470515UL))
      {
        if (!Sector->GetCube(8,32,8))
        {
          Ext = new ZVoxelExtension_Egmy_T1;
          Sector->SetCube_WithExtension(8,32,8,213, (ZMemSize)Ext);
          Sector->Flag_IsActiveVoxels = true;
          Sector->Flag_IsModified = true;
          Count_T1--;
        }
      }
    }

    void ResetWave()
    {
      Count_T1 = 100;
    }




};



#endif /* Z_ZEGMYSCATTER_H */
