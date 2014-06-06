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
 * ZScripting.h
 *
 *  Created on: 27 déc. 2012
 *      Author: laurent
 */

#ifndef Z_ZSCRIPTING_H
#define Z_ZSCRIPTING_H

//#ifndef Z_ZSCRIPTING_H
//#  include "ZScripting.h"
//#endif

#ifndef Z_ZSTRING_H
#  include "z/ZString.h"
#endif

class ZScript
{
  public:
    ZString ScriptText;
    void    * CompiledInfos;
    bool Compiled;
};

class ZScripting
{
  public:
    virtual bool Init() = 0;
    virtual void Cleanup() = 0;
    virtual void RunScript( ZScript * Script) = 0;
    virtual ~ZScripting() {};
};

#endif /* Z_ZSCRIPTING_H */
