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
 * ZScripting_Squirrel3.h
 *
 *  Created on: 27 déc. 2012
 *      Author: laurent
 */

#ifndef Z_ZSCRIPTING_SQUIRREL3_H
#define Z_ZSCRIPTING_SQUIRREL3_H

//#ifndef Z_ZSCRIPTING_SQUIRREL3_H
//#  include "ZScripting_Squirrel3.h"
//#endif

#ifndef Z_ZSCRIPTING_H
#  include "ZScripting.h"
#endif

#ifndef Z_ZSTRING_H
#  include "ZString.h"
#endif

class ZGame;
class ZVoxelExtension_Programmable;

class ZScripting_Squirrel3
{
  protected:
    void * Ls;
    ZString ScriptFilename;
    ZGame * GameEnv;
  public:
    enum {RUNCONTEXT_CREATE = 0, RUNCONTEXT_DESTROY=1, RUNCONTEXT_NORMALSTEP=2, RUNCONTEXT_LOADING=3, RUNCONTEXT_PROGRAMCHANGE=4};
    bool ScriptCompiledOK;
    bool IsReady;

    ZScripting_Squirrel3();
    virtual ~ZScripting_Squirrel3();

    virtual bool IsVMReady() { return(IsReady); }

    virtual bool Init();
    virtual void Cleanup();
    virtual void SetScriptFileSpec(const char * FileSpec) { ScriptFilename = FileSpec; }
    virtual bool LoadAndCompileScript();
    virtual bool IsCompiledOk();
    virtual void RunScript( const char * FunctionName, bool AllowWorldManipulations, ULong LaunchContext);
    virtual void SetVoxelPosition(ZVector3L * VoxelPosition);
    virtual void SetExtension(ZVoxelExtension_Programmable * Extension);
    virtual void StopProgram();
    virtual bool IsRunningProgram();

};



#endif /* Z_ZSCRIPTING_SQUIRREL3_H */
