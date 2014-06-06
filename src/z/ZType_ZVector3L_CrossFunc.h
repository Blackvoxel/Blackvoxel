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
 * ZType_ZVector3L_CrossFunc.h
 *
 *  Created on: 10 févr. 2012
 *      Author: laurent
 */

#ifndef Z_ZTYPE_ZVECTOR3L_CROSSFUNC_H
#define Z_ZTYPE_ZVECTOR3L_CROSSFUNC_H

//#ifndef Z_ZTYPE_ZVECTOR3L_CROSSFUNC_H
//#  include "ZType_ZVector3L_CrossFunc.h"
//#endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

#ifndef Z_TYPE_ZVECTOR3L_H
#  include "ZType_ZVector3L.h"
#endif

#ifndef Z_ZTYPE_ZVECTOR3D_H
#  include "ZType_ZVector3d.h"
#endif

inline ZVector3L & ZVector3L::operator = (const ZVector3d & Vec3d)
{
  x = Vec3d.x;
  y = Vec3d.y;
  z = Vec3d.z;
  return(*this);
}

inline ZVector3L::ZVector3L ( const ZVector3d & Vec3d )
{
  x = Vec3d.x;
  y = Vec3d.y;
  z = Vec3d.z;
}

#endif /* Z_ZTYPE_ZVECTOR3L_CROSSFUNC_H */
