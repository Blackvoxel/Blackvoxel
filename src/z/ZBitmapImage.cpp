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
#include "ZBitmapImage.h"

#include <stdio.h>
#include <string.h>


    ZBitmapImage::ZBitmapImage()
    {
      Width = 0;
      Height = 0;
      BitsPerPixel = 0;
      BytesPerPixel = 0;
      BitmapMemory = 0;
      BitmapMemorySize = 0;
    }

    ZBitmapImage::~ZBitmapImage()
    {
      if (BitmapMemory) delete [](BitmapMemory);
      Width = 0;
      Height = 0;
      BitsPerPixel = 0;
      BytesPerPixel = 0;
      BitmapMemory = 0;
      BitmapMemorySize = 0;
    }

    bool ZBitmapImage::CreateBitmap(ULong Width, ULong Height, ULong BitsPerPixel)
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
    
    void ZBitmapImage::FlipY()
    {
      ULong BPLine,y,x,Offset1,Offset2;
      char c;

      if ( (this->Height == 0) || (this->Width == 0) ) return;
      BPLine = this->Width * this->BytesPerPixel;

      for (y=0;y<this->Height/2;y++)
      {
    	// printf("Go %ld \n",y);
    	Offset1 = BPLine * y;
    	Offset2 = BPLine * ((this->Height-1) - y);
    	for (x=0;x<BPLine;x++) {c = BitmapMemory[x+Offset1]; BitmapMemory[x+Offset1] = BitmapMemory[x+Offset2]; BitmapMemory[x+Offset2] = c;}
      }
    }

    bool ZBitmapImage::LoadBMP( const char * FileName )
    {
      FILE * fh;
      char   Buffer[4096];
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
      ULong  RedMask = 0;
      ULong  GreenMask = 0;
      ULong  BlueMask = 0;
      ULong  AlphaMask = 0;
      UByte  RedMask_Offset = 0;
      UByte  GreenMask_Offset = 0;
      UByte  BlueMask_Offset = 0;
      UByte  AlphaMask_Offset = 0;

      ULong  i, AlreadyReaded;
      
      if (! (fh = fopen(FileName, "rb"))) return(false);
      
      if ( 1 != fread(&MagicNumber, 2, 1, fh)) return(false);
      //((unsigned short *)Buffer)[0] = MagicNumber; Buffer[2]=0;
      //printf ("Magic number :%s\n",Buffer);
      if ( MagicNumber != 0x4D42 ) {printf( "Not a BMP file\n" ); return(false);}
      
      if ( 1 != fread(&BMPFileSize, 4, 1, fh)) return(false);
      if ( 1 != fread(&SoftwareID, 4, 1, fh)) return(false);
      if ( 1 != fread(&DataOffset, 4, 1, fh)) return(false);
      if ( 1 != fread(&HeaderSize, 4, 1, fh)) return(false);
      
      if (HeaderSize < 40) {printf("Unsupported BMP format\n"); return(false);}

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
      AlreadyReaded = 54;

      if (nBitsPerPixel < 32 && CompressionMethod == 3) {printf("Unsupported BMP format : Must use 32 bit per pixels A8R8G8B8.\n"); return(false);}
      // New Headers V4 and V5.
      if (!(CompressionMethod ==0 || CompressionMethod == 3)) {printf("Unsupported BMP compression format. Use only uncompressed formats.\n"); return(false);}
      if (HeaderSize > 40)
      {
        if ( 1 != fread(&RedMask, 4, 1, fh))   return(false);
        if ( 1 != fread(&GreenMask, 4, 1, fh)) return(false);
        if ( 1 != fread(&BlueMask, 4, 1, fh))  return(false);
        if ( 1 != fread(&AlphaMask, 4, 1, fh)) return(false);
        AlreadyReaded += 16;

        RedMask_Offset   = GetBitMaskOffset(RedMask);
        GreenMask_Offset = GetBitMaskOffset(GreenMask);
        BlueMask_Offset  = GetBitMaskOffset(BlueMask);
        AlphaMask_Offset = GetBitMaskOffset(AlphaMask);

        if (RedMask_Offset > 31 || GreenMask_Offset > 31 || BlueMask_Offset > 31 || AlphaMask_Offset > 31) {printf("Wrong values in BMP colormasks.\n"); return(false);}
      }

      /*
      printf("Bmp File Size : %lu\n", BMPFileSize);
      //((ULong *)Buffer)[0] = SoftwareID; Buffer[4]=0;
      //printf("Software ID    : %lu\n",SoftwareID);
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
      if (DataOffset > AlreadyReaded ) { for(i=0;i<(DataOffset - AlreadyReaded); i++) if ( 1 != fread(&Buffer, 1, 1, fh)) return(false); }
      
      // Bitmap data
      
      CreateBitmap(ImageWidth, ImageHeight, nBitsPerPixel);

//      printf("--------------------\n");
//      printf("Memory Size : %lu\n",BitmapMemorySize);
      if ( BitmapMemorySize != fread(BitmapMemory, 1, BitmapMemorySize, fh)) return(false);

      if (CompressionMethod == 3)
      {
        ULong PixelCount, PixelField;
        UByte * Out;
        ULong * In;

        PixelCount = ImageWidth * ImageHeight;
        In = (ULong *)BitmapMemory;
        Out = BitmapMemory;

        for (i=0; i<PixelCount ;i++)
        {
          PixelField = *(In++);

          *(Out++) = (PixelField & BlueMask ) >> BlueMask_Offset;
          *(Out++) = (PixelField & GreenMask) >> GreenMask_Offset;
          *(Out++) = (PixelField & RedMask  ) >> RedMask_Offset;
          *(Out++) = (PixelField & AlphaMask) >> AlphaMask_Offset;

        }
      }
/*
      for (j=0;j<10;j++)
      {
        for (i=0;i<9;i++)
        {
          printf("%02x ",(unsigned int)BitmapMemory[i+(9*j)]);
        
        }
        printf("\n");
      }
*/
      FlipY();
      fclose(fh);
      return(true);
    }


    bool ZBitmapImage::SaveBMP( const char * FileName )

    {
      FILE * fh;

      ULong  BMPFileSize, DataOffset, HeaderSize,Data_ULong, BitmapSize;
      UShort Data_UShort;

      // Compute Size of file parts

      BitmapSize  = Width * Height * BytesPerPixel;
      DataOffset = 54;
      HeaderSize = 40;
      BMPFileSize = DataOffset + BitmapSize;

      // Open Filename

      if (! (fh = fopen(FileName, "wb"))) return(false);

      // BMP Magic number

/*00*/fputc('B',fh); fputc('M',fh);

      // BMP File Size

/*02*/if ( 1 != fwrite(&BMPFileSize, 4, 1, fh)) {fclose(fh); return(false); }

      // Application specific

/*06*/Data_ULong = 0; if ( 1 != fwrite(&Data_ULong, 4, 1, fh)) {fclose(fh); return(false); }  // nBitPlanes

      // Data Offset

/*10*/if ( 1 != fwrite(&DataOffset, 4, 1, fh)) {fclose(fh); return(false); }

      // Header Size

/*14*/if ( 1 != fwrite(&HeaderSize, 4, 1, fh)) {fclose(fh); return(false); }

      // Image Width

/*18*/if ( 1 != fwrite(&Width, 4, 1, fh)) {fclose(fh); return(false); }

      // Image Height

/*22*/if ( 1 != fwrite(&Height, 4, 1, fh)) {fclose(fh); return(false); }

      // nBitplanes

/*26*/Data_UShort = 1; if ( 1 != fwrite(&Data_UShort, 2, 1, fh)) {fclose(fh); return(false); }  // nBitPlanes

      // nBitsPerPixels

/*28*/Data_UShort = BitsPerPixel; if ( 1 != fwrite(&Data_UShort, 2, 1, fh)) {fclose(fh); return(false); }  // nBitsPerPixel

      // Compression

/*30*/Data_ULong  = 0; if ( 1 != fwrite(&Data_ULong,  4, 1, fh)) {fclose(fh); return(false); }  // no pixel array compression used

      // Bitmap Size

/*34*/if ( 1 != fwrite(&BitmapSize,  4, 1, fh)) {fclose(fh); return(false); }  // Bitmap Size

      // Print resolution of image (Horizontal)

/*38*/Data_ULong  = 300; if ( 1 != fwrite(&Data_ULong,  4, 1, fh)) {fclose(fh); return(false); }  // Print Resolution of image (Horizontal)

      // Print resolution of image (Vertical)

/*42*/Data_ULong  = 300; if ( 1 != fwrite(&Data_ULong,  4, 1, fh)) {fclose(fh); return(false); }  // Print Resolution of image (Vertical)

     // Coloros in the palette

/*46*/Data_ULong  = 0;   if ( 1 != fwrite(&Data_ULong,  4, 1, fh)) {fclose(fh); return(false); }  // Colors in the palette

    // Important colors

/*50*/Data_ULong  = 0;   if ( 1 != fwrite(&Data_ULong,  4, 1, fh)) {fclose(fh); return(false); }  // Important colors

    // Pixel Array

    Long y;
    ULong LineBytes = BytesPerPixel * Width;

    for (y=(Long)(Height-1) ; y>=0 ; y--)
    {
      if ( 1 != fwrite(BitmapMemory+LineBytes*y, LineBytes, 1, fh)) {fclose(fh); return(false); }
    }

    // close

    fclose(fh);

    // End

    return(true);
}

void ZBitmapImage::Clear()
{
  if ( BitmapMemory )
  {
    memset(BitmapMemory, 0, BitmapMemorySize);
  }
}
