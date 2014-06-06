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
 * ZFastRandom.h
 *
 *  Created on: 10 sept. 2011
 *      Author: laurent
 */

#ifndef Z_ZFASTRANDOM_H
#define Z_ZFASTRANDOM_H

// #ifndef Z_ZFASTRANDOM_H
// #  include "ZFastRandom.h"
// #endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

class ZRandom_LFSR
{
  protected:

    ULong State;

  public:

    ZRandom_LFSR()
    {
      State = 1;
    }

    void SetSeed(ULong Seed)
    {
      State = Seed;
    }

    ULong GetNumber()
    {
      ULong i;

      for(i=0;i<4;i++)
      {
        State = (State >> 1) ^ (-(State & 1ul) & 0xD0000001ul);
        State = (State >> 1) ^ (-(State & 1ul) & 0xD0000001ul);
        State = (State >> 1) ^ (-(State & 1ul) & 0xD0000001ul);
        State = (State >> 1) ^ (-(State & 1ul) & 0xD0000001ul);
        State = (State >> 1) ^ (-(State & 1ul) & 0xD0000001ul);
        State = (State >> 1) ^ (-(State & 1ul) & 0xD0000001ul);
        State = (State >> 1) ^ (-(State & 1ul) & 0xD0000001ul);
        State = (State >> 1) ^ (-(State & 1ul) & 0xD0000001ul);
      }
      return State;
    }

    ULong Get8Bits()
    {
      State = (State >> 1) ^ (-(State & 1ul) & 0xD0000001ul);
      State = (State >> 1) ^ (-(State & 1ul) & 0xD0000001ul);
      State = (State >> 1) ^ (-(State & 1ul) & 0xD0000001ul);
      State = (State >> 1) ^ (-(State & 1ul) & 0xD0000001ul);
      State = (State >> 1) ^ (-(State & 1ul) & 0xD0000001ul);
      State = (State >> 1) ^ (-(State & 1ul) & 0xD0000001ul);
      State = (State >> 1) ^ (-(State & 1ul) & 0xD0000001ul);
      State = (State >> 1) ^ (-(State & 1ul) & 0xD0000001ul);

      return( State >> 24 );
    }

    inline bool RandomProbability(ULong Seed, ULong Proba32)
    {
      ULong i;

      for(i=0;i<40;i++)
      {
        Seed = (Seed >> 1) ^ (-(Seed & 1ul) & 0xD0000001ul);
        Seed = (Seed >> 1) ^ (-(Seed & 1ul) & 0xD0000001ul);
        Seed = (Seed >> 1) ^ (-(Seed & 1ul) & 0xD0000001ul);
        Seed = (Seed >> 1) ^ (-(Seed & 1ul) & 0xD0000001ul);
        Seed = (Seed >> 1) ^ (-(Seed & 1ul) & 0xD0000001ul);
        Seed = (Seed >> 1) ^ (-(Seed & 1ul) & 0xD0000001ul);
        Seed = (Seed >> 1) ^ (-(Seed & 1ul) & 0xD0000001ul);
        Seed = (Seed >> 1) ^ (-(Seed & 1ul) & 0xD0000001ul);
      }
      if (Seed < Proba32) return(true);
      return(false);
    }

};


class ZFastRandom
{
  protected:

    ULong State;

  public:

    ZFastRandom()
    {
      State = 1;
    }

    void SetSeed(ULong Seed)
    {
      State = Seed;
    }

    inline ULong GetNumber()
    {
      State = (State * 16807) % 2147483647;
      return(State);
    }

    inline ULong GetNumber(ULong Seed)
    {
      return( (Seed * 16807) % 2147483647);
    }

    inline bool RandomProbability(ULong Seed, ULong Proba32)
    {

      if (Seed < Proba32) return(true);
      return(false);
    }
};

// Pooled random

#define ZLIGHTSPEEDRANDOM_POOLLEN 1024*1024
#define ZLIGHTSPEEDRANDOM_LENMASK 0xFFFFF

class ZLightSpeedRandom
{
  public:
    static ULong * Pool;
    static ULong PoolLen;

    ULong Position;

  public:

    ZLightSpeedRandom();
    ~ZLightSpeedRandom();

    inline ULong GetNumber()
    {
      return Pool[ Position++ & (ZLIGHTSPEEDRANDOM_POOLLEN - 1) ];
    }

    inline ULong GetNumber(ULong Seed)
    {
      return( Pool[Seed & (ZLIGHTSPEEDRANDOM_POOLLEN - 1)] );
    }

    inline bool RandomProbability(ULong Seed, ULong Proba32)
    {
      Seed &= (ZLIGHTSPEEDRANDOM_POOLLEN - 1);
      ULong RandomNum = Pool[Seed];
      bool ret = RandomNum < Proba32;
      return(ret);
      // return( Pool[ Seed & (ZLIGHTSPEEDRANDOM_POOLLEN - 1)] < Proba32 );
    }

    void Init(ULong Seed=0)
    {
      Position = Seed;
    }
};


#endif /* Z_ZFASTRANDOM_H */
