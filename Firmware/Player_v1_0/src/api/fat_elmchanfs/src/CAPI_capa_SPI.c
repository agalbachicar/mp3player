#include <CAPI_Definitions.h>
#include <SSP.h>
#include <GPIO.h>
#include <lpc_176X_PinNames.h>
#include <lpc_176X_PeriphNames.h>
#include <CAPI_capa_SPI.h>

/* Local functions */
static uint8_t CAPI_SPI_SendRecvByte (uint8_t byte_s);

static GPIO slaveSelect;
SSP tarjeta;

void CAPI_SPI_Init (void) 
{
	SSPInit(&tarjeta, SSP_1, SPI_MASTER, 400000, 8, CPOL_HIGH, CPHA_FIRST_T);
	//GPIO_Init(&slaveSelect, SALIDA, BAJO, GPIO__0_6);
	GPIO_Init(&slaveSelect, SALIDA, BAJO, GPIO__2_2);
	CAPI_SPI_DeSelect ();

	while(SSPisBusy(&tarjeta)) // wait for busy gone
		;

	SSPvaciarFIFO(&tarjeta); // drain SPI RX FIFO
}

void CAPI_SPI_DeInit( void )
{
//FIXME: Agregale a CAPI un DeInitPeripheral
#if 0
	// disable SPI
	LPC_SSP1->CR1  = 0;

	// Pins to GPIO
	LPC_PINCON->PINSEL0 &= ~(3UL<<30);
	LPC_PINCON->PINSEL1 &= ~((3<<2) | (3<<4));

	// disable SSP power
	LPC_SC->PCONP &= ~(1 << 21);
#endif
}

void CAPI_SPI_Select ()
{
	Activar(&slaveSelect);
}

void CAPI_SPI_DeSelect ()
{
	Pasivar(&slaveSelect);
}

//FIXME: Esto no tiene que existir, tenco SSPexchangei
/* Send one byte then recv one byte of response. */
static uint8_t CAPI_SPI_SendRecvByte (uint8_t byte_s)
{
	return (uint8_t) SSPexchangei(&tarjeta, (short int)byte_s);
}

//FIXME: Esto no tiene que existir, tenco SSputi
void CAPI_SPI_SendByte (uint8_t data)
{
	CAPI_SPI_SendRecvByte (data);
	//SSPputi(&tarjeta, (short int) data);
}

//FIXME: Esto no tiene que existir, tenco SSgeti
uint8_t CAPI_SPI_RecvByte ()
{
	return CAPI_SPI_SendRecvByte (0xFF);
	//return (uint8_t) SSPgeti(&tarjeta);
}

void CAPI_SPI_Release (void)
{
	CAPI_SPI_DeSelect ();
	CAPI_SPI_RecvByte ();
}

