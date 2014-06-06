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

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZSTREAM_SPECIALRAMSTREAM_H
#  include "z/ZStream_SpecialRamStream.h"
#endif

class ZGame;

class ZVoxelExtension
{
  public:
  ULong ExtensionType;



  enum ExtensionTypes { Extension_None = 0,Extension_Storage=1, Extension_UserTextureTransformer = 2, Extension_PlaneZ1 = 3, Extension_TransformationMachine = 4,
                        Extension_Programmable = 5, Extension_FusionElement = 6, Extension_BlastFurnace = 7, Extension_MiningRobot_xr1 = 8,
                        Extension_Sequencer = 9, Extension_Egmy_T1 = 10 };

  virtual ULong GetExtensionID() {return(0);}
  virtual bool Save(ZStream_SpecialRamStream * Stream)=0;
  virtual bool Load(ZStream_SpecialRamStream * Stream)=0;
  virtual void SetGameEnv(ZGame * GameEnv) { }
  virtual ZVoxelExtension * GetNewCopy() { return(0); }

  virtual ~ZVoxelExtension() {}
};



#endif /* Z_ZVOXELEXTENSION_H */
