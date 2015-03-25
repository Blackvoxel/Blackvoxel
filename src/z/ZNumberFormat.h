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
 * ZNumberFormat.h
 *
 *  Created on: 15 janv. 2015
 *      Author: laurent
 */

#ifndef Z_ZNUMBERFORMAT_H
#define Z_ZNUMBERFORMAT_H

//#ifndef Z_ZNUMBERFORMAT_H
//#  include "ZNumberFormat.h"
//#endif

class ZNumberFormat
{
  public:
    UShort Base;
    bool   AlwaysDisplaySign;
    bool   DisplayTrailingZero;
    UShort MaxDigits;
    UShort MaxDecimals;

    ZNumberFormat()
    {
      Base = 10;
      AlwaysDisplaySign = false;
      DisplayTrailingZero = false;
      MaxDigits = (UShort)-1;
      MaxDecimals = (UShort)-1;
    }
};



#endif /* Z_ZNUMBERFORMAT_H */
