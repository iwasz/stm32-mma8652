/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "mma8652.h"
#include "config.h"


static void mma8652Reg8Writre (I2C_HandleTypeDef *i2cHandle, uint8_t reg, uint8_t value)
{
        uint8_t txBuffer[2] = {reg, value};

        while (HAL_I2C_Master_Transmit (i2cHandle, (uint8_t) MMA8652_I2C_ADDR, (uint8_t*) txBuffer, sizeof(txBuffer), 10000) != HAL_OK) {
                /* Error_Handler() function is called when Timeout error occurs.
                 When Acknowledge failure occurs (Slave don't acknowledge it's address)
                 Master restarts communication */
                if (HAL_I2C_GetError (i2cHandle) != HAL_I2C_ERROR_AF) {
                        Error_Handler ();
                }
        }
}

void mma8652Init (I2C_HandleTypeDef *i2cHandle)
{
        // 0x00 to CTRL_REG1 to place MMA8652 into standby.
        mma8652Reg8Writre (i2cHandle, MMA8652_CTRL_REG1, 0x00);


        /*
         * Write 0000 0001 = 0x01 to XYZ_DATA_CFG register to set g range
         * [7-5]: reserved=000
         * [4]: HPF_OUT=0
         * [3-2]: reserved=00
         * [1-0]: FS=01 for +/-4g: 512 counts / g = 8192 counts / g after 4 bit left shift
         */
        mma8652Reg8Writre (i2cHandle, MMA8652_XYZ_DATA_CFG, 0x01);

        /*
         * write 0000 0010 = 0x02 to CTRL_REG2 to set MODS bits
         * [7]: ST=0: self test disabled
         * [6]: RST=0: reset disabled
         * [5]: unused
         * [4-3]: SMODS=00
         * [2]: SLPE=0: auto sleep disabled
         * [1-0]: mods=10 for high resolution (maximum over sampling)
         */
        mma8652Reg8Writre (i2cHandle, MMA8652_CTRL_REG2, 0x02);


        /*
         * write 0001 0001 = 0x11 to CTRL_REG1
         * [7-6]: aslp_rate=00
         * [5-3]: dr=010 for 200Hz data rate
         * [2]: unused=0
         * [1]: f_read=0 for normal 16 bit reads
         * [0]: active=1 to take the part out of standby and enable sampling
         */
         mma8652Reg8Writre (i2cHandle, MMA8652_CTRL_REG1, 0x11);
}

#if 0

