/*
 * sta013_driver.c
 *
 *  Created on: 06/11/2014
 *      Author: Maximiliano
 */

/*
 * @note Includes
 */
#include <sta013_driver.h>

/*
 * @note Defines
 */

/*
 * @note Typedefs & enums
 */

/*
 * @note Variables globales
 */

/*
 * @note Funciones publicas
 */


/*
 *@fn void sta013_SSPInit(void);
 *@brief Inicializacion del SSP
 *@param void
 *@return void
 */
void sta013_SSPInit (void)
{
	PINSEL_CFG_Type pin;
	SSP_CFG_Type conf;

	//-----------PINSEL----------
	pin.Funcnum = SSP_FUNC;
	pin.OpenDrain = PINSEL_PINMODE_NORMAL;
	pin.Pinmode = PINSEL_PINMODE_PULLUP;
	pin.Portnum = SSP_PORT;
	pin.Pinnum = STA_SDO_PIN;

	PINSEL_ConfigPin(&pin);

	pin.Pinnum = STA_SCKT_PIN ;

	PINSEL_ConfigPin(&pin);

	//------------SSP-----------
	//TODO: CORRECCIONES EN POLARIDAD Y FASE DE SSP
	conf.CPHA = SSP_CPHA_FIRST;
	conf.CPOL = SSP_CPOL_LO;
	conf.ClockRate = SSP_ClkRate;
	conf.FrameFormat = SSP_FRAME_SPI;
	//TODO: Puede haber problemas con el protocolo SPI de TI o MOTOROLA
	conf.Mode = SSP_MASTER_MODE;
	conf.Databit = SSP_DATABIT_8;

	SSP_Init( sta013_SSP , &conf );

	//----------Activo SSP-------------------
	SSP_Cmd( sta013_SSP , ENABLE );

	return;
}


/*
 * @fn void sta013_I2CInit(void)
 * @brief Inicializacion de I2C
 * @param void
 * @return void
 */
void sta013_I2CInit(void)
{
	PINSEL_CFG_Type pin;

	//------------PINSEL---------------
	pin.Funcnum = I2C_FUNC;
	pin.OpenDrain = PINSEL_PINMODE_NORMAL;
	pin.Pinmode = PINSEL_PINMODE_PULLUP;
	pin.Portnum = I2C_PORT;
	pin.Pinnum = STA_SDA_PIN;

	PINSEL_ConfigPin(&pin);

	pin.Pinnum = STA_SCL_PIN;

	PINSEL_ConfigPin(&pin);

	//-------------I2C-----------------
	 I2C_Init( sta013_I2C , I2C_ClkRate );

	//----------Activo I2C-------------
	 I2C_Cmd( sta013_I2C , ENABLE);

	return;
}


/*
 * @fn void sta013_GPIOInit(void)
 * @brief Inicializacion de los GPIO
 * @param void
 * @return void
 */
void sta013_GPIOInit(void)
{
	//-----------Seteo de direcciones-----------

	FIO_SetDir( STA_DATA_REQ_PORT , pin_port(STA_DATA_REQ_PIN) , entrada );
	FIO_SetDir( STA_RST_PORT , pin_port(STA_RST_PIN) , salida );

	GPIO_SetDir(MUXSW_PORT,pin_port(MUX_SW2_PIN),salida);
	GPIO_SetDir(MUXSW_PORT,pin_port(MUX_SW1_PIN),salida);

	//----------------Seteo SSEL----------------
	FIO_SetDir( BIT_EN_PORT , pin_port(BIT_EN_PIN) , salida );

	//-----------Condiciones iniciales----------
	//Sin reseteo inicial
	FIO_SetValue( STA_RST_PORT , pin_port(STA_RST_PIN) );
	//BIT_EN del sta013 en alto, para habilitar recepcion de datos seriales
	FIO_SetValue( BIT_EN_PORT , pin_port(BIT_EN_PIN) );

	//Seteo de Multiplexores
	GPIO_ClearValue(MUXSW_PORT,pin_port(MUX_SW1_PIN));
	GPIO_ClearValue(MUXSW_PORT,pin_port(MUX_SW2_PIN));

	return;
}

