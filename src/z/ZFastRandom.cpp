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
 * ZFastRandom.cpp
 *
 *  Created on: 14 sept. 2011
 *      Author: laurent
 */

#include "ZFastRandom.h"
#include <stdio.h>

#ifndef A_COMPILESETTINGS_H
#  include "../ACompileSettings.h"
#endif

#ifndef Z_ZSTRING_H
#  include "ZString.h"
#endif

ULong * ZLightSpeedRandom::Pool = 0;
ULong ZLightSpeedRandom::PoolLen = 0;

ZLightSpeedRandom::ZLightSpeedRandom()
{
  if (Pool == 0)
  {
    FILE * fh;
    ZString FileName;

    Pool = new ULong[ZLIGHTSPEEDRANDOM_POOLLEN];
    FileName = COMPILEOPTION_DATAFILESPATH;
    FileName.AddToPath("randomnum.dat");
    fh = fopen(FileName.String,"rb"); if (!fh) { printf("Error loading random file\n"); throw;}
    if (fh)
    {
      if (ZLIGHTSPEEDRANDOM_POOLLEN!=fread(Pool,4,ZLIGHTSPEEDRANDOM_POOLLEN,fh)) { printf("Error loading random file\n"); throw; }
      fclose(fh);
    }
    PoolLen = ZLIGHTSPEEDRANDOM_POOLLEN;
  }
  Position = 0;
}

ZLightSpeedRandom::~ZLightSpeedRandom()
{
  /*
  if (Pool !=0)
  {
    delete [] Pool;
    Pool = 0;
    PoolLen = 0;
  }
  */
}
