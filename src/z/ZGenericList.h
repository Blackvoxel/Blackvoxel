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
 * ZGenericList.h
 *
 *  Created on: 26 sept. 2013
 *      Author: laurent
 */

#ifndef Z_ZGENERICLIST_H
#define Z_ZGENERICLIST_H

class ZGenericList_Entry
{
    ZGenericList_Entry * Next;
    ZGenericList_Entry * Pred;

    virtual ~ZGenericList_Entry() {};
};

class ZGenericList
{
    ZGenericList_Entry Head;
    ZGenericList_Entry Tail;


};




#endif /* Z_ZGENERICLIST_H */
