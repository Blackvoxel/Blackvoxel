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
 * ZTool_Constructor.h
 *
 *  Created on: 15 juil. 2011
 *      Author: laurent
 */

#ifndef Z_ZTOOL_CONSTRUCTOR_H
#define Z_ZTOOL_CONSTRUCTOR_H

//#ifndef Z_ZTOOL_CONSTRUCTOR_H
//#  include "ZTool_Constructor.h"
//#endif

#ifndef Z_ZTOOLS_H
#  include "ZTools.h"
#endif

class ZTool_Constructor : public ZTool
{
  protected:
    ULong OldToolNum;
    bool MiningInProgress;
    ZVoxelCoords MinedVoxel;
    double Mining_MaterialResistanceCounter;
    double ToolForce[ZTOOL_TOOLTYPESNUMBER];
    bool   ToolCompatibleTypes[ZTOOL_TOOLTYPESNUMBER];
    void * SoundHandle;
  public:
    ZTool_Constructor()
    {
      ULong i;
      OldToolNum = (ULong)-1;
      MiningInProgress = false;
      for (i=0;i<ZTOOL_TOOLTYPESNUMBER;i++) ToolForce[i]=0.0;
      for (i=0;i<ZTOOL_TOOLTYPESNUMBER;i++) ToolCompatibleTypes[i]=false;
      ToolForce[1] = 0.8;
      ToolCompatibleTypes[1] = true;
      Mining_MaterialResistanceCounter = 0;
      SoundHandle = 0;
    }
    virtual bool Tool_MouseButtonClick(ULong Button);
    virtual bool Tool_MouseButtonRelease(ULong Button);

    virtual bool Tool_StillEvents(double FrameTime, bool * MouseButtonMatrix, UByte * KeyboardMatrix );

    virtual void Start_Tool();
    virtual void End_Tool();
    virtual void Display();

};

class ZTool_Constructor_P2 : public ZTool_Constructor
{
  public:

    ZTool_Constructor_P2()
    {
      ToolForce[1] = 1.6;
      ToolForce[2] = 0.8;
      ToolCompatibleTypes[1] = true;
      ToolCompatibleTypes[2] = true;
    }
};

class ZTool_Constructor_P3 : public ZTool_Constructor
{
  public:

    ZTool_Constructor_P3()
    {
      ToolForce[1] = 3.2;
      ToolForce[2] = 1.6;
      ToolForce[3] = 0.8;
      ToolCompatibleTypes[1] = true;
      ToolCompatibleTypes[2] = true;
      ToolCompatibleTypes[3] = true;
    }
};

class ZTool_Constructor_P4 : public ZTool_Constructor
{
  public:

    ZTool_Constructor_P4()
    {
      ToolForce[1] = 6.4;
      ToolForce[2] = 6.4;
      ToolForce[3] = 3.2;
      ToolCompatibleTypes[1] = true;
      ToolCompatibleTypes[2] = true;
      ToolCompatibleTypes[3] = true;
    }
};

class ZTool_Constructor_P5 : public ZTool_Constructor
{
  public:

    ZTool_Constructor_P5()
    {
      ToolForce[1] = 12.8;
      ToolForce[2] = 12.8;
      ToolForce[3] = 12.8;
      ToolForce[4] = 12.8;
      ToolForce[5] = 12.8;
      ToolCompatibleTypes[1] = true;
      ToolCompatibleTypes[2] = true;
      ToolCompatibleTypes[3] = true;
      ToolCompatibleTypes[4] = true;
      ToolCompatibleTypes[5] = true;
    }
};
#endif /* Z_ZTOOL_CONSTRUCTOR_H */

