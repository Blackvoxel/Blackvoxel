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
 * PolySpace.h
 *
 *  Created on: 13 déc. 2010
 *      Author: laurent
 */

#ifndef Z_POLYSPACE_H
#define Z_POLYSPACE_H

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

#include "stdio.h"

class ZPolySpace
{
  public:
  Long * Min;
  Long * Max;

  ULong Size;
  ULong ZeroPoint;
  Long Start, End;

  void Init()
  {
    ULong i;

    for (i=0;i<Size;i++)
    {
      Min[i] = 1000000;
      Max[i] = -1000000;
      Start = Size-1;
      End   = 0;
    }
  }

  ZPolySpace()
  {
	Size = 1024;
	ZeroPoint = Size / 2;
	Min = new Long[1024];
    Max   = new Long[1024];
    Start = ZeroPoint;
    End   = ZeroPoint;
  }

  ~ZPolySpace()
  {
    if (Min) delete Min;
    if (Max) delete Max;
  }

  void Line(Long Xs, Long Ys, Long Xe, Long Ye)
  {

    Long  Iterations,i;
    Long x,y,Xd, Yd, YFactor;
    float XFactor;

    // printf("Poly: %ld,%ld - %ld,%ld\n",Xs,Ys,Xe,Ye);

    Xd = (Xe - Xs);
    Yd = (Ye - Ys);
    if (Yd < 0) Iterations = -Yd;
    else        Iterations = Yd;
    XFactor = ((float)Xd) / Iterations;
    if (Iterations) YFactor = Yd / Iterations;

    x= Xs;
    y = Ys+ZeroPoint;
    if (x < Min[y]) Min[y] = x;
    if (x > Max[y]) Max[y] = x;

    x= Xe;
    y = Ye+ZeroPoint;
    if (x < Min[y]) Min[y] = x;
    if (x > Max[y]) Max[y] = x;

    for (i=0;i<Iterations;i++)
    {
      x = (Xs + (Long)(XFactor * i + 0.5));
      y = Ys + (YFactor * i) + ZeroPoint;

      if (y<Start) Start = y;
      if (y>End)   End = y;
      if (x < Min[y]) Min[y] = x;
      if (x > Max[y]) Max[y] = x;

    }
  }



};






class ZPolySpace2
{
  public:
  Long * Min1;
  Long * Max1;
  Long * Min2;
  Long * Max2;

  ULong Size;
  ULong ZeroPoint;
  Long Start, End;

  void Init()
  {
    ULong i;

    for (i=0;i<Size;i++)
    {
      Min1[i] = 1000000;
      Max1[i] = -1000000;
      Min2[i] = 1000000;
      Max2[i] = -1000000;
      Start = Size-1;
      End   = 0;
    }
  }

  ZPolySpace2()
  {
	Size = 1024;
	ZeroPoint = Size / 2;
	Min1 = new Long[1024];
    Max1   = new Long[1024];
	Min2 = new Long[1024];
    Max2   = new Long[1024];
    Start = ZeroPoint;
    End   = ZeroPoint;
  }

  ~ZPolySpace2()
  {
    if (Min1) delete Min1;
	if (Max1) delete Max1;
	if (Min2) delete Min2;
	if (Max2) delete Max2;
  }

  void Line(Long Xs, Long Ys, Long Zs, Long Xe, Long Ye, Long Ze)
  {

    Long  Iterations,i;
    Long x,y,z, Xd, Yd,Zd, YFactor;
    float XFactor,ZFactor;

    // printf("Poly: %ld,%ld,%ld - %ld,%ld,%ld\n",Xs,Ys,Zs,Xe,Ye,Ze);

    Xd = (Xe - Xs);
    Yd = (Ye - Ys);
    Zd = (Ze - Zs);
    if (Yd < 0) Iterations = -Yd;
    else        Iterations = Yd;

    XFactor = ((float)Xd) / Iterations;
    ZFactor = ((float)Zd) / Iterations;
    if (Iterations) YFactor = Yd / Iterations;


//
    x= Xs;
    z = Zs;
    y = Ys+ZeroPoint;
    if (x < Min1[y]) Min1[y] = x;
    if (x > Max1[y]) Max1[y] = x;
    if (z < Min2[y]) Min2[y] = z;
    if (z > Max2[y]) Max2[y] = z;

    x= Xe;
    z= Ze;
    y = Ye+ZeroPoint;
    if (x < Min1[y]) Min1[y] = x;
    if (x > Max1[y]) Max1[y] = x;
    if (z < Min2[y]) Min2[y] = z;
    if (z > Max2[y]) Max2[y] = z;
//

    for (i=0;i<Iterations;i++)
    {
      x = (Xs + (Long)(XFactor * i + 0.5));
      z = (Zs + (Long)(ZFactor * i + 0.5));
      y = Ys + (YFactor * i) + ZeroPoint;

      if (y<Start) Start = y;
      if (y>End)   End = y;
      if (x < Min1[y]) Min1[y] = x;
      if (x > Max1[y]) Max1[y] = x;
      if (z < Min2[y]) Min2[y] = z;
      if (z > Max2[y]) Max2[y] = z;

    }
  }



};



#endif
