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
 * ZMemPool.h
 *
 *  Created on: 28 févr. 2011
 *      Author: laurent
 */

#ifndef Z_ZMEMPOOL_H
#define Z_ZMEMPOOL_H

// #ifndef Z_ZMEMPOOL_H
// #  include "ZMemPool.h"
// #endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

#define Z_ZBASICMEMORYPOOL_SIZE 256
#define Z_ZBASICMEMORYPOOL_MINMEMSIZE 64

// Interface for memory pool

class ZMemoryPool
{
  public:
    virtual ~ZMemoryPool() {}
    virtual void * AllocMem( const ZMemSize Size, ZMemSize & EffectiveSize) {return(0);}
    virtual void FreeMem ( void * Block, const ZMemSize BlockSize) {}

    virtual void * AllocMem( const ZMemSize Size ) {return(0);}
    virtual void FreeMem( void * Block ) {}
};

class ZTestMemoryPool : public ZMemoryPool
{
  public:
    virtual ~ZTestMemoryPool() {};
    virtual void * AllocMem( const ZMemSize Size, ZMemSize & EffectiveSize) { EffectiveSize = Size; return( new char[Size] ); }
    virtual void FreeMem ( void * Block, const ZMemSize BlockSize) { delete [] (char *)Block;}

    virtual void * AllocMem( const ZMemSize Size ) {return(new char[Size]);}
    virtual void FreeMem( void * Block ) { delete [] (char *)Block; }
};

// Basic implementation for memory pool

class ZBasicMemoryPool : public ZMemoryPool
{
  protected:

    struct PoolNode
    {
      PoolNode * Next;
    };

    PoolNode * MemTable[Z_ZBASICMEMORYPOOL_SIZE];

  public:

    ZBasicMemoryPool();
    virtual ~ZBasicMemoryPool();


    virtual void * AllocMem( const ZMemSize Size, ZMemSize & EffectiveSize);
    virtual void FreeMem ( void * Block, const ZMemSize BlockSize);

    virtual void * AllocMem( const ZMemSize Size ) { return(0); };
    virtual void FreeMem( void * Block )           { }
};


// A very fast memory pool. But limited to only ONE size of memory block per pool. LIMITED COMPATIBILITY.
//
// First Allocation set size permanently.
// No security. No checkings. No memory freeing before destruction of the pool. Speed is the ONLY priority.
//


class ZMonoSizeMemoryPool : public ZMemoryPool
{
  private:
    struct PoolNode
    {
      PoolNode * Next;
    };

    ZMemSize   MemorySize;
    PoolNode * MemoryPool;
    ZMemSize AllocSize;

    ULong AllocCount;
    ULong FreeCount;

  public:

    ZMonoSizeMemoryPool()
    {
      MemoryPool = 0;
      MemorySize = 0;
      AllocCount = 0;
      FreeCount  = 0;
      AllocSize  = 0;
    }

    virtual ~ZMonoSizeMemoryPool()
    {
      PoolNode * Node, * NewNode;

      Node = MemoryPool;

      for (Node = MemoryPool; Node ; Node = NewNode)
      {
        NewNode = Node->Next;
        delete [] Node;
      }

      MemoryPool = 0;
      MemorySize = 0;
    }

    virtual void * AllocMem( const ZMemSize Size, ZMemSize & EffectiveSize) { EffectiveSize = (MemorySize) ? MemorySize : Size; return (AllocMem(Size));}
    virtual void   FreeMem ( void * Block, const ZMemSize BlockSize)        { }

    virtual void * AllocMem( const ZMemSize Size )
    {
      PoolNode * Node;

      if (!MemorySize) MemorySize = Size;

      AllocCount++;

      if (MemoryPool)
      {
        Node = MemoryPool;
        MemoryPool = MemoryPool->Next;
        return(Node);
      }
      else
      {
        return( new UByte[MemorySize] );
      }
    }

    virtual void FreeMem( void * Block )
    {
      ((PoolNode *)Block)->Next = MemoryPool;
      MemoryPool = (PoolNode *)Block;
      FreeCount ++;
    }
};



#endif /* ZMEMPOOL_H_ */
