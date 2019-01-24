/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2018 Laurent Thiebaut & Olivia Merle
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
 * ZGameInfo.h
 *
 *  Created on: 25 sept. 2018
 *      Author: laurent
 */

#ifndef Z_ZGAMEINFO_H
#define Z_ZGAMEINFO_H

//#ifndef Z_ZGAMEINFO_H
//#  include "ZGameInfo.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

class ZString;


class ZGameInfo
{
  public:
  ULong GameType;
  ULong RobotNextSerial;
  ULong Previous_GameVersion;

  ZGameInfo();
  bool Save(char * FileSpec);
  bool Load(char * FileSpec);

  static ULong GetGameType(ULong UniverseNum);
  static bool  GetWorldCustomDescription(ULong UniverseNum, ZString &OutDescription);

};



#endif /* Z_ZGAMEINFO_H */
