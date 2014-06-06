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
 * ZWorldConvert.h
 *
 *  Created on: 21 juin 2011
 *      Author: laurent
 */

#ifndef Z_ZWORLDCONVERT_H
#define Z_ZWORLDCONVERT_H

//#ifndef Z_ZWORLDCONVERT_H
//#  include "ZWorldConvert.h"
//#endif

#ifndef Z_ZVOXELSECTOR_H
#  include "ZVoxelSector.h"
#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

class ZWorldConverter
{
    UShort * ConversionTable;

    void SectorConvert(ZVoxelSector * Sector);
  public:

    ZWorldConverter()
    {
      ConversionTable = new UShort[65536];
      InitTable();
    }

    ~ZWorldConverter()
    {
      if (ConversionTable) delete [] ConversionTable;
    }

    bool WorldConvert(char const * Path);
    void InitTable()
    {
      ULong i;

      for (i=0;i<65536;i++) ConversionTable[i] = (UShort)i;

    }

    void SetConversion(UShort OldNum, UShort NewNum)
    {
      ConversionTable[OldNum] = NewNum;
    }



};

#endif /* Z_ZWORLDCONVERT_H */
