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
 * ZLinearInterpolator.h
 *
 *  Created on: 13 févr. 2012
 *      Author: laurent
 */

#ifndef Z_ZLINEARINTERPOLATOR_H
#define Z_ZLINEARINTERPOLATOR_H

// #ifndef Z_ZLINEARINTERPOLATOR_H
// #  include "ZLinearInterpolator.h"
// #endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

class ZLinearInterpolator
{
  protected:
    double * ValuesTable;
    ULong  nValues;
    double Start, End;
    double DeltaVal,Factor;

  public:
    ZLinearInterpolator() { ValuesTable = 0; nValues = 0; Start = 0.0; End = 0.0; }
    ZLinearInterpolator( double & ValuesTable, ULong ValuesCount ) { this->ValuesTable = & ValuesTable; nValues = ValuesCount; Start = 0.0; End = 0.0;  }
    ZLinearInterpolator( double * ValuesTable, ULong ValuesCount)  { this->ValuesTable = ValuesTable;   nValues = ValuesCount; Start = 0.0; End = 0.0; }
    inline void SetValuesTable( double & ValuesTable, ULong ValuesCount ) { this->ValuesTable = & ValuesTable; nValues = ValuesCount; }
    inline void SetValuesTable( double * ValuesTable, ULong ValuesCount)  { this->ValuesTable = ValuesTable;   nValues = ValuesCount; }

    inline void SetBounds(double Start, double End)
    {
      this->Start = Start;
      this->End = End;
      DeltaVal = End-Start;
      Factor = ((double)(nValues-1))/DeltaVal;
    }

    inline double Interpolate(double Position)
    {
      double TablePos, FracPos;

      ULong P1,P2;
      double Value1, Value2, FinalValue;

      if (Position < Start) Position = Start;
      if (Position > End )  Position = End;

      TablePos = (Position-Start) * Factor;

      P1 = floor(TablePos);
      P2 = ceil(TablePos);
      FracPos = TablePos - (double)(ULong)(TablePos);
      if (P2>=nValues) P2 = nValues-1;

      Value1 = ValuesTable[P1];
      Value2 = ValuesTable[P2];

      FinalValue = ( Value1 * (1.0 - FracPos) ) + ( Value2 * FracPos );

      return(FinalValue);
    }


};


#endif /* Z_ZLINEARINTERPOLATOR_H */
