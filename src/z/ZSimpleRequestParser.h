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
 * ZSimpleRequestParser.h
 *
 *  Created on: 25 mai 2017
 *      Author: laurent
 */

#ifndef Z_ZSIMPLEREQUESTPARSER_H
#define Z_ZSIMPLEREQUESTPARSER_H

//#ifndef Z_ZSIMPLEREQUESTPARSER_H
//#  include "ZSimpleRequestParser.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

#ifndef Z_ZSTRING_H
#  include "ZString.h"
#endif

class ZSimpleRequestParser
{
  public:
    bool ParseRequest(ZString & Text);
    void Dump();
    bool FindEntry(char * Name, ULong & Num);
    bool FindEntryText(char * Name, ZString & Text);

    ZString RequestType;
    ZString Command;
    ZString Proto;
    ZString Arg_Name[16];
    ZString Arg_Value[16];
};



#endif /* Z_ZSIMPLEREQUESTPARSER_H */
