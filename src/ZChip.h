/*
 * ZCircuit.h
 *
 *  Created on: 8 nov. 2014
 *      Author: laurent
 */

#ifndef Z_ZCIRCUIT_H
#define Z_ZCIRCUIT_H

class ZChip
{
  public:

    inline void  PutRegister(ULong Register, ULong Value);
    inline ULong GetRegister(ULong Register, ULong Value);

    inline void  DoChipManagement();
};



#endif /* Z_ZCIRCUIT_H */
