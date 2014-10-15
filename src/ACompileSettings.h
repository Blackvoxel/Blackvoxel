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
 * ACompileSettings.h
 *
 *  Created on: 21 nov. 2011
 *      Author: laurent
 */

#ifndef A_COMPILESETTINGS_H
#define A_COMPILESETTINGS_H

//#ifndef A_COMPILESETTINGS_H
//#  include "ACompileSettings.h"
//#endif

// Options à inclure : -mfpmath=sse -msse2

#define DEVELOPPEMENT_FORCE_DEV 0 // If you are making development, set it to 1

// Dev options forced ?

#if DEVELOPPEMENT_FORCE_DEV == 1
  #undef DEVELOPPEMENT_ON
  #define DEVELOPPEMENT_ON 1
#endif

#ifndef DEVELOPPEMENT_ON
  #define DEVELOPPEMENT_ON 1
#endif

#ifndef COMPILEOPTION_DEMO
  #  define COMPILEOPTION_DEMO 0
  #endif
#define COMPILEOPTION_VERSIONNUM 137

// Options du jeu

#define COMPILEOPTION_ALLOWSTARTINGSTORAGE 0 // Allow starting storage to be present. Never introduced in the game.
#define COMPILEOPTION_ALLOWJUMPANDBUILD 1 // Jump and build allow building when floating in the air (Like when jumping).

// Version de démo

#if DEVELOPPEMENT_ON == 0
  #if COMPILEOPTION_DEMO == 1
    #define COMPILEOPTION_INVENTORY_DEMOCONTENT 1
    #define COMPILEOPTION_ALLOWSAVE 0
    #define COMPILEOPTION_ALLOWSAVEPLAYERSTATE 0
    #define COMPILEOPTION_ALLOWVEHICLE 1
    #define COMPILEOPTION_VERSIONSTRING "VERSION DEMO 1.37 (c) 2014 LAURENT THIEBAUT & OLIVIA MERLE"
  #else
    #define COMPILEOPTION_INVENTORY_DEMOCONTENT 0
    #define COMPILEOPTION_ALLOWSAVE 1
    #define COMPILEOPTION_ALLOWSAVEPLAYERSTATE 1
    #define COMPILEOPTION_ALLOWVEHICLE 1
    #define COMPILEOPTION_VERSIONSTRING "VERSION 1.37 (c) 2014 LAURENT THIEBAUT & OLIVIA MERLE"
  #endif
#else
  #define COMPILEOPTION_INVENTORY_DEMOCONTENT 0
  #define COMPILEOPTION_ALLOWSAVE 0
  #define COMPILEOPTION_ALLOWSAVEPLAYERSTATE 1
  #define COMPILEOPTION_ALLOWVEHICLE 1
  #define COMPILEOPTION_VERSIONSTRING "VERSION 1.37 : CONFIDENTIAL TESTING (c) LAURENT THIEBAUT & OLIVIA MERLE"
#endif

// Options de debogage

#if DEVELOPPEMENT_ON == 1
#  define COMPILEOPTION_FABDATABASEOUTPUT 0
#  define COMPILEOPTION_BOUNDCHECKING 1
#  define COMPILEOPTION_BOUNDCHECKINGSLOW 1
#  define COMPILEOPTION_FALLAREFATALS 0
#  define COMPILEOPTION_FINETIMINGTRACKING 0
#  define COMPILEOPTION_DEBUGFACILITY 1
#  define COMPILEOPTION_NOMOUSECAPTURE 1
#  define COMPILEOPTION_VOXELPROCESSOR 1
#  define COMPILEOPTION_DONTEMPTYINVENTORYONDEATH 1
#  define COMPILEOPTION_ALPHA_SOUNDS_1 1
#  define COMPILEOPTION_FNX_SOUNDS_1 1
#  define COMPILEOPTION_LOWRESTEXTURING 0
#  define COMPILEOPTION_VERBOSELOGS 0
#  define COMPILEOPTION_SQUIRRELUNSAFE 1
#  define COMPILEOPTION_SAVEONLYMODIFIED 1
#else
#  define COMPILEOPTION_FABDATABASEOUTPUT 0
#  define COMPILEOPTION_BOUNDCHECKING 0
#  define COMPILEOPTION_BOUNDCHECKINGSLOW 0
#  define COMPILEOPTION_FALLAREFATALS 1
#  define COMPILEOPTION_FINETIMINGTRACKING 0
#  define COMPILEOPTION_DEBUGFACILITY 0
#  define COMPILEOPTION_NOMOUSECAPTURE 0
#  define COMPILEOPTION_VOXELPROCESSOR 1
#  define COMPILEOPTION_DONTEMPTYINVENTORYONDEATH 0
#  define COMPILEOPTION_ALPHA_SOUNDS_1 1
#  define COMPILEOPTION_FNX_SOUNDS_1 1
#  define COMPILEOPTION_LOWRESTEXTURING 0
#  define COMPILEOPTION_VERBOSELOGS 0
#  define COMPILEOPTION_SQUIRRELUNSAFE 1
#  define COMPILEOPTION_SAVEONLYMODIFIED 1
#endif

// Plateformes

// Linux

#ifdef __linux__

  #define COMPILEOPTION_SAVEFOLDERNAME "Blackvoxel"
  #define COMPILEOPTION_USEHOMEDIRSTORAGE 1
  #define COMPILEOPTION_WINXPTIMECOMPUTING 0

  #if DEVELOPPEMENT_ON == 1
    #define COMPILEOPTION_DATAFILESPATH ""
  #else
    #if COMPILEOPTION_DEMO == 1
      #define COMPILEOPTION_DATAFILESPATH "/usr/share/blackvoxeldemo"
    #else
      #ifndef COMPILEOPTION_DATAFILESPATH
        #define COMPILEOPTION_DATAFILESPATH "/usr/share/blackvoxel"
      #endif
    #endif
  #endif

// Windows Win32

#elif _WIN32

  #define COMPILEOPTION_SAVEFOLDERNAME "Blackvoxel"
  #define COMPILEOPTION_USEHOMEDIRSTORAGE 1
  #define COMPILEOPTION_DATAFILESPATH ""
  #define COMPILEOPTION_WINXPTIMECOMPUTING 1

#endif

#endif /* A_COMPILESETTINGS_H */
