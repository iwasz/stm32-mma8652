/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef SRC_MMA8652_MMA8652_H_
#define SRC_MMA8652_MMA8652_H_

#include <stm32f4xx_hal.h>

#define MMA8652_I2C_ADDR        ((0x1D << 1))

#define MMA8652_STATUS                  0x00
#define MMA8652_OUT_X_MSB               0x01
#define MMA8652_WHO_AM_I                0x0D
#define MMA8652_XYZ_DATA_CFG            0x0E
#define MMA8652_CTRL_REG1               0x2A
#define MMA8652_CTRL_REG2               0x2B
#define MMA8652_WHO_AM_I_VALUE          0x4A


extern void mma8652Init (I2C_HandleTypeDef *i2cHandle);


#endif /* SRC_MMA8652_MMA8652_H_ */
