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

#ifndef Z_BMP_LOADER_H
#define Z_BMP_LOADER_H

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif


class ZBitmapImage_0
{
  public:
    ULong  Width;
    ULong  Height;
    ULong  BitsPerPixel;
    ULong  BytesPerPixel;

    unsigned char * BitmapMemory;
    ULong  BitmapMemorySize;

    ZBitmapImage_0();
    ~ZBitmapImage_0();

    bool CreateBitmap(ULong Width, ULong Height, ULong BitsPerPixel);

    void FlipY();
    bool LoadBMP( const char * FileName );
};

#endif
