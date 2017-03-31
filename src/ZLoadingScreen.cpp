/*
 * This file is part of Blackvoxel.
 *
 * Copyright 2010-2017 Laurent Thiebaut & Olivia Merle
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
 * ZLoadingScreen.cpp
 *
 *  Created on: 26 févr. 2017
 *      Author: laurent
 */

#include <GL/glew.h>
#include "SDL/SDL.h"

#ifndef A_COMPILESETTINGS_H
#  include "ACompileSettings.h"
#endif

#ifndef Z_BITMAP_IMAGE_H
#  include "z/ZBitmapImage.h"
#endif

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

#include "ZLoadingScreen.h"

// Display a loading screen.
//
// The Blackvoxel's engine is only partially initialized
// at this stage, so we do not use it to display the
// loading screen.
// Instead, we'll use direct OpenGL calls in order to
// display the loading screen immediately.
// The used texture is deleted at the end in order
// to save GPU memory.

void ZLoadingScreen::Display(int Width, int Height)
{
  ZBitmapImage Texture;
  ZString FileSpec;
  unsigned int TextureRef;

  // Get filename with complete path.

  FileSpec = COMPILEOPTION_DATAFILESPATH; FileSpec.AddToPath("gui/Loading.bmp");

  // Load the texture bitmap from file.

  Texture.LoadBMP(FileSpec.String);

  // OpenGL Settings

  glEnable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glColor3f(0.0f, 0.0f, 0.0f);

  // Set projection

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, Width, Height , 0.0,-100.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Load the texture to GPU

  glGenTextures(1,&TextureRef);
  glBindTexture(GL_TEXTURE_2D,TextureRef);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // GL_LINEAR GL_NEAREST
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  gluBuild2DMipmaps(GL_TEXTURE_2D,      //Type : texture 2D
                    GL_RGBA8,          //Format : RGBA
                    Texture.Width,         //Width
                    Texture.Height,        //Height
                    GL_BGRA,    //Format : RGBA
                    GL_UNSIGNED_BYTE,   //Type des couleurs
                    (GLubyte *)Texture.BitmapMemory//Addresse de l'image
                   );

  // Compute coordinates for display

  GLfloat sx,sy,ex,ey;
  sx = (Width - (Texture.Width*2)) / 2;
  sy = (Height- (Texture.Height*2))/ 2;
  ex = sx + Texture.Width*2;
  ey = sy + Texture.Height*2;

  // Draw the texture to the buffer

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glBindTexture(GL_TEXTURE_2D,TextureRef );
    glBegin(GL_TRIANGLES);
      glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0,0.0); glVertex3f(sx, sy , 0.0);
        glTexCoord2f(1.0,0.0); glVertex3f(ex, sy , 0.0);
        glTexCoord2f(1.0,1.0); glVertex3f(ex, ey ,0.0);
        glTexCoord2f(1.0,1.0); glVertex3f(ex, ey , 0.0);
        glTexCoord2f(0.0,1.0); glVertex3f(sx, ey , 0.0);
        glTexCoord2f(0.0,0.0); glVertex3f(sx, sy , 0.0);
    glEnd();

   // Display the buffer

   SDL_GL_SwapBuffers( );

   // Delete the used texture to free some memory in GPU.

   glBindTexture(GL_TEXTURE_2D,0);
   glDeleteTextures(1,&TextureRef);

}