// initialize MMA8652 accelerometer sensor
void mma8652Init (LDD_TDeviceData *DeviceDataPtr, struct AccelSensor *pthisAccel)
{
        LDD_I2C_TBusState BusState;     // I2C bus state

        // set up the MMA8652 I2C address
        I2C_SelectSlaveDevice (DeviceDataPtr, LDD_I2C_ADDRTYPE_7BITS, MMA8652_I2C_ADDR);

        // write 0000 0000 = 0x00 to CTRL_REG1 to place MMA8652 into standby
        // [7-1] = 0000 000
        // [0]: active=0
        I2C_Buf[0] = MMA8652_CTRL_REG1;
        I2C_Buf[1] = 0x00;
        mqxglobals.I2C_Status &= ~I2C_SENT_FLAG;
        I2C_MasterSendBlock (DeviceDataPtr, I2C_Buf, 2, LDD_I2C_SEND_STOP);
        // wait until the I2C sent callback function sets the sent flag
        while ((mqxglobals.I2C_Status & I2C_SENT_FLAG) == 0)
                ;
        // wait until the I2C bus is idle
        do {
                I2C_CheckBus (DeviceDataPtr, &BusState);
        } while (BusState != LDD_I2C_IDLE);

        // write 0000 0001 = 0x01 to XYZ_DATA_CFG register to set g range
        // [7-5]: reserved=000
        // [4]: HPF_OUT=0
        // [3-2]: reserved=00
        // [1-0]: FS=01 for +/-4g: 512 counts / g = 8192 counts / g after 4 bit left shift
        I2C_Buf[0] = MMA8652_XYZ_DATA_CFG;
        I2C_Buf[1] = 0x01;
        mqxglobals.I2C_Status &= ~I2C_SENT_FLAG;
        I2C_MasterSendBlock (DeviceDataPtr, I2C_Buf, 2, LDD_I2C_SEND_STOP);
        // wait until the I2C sent callback function sets the sent flag
        while ((mqxglobals.I2C_Status & I2C_SENT_FLAG) == 0)
                ;
        // wait until the I2C bus is idle
        do {
                I2C_CheckBus (DeviceDataPtr, &BusState);
        } while (BusState != LDD_I2C_IDLE);

        // write 0000 0010 = 0x02 to CTRL_REG2 to set MODS bits
        // [7]: ST=0: self test disabled
        // [6]: RST=0: reset disabled
        // [5]: unused
        // [4-3]: SMODS=00
        // [2]: SLPE=0: auto sleep disabled
        // [1-0]: mods=10 for high resolution (maximum over sampling)
        I2C_Buf[0] = MMA8652_CTRL_REG2;
        I2C_Buf[1] = 0x02;
        mqxglobals.I2C_Status &= ~I2C_SENT_FLAG;
        I2C_MasterSendBlock (DeviceDataPtr, I2C_Buf, 2, LDD_I2C_SEND_STOP);
        // wait until the I2C sent callback function sets the sent flag
        while ((mqxglobals.I2C_Status & I2C_SENT_FLAG) == 0)
                ;
        // wait until the I2C bus is idle
        do {
                I2C_CheckBus (DeviceDataPtr, &BusState);
        } while (BusState != LDD_I2C_IDLE);

        // write 0001 0001 = 0x11 to CTRL_REG1
        // [7-6]: aslp_rate=00
        // [5-3]: dr=010 for 200Hz data rate
        // [2]: unused=0
        // [1]: f_read=0 for normal 16 bit reads
        // [0]: active=1 to take the part out of standby and enable sampling
        I2C_Buf[0] = MMA8652_CTRL_REG1;
        I2C_Buf[1] = 0x11;
        mqxglobals.I2C_Status &= ~I2C_SENT_FLAG;
        I2C_MasterSendBlock (DeviceDataPtr, I2C_Buf, 2, LDD_I2C_SEND_STOP);
        // wait until the I2C sent callback function sets the sent flag
        while ((mqxglobals.I2C_Status & I2C_SENT_FLAG) == 0)
                ;
        // wait until the I2C bus is idle
        do {
                I2C_CheckBus (DeviceDataPtr, &BusState);
        } while (BusState != LDD_I2C_IDLE);

        // store the gain terms in the accelerometer and magnetometer sensor structures

#define MMA8652_GPERCOUNT 0.0001220703125F          // equal to 1/8192

        pthisAccel->fgPerCount = MMA8652_GPERCOUNT;

        return;
}

// read MMA8652 accelerometer and magnetometer data over I2C
void mma8652ReadData (LDD_TDeviceData *DeviceDataPtr, struct AccelSensor *pthisAccel)
{
        LDD_I2C_TBusState BusState;     // I2C bus state

        // set up the MMA8652 I2C address
        I2C_SelectSlaveDevice (DeviceDataPtr, LDD_I2C_ADDRTYPE_7BITS, MMA8652_I2C_ADDR);

        // set up the address of the first output register
        I2C_Buf[0] = MMA8652_OUT_X_MSB;
        mqxglobals.I2C_Status &= ~I2C_SENT_FLAG;

        I2C_MasterSendBlock (DeviceDataPtr, I2C_Buf, 1, LDD_I2C_NO_SEND_STOP);

        // wait until the I2C sent callback function sets the sent flag
        while ((mqxglobals.I2C_Status & I2C_SENT_FLAG) == 0)
                ;

        // read the 6 bytes of sequential sensor data
        mqxglobals.I2C_Status &= ~I2C_RCVD_FLAG;
        I2C_MasterReceiveBlock (DeviceDataPtr, I2C_Buf, 6, LDD_I2C_SEND_STOP);
        // wait until the I2C received callback function sets the received flag
        while ((mqxglobals.I2C_Status & I2C_RCVD_FLAG) == 0)
                ;
        // wait until the I2C bus is idle
        do {
                I2C_CheckBus (DeviceDataPtr, &BusState);
        } while (BusState != LDD_I2C_IDLE);

        // place the 12 bytes read into the 16 bit accelerometer structure
        pthisAccel->iGpFast[X] = (I2C_Buf[0] << 8) | I2C_Buf[1];
        pthisAccel->iGpFast[Y] = (I2C_Buf[2] << 8) | I2C_Buf[3];
        pthisAccel->iGpFast[Z] = (I2C_Buf[4] << 8) | I2C_Buf[5];

        // finally check for -32768 in the accelerometer since
        // this value cannot be negated in a later HAL operation
        if (pthisAccel->iGpFast[X] == -32768)
                pthisAccel->iGpFast[X]++;
        if (pthisAccel->iGpFast[Y] == -32768)
                pthisAccel->iGpFast[Y]++;
        if (pthisAccel->iGpFast[Z] == -32768)
                pthisAccel->iGpFast[Z]++;

        return;
}

#endif
