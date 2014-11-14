/*
 * ZMachine_Circuit_Parallel_Interface.h
 *
 *  Created on: 7 nov. 2014
 *      Author: laurent
 */

#ifndef Z_ZCIRCUIT_PARALLEL_INTERFACE_H
#define Z_ZCIRCUIT_PARALLEL_INTERFACE_H

#ifndef Z_ZTYPES_H
#  include "z/ZTypes.h"
#endif

class ZCircuit_Parallel_Interface : public ZChip
{
  UByte Register;
  UByte Direction;
  UByte Interrupt;

  inline void  PutRegister(ULong Register, ULong Value)
  {
    switch(Register & 3)
    {
      case 0: Register = Value;
      case 1:
      case 2:
      case 3:
    }
  }

  inline ULong GetRegister(ULong Register, ULong Value);

  inline void  DoChipManagement();
};




#endif /* Z_ZCIRCUIT_PARALLEL_INTERFACE_H */
