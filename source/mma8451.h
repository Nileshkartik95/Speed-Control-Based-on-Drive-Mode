/*****************************************************************************
​ ​*​ ​Copyright​ ​(C)​ ​2022 ​by​ ​Nileshkartik Ashokkumar
​ ​*
​ ​*​ ​Redistribution,​ ​modification​ ​or​ ​use​ ​of​ ​this​ ​software​ ​in​ ​source​ ​or​ ​binary
​ ​*​ ​forms​ ​is​ ​permitted​ ​as​ ​long​ ​as​ ​the​ ​files​ ​maintain​ ​this​ ​copyright.​ ​Users​ ​are
​ ​*​ ​permitted​ ​to​ ​modify​ ​this​ ​and​ ​use​ ​it​ ​to​ ​learn​ ​about​ ​the​ ​field​ ​of​ ​embedded
​ ​*​ ​software.​ ​Nileshkartik Ashokkumar​ ​and​ ​the​ ​University​ ​of​ ​Colorado​ ​are​ ​not​ ​liable​ ​for
​ ​*​ ​any​ ​misuse​ ​of​ ​this​ ​material.
​ ​*
 *****************************************************************************/
/**
​ ​*​ ​@file​ ​mma8451.h
​ ​*​ ​@brief​  header file provides the API for initialization of  acceleration sensor module and read acceleration sensor value
​ ​*
 * Code has been referred from the Alexander G Dean Chapter 8.
​ ​*​ ​@author​ ​Nileshkartik Ashokkumar​
​ ​*​ ​@date​ ​Dec ​9 ​2022
​ ​*​ ​@version​ ​1.0
​ ​*
​ ​*/

#ifndef MMA8451_H_
#define MMA8451_H_

#include <stdint.h>

#define MMA_ADDR 0x3A

#define REG_XHI 0x01
#define REG_XLO 0x02
#define REG_YHI 0x03
#define REG_YLO 0x04
#define REG_ZHI	0x05
#define REG_ZLO 0x06

#define REG_WHOAMI 0x0D
#define REG_CTRL1  0x2A
#define REG_CTRL4  0x2D

#define WHOAMI 0x1A

#define COUNTS_PER_G (4096.0)
#define M_PI (3.14159265)


/** @brief Initialization for the acceleration sensor module
 *
 */
int init_mma(void);

/** @brief Read the i2c Acceleration sensor value
 *
 */
void read_full_xyz(void);

/** @brief Read the x y z parameters from the accleration sensor module
 *
 */
void read_xyz(void);

/** @brief Convert the obtained xyz to roll and pitch
 *
 */
void convert_xyz_to_roll_pitch(void);

extern float roll, pitch;
extern int16_t acc_X, acc_Y, acc_Z;

#endif
