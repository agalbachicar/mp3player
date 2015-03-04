/*
 * sta013_driver.h
 *
 *  Created on: 06/11/2014
 *      Author: Maximiliano
 */

#ifndef STA013_DRIVER_H_
#define STA013_DRIVER_H_

/*
 *@note Includes
 */
#include <lpc17xx_ssp.h>
#include <lpc17xx_pinsel.h>
#include <lpc17xx_gpio.h>
#include <lpc17xx_i2c.h>

#include <sta_regs.h>


/*
 * @note Defines
 */

//---------------sta013 ADDR_SLAVE-------------
#define addr_slv ((char) 0x086)

/*
 * @note Macros
 */

/*
 * @fn get_pin ( port , pin )
 * @brief Macro para a partir de una word obtener el valor de un pin
 * @param uint32_t port: Word representativa del port
 * 	 	  uint8_t pin: Locacion del pin
 * @return uint8_t : Estado del pin
 */
#define get_pin(port,pin) (((port >> pin) & 1) == 1 )?  1:0

/*
 * @fn pin_port (  pin )
 * @brief Macro para obtener word con pin_bit en "1"
 * @param uint32_t pin: Numero de pin a poenr en "1"
 * @return uint32_t con ceros y en bit pin en "1"
 */
#define pin_port(pin) (1<<pin)

//--------------------SSP---------------------
#define sta013_SSP LPC_SSP0
#define SSP_PORT PINSEL_PORT_0 //P0[]
#define STA_SDO_PIN PINSEL_PIN_18 //MOSI P0[18]
#define STA_SCKT_PIN PINSEL_PIN_15 //SCL P0[15]
#define SSP_FUNC PINSEL_FUNC_2 //Funcion para el PINSEL

#define SSP_ClkRate 5000000 //5MHz

//--------------------I2C---------------------
#define sta013_I2C LPC_I2C2
#define I2C_PORT PINSEL_PORT_0 //P0[]
#define STA_SDA_PIN PINSEL_PIN_10 //P0[10]
#define STA_SCL_PIN PINSEL_PIN_11 //P0[11]
#define I2C_FUNC PINSEL_FUNC_2 //Funcion para el PINSEL

#define I2C_ClkRate 100000 //100 KHz

//--------------------GPIO--------------------
#define STA_DATA_REQ_PORT 0 //P0[]
#define STA_DATA_REQ_PIN 2 //P0[2]

#define BIT_EN_PORT 0 //P0[]
#define BIT_EN_PIN 16 //P0[16]

#define STA_RST_PORT 0 //P0[]
#define STA_RST_PIN 3 //P0[3]

#define MUXSW_PORT 0 //P0[]
#define MUX_SW1_PIN 21 //P0[21]
#define MUX_SW2_PIN 22 //P0[22]
//--------------------------------------------

/*
 * @note Enums & typedefs
 */

/*
 * @enum OP_STATE
 * @brief Estado de operacion
 */
typedef enum {
	OP_BUSY, //Operacion en proceso
	OP_ERROR, //Operacion terminada por error
	OP_READY, //Operacion lista para comenzar
	OP_END //Operacion termoinada con exito
} OP_STATE;


/*
 * @enum PIN_STATE
 * @brief Estado de pin
 */
typedef enum{
	ON = 1,
	OFF = 0
} PIN_STATE;

/*
 * @enum PIN_DIR
 * @brief Direccion del pin
 */
typedef enum{
	salida = 1,
	entrada = 0
}PIN_DIR;

/*
 * @note Funciones publicas
 */

/*
 *@fn void sta013_SSPInit(void);
 *@brief Inicializacion del SSP
 *@param void
 *@return void
 */
void sta013_SSPInit (void);

/*
 * @fn void sta013_I2CInit(void)
 * @brief Inicializacion de I2C
 * @param void
 * @return void
 */
void sta013_I2CInit(void);

/*
 * @fn void sta013_GPIOInit(void)
 * @brief Inicializacion de los GPIO
 * @param void
 * @return void
 */
void sta013_GPIOInit(void);

/*
 * @fn OP_STATE sta013_Receive( char *buffer )
 * @brief Funcion de recepcion del STA013
 * @param
 *        char *buffer: Posicion de memoria al buffer que contendra el valor recivido
 * @return OP_STATE
 *                 -OP_END: Fin de recepcion
 *                 -OP_BUSY: Recepcion en transito
 */
OP_STATE sta013_Receive( char *buffer);

//TODO: PROBARE SACANDO LA FUNCION Y EJECUTANDO DIRECTAMENTE
/*
 * @fn OP_STATE strm_Ready(void)
 * @brief Funicion para verificar si puede enviarse datos por streaming al sta013
 * @param void
 * @return OP_STATE:
 *                  - OP_READY: El STA013 se encuentra en condiciones
 *                  de recibir datos, tiene lugar en su buffer
 *                  - OP_BUSY: El STA013 no se encuentra en condiciones
 *                  de recibir datos, buffer propio lleno
 */
//OP_STATE strm_Ready(void);


/*
 * @fn OP_STATE sta013_Send(   char *buff , uint32_t size)
 * @brief Funcion de envio de envio al STA013
 * @param
 * 		  -   char *  buff; Direccion de memoria del buffer de datos a transmitir
 *        - uint32_t size: cantidad de bytes que contiene el buffer
 * @return
 *         OP_STATE:
 *         			- OP_END: Fin de la operacion
 *         			= OP_BUSY: Operacion en proceso
*/
OP_STATE sta013_Send(char *buff , uint32_t size);

//TODO: PRUEBO SACANDO ESTA FUNCION PARA REDUCIR TIEMPOS
/*
 * @fn OP_STATE sta013_strm(   char *block , uint32_t block_size)
 * @param
 *        -   char *block: Direccion de memoria al bloque que se desea transmitir
 *        - uint32_t block_size: Cantdiad de bytes que posee el bloque a transmitir
 * @return OP_STATE:
 *         - OP_END: Envio del bloque terminado
 *         - OP_BUSY: Envio en transito
 */
//OP_STATE sta013_strm ( char *block , uint32_t block_size);


/*
 * @note Funciones privadas
 */

/*
 * @fn static void sta013_HwRst(void)
 * @brief Hard reset del sta013
 * @param void
 * @return void
 */
void sta013_HwRst(void);



/*
 * @fn void SSEL_force( PIN_STATE stat )
 * @brief Funcion para forzar accion de SSEL, de utilizacion en la comunicacion
 * 		  como master del SSP
 * @param PIN_STATE stat: estado al que quiere forzarse el SSEL
 * @return void
 */
//void SSEL_force( PIN_STATE stat);

#endif



