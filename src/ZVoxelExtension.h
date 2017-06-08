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
 * ZVoxelExtension.h
 *
 *  Created on: 24 sept. 2011
 *      Author: laurent
 */

#ifndef Z_ZVOXELEXTENSION_H
#define Z_ZVOXELEXTENSION_H

//#ifndef Z_ZVOXELEXTENSION_H
//#  include "ZVoxelExtension.h"
//#endif

#include <stddef.h>

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZSTREAM_SPECIALRAMSTREAM_H
#  include "z/ZStream_SpecialRamStream.h"
#endif

#ifndef Z_ZMEMPOOL_OPTIMIZED_H
#  include "ZMemPool_Optimized.h"
#endif

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

class ZGame;
#include <stdio.h>
class ZVoxelExtension
{
  public:

    // Extension type

    ULong ExtensionType;

    // Extension type definition.

    enum ExtensionTypes { Extension_None = 0,Extension_Storage=1, Extension_UserTextureTransformer = 2, Extension_PlaneZ1 = 3, Extension_TransformationMachine = 4,
                          Extension_Programmable = 5, Extension_FusionElement = 6, Extension_BlastFurnace = 7, Extension_MiningRobot_xr1 = 8,
                          Extension_Sequencer = 9, Extension_Egmy_T1 = 10, Extension_GeneralModing = 11, Extension_WebRobot=12 };

  protected:

    bool _ThrowExtension(ZStream_SpecialRamStream * Stream, ZMemSize ExtensionSize);

  public:

    virtual ULong GetExtensionID() { return(MulticharConst('N','S','P','C'));}
    virtual bool Save(ZStream_SpecialRamStream * Stream)=0;
    virtual bool Load(ZStream_SpecialRamStream * Stream)=0;
    virtual void SetGameEnv(ZGame * GameEnv) { }
    virtual ZVoxelExtension * GetNewCopy() { return(0); }

    virtual ~ZVoxelExtension() {}

#if COMPILEOPTION_EXPERIMENTAL_POOLED_EXTENSIONS == 1
    inline void * operator new (size_t Size)
    {
      return(ZMemPool_Optimized::GetMainPublicPool()->AllocMem(Size));
    }

    inline void operator delete (void * Memory)
    {
      ZMemPool_Optimized::GetMainPublicPool()->FreeMem(Memory);
    }
#endif

};



#endif /* Z_ZVOXELEXTENSION_H */
