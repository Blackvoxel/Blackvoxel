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

#ifndef Z_ZGENERAL_OBJECT_H
#define Z_ZGENERAL_OBJECT_H

//#ifndef Z_ZGENERAL_OBJECT_H
//#  include "ZGeneralObject.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

#ifndef Z_ZOBJECT_H
#  include "ZObject.h"
#endif

//#ifndef Z_ZSTREAMS_H
//#  include "ZStreams.h"
//#endif

enum { ZSAVEFORMAT_BINARY=0, ZSAVEFORMAT_XML=1 };

class ZGeneralObject : public ZObject
{

  //virtual Bool  Save(ZStream & Stream, int SaveFormat ) {}
  //virtual Bool  Load(ZStream & Stream, int SaveFormat ) {}
  //virtual ZSize GetSavedLen(int SaveFormat)             {}

};

#endif
