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
 * Z3DBlit.h
 *
 *  Created on: 2 févr. 2013
 *      Author: laurent
 */

#ifndef Z_Z3DBLIT_H
#define Z_Z3DBLIT_H

//#ifndef Z_Z3DBLIT_H
//#  include "Z3DBlit.h"
//#endif

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

// UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE
// UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE
// UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE
// UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE
// UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE UNUSED FILE

class ZVoxelMap
{
  ZVector3L Size;
  ZVector3L HandlePoint;

  ULong    TableSize;
  UShort * VoxelTable;

  ZVoxelMap()
  {
    VoxelTable = 0;
    Size = 0;
    HandlePoint = 0;
    TableSize = 0;
  }

  ~ZVoxelMap()
  {
    if (VoxelTable) delete[] VoxelTable;
  }

  void SetSize(Long x, Long y, Long z)
  {

    Size.x = x;
    Size.y = y;
    Size.z = z;

    if (VoxelTable) delete [] VoxelTable;
    TableSize = (ULong)( Size.x * Size.y * Size.z );
    VoxelTable = new UShort[TableSize];
  }

  void inline SetVoxel( Long x, Long y, Long z, UShort VoxelType)
  {
    ULong Offset;

    if (y >= Size.y || x >= Size.x || z >= Size.z) return;
    Offset = y + (x * Size.y) + (z * Size.x * Size.y);

    VoxelTable[Offset] = VoxelType;
  }

  UShort inline GetVoxel(Long x, Long y, Long z)
  {
    ULong Offset;

    if (y >= Size.y || x >= Size.x || z >= Size.z) return(0);

    Offset = y + (x * Size.y) + (z * Size.x * Size.y);

    // Optionnal bound checking
    #if COMPILEOPTION_BOUNDCHECKING
    if (Offset >= TableSize) throw;
    #endif

    return( VoxelTable[Offset] );
  }

  void SetHandlePoint(Long x, Long y, Long z)
  {
    HandlePoint.x = x;
    HandlePoint.y = y;
    HandlePoint.z = z;
  }

};

class Z3DBlitter
{


};



#endif /* Z_Z3DBLIT_H */
