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
/*******************************************************************************
*                                                                              *
*                         GNU GPL V3 Free Software                             *
*                                                                              *
*  (C) 1997-2011 Laurent Thiebaut & Olivia Merle                               *
*                                                                              *
*  This file is part of EXecutePage.                                           *
*                                                                              *
*  EXecutePage is free software: you can redistribute it and/or modify         *
*  it under the terms of the GNU General Public License as published by        *
*  the Free Software Foundation, either version 3 of the License, or           *
*  (at your option) any later version.                                         *
*                                                                              *
*  NodeKiller is distributed in the hope that it will be useful,               *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of              *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
*  GNU General Public License for more details.                                *
*                                                                              *
*  You should have received a copy of the GNU General Public License           *
*  along with EXecutePage.  If not, see <http://www.gnu.org/licenses/>.        *
*                                                                              *
*******************************************************************************/

#ifndef Z_BITMAP_IMAGE_H
#define Z_BITMAP_IMAGE_H

// #ifndef Z_BITMAP_IMAGE_H
// #  include "ZBitmapImage.h"
// #endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

#ifndef Z_ZGENERAL_OBJECT_H
#  include "ZGeneralObject.h"
#endif

#include <stdio.h>

class ZBitmapImage : public ZGeneralObject
{
  public:
    ULong Width;
    ULong Height;
    ULong BitsPerPixel;
    ULong BytesPerPixel;
    
    unsigned char * BitmapMemory;
    ZMemSize        BitmapMemorySize;
    
    ZBitmapImage();
    ~ZBitmapImage();
    
    void FlipY();
    bool CreateBitmap(ULong Width, ULong Height, ULong BitsPerPixel);
    

    void SwapBitmap( ZBitmapImage * OtherImage )
    {
      ULong Tmp;
      ZMemSize MTmp;
      unsigned char * PTmp;

      Tmp = OtherImage->Width;  OtherImage->Width  = Width;   Width = Tmp;
      Tmp = OtherImage->Height; OtherImage->Height = Height;  Height = Tmp;
      Tmp = OtherImage->BitsPerPixel;  OtherImage->BitsPerPixel  = BitsPerPixel;   BitsPerPixel  = Tmp;
      Tmp = OtherImage->BytesPerPixel;  OtherImage->BytesPerPixel  = BytesPerPixel;   BytesPerPixel  = Tmp;

      MTmp = OtherImage->BitmapMemorySize;  OtherImage->BitmapMemorySize  = BitmapMemorySize;   BitmapMemorySize  = MTmp;
      PTmp = OtherImage->BitmapMemory;  OtherImage->BitmapMemory  = BitmapMemory;   BitmapMemory  = PTmp;
    }

    void inline GetPixel( ULong x, ULong y, UByte &R, UByte &G, UByte &B, UByte &A)
    {
      ULong Offset;

      if (x>=Width || y>= Height) {R=G=B=A=0; return;}
      if (BytesPerPixel != 4) return;
      Offset = (x + (Width * y)) * BytesPerPixel;

      R = (BitmapMemory + Offset)[0];
      G = (BitmapMemory + Offset)[1];
      B = (BitmapMemory + Offset)[2];
      A = (BitmapMemory + Offset)[3];
    }

    ULong inline GetPixel( ULong x, ULong y)
    {
      ULong Offset;

      if (x>=Width || y>= Height) {return(0);}
      if (BytesPerPixel != 4) return(0);
      Offset = (x + (Width * y)) * BytesPerPixel;

      return( *(reinterpret_cast<ULong *>(BitmapMemory + Offset)) );

    }

    void inline SetPixel( ULong x, ULong y, UByte R, UByte G, UByte B, UByte A)
    {
      ULong Offset;

      if (x>=Width || y>= Height) {return;}
      if (BytesPerPixel != 4) return;
      Offset = (x + (Width * y)) * BytesPerPixel;

      (BitmapMemory + Offset)[0] = R;
      (BitmapMemory + Offset)[1] = G;
      (BitmapMemory + Offset)[2] = B;
      (BitmapMemory + Offset)[3] = A;
    }

    void ReduceSize()
    {
      ULong NewWidth, NewHeight, xs, ys , xd,yd;
      ZBitmapImage NewImage;
      UByte R[4],G[4],B[4],A[4];
      ULong Rd,Gd,Bd,Ad;

      NewWidth = Width >> 1;
      NewHeight = Height >> 1;

      NewImage.CreateBitmap(NewWidth, NewHeight, this->BitsPerPixel);

      for (yd=0, ys=0  ; yd < NewHeight; yd++, ys+=2)
      {
        for (xd=0, xs=0; xd < NewWidth;  xd++, xs+=2)
        {
          GetPixel( xs + 0 ,ys + 0, R[0], G[0], B[0], A[0] );
          GetPixel( xs + 1 ,ys + 0, R[1], G[1], B[1], A[1] );
          GetPixel( xs + 0 ,ys + 1, R[2], G[2], B[2], A[2] );
          GetPixel( xs + 1 ,ys + 1, R[3], G[3], B[3], A[3] );

          Rd = (R[0]+R[1]+R[2]+R[3]) >> 2;
          Gd = (G[0]+G[1]+G[2]+G[3]) >> 2;
          Bd = (B[0]+B[1]+B[2]+B[3]) >> 2;
          Ad = (A[0]+A[1]+A[2]+A[3]) >> 2;
          // Rd = xd;
          // Ad = 255;
          //GetPixel( xs ,ys, Rd, Gd, Bd, Ad );
          NewImage.SetPixel(xd,yd,Rd,Gd,Bd,Ad);
        }
      }
      SwapBitmap(&NewImage);

    }

    UByte inline GetBitMaskOffset(ULong BitMask)
    {
      ULong i;

      for (i=0;i < 32; i++)
      {
        if (BitMask & 1) return(i);
        BitMask >>= 1;
      }
      return(i);
    }



    bool LoadBMP( const char * FileName );
};

#endif
