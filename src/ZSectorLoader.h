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
 * ZSectorLoader.h
 *
 *  Created on: 16 mars 2011
 *      Author: laurent
 */

#ifndef Z_ZSECTORLOADER_H
#define Z_ZSECTORLOADER_H

// #ifndef Z_ZSECTORLOADER_H
// #  include "ZSectorLoader.h"
// #endif

class ZSectorLoader
{
  public:

    ZSectorLoader();
    virtual ~ZSectorLoader();

    virtual bool Init() = 0;
    virtual void Cleanup() = 0;
    virtual void Request_Sector(Long x, Long y, Long z, Long Priority) = 0;
    virtual bool Is_EjectFileNotFull() = 0;
    virtual void Eject_Sector( ZVoxelSector * Sector) = 0;
    virtual ZVoxelSector * GetRequested() = 0;
    virtual void SetWorldType(UShort WorldType) = 0;

};

#endif
