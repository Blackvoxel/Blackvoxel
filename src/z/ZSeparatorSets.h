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
 * ZSeparatorSets.h
 *
 *  Created on: Pre-2000 code.
 *      Author: laurent
 */


#ifndef Z_ZSEPARATORSETS_H
#define Z_ZSEPARATORSETS_H

//#ifndef Z_ZSEPARATORSETS_H
//#  include "ZSeparatorSets.h"
//#endif

// ***********************************************************************
// *                           Separator Set                             *
// ***********************************************************************


class ZSeparatorSet
{
    char Array[256];
  public:

    // Creation (Nouveau et Copie) d'un ensemble de separateurs.

    ZSeparatorSet();
    ZSeparatorSet(ZSeparatorSet &Set);

    // Initialisation d'un ensemble.

    inline void            Include     (char Separator);
    inline void            Exclude     (char Separator);
    inline ZSeparatorSet & operator+=  (char Separator);
    inline ZSeparatorSet & operator-=  (char Separator);
    inline void            IncludeRange(char Start,char End);
    inline void            ExcludeRange(char Start,char End);
    inline void            ClearAll    ();
    inline void            SetAll      ();

    // Operations entre ensembles.

    inline ZSeparatorSet & operator= (ZSeparatorSet &Set);
    inline ZSeparatorSet & operator+=(ZSeparatorSet &Set);
    inline ZSeparatorSet & operator-=(ZSeparatorSet &Set);
    inline ZSeparatorSet   operator+ (ZSeparatorSet &Set);
    inline ZSeparatorSet   operator- (ZSeparatorSet &Set);
    inline ZSeparatorSet   operator+ (char Separator);
    inline ZSeparatorSet   operator- (char Separator);
    inline ZSeparatorSet   operator& (ZSeparatorSet &Set);
    inline ZSeparatorSet   operator! ();
    inline ZSeparatorSet   operator~ ();

    // Tests d'appartenance d'un caractere a l'ensemble.

    inline Bool IsIncluded   (char Char);
    inline Bool IsNotIncluded(char Char);
    inline Bool operator==   (char Char);
    inline Bool operator!=   (char Char);
};

#include "ZSeparatorSets_InLines.h"

#endif
