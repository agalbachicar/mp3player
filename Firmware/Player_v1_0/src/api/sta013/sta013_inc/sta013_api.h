/*
 * sta013_api.h
 *
 *  Created on: 06/11/2014
 *      Author: Maximiliano
 */

#ifndef STA013_API_H_
#define STA013_API_H_

/*
 * @note Includes
 */
#include <sta013_driver.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * @note Defines
 */
/*
 * @def 	STR_UNIT_SIZE
 * @brief	Cantidad de bytes a enviar al decoder sin que llene su buffer
 */
#define		STR_UNIT_SIZE					190 //190 bytes, se supone el decoder posee 200 bytes de FIFO


/*
 * @note Variables globales
 */


/*
 * @note Funciones publicas
 */

/*
 * @fn OP_STATE sta013_Init(void)
 * @brief Inicializacion del sta013
 * @param void
 * @return OP_STATE :
 *         - OP_END: inicializacion terminada
 *         - OP_ERROR: erro en inicializacion
 *         - OP_BUSY: operacion en curso
 */
OP_STATE sta013_Init(void);

/*
 * @fn OP_STATE sta013_ReadReg (char addr, char val)
 * @brief  Lectura de un registro del STA013
 * @param
 * 		  -  char addr: Direccion de memoria del registro a leer
 * 		  -  char val: Valor del registro leido
 * @return OP_STATE:
 *         - OP_END: Lectura terminada con exito
 *         - OP_ERROR: Lectura aboratda
 */
OP_STATE sta013_ReadReg (char addr,char *val);

/*
 * @fn OP_STATE delay_1ms (void)
 * @brief Delay forzado para inicializacion
 * @param
 * 		 - void
 * @return
 *       - void
 */
void delay_1ms(void);

/*
 * @fn OP_STATE sta013_WriteReg (uint8_t addr,  uint8_t val)
 * @brief  Escritura de un registro del STA013
 * @param
 * 		  -  char addr: Direccion de memoria del registro a escribir
 * 		  -  char val: Valor del registro a escribir
 * @return OP_STATE:
 *         - OP_END: Escritura terminada con exito
 *         - OP_ERROR: Escritura aboratda
 */
OP_STATE sta013_WriteReg (  char addr,  char val);


/*
 * @fn OP_STATE sta013_Stream (  char *block, uint32_t block_size)
 * @brief Comunicacion del LPC1769 con el stream de datos al sta013
 * @param - char *  block: Direccion de memoria inicial del blocke a transmitir
 * 		  - uint32_t block_size: Tamaño de bytes del bloque a transmitir
 * @return OP_STATE:
 *         - OP_BUSY: streaming en proceso, bloque no terminado, buffer de sta013 no lleno
 *         - OP_END: streaming detenido
 */
OP_STATE sta013_Stream (  char *block, uint32_t block_size);

/*
 * @fn OP_STATE sta013_DeInit(void)
 * @brief Desinicializacion del STA013
 * @param void
 * @return OP_STATE:
 * 		   - OP_END: STA013 deinicializado
 * 		   - OP_ERROR: Desinicializacion incompleta
 */
OP_STATE sta013_DeInit (void);


/*
 * @note Funciones privadas
 */

/*
 * @fn static OP_STATE sta013_Ready (void)
 * @brief Testeo de correcta comunicacion con el decoder
 * @param void
 * @return OP_STATE:
 * 		  - OP_END: STA013	preparado ready
 * 		  - OP_ERROR: STA013 incomunicado
 */
OP_STATE sta013_Ready (void);

/*
 * @fn OP_STATE sta013_VOL(char att_tone, char att_left, char att_right)
 * @brief Control de volumen en forma dogital del sta013
 * @param char att_tone: Ateniuacion entre (0 - 18dB)
 * 		  char att_left: Atenuacion del canal izquierdo entre (0 - 96dB)
 * 		  char att_right: Atenuacion del canal derecho entre (0 - 96dB)
 * @return OP_STATE:
 * 		   -OP_END: Fin de opreacion
 * 		   -OP_ERROR: Atenuacion fuera de escala
 */
OP_STATE sta013_VOL(char att_tone, char att_left, char att_right);


inline uint8_t sta013_IsAvailable();

#endif /* STA013_API_H_ */
