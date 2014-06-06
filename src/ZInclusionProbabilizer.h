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
 * ZInclusionProbabilizer.h
 *
 *  Created on: 9 févr. 2013
 *      Author: laurent
 */

#ifndef Z_ZINCLUSIONPROBABILIZER_H
#define Z_ZINCLUSIONPROBABILIZER_H

//#ifndef Z_ZINCLUSIONPROBABILIZER_H
//#  include "ZInclusionProbabilizer.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif


class ZInclusionProbabilizer
{
  protected:
    enum {MaxEntryCount = 32, MaxValue = (4294967295) };
    struct EntryInfo
    {
      UShort VoxelType;
      UShort RepeatChance;
      double Percent;
    };

    struct Bnd
    {
      ULong Low;
      ULong High;
    };

    ULong     EntryCount;
    Bnd     Bounds[MaxEntryCount];
    EntryInfo Infos[MaxEntryCount];
    double    CombinedPercent;

  public:

    void Clear()
    {
      EntryCount = 0;
      CombinedPercent = 0.0;
    }

    ZInclusionProbabilizer()
    {
      Clear();
    }

    void AddEntry(UShort VoxelType, UShort RepeatChance, double Percent)
    {
      if (EntryCount >= MaxEntryCount) while (true) {}

      Infos[EntryCount].VoxelType = VoxelType;
      Infos[EntryCount].RepeatChance = RepeatChance;
      Infos[EntryCount].Percent = Percent;
      CombinedPercent += Percent;
      EntryCount++;
    }

    ULong ComputeProbabilities(double MainInclusionCoef = 1.0)
    {
      ULong i;
      double PercentValue = ((double)MaxValue) / 100.0;
      ULong Bound = 0;
      double CombinedCoef = 100.0 / CombinedPercent;

      for (i=0;i<EntryCount;i++)
      {
        Bounds[i].Low = Bound;
        Bound += (ULong)(CombinedCoef * Infos[i].Percent * PercentValue);
        Bounds[i].High = Bound;
      }
      Bounds[0].Low = 0;
      Bounds[EntryCount-1].High = MaxValue;
      ULong FirstFenceBound =  (ULong)(PercentValue * CombinedPercent * MainInclusionCoef );

      return(FirstFenceBound);
    }

    inline ULong GetTypeNum(ULong RandomNumber)
    {
      ULong Pivot, Step;

      //RandomNumber &= 0x7fffffff;
      Pivot = EntryCount >> 1;
      Step = Pivot >> 1;

      while (true)
      {

        if      (RandomNumber < Bounds[Pivot].Low)  Pivot -= Step;
        else if (RandomNumber > Bounds[Pivot].High) Pivot += Step;
        else return(Pivot);
        Step = Step >> 1;
        if (!Step) Step = 1;

        // Bound checking enabled to track errors.
        #if COMPILEOPTION_BOUNDCHECKING==1
        if (Pivot >= EntryCount) throw;
        #endif
      }
      return(0);
    }

    inline UShort GetVoxelType(ULong EntryNum) { return( Infos[EntryNum].VoxelType ); }
    inline ULong  GetRepeatChance(ULong EntryNum) { return( Infos[EntryNum].RepeatChance ); }

    void PrintInfos() {}
};



#endif /* Z_ZINCLUSIONPROBABILIZER_H */
