/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2021 Laurent Thiebaut & Olivia Merle
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
 * ZVoxel.h
 *
 *  Created on: 6 févr. 2021
 *      Author: laurent
 */

#ifndef Z_ZVOXEL_H
#define Z_ZVOXEL_H

// #ifndef Z_ZVOXEL_H
// #  include "ZVoxel.h"
// #endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZVOXELEXTENSION_H
#  include "ZVoxelExtension.h"
#endif

class ZVoxelTypeManager;
class ZStream_SpecialRamStream;

struct ZVoxel
{
  static ZVoxelTypeManager * VoxelTypeManager;


  public:

    static void SetVoxelTypeManager(ZVoxelTypeManager * VoxelTypeManager) {ZVoxel::VoxelTypeManager = VoxelTypeManager;}

    ZVoxel()
    {
      VoxelType = 0;
      OtherInfos = 0;
      Temperature = 0;
    }

    void Clear();

    UShort            VoxelType;
    ZMemSize          OtherInfos;
    UShort            Temperature;

    bool Save(ZStream_SpecialRamStream * Stream, bool EmbedVersion=true);
    bool Load(ZStream_SpecialRamStream * Stream, ULong & FormatVersion);

  ~ZVoxel();
};



#endif /* Z_ZVOXEL_H */
