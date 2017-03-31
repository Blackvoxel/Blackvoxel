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


#ifndef Z_ZTYPES_H
#define Z_ZTYPES_H

//#ifndef Z_ZTYPES_H
//#  include "ZTypes.h"
//#endif

#ifndef _MATH_H
#  include <math.h>
#endif

// OS CONFIGURATION

#ifdef __linux__
#  define ZENV_OS_LINUX
#elif _WIN32
#  define ZENV_OS_WINDOWS
#endif

#define MulticharConst(a,b,c,d) (a + (b << 8) + (c<<16) + (d<<24))

  enum {ZFALSE = false ,ZTRUE = true};
  enum {ZERROR = 0, ZSUCCESS =-1};

  typedef bool                   Bool    ;  // 8  bits unsigned
  typedef signed char            Byte    ;  // 8  bits signed
  typedef unsigned char          UByte   ;  // 8  bits unsigned
  typedef signed short           Short   ;  // 16 bits signed
  typedef unsigned short         UShort  ;  // 16 bits unsigned
  typedef signed int             Long    ;  // 32 bits signed
  typedef unsigned int           ULong   ;  // 32 bits unsigned
  typedef signed long long int   ELong   ;  // 64 bits signed
  typedef unsigned long long int UELong  ;  // 64 bits unsigned
  typedef unsigned long long int ZSize   ;  // Size of an object

  typedef unsigned long          ZMemSize;  // Data of pointer size
  typedef          long          ZSMemSize; // Data of pointer size (signed);

  typedef unsigned long          UNum; // Unsigned integer of the standard size on the platform : 32 or 64 bits.
  typedef long                   SNum;

  class ZVector3d;
  class ZVector3L;

  typedef struct { double x,y,z,w; } ZVector4d;
  typedef struct { double x,y; } ZVector2d;
  typedef struct { float  x,y,z; } ZVector3f;
  typedef struct { float  x,y; }   ZVector2f;
  typedef struct { Long   x,y; } ZVector2L;
  typedef struct { float Position_x, Position_y, Position_z, Width, Height, Depth; } ZBox3f;
  typedef struct { float  r,v,b;} ZColor3f;

  typedef struct { Long   sx,sy,sz, ex,ey,ez; } ZRect3L_2;
  typedef struct { double Start, End; } ZRect1d;
  typedef struct { ZVector2d Start, End; } ZLineCoords;

// Manual breakpoint

#ifndef __arm__
  #define MANUAL_BREAKPOINT  asm volatile ("int3;")
#else
  #define MANUAL_BREAKPOINT
#endif

  // User defined complex class types.

#ifndef Z_ZTYPE_ZVECTOR3D_H
#  include "ZType_ZVector3d.h"
#endif

#ifndef Z_ZTYPE_ZPOLAR3D_H
#  include "ZType_ZPolar3d.h"
#endif

#ifndef Z_TYPE_ZVECTOR3L_H
#  include "ZType_ZVector3L.h"
#endif

#ifndef Z_TYPE_ZVECTOR3B_H
#  include "ZType_ZVector3B.h"
#endif


  // User defined class cross functions and aftermath code needed to be declared after main classes.

#ifndef Z_ZTYPE_ZVECTOR3D_CROSSFUNC_H
#  include "ZType_ZVector3d_CrossFunc.h"
#endif

#ifndef Z_ZTYPE_ZPOLAR3D_CROSSFUNC_H
#  include "ZType_ZPolar3d_CrossFunc.h"
#endif

#ifndef Z_ZTYPE_ZVECTOR3L_CROSSFUNC_H
#  include "ZType_ZVector3L_CrossFunc.h"
#endif

typedef struct { ZVector3L Start, End; } ZRect3L;

class ZVoxelSector;
struct ZVoxelLocation
{
  ZVoxelSector * Sector;
  ULong          Offset;

};

class ZVoxelCoords
{
    public:
      Long x,y,z;

      bool operator != (const ZVoxelCoords & Other ) const
      {
        if (x!=Other.x) return(true);
        if (y!=Other.y) return(true);
        if (z!=Other.z) return(true);
        return(false);
      }

      void GetVoxelCenterCoords(ZVector3d * VoxelCoords)
      {
        ELong nx,ny,nz;

        nx = x; ny = y; nz = z;

        VoxelCoords->x = ((double)((nx << 8))) + 128.0;
        VoxelCoords->y = ((double)((ny << 8))) + 128.0;
        VoxelCoords->z = ((double)((nz << 8))) + 128.0;
      }

      inline void GetCo(ZVector3L * Coords) {Coords->x = x; Coords->y = y; Coords->z = z; }
};






#define ZNULL 0;






#endif
