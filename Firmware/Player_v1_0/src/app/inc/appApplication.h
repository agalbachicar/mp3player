/**
 * @author	Agustin Alba Chicar
 * @file	appAplication.h
 * @brief	Archivo de cabecera de la tarea principal de aplicacion
 * @date	Enero/2015
 * @version	1
 */

#ifndef	 	APP_APPLICATION_H
#define		APP_APPLICATION_H

/**
 * @note        Includes
 */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "appConfig.h"
#include "Player.h"

#include "tim.h"

#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"

#include <stdio.h>
#include <stdint.h>


/**
 * @note        Defines
 */
#ifndef ERR
        #define ERR     ((int32_t)-1)
#endif

/**
 * @def		APP_APPLICATION_STACKSIZE
 * @brief	Es el tamaño en bytes del stack de la aplicacion
 */
#define		APP_APPLICATION_STACKSIZE				1500
/**
 * @def		APP_APPLICATION_PRIORITY
 * @brief	Es la prioridad de la tarea de la aplicacion
 */
#define		APP_APPLICATION_PRIORITY				2
/**
 * @def		APP_APPLICATION_TIMER
 * @brief	Definicion del timer que utiliza la API para el control de las entradas
 */
#define		APP_APPLICATION_TIMER					1
/**
 * @def		APP_APPLICATION_SAMPLE_GPIO_PERIOD_US
 * @brief	Es el periodo en microsegundos cada cuanto se samplean los botones
 */
#define		APP_APPLICATION_SAMPLE_GPIO_PERIOD_US	50000
/**
 * @def		APP_APPLICATION_GPIO_PORT
 * @brief	Es el puerto de los botones
 */
#define		APP_APPLICATION_GPIO_PORT				0
/**
 * @def		APP_APPLICATION_DOWN_GPIO_PIN
 * @brief	Es el pin del boton abajo
 */
#define		APP_APPLICATION_DOWN_GPIO_PIN			26
/**
 * @def		APP_APPLICATION_OK_GPIO_PIN
 * @brief	Es el pin del boton OK
 */
#define		APP_APPLICATION_OK_GPIO_PIN				25
/**
 * @def		APP_APPLICATION_UP_GPIO_PIN
 * @brief	Es el pin del boton arriba
 */
#define		APP_APPLICATION_UP_GPIO_PIN				23
/**
 * @def		APP_APPLICATION_BACK_GPIO_PIN
 * @brief	Es el pin del boton atras
 */
#define		APP_APPLICATION_BACK_GPIO_PIN			24
/**
 * @def		APP_APPLICATION_DEBOUNCE_MASK
 * @brief	Mascara de flags activos
 */
#define		APP_APPLICATION_DEBOUNCE_MASK			((uint8_t)0x03)
/**
 * @def		APP_APPLICATION_MUX_PORT
 * @brief	El puerto de los pines del multiplexor
 */
#define		APP_APPLICATION_MUX_PORT				0
/**
 * @def		APP_APPLICATION_MUX_PIN_1
 * @brief	Numero del pin de control del multiplexor
 */
#define		APP_APPLICATION_MUX_PIN_1				21
/**
 * @def		APP_APPLICATION_MUX_PIN_2
 * @brief	Numero del pin de control del multiplexor
 */
#define		APP_APPLICATION_MUX_PIN_2				22
/**
 * @def		APP_APPLICATION_PINS
 * @brief	La mascara para modificar el estado del multiplexor
 */
#define		APP_APPLICATION_PINS					((1 << APP_APPLICATION_MUX_PIN_1) | (1 << APP_APPLICATION_MUX_PIN_2))

/**
 * @note        Tipos de datos, estructuras, typdefs
 */

/**
 * @enum	appApplicationEState
 * @brief	Mantiene el estado de la aplicacion para concer en que ventana y modos esta el programa
 */
typedef enum appApplicationEnumState
{
	SELECT_MODE = 0,			/**< Pantalla de seleccion de modo */
	SD_MODE = 1,				/**< Modo de reproduccion por la SD. */
	ETHERNET_MODE = 2,			/**< Modo de reproduccion por Ethernet */
	CONFIGURATION_MODE = 3,		/**< Modo de configuracion */
	HELP_MODE = 4,				/**< Modo de reproduccion por auxiliar. */
}appApplicationEState;

/**
 * @enum	appApplicationEButton
 * @brief	Es la definicion de la funcionalidad de cada boton presionado
 */
typedef enum appApplicationEnumButton
{
	BUTTON_NONE = -1,				/**< No se presiono ningun boton */
	BUTTON_UP = 0,					/**< Boton arriba presionado. */
	BUTTON_DOWN = 1,				/**< Boton abajo presionado. */
	BUTTON_BACK = 2,				/**< Boton atras presionado. */
	BUTTON_OK = 3,					/**< Boton OK o aceptar presionado. */
	BUTTON_BACKWARD = BUTTON_DOWN,	/**< Boton anterior cancion presionado. */
	BUTTON_FORWARD = BUTTON_UP,		/**< Boton siguiente cancion presionado. */
	BUTTON_PLAYPAUSE = 3,			/**< Boton play/pausa presionado. */
}appApplicationEButton;

/**
 * @enum	appApplicationESDState
 * @brief	Es la definicion de los posibles estados tras trabajar con la SD
 */
typedef enum appApplicationEnumSDState
{
	SD_INIT = 0,
	SD_GET_MP3_FILES = 1,
	SD_EMPTY = 2,
	SD_SHOW_MP3_FILES = 3,
	SD_PLAY = 4,
	SD_ERROR = 5,
}appApplicationESDState;

typedef enum appApplicationEnumPlayerState
{
	INIT = 0,
	GET_MP3_FILES = 1,
	EMPTY = 2,
	SHOW_MP3_FILES = 3,
	PLAY = 4,
	GET_NEXT_SONG = 5,
	ERROR_PLAYER = 6,
	EXIT = 7,
	PLAYING = 8,
	PAUSED = 9,
	GET_PREVIOUS_SONG = 10,
	GET_PREV_MP3_FILES = 11,
	ERROR_BLOCK = 12,
}appApplicationEPlayerState;



/**
 * @note        Prototipos de funciones publicas
 */

void appApplicationGeneralTask(void *argument);
void appApplicationPlayerTask(void *arguments);

void appApplicationTimerHandler();

void appApplicationTestTask(void *argument);

/**
 * @note        Prototipos de funciones privadas
 */

void appApplicationSetOutput(appApplicationEState state);

void appApplicationConfiguration();

void printWelcomeScreen();
void printSelectScreen(int option);
void printSDScreen();
void printEthernetScreen();
void printHelpScreen();
void printConfigurationScreen();
void printWaitingConfigurationScreen();

void printError(playerEType playerType);
void printEmpty();
void printPlay();
void printSongList(int currentOption, playerTSong *songList, playerEType playerType, int32_t lastSongTitle);
void printPlaySong(playerTSong *song);

/**
 * @note        Variables publicas
 */
extern xSemaphoreHandle appApplicationButtonSemaphore;


#endif	/* APP_APPLICATION_H */
