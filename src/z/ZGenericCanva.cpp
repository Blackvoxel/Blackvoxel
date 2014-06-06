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
 * ZGenericCanva.cpp
 *
 *  Created on: 25 févr. 2012
 *      Author: laurent
 */

#include "ZGenericCanva.h"
#include <stdio.h>
#include <math.h>

void ZGenericByteCanva::DebugDump()
{
  Long x,y;
  UByte c;

  for (x=0;x<Width;x++) putchar('-');
  for (y=0;y<Height;y++)
  {
    for (x=0;x<Width;x++)
    {
      c = Canva[x+(y*Width)];
      if (c>=0 && c<=9) c+=48;
      else c='.';
      putchar(c);

    }
    printf("\n");

  }
}

void ZGenericByteCanva::DrawCircleFilled(double x, double y, double radius, UByte Color)
{
  double IntRadius, px,py;
  radius = fabs(radius);
  IntRadius = ceil(radius);

  for (py=-IntRadius;py<=IntRadius;py++)
    for (px=-IntRadius;px<=IntRadius;px++)
    {
      if (sqrt( (px*px + py*py)) <=radius) SetPoint_Secure(((Long)(px+0.5))+x,((Long)(py+0.5))+y,Color);
    }
}
