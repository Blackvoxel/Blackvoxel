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
 * ZSectorSphere.h
 *
 *  Created on: 25 oct. 2012
 *      Author: laurent
 */

#ifndef Z_ZSECTORSPHERE_H
#define Z_ZSECTORSPHERE_H

// #ifndef Z_ZSECTORSPHERE_H
// #include "ZSectorSphere.h"
// #endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZVOXELSECTOR_H
#  include "ZVoxelSector.h"
#endif

class ZSectorSphere
{
  public:
    struct ZSphereEntry
    {
      Long x,y,z;
      double SectorDistance;
    };

  protected:
    ZSphereEntry * SectorList;
    ULong nSlots;

    void PartSort(ULong Start, ULong ItemCount, ZSphereEntry * SortBuffer);
    void Sort();

  public:

    ZSectorSphere();
    ~ZSectorSphere();

    void Init(Long Render_Distance_h, Long Render_Distance_v);
    inline ULong GetEntryCount() {return nSlots;}
    inline ZSphereEntry * GetEntry(ULong EntryNum) { return (&SectorList[EntryNum]); }

    void debugout(char * FileSpec);

};



#endif /* Z_ZSECTORSPHERE_H */
