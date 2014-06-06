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
 * ZWorldInfo.h
 *
 *  Created on: 4 nov. 2012
 *      Author: laurent
 */

#ifndef Z_WORLDINFO_H
#define Z_WORLDINFO_H

//#ifndef Z_WORLDINFO_H
//#  include "ZWorldInfo.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

#ifndef Z_STREAMS_H
#  include "z/ZStreams.h"
#endif

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

#ifndef Z_ZSTREAMS_FILE_H
#  include "z/ZStream_File.h"
#endif

class ZWorldInfo
{

  ULong Creator_GameVersion;
  ULong Compatibility_Class;

  public:

  ZWorldInfo()
  {
    Creator_GameVersion = COMPILEOPTION_VERSIONNUM;
    Compatibility_Class = 2;
  }

  bool Save(char * FileSpec)
  {
    ZStream_File Stream;

    Stream.SetFileName(FileSpec);

    if (!Stream.OpenWrite()) return(false);

    Stream<<(ULong)0xB14C08E1;
    Stream<<Creator_GameVersion;
    Stream<<Compatibility_Class;
    Stream.Close();

    return(true);
  }

  bool Load(char * FileSpec)
  {
    ZStream_File Stream;

    Stream.SetFileName(FileSpec);

    if (!Stream.OpenRead()) return(false);

    ULong MagicCookie;

    Stream>>MagicCookie;
    if (MagicCookie!=0xB14C08E1) { Stream.Close(); return(false); }
    Stream>>Creator_GameVersion;
    Stream>>Compatibility_Class;
    Stream.Close();

    return(true);
  }

};

#endif /* ZWORLDINFO_H_ */
