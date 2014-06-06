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
 * ZMemPool_Optimized.h
 *
 *  Created on: 17 nov. 2011
 *      Author: laurent
 */

#ifndef Z_ZMEMPOOL_OPTIMIZED_H
#define Z_ZMEMPOOL_OPTIMIZED_H

//#ifndef Z_ZMEMPOOL_OPTIMIZED_H
//#include "ZMemPool_Optimized.h"
//#endif

#ifndef Z_ZMEMPOOL_H
#  include "ZMemPool.h"
#endif

#define Z_ZMEMPOOL_OPTIMISED_POOLSIZE 64
#define Z_ZMEMPOOL_OPTIMISED_MINSIZE  128

class ZMemPool_Optimized : public ZMemoryPool
{
  protected:

    static bool Initialized;
    static char BitPosTable[65536];

    struct PoolNode
    {
      PoolNode * Next;
      ZMemSize   Size;
    };

    PoolNode * MemTable[Z_ZMEMPOOL_OPTIMISED_POOLSIZE];


  public:

    ZMemPool_Optimized()
    {
      ZMemSize i,Size,Index;

      for(i=0;i<Z_ZMEMPOOL_OPTIMISED_POOLSIZE;i++) MemTable[i] = 0;

      // Init the

      if (! Initialized)
      {
        for (i=0,Index = 0, Size=1;i<65536;i++)
        {
          if (i>Size) {Index++; Size+=Size;}
          BitPosTable[i] = Index;
        }
      }
    }
    virtual ~ZMemPool_Optimized() {}


    virtual void * AllocMem( const ZMemSize Size, ZMemSize & EffectiveSize);
    virtual void FreeMem ( void * Block, const ZMemSize BlockSize) { return(FreeMem(Block)); }

    virtual void * AllocMem( const ZMemSize Size );
    virtual void FreeMem( void * Block );
};


#endif /* Z_ZMEMPOOL_OPTIMIZED_H */
