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
#  include "bmploader.h"
#endif

#include "stdio.h"

    ZBitmapImage_0::ZBitmapImage_0()
    {
      Width = 0;
      Height = 0;
      BitsPerPixel = 0;
      BytesPerPixel = 0;
      BitmapMemory = 0;
      BitmapMemorySize = 0;
    }

    ZBitmapImage_0::~ZBitmapImage_0()
    {
      delete (BitmapMemory);
      Width = 0;
      Height = 0;
      BitsPerPixel = 0;
      BytesPerPixel = 0;
      BitmapMemory = 0;
      BitmapMemorySize = 0;
    }

    void ZBitmapImage_0::FlipY()
    {
      ULong BPLine,y,x,Offset1,Offset2;
      char c;

      if ( (this->Height == 0) || (this->Width == 0) ) return;
      BPLine = this->Width * this->BytesPerPixel;

      for (y=0;y<this->Height/2;y++)
      {
    	Offset1 = BPLine * y;
    	Offset2 = BPLine * ((this->Height-1) - y);
    	for (x=0;x<BPLine;x++) {c = BitmapMemory[x+Offset1]; BitmapMemory[x+Offset1] = BitmapMemory[x+Offset2]; BitmapMemory[x+Offset2] = c;}
      }
    }


    bool ZBitmapImage_0::CreateBitmap(ULong Width, ULong Height, ULong BitsPerPixel)
    {
      ULong Bytes, Size;

      switch(BitsPerPixel)
      {
        case 8:  Bytes = 1; break;
        case 16: Bytes = 2; break;
        case 24: Bytes = 3; break;
        case 32: Bytes = 4; break;
        default: return(false);
      }
      Size = Bytes * Width * Height;

      BitmapMemory = new unsigned char[Size];
      BitmapMemorySize = Size;
      BytesPerPixel = Bytes;
      this->Width  = Width;
      this->Height = Height;
      this->BitsPerPixel = BitsPerPixel;

      return(true);
    }

    bool ZBitmapImage_0::LoadBMP( const char * FileName )
    {
      FILE * fh;
      char Buffer[4096];
      UShort MagicNumber;
      ULong  BMPFileSize;
      ULong  SoftwareID;
      ULong  DataOffset;
      ULong  HeaderSize;
      ULong  ImageWidth;
      ULong  ImageHeight;
      UShort nBitPlanes;
      UShort nBitsPerPixel;
      ULong  CompressionMethod;
      ULong  ImageDataWidth;
      ULong  HorizontalResolution;
      ULong  VerticalResolution;
      ULong  nColorsInPalette;
      ULong  nColorsUsed;
      ULong i,j;

      if (! (fh = fopen(FileName, "rb"))) return(false);

      if ( 1 != fread(&MagicNumber, 2, 1, fh)) return(false);
      if ( MagicNumber != 0x4D42 ) {printf( "Not a BMP file\n" ); return(false);}

      if ( 1 != fread(&BMPFileSize, 4, 1, fh)) return(false);
      if ( 1 != fread(&SoftwareID, 4, 1, fh)) return(false);
      if ( 1 != fread(&DataOffset, 4, 1, fh)) return(false);

      if ( 1 != fread(&HeaderSize, 4, 1, fh)) return(false);
      if ( 1 != fread(&ImageWidth, 4, 1, fh)) return(false);
      if ( 1 != fread(&ImageHeight, 4, 1, fh)) return(false);
      if ( 1 != fread(&nBitPlanes, 2, 1, fh)) return(false);
      if ( 1 != fread(&nBitsPerPixel, 2, 1, fh)) return(false);
      if ( 1 != fread(&CompressionMethod, 4, 1, fh)) return(false);
      if ( 1 != fread(&ImageDataWidth, 4, 1, fh)) return(false);
      if ( 1 != fread(&HorizontalResolution, 4, 1, fh)) return(false);
      if ( 1 != fread(&VerticalResolution, 4, 1, fh)) return(false);
      if ( 1 != fread(&nColorsInPalette, 4, 1, fh)) return(false);
      if ( 1 != fread(&nColorsUsed, 4, 1, fh)) return(false);

/*
      printf("Bmp File Size : %lu\n", BMPFileSize);
      ((ULong *)Buffer)[0] = SoftwareID; Buffer[4]=0;
      printf("Software ID    : %lu\n",SoftwareID);
      printf("Data Offset    : %ld\n", DataOffset);
      printf("Header Size    : %lu\n", HeaderSize);
      printf("Image Width    : %lu\n", ImageWidth);
      printf("Image Height   : %lu\n", ImageHeight);
      printf("Bitplanes      : %lu\n", (ULong)nBitPlanes);
      printf("Bits per pixel : %lu\n", (ULong)nBitsPerPixel);
      printf("Compression Method: %lu\n", CompressionMethod);
      printf("Image Data Size: %lu\n", ImageDataWidth);
      printf("Horizontal Resolution : %lu\n", HorizontalResolution);
      printf("Vertical Resolution : %lu\n", VerticalResolution);
      printf("Colors In Palette : %lu\n", nColorsInPalette);
      printf("Colors Used : %lu\n",nColorsUsed);
*/
      if (DataOffset < 54 ) { printf("Header too small\n"); return(false); }

      // Remaining infos
      if (DataOffset > 54 ) { for(i=0;i<(DataOffset - 54); i++) if ( 1 != fread(&Buffer, 1, 1, fh)) return(false); }

      // Bitmap data

      CreateBitmap(ImageWidth, ImageHeight, nBitsPerPixel);

      printf("--------------------\n");
      printf("Memory Size : %lu\n",(UNum)BitmapMemorySize);
      if ( BitmapMemorySize != fread(BitmapMemory, 1, BitmapMemorySize, fh)) return(false);

      for (j=0;j<10;j++)
      {
        for (i=0;i<9;i++)
        {
          printf("%02x ",(unsigned int)BitmapMemory[i+(9*j)]);

        }
        printf("\n");
      }

      FlipY();
      return(true);
    }
