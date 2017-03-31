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

#ifndef Z_ZTEXTUREMANAGER_H
#  define Z_ZTEXTUREMANAGER_H

// #ifndef Z_ZTEXTUREMANAGER_H
// #  include "ZTextureManager.h"
// #endif

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

#ifndef Z_ZOBJECT_H
#  include "z/ZObject.h"
#endif

#ifndef Z_ZOBJECT_ARRAY_H
#  include "z/ZObject_Array.h"
#endif

#ifndef Z_BITMAP_IMAGE_H
#  include "z/ZBitmapImage.h"
#endif


#include <stdio.h>

class ZTexture_Entry : public ZObject
{
  public:
    ZBitmapImage * Texture;
    bool           LinearInterpolation;
    unsigned int   OpenGl_TextureRef;

    ZTexture_Entry()
    {
      Texture = 0;
      LinearInterpolation = 0;
      OpenGl_TextureRef = 0;
    }
    ~ZTexture_Entry()
    {
      if (Texture) delete Texture;
      Texture = 0;
      LinearInterpolation = 0;
      OpenGl_TextureRef = 0;
    }
};

class ZTextureManager : public ZObject
{
  protected:
    ZObjectArray Array;

  public:
    ZTextureManager()
    {
      //printf("Création du texturemanager\n");
    }

    virtual ~ZTextureManager()
    {
      //printf("Libération du texturemanager\n");
    }

    bool LoadBMPTexture(char * FileSpec, ULong TextureNum, bool LinearInterpolation = true, bool AllowReduceSize = true, ULong MinReducedSize=128)
    {
      ZBitmapImage * NewImage;
      ZTexture_Entry * Texture;

      NewImage = new ZBitmapImage();
      if (!NewImage->LoadBMP(FileSpec)) { delete NewImage; return(false); }

      #if COMPILEOPTION_LOWRESTEXTURING>0
      if (NewImage->Width > 128 && (AllowReduceSize || COMPILEOPTION_LOWRESTEXTURING>1) )
      {
        NewImage->ReduceSize(); NewImage->ReduceSize();
        if (COMPILEOPTION_LOWRESTEXTURING>2) while(NewImage->Width>MinReducedSize) NewImage->ReduceSize();
      }
      #endif

      Texture = new ZTexture_Entry;
      Texture->Texture = NewImage;
      Texture->LinearInterpolation = LinearInterpolation;
      Array.SetEntry( TextureNum, *Texture );

      return(true);
    }

    ULong GetTexture_Count()
    {
      return(Array.GetCount());
    }

    ZTexture_Entry * GetTextureEntry(ULong TextureNum)
    {
      return( (ZTexture_Entry *)Array.GetEntry(TextureNum));
    }

};

#endif