/*
 * @fn OP_STATE sta013_Receive( char *buffer )
 * @brief Funcion de recepcion del STA013
 * @param
 *        char *buffer: Posicion de memoria al buffer que contendra el valor recivido
 * @return OP_STATE
 *                 -OP_END: Fin de recepcion
 *                 -OP_BUSY: Recepcion en transito
 */

OP_STATE sta013_Receive( char *buffer )
{
	I2C_M_SETUP_Type setup;

	setup.rx_data = (uint8_t*) buffer;
	setup.rx_length = sizeof(*buffer);
	setup.sl_addr7bit = (addr_slv>>1);
	setup.tx_data = NULL;
	setup.tx_length = 0;
	setup.retransmissions_max = 3;
	if( I2C_MasterTransferData( sta013_I2C	, &setup , I2C_TRANSFER_POLLING) == SUCCESS )
	{
		return (OP_END);
	}
	else
	{
		return(OP_BUSY);
	}
}

//TODO: PLANEO SACAR LA FUNCION APRA HACERLERAR EL STREAMING
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
/*OP_STATE strm_Ready(void){

	//Si DATA_REQ se encuentra en alto se pueden enviar datos al STA013
	if(get_pin( FIO_ReadValue( STA_DATA_REQ_PORT ) , STA_DATA_REQ_PIN ) )
		return(OP_READY);
	else
		return(OP_BUSY);

}*/



/*
 * @fn OP_STATE sta013_Send(char *buff , uint32_t size)
 * @brief Funcion de envio de envio al STA013
 * @param
 * 		  -   char *buff; Direccion de memoria del buffer de datos a transmitir
 *        - uint32_t size: cantidad de bytes que contiene el buffer
 * @return
 *         OP_STATE:
 *         			- OP_END: Fin de la operacion
 *         			= OP_BUSY: Operacion en proceso
*/

OP_STATE sta013_Send(char *buff , uint32_t size)
{
	I2C_M_SETUP_Type setup;

	setup.rx_data = NULL;
	setup.rx_length = 0;
	setup.sl_addr7bit = (addr_slv>>1);
	setup.tx_data = (uint8_t *) buff;
	setup.tx_length = size;
	setup.retransmissions_max = 3;
	if( I2C_MasterTransferData( sta013_I2C	, &setup , I2C_TRANSFER_POLLING) == SUCCESS )
	{
	  return(OP_END);
	}
	else
	{
	  return(OP_BUSY);
	}
}


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
/*OP_STATE sta013_strm (  char *block , uint32_t block_size){

	SSP_DATA_SETUP_Type setup;

	//Fuerzo comienzo de transmision

	setup.length = block_size;
	setup.rx_data = NULL;
	setup.tx_data = (uint8_t*) block;

	SSP_ReadWrite( sta013_SSP , &setup , SSP_TRANSFER_POLLING);

	return(OP_END);

}
*/


/*
 * @note Funciones privadas
 */

/*
 * @fn static void sta013_HwRst(void)
 * @brief Hard reset del sta013
 * @param void
 * @return void
 */
void sta013_HwRst(void)
{
	uint32_t i;

	FIO_ClearValue( STA_RST_PORT , pin_port(STA_RST_PIN) );

	for(i = 0; i< 1000 ; i++);

	FIO_SetValue( STA_RST_PORT , pin_port(STA_RST_PIN) );

	return;
}


/*
 * @fn void SSEL_force( PIN_STATE stat )
 * @brief Funcion para forzar accion de SSEL, de utilizacion en la comunicacion
 * 		  como master del SSP
 * @param PIN_STATE stat: estado al que quiere forzarse el SSEL
 * @return void
 */
/*
void SSEL_force( PIN_STATE stat){

	if(stat == ON)
		FIO_SetValue( STA_SSEL_PORT , pin_port(STA_SSEL_PIN) );
	else
		FIO_ClearValue( STA_SSEL_PORT , pin_port(STA_SSEL_PIN) );

	return;
}
*/

