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
 * ZSectorStreamLoader.h
 *
 *  Created on: 8 mars 2011
 *      Author: laurent
 */

#ifndef Z_ZSECTORSTREAMLOADER_H
#define Z_ZSECTORSTREAMLOADER_H

//#ifndef Z_ZSECTORSTREAMLOADER_H
//#  include "ZSectorStreamLoader.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZOBJECT_H
#  include "z/ZObject.h"
#endif

#ifndef Z_ZOBJECT_LISTS_H
#  include "z/ZObject_Lists.h"
#endif

#ifndef Z_ZMEMPOOL_H
#  include "ZMemPool.h"
#endif

#ifndef Z_ZVOXELSECTOR_H
#  include "ZVoxelSector.h"
#endif

#ifndef Z_ZSECTORLOADER_H
#  include "ZSectorLoader.h"
#endif

#ifndef Z_ZVOXELTYPE_H
#  include "ZVoxelType.h"
#endif

#ifndef Z_ZVOXELTYPEMANAGER_H
#  include "ZVoxelTypeManager.h"
#endif


#ifndef Z_ZSECTORRINGLIST_H
#  include "ZSectorRingList.h"
#endif

#ifndef Z_ZSECTORREQUESTRINGLIST_H
#  include "ZSectorRequestRingList.h"
#endif

#ifndef Z_ZSECTORTAGHASH_H
#  include "ZSectorTagHash.h"
#endif

#ifndef _SDL_thread_h
#  include "SDL/SDL_thread.h"
#endif

#ifndef Z_ZWORLDGENESIS_H
#  include "ZWorldGenesis.h"
#endif

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif






class ZGame;




class ZFileSectorLoader : public ZSectorLoader
{
    static UByte STableX[];
    static UByte STableZ[];
    static UByte STableY[];

    static UShort OfTableY [];
    static UShort OfTableX [];
    static UShort OfTableZ [];

  protected:
    void * Thread;
    Bool   ThreadContinue;

    ZSectorRequestRingList RequestList[8];
    ZSectorTagHash      RequestTag;
    ZSectorRingList * ReadySectorList;
    ZSectorRingList * EjectedSectorList;
    ZSectorRingList * SectorRecycling;

    ZVoxelTypeManager * VoxelTypeManager;

    ZVoxelSector      * WorkingEmptySector;
    ZVoxelSector      * WorkingFullSector;

    ULong UniverseNum;

    bool                   LoadSector(Long x, Long y, Long z);

    ZWorldGenesis          SectorCreator;
  public:

    ZFileSectorLoader();
    virtual                ~ZFileSectorLoader();

    virtual bool           Init();
    virtual void           Cleanup();
    virtual bool           Is_EjectFileNotFull() { return(EjectedSectorList->IsNotFull()); }
    virtual void           Eject_Sector( ZVoxelSector * Sector);
    virtual void           Request_Sector(Long x, Long y, Long z, Long Priority);
    virtual ZVoxelSector * GetRequested();
    void                   SetVoxelTypeManager(ZVoxelTypeManager * VoxelTypeManager) { this->VoxelTypeManager = VoxelTypeManager; }
    void                   SetUniverseNum(ULong UniverseNum) { this->UniverseNum = UniverseNum; }

    bool MakeTasks();

    void LimitedUpdateFaceCulling(ZVoxelSector * Sector );
    void NoDrawFaceCulling(ZVoxelSector * Sector );

    static int thread_func(void * Data);

};

#endif /* ZSECTORSTREAMLOADER_H_ */
