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
 * ZCircuit_Parallel_Interface_Servo.h
 *
 *  Created on: 15 déc. 2014
 *      Author: laurent
 */

#ifndef Z_ZCHIP_PARALLEL_INTERFACE_SERVO_H
#define Z_ZCHIP_PARALLEL_INTERFACE_SERVO_H

//#ifndef Z_ZCHIP_PARALLEL_INTERFACE_SERVO_H
//#  include "ZChip_Parallel_Interface_Servo.h"
//#endif

#ifndef Z_ZCHIP_PARALLEL_INTERFACE_H
#  include "ZChip_Parallel_Interface.h"
#endif






class ZChip_Parallel_Interface_Servo : public ZChip_Parallel_Interface
{
  public:
    void  SetRegister_Data(ULong Value);
    void  SetRegister(ULong Register, ULong Value);
    void  DoChipManagement();
    void  Reset();
};


#endif /* Z_ZCHIP_PARALLEL_INTERFACE_SERVO_H */
