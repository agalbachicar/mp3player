/*
 * sta_regs.h
 *
 *  Created on: 27/12/2014
 *      Author: Maximiliano
 */

#ifndef STA_REGS_H_
#define STA_REGS_H_

/*
 *@note Includes
 */
#include <stdint.h>

/*
 * @note Defines
 */
//--------------Vector de inicializacion-----------------------
#define init_size ((uint32_t) 4040)

//----------------Registros ADDR & VAL-------------------------

/*
 * Registro: RUN
 * Addr: 0x72 (114)
 * Use: Paso de estado Idle a Decode
 */
#define RUN ((char) 0x72)
#define GO_DEC ((char) 0x01)
#define GO_IDLE ((char) 0x00)

/*
 * Registro: IDENT
 * Addr: 0x01 (1)
 * Use: Corroborar coneccion con decoder via I2C
 */
#define IDENT ((char) 0x01)
#define ID_OK ((char) 0xAC) //(172)

/*
 * Registro: DATA_REQ_ENABLE
 * Addr: 0x18 (24)
 * Use: onfigura pin .28 como DATA_REQUEST
 */
#define DATA_REQ_ENABLE ((char) 0x18)
#define DREQ_ON ((char) 0x04)

/*
 * Registro: SOFT_RESET
 * Addr: 0x10 (16)
 * Use: Soft Reset
 */
#define SOFT_RESET ((char) 0x10)
#define SFT_RST ((char) 0x01)
#define SFT_OFF ((char) 0x00)

#define SFT_DELAY ((char) 300) //300 ms

/*
 * Registro: PLAY_REG
 * Addr: 0x13 (19)
 * Use: Control de la decoficicacion (apaga o prende codificacion)
 */
#define PLAY_REG ((char) 0x13)
#define DEC_ON ((char)  0x01)
#define DEC_OFF ((char) 0x00)

/*
 * Registro: MUTE
 * Addr: 0x14 (32)
 * Use: Silenciar salida del decoder junto a PLAY
 */
#define MUTE ((char) 0x14)
#define OUT_ON ((char) 0x00)
#define OUT_MUT ((char) 0x01)

/*
 * Registro: SCLK_POL
 * Addr: 0x0D (13)
 * Use: Polaridad del clock para entrada SPI
 */
#define SCK_POL ((char) 0x0D)
#define SCK_HI ((char) 0x00)
#define SCK_LOW ((char) 0x04)

/*
 * Registro: REQ_POL
 * Addr: 0x0C (12)
 * Use: Polaridad del DATA_REQ
 */
#define REQ_POL ((char) 0x0C)
#define REQ_HI ((char) 0x00)
#define REQ_LOW ((char) 0x05)

/*
 * Registro: DLA
 * Addr: 0x46 (70)
 * Use: Atenuacion del canal izquierdo
 */
#define DLA ((char) 0x46)
#define LEFT_AT ((char) 96)
#define MAX_LAten ((uint8_t) 96)

/*
 * Registro: DRA
 * Addr: 0x47 (71)
 * Use: Atenuacion del canal derecho
 */
#define DRA ((char) 0x47)
#define RIGHT_AT ((char) 96)
#define MAX_RAten ((uint8_t) 96)

/*
 * Registro: TONE_ATTEN
 * Addr: 0x7D (125)
 * Use: Atenuacion
 */
#define TONE_ATTEN ((char) 0x7D)
#define ATT ((char) 18)
#define MAX_TAten ((uint8_t) 18)


/*
 * Registro: ERROR_CODE
 * Addr: 0x0F (15)
 * Use: Codigo de error
 */
#define ERROR_CODE ((char) 0x0F)

#endif /* STA_REGS_H_ */
