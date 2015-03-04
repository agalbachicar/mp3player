/**
 * @author	Agustin Alba Chicar
 * @file	appApplication.c
 * @brief	Contiene el codigo de aplicacion que vincula la interfaz de usuario con la logica del programa
 * @date	Enero/2015
 * @version	1
 */

/**
 * @note Includes
 */
#include "appApplication.h"

/**
 * @note Variables globales
 */
extern dir_type actual_dir;


/**
 *	@var	appApplicationButtonPressed
 *	@brief	Contiene el estado del boton presionado
 */
appApplicationEButton appApplicationButtonPressed = BUTTON_NONE;
/**
 *	@var	appApplicationButtonSemaphore
 *	@brief	Sincroniza la presion de los botones con la maquina de estados principal
 */
xSemaphoreHandle appApplicationButtonSemaphore;

/**
 * @var		appApplicationPlayer
 * @brief	Estructura con las funciones a utilizar segun el caso para cada modo del player
 */
playerTFunction appApplicationPlayer;

static char appApplicationNetworkInterfaces[3][20];

/**
 * @note Funciones publicas
 */

/**
 * @fn		appApplicationGeneralTask(void *argument)
 * @brief	Tarea de aplicacion. Controla el flujo del programa segun como interactua el usuario
 * @param	argument	No se utiliza
 */
void appApplicationGeneralTask(void *argument)
{
	appApplicationEState state = SELECT_MODE;
	uint8_t keep = 0;
	int option = 0;

	//Imprimo la pantalla de
	printWelcomeScreen();
	//Espero un delay de un segundo
	vTaskDelay( 1000 / portTICK_RATE_MS );
	//Imprimo la pantalla de seleccion de modo
	printSelectScreen(option);

	//Corro el timer de la aplicacion
	timerRun(APP_APPLICATION_TIMER);

	//Tomo el semaforo la primera vez para volver a bloquearlo luego
	xSemaphoreTake( appApplicationButtonSemaphore, portMAX_DELAY );

	while(1)
	{
		//Tomo el semaforo
		if(xSemaphoreTake( appApplicationButtonSemaphore, portMAX_DELAY ) != pdPASS)
		{
			continue;
		}
		while(1)
		{
			//Evaluo el estado
			switch(state)
			{
				case SELECT_MODE:
					switch(appApplicationButtonPressed)
					{
						case BUTTON_UP:
							option++;
							if(option >= 4)
							{
								option = 0;
							}
							printSelectScreen(option);
							keep = 0;
							break;

						case BUTTON_DOWN:
							option--;
							if(option < 0)
							{
								option = 3;
							}
							printSelectScreen(option);
							keep = 0;
							break;

						case BUTTON_OK:
							if(option == 0)
							{
								state = SD_MODE;
								playerLoadPointers(SD_PLAYER, &appApplicationPlayer);
								printSDScreen();
								keep = 1;
							}
							else if(option == 1)
							{
								state = ETHERNET_MODE;
								playerLoadPointers(ETHERNET_PLAYER, &appApplicationPlayer);
								printEthernetScreen();
								keep = 1;
							}
							else if(option == 2)
							{
								state = CONFIGURATION_MODE;
								keep = 1;
							}
							else if(option == 3)
							{
								state = HELP_MODE;
								printHelpScreen();
								keep = 0;
							}

							//Seteo el multiplexor al modo correspondiente
							appApplicationSetOutput(state);
							break;

						case BUTTON_BACK:
						default:

							break;
					}
					break;

				case ETHERNET_MODE:
				case SD_MODE:
					appApplicationPlayerTask(NULL);

					state = SELECT_MODE;
					printSelectScreen(option);
					//Seteo el multiplexor al modo correspondiente
					appApplicationSetOutput(state);
					keep = 0;
					break;

				case HELP_MODE:
					if(appApplicationButtonPressed == BUTTON_BACK)
					{
						state = SELECT_MODE;
						printSelectScreen(option);
						//Seteo el multiplexor al modo correspondiente
						appApplicationSetOutput(state);
					}
					keep = 0;
					break;

				case CONFIGURATION_MODE:
					//Entro en el modo configuracion
					appApplicationConfiguration();
					//Vuelvo a la pantalla de seleccion de modo
					state = SELECT_MODE;
					printSelectScreen(option);
					//Seteo el multiplexor al modo correspondiente
					appApplicationSetOutput(state);
					keep = 0;
					break;
			}

			if(!keep)
			{
				break;
			}
		}

	}
}

void appApplicationPlayerTask(void *arguments)
{
	appApplicationEPlayerState status = INIT;
	appApplicationEPlayerState songPlayerStatus;
	playerTSong songs[3];
	int32_t i, result, currentOption, lastSongTitle;

	//Inicializo las estructuras en cero
	memset(songs, 0, sizeof(playerTSong) * 3);

	while(1)
	{
		switch(status)
		{
			case INIT:
				//Inicializo el periferico
				if(appApplicationPlayer.init() != 0)
				{
					//Se produjo un error al inicializar el periferico
					//Voy al estado de error
					status = ERROR_PLAYER;
				}
				else
				{
					//Coloco la variable en su estado inicial
					lastSongTitle = -1;
					songPlayerStatus = PLAYING;
					status = GET_MP3_FILES;
				}

				break;

			case GET_MP3_FILES:
				//Obtengo tres canciones
				for(i = 0; i < 3; i++, lastSongTitle ++)
				{
					//Obtengo la cancion
					if(i == 0)
					{
						//Evaluo el estado de la cancion numero 3 ya que puede ser que ya haya pedido
						//temas
						if(lastSongTitle == -1 || songs[lastSongTitle].id == 0)
						{
							result = appApplicationPlayer.getNextSong(NULL, &(songs[i]));
						}
						else
						{
							result = appApplicationPlayer.getNextSong(&(songs[lastSongTitle]), &(songs[i]));
						}
						//Coloco la variable en menos 1 para saber para que se incremente si el pedido fue exitoso
						lastSongTitle = -1;
					}
					else
					{
						result = appApplicationPlayer.getNextSong(&(songs[i-1]), &(songs[i]));
					}

					if(result == NO_SONG)
					{
						//Ya no hay mas canciones, aviso que tengo que mostrar hasta la anterior
						break;
					}
					else if(result != OK)
					{
						//Se produjo un error al obtener una cancion
						//Voy al estado de error
						status = ERROR_PLAYER;
						//Salgo
						break;

					}
					//En casod e OK contrasto contra anteriro para no repetir canciones
					else if(result == OK){
							if(i){
								if(songs[i].id == songs[0].id) 	break;
							}
						}
				}
				if(status == GET_MP3_FILES)
				{
					//Paso a mostrar las canciones
					currentOption = 0;
					//Muestro la lista de canciones
					printSongList(currentOption, songs, appApplicationPlayer.type, lastSongTitle);
					//Paso al estado de las canciones
					status = SHOW_MP3_FILES;
				}
				break;

			case GET_PREV_MP3_FILES:
				//Obtengo tres canciones anteriores, si hay que falla este modo falla.
				//Si tengo una cancion antes, deberia tener 3 porque pagino de a 3 siempre
				for(i = 2, currentOption = -1, lastSongTitle = -1; i < 3; i--, currentOption++, lastSongTitle ++)
				{

					if(i == 2) result = appApplicationPlayer.getPreviousSong(&(songs[0]), &(songs[2]));
					else{
						result = appApplicationPlayer.getPreviousSong(&(songs[i+1]), &(songs[i]));
					}

					if(result == NO_SONG)
					{
						break;
					}
					else if(result != OK)
					{
						status = ERROR_PLAYER;
						break;
					}
				}

				if(status == GET_PREV_MP3_FILES)
				{
					if(i == -1)
					{
						//Tengo dos canciones
						lastSongTitle = 2;
						//Paso a mostrar las canciones
						if(currentOption == -1)
							currentOption = 0;
						else
							currentOption = 2;
						//Muestro la lista de canciones
						printSongList(currentOption, songs, appApplicationPlayer.type, lastSongTitle);
						//Paso al estado de las canciones
						status = SHOW_MP3_FILES;
					}
					else
					{
						lastSongTitle = -1;
						//Paso al estado de las canciones
						status = GET_MP3_FILES;
					}
				}
				break;



			case SHOW_MP3_FILES:
				//Tomo el semaforo
				if(xSemaphoreTake( appApplicationButtonSemaphore, portMAX_DELAY ) != pdPASS)
				{
					continue;
				}
				//Evaluo el boton presionado
				switch(appApplicationButtonPressed)
				{
					case BUTTON_BACK:
						//Paso el estado a no inicializado
						status = EXIT;
						break;

					case BUTTON_DOWN:
						//Incremento la opcion actual
						currentOption --;
						if(currentOption < 0)
						{
							//Obtenemos las siguientes canciones
							status = GET_PREV_MP3_FILES;
							//Avanzo en la iteracion
							continue;
						}
						//Imprimos el listado de canciones
						printSongList(currentOption, songs, appApplicationPlayer.type, lastSongTitle);
						break;

					case BUTTON_UP:
						currentOption ++;
						if(currentOption  > lastSongTitle)
						{
							//Obtenemos las siguientes canciones
							status = GET_MP3_FILES;
							//Avanzo en la iteracion
							continue;
						}
						//Imprimos el listado de canciones
						printSongList(currentOption, songs, appApplicationPlayer.type, lastSongTitle);
						break;

					case BUTTON_OK:
						//Pasamos al PLAY
						status = PLAY;
						appApplicationButtonPressed = BUTTON_NONE;
						//Imprimimos en panta la  cancion
						printPlaySong(&(songs[currentOption]));
						break;
				}
				break;

			case PLAY:
				//Preparamos  la cancion
				result = appApplicationPlayer.prepaireSong(&(songs[currentOption]));
				if(result != 0)
				{
					//Voy al estado de error
					status = ERROR_PLAYER;
					//Salgo
					break;
				}
				//Paso el estado a reproduccion
				songPlayerStatus = PLAYING;

				//Evaluamos el estado de reproduccion y corrermos la aplicacion
				while(1)
				{
					if(songPlayerStatus == PLAYING)
					{

						//Movida de ir pidiendo pedazos y enviando con la tarea de reproduccion
						result = appApplicationPlayer.playSong(&(songs[currentOption]));

						if(result == DIR_CHANGED){

							status = GET_MP3_FILES;
							break;
						}


						if(result == END_OF_FILE)
						{
							status = GET_NEXT_SONG;
							break;
						}
						if(result != OK)
						{
							//Bajamos la cancion actual
							appApplicationPlayer.disposeSong(&(songs[currentOption]));
							//Voy al estado de error
							status = ERROR_PLAYER;
							//Salgo
							break;
						}
					}
					if(appApplicationButtonPressed == BUTTON_BACKWARD)
					{
						appApplicationButtonPressed = BUTTON_NONE;
						status = GET_PREVIOUS_SONG;
						break;
					}
					else if(appApplicationButtonPressed == BUTTON_FORWARD)
					{
						//Pasamos a la siguiente cancion
						appApplicationButtonPressed = BUTTON_NONE;
						status = GET_NEXT_SONG;
						break;
					}
					else if(appApplicationButtonPressed == BUTTON_PLAYPAUSE)
					{
						//Cambio de estado el reproductor
						if(songPlayerStatus == PLAYING)
						{
							songPlayerStatus = PAUSED;
							//Tomo el semaforo hasta que se presione algo
							//while(!(xSemaphoreTake( appApplicationButtonSemaphore, portMAX_DELAY ) == pdPASS && appApplicationButtonPressed == BUTTON_PLAYPAUSE));
							appApplicationButtonPressed = BUTTON_NONE;
						}
						else
						{
							//Paso a reproducir nuevamnente la cancion.
							songPlayerStatus = PLAYING;
							appApplicationButtonPressed = BUTTON_NONE;
						}
					}
					else if(appApplicationButtonPressed == BUTTON_BACK)
					{

						//Bajamos la cancion actual
						appApplicationPlayer.disposeSong(&(songs[currentOption]));
						//Vuelvo a mostrar la lista de canciones
						lastSongTitle = -1;
						appApplicationButtonPressed = BUTTON_NONE;
						status = GET_MP3_FILES;
						break;
					}
				}
				break;

			case GET_NEXT_SONG:
				//Bajamos la cancion actual
				result = appApplicationPlayer.disposeSong(&(songs[currentOption]));
				if(result != 0)
				{
					//Voy al estado de error
					status = ERROR_PLAYER;
					//Salgo
					break;
				}
				//Obtenemos la siguiente cancion
				currentOption++;
				if(currentOption > 2)
				{
					currentOption = 0;
					result = appApplicationPlayer.getNextPlayingSong(&(songs[2]), &(songs[0]));

				}
				else
				{
					result = appApplicationPlayer.getNextPlayingSong(&(songs[currentOption-1]), &(songs[currentOption]));
				}
				if(result == OK)
				{
					//Imprimimos en pantalla la cancion
					printPlaySong(&(songs[currentOption]));
					//Pasamos a reproducirla nuevamente
					status = PLAY;
				}
				else if(result == NO_SONG)
				{
					//Ya terminamos de reproducir las canciones
					//Pasamos al estado de muestra de las mismas
					lastSongTitle = -1;
					status = GET_MP3_FILES;
				}
				else if(result != OK)
				{
					//Voy al estado de error
					status = ERROR_PLAYER;
					//Salgo
					break;
				}
				break;

			case GET_PREVIOUS_SONG:
				//Bajamos la cancion actual
				result = appApplicationPlayer.disposeSong(&(songs[currentOption]));
				if(result != 0)
				{
					//Voy al estado de error
					status = ERROR_PLAYER;
					//Salgo
					break;
				}
				//Obtenemos la siguiente cancion
				currentOption--;
				if(currentOption < 0)
				{
					currentOption = 2;
					result = appApplicationPlayer.getPreviousPlayingSong(&(songs[0]), &(songs[2]));
				}
				else
				{
					result = appApplicationPlayer.getPreviousPlayingSong(&(songs[currentOption + 1]), &(songs[currentOption]));
				}
				if(result == OK)
				{
					//Imprimimos en pantalla la cancion
					printPlaySong(&(songs[currentOption]));
					//Pasamos a reproducirla nuevamente
					status = PLAY;
				}
				else if(result == NO_SONG)
				{
					//Ya terminamos de reproducir las canciones
					//Pasamos al estado de muestra de las mismas
					lastSongTitle = -1;
					status = GET_MP3_FILES;
				}
				else if(result != OK)
				{
					//Voy al estado de error
					status = ERROR_PLAYER;
					//Salgo
					break;
				}
				break;

			case ERROR_PLAYER:
				printError(appApplicationPlayer.type);
			case ERROR_BLOCK:
				status = ERROR_BLOCK;
				//Tomo el semaforo hasta que se presione algo
				if(xSemaphoreTake( appApplicationButtonSemaphore, portMAX_DELAY ) != pdPASS)
				{
					continue;
				}
				//Cambio de estado el boton a nada
				appApplicationButtonPressed = BUTTON_NONE;
				//Paso al estado de salida
				status = EXIT;
				break;

			case EXIT:
				//Desinicializo el periferico
				appApplicationPlayer.deinit();
				return;
				break;
		}
	}
}

#ifdef	TEST_MODE
/**
 * @fn		appApplicationTestTask(void *arguments)
 * @brief	Tarea de test de la maquina de estados de la aplicacion
 * @param	argument	No se utiliza
 */
void appApplicationTestTask(void *arguments)
{
	int i;
	/**
	 * Test 1: Recorro en Ethernet hasta listar la primer pagina de canciones.
	 */
#if(TEST_ID == 1)
	while(1)
	{
		//Delay de 3 segundos
		vTaskDelay( 3 * 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_BACK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_UP;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_OK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_OK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		vTaskSuspend( NULL );
	}
#endif
	/**
	 * Test 2: recorro el navegador hasta llegar a reproducir la cancion.
	 */
#if(TEST_ID == 2)
	while(1)
	{
		for(i = 0; i < 3; i++)
		{
			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_OK;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}

		vTaskSuspend( NULL );
	}
#endif

	/**
	 * Test 3: SD - Avanzo de cancion - OK - Play
	 */
#if(TEST_ID == 3)
	while(1)
	{
		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_OK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_DOWN;
		xSemaphoreGive( appApplicationButtonSemaphore );


		for(i = 0; i < 2; i++)
		{
			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_OK;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}

		vTaskSuspend( NULL );
	}
#endif

	/**
	 * Test 4: SD - Avanzo de cancion 5 veces - OK - Play
	 */
/*#if(TEST_ID == 4)
	while(1)
	{
		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_OK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		for(i = 0; i < 3; i++)
		{
			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_DOWN;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}

		for(i = 0; i < 2; i++)
		{
			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_OK;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}

		for(i = 0; i < 5; i++)
		{
			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_DOWN;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}

		for(i = 0; i < 2; i++)
		{
			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_OK;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}


		vTaskSuspend( NULL );
	}
#endif*/

	/**
	 * Test 5: SD - Avanzo de cancion 2 veces - Retorcedo 1 - OK - Play
	 */
#if(TEST_ID == 5)
	while(1)
	{
		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_OK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		for(i = 0; i < 2; i++)
		{
			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_DOWN;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_UP;
		xSemaphoreGive( appApplicationButtonSemaphore );

		for(i = 0; i < 2; i++)
		{
			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_OK;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}

		vTaskSuspend( NULL );
	}
#endif

	/**
	 * Test 6: SD - Menu - SD- OK - Play
	 */
#if(TEST_ID == 6)
	while(1)
	{
		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_OK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_BACK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		for(i = 0; i < 3; i++)
		{
			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_OK;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}

		vTaskSuspend( NULL );
	}
#endif


	/**
	 * Test 7: SD - Cancion 1 - Play - Back - Down - Cancion 2 - Play
	 */
#if(TEST_ID == 7)
	while(1)
	{
		for(i = 0; i < 3; i++)
		{
			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_OK;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_BACK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_DOWN;
		xSemaphoreGive( appApplicationButtonSemaphore );

		for(i = 0; i < 2; i++)
		{
			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_OK;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}

		vTaskSuspend( NULL );
	}
#endif

	/**
	 * Test 8: SD - DOWN x 2 - UP x 3
	 */
#if(TEST_ID == 8)
	while(1)
	{
		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_OK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		for(i = 0; i < 2; i++)
		{
			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_DOWN;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}

		for(i = 0; i < 3; i++)
		{
			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_UP;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}

		vTaskSuspend( NULL );
	}
#endif

	/**
	 * Test 9: Recorro en Ethernet para avanzar en la paginacion
	 */
#if(TEST_ID == 9)
	while(1)
	{
		//Delay de 3 segundos
		vTaskDelay( 3 * 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_BACK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_UP;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_OK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_OK;
		xSemaphoreGive( appApplicationButtonSemaphore );


		//Delay de 1 segundo
		for(i = 0; i < 6; i++)
		{
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_DOWN;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}
		vTaskSuspend( NULL );
	}
#endif

	/**
	 * Test 10: en modo Ethernet recorro el panel hasta llegar a reproducir por Ethernet
	 */
#if(TEST_ID == 10)
	while(1)
	{
		//Delay de 3 segundos
		vTaskDelay( 3 * 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_BACK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_UP;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_OK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_OK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		vTaskSuspend( NULL );
	}
#endif

	/**
	 * Test 11: en modo Ethernet, dejo reproducir medio segundo de cada cancion y le doy un siguiente
	 */
#if(TEST_ID == 11)
	while(1)
	{
		//Delay de 3 segundos
		vTaskDelay( 3 * 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_BACK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_UP;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_OK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_OK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		while(1)
		{
			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_DOWN;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}


		vTaskSuspend( NULL );
	}
#endif

	/**
	 * Test 12: en modo Ethernet, dejo reproducir y voy intercalando play - pausa
	 */
#if(TEST_ID == 12)
	while(1)
	{
		//Delay de 3 segundos
		vTaskDelay( 3 * 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_BACK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_UP;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_OK;
		xSemaphoreGive( appApplicationButtonSemaphore );



		while(1)
		{
			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_OK;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}


		vTaskSuspend( NULL );
	}
#endif

	/**
	 * Test 13: en modo Ethernet, intercalo entre reproducir y volver a atras al menu de canciones
	 */
#if(TEST_ID == 13)
	while(1)
	{
		//Delay de 3 segundos
		vTaskDelay( 3 * 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_BACK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_UP;
		xSemaphoreGive( appApplicationButtonSemaphore );

		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_OK;
		xSemaphoreGive( appApplicationButtonSemaphore );


		//Delay de 1 segundo
		vTaskDelay( 1000 / portTICK_RATE_MS );
		//Cargo la variable y libero el semaforo
		appApplicationButtonPressed = BUTTON_OK;
		xSemaphoreGive( appApplicationButtonSemaphore );

		while(1)
		{
			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_BACK;
			xSemaphoreGive( appApplicationButtonSemaphore );

			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_DOWN;
			xSemaphoreGive( appApplicationButtonSemaphore );

			//Delay de 1 segundo
			vTaskDelay( 1000 / portTICK_RATE_MS );
			//Cargo la variable y libero el semaforo
			appApplicationButtonPressed = BUTTON_OK;
			xSemaphoreGive( appApplicationButtonSemaphore );
		}


		vTaskSuspend( NULL );
	}
#endif
}
#endif

/**
 * @note Funciones Privadas
 */
/**
 * @def		appApplicationTimerHandler()
 * @brief	Controla la operacion con los botones y libera el semaforo cada vez que se presiono un boton
 */
void appApplicationTimerHandler()
{
	static uint8_t btnStatus[] = {0, 0, 0, 0};
	static signed portBASE_TYPE xHigherPriorityTaskWoken;
	uint32_t aux;

	//Consulto boton DOWN
	aux = GPIO_ReadValue(APP_APPLICATION_GPIO_PORT);
	aux = (aux >> APP_APPLICATION_DOWN_GPIO_PIN);
	aux = (aux & 0x01);
	if(!aux)
	{
		//Paso un 1
		btnStatus[0] = (btnStatus[0] << 1) | 0x01;
	}
	else
	{
		//Paso un 0
		btnStatus[0] = (btnStatus[0] << 1) & 0xFD;
	}

	//Consulto el boton OK
	aux = GPIO_ReadValue(APP_APPLICATION_GPIO_PORT);
	aux = (aux >> APP_APPLICATION_OK_GPIO_PIN);
	aux = (aux & 0x01);
	if(!aux)
	{
		//Paso un 1
		btnStatus[1] = (btnStatus[1] << 1) | 0x01;
	}
	else
	{
		//Paso un 0
		btnStatus[1] = (btnStatus[1] << 1) & 0xFD;
	}

	//Consulto boton UP
	aux = GPIO_ReadValue(APP_APPLICATION_GPIO_PORT);
	aux = (aux >> APP_APPLICATION_UP_GPIO_PIN);
	aux = (aux & 0x01);
	if(!aux)
	{
		//Paso un 1
		btnStatus[2] = (btnStatus[2] << 1) | 0x01;
	}
	else
	{
		//Paso un 0
		btnStatus[2] = (btnStatus[2] << 1) & 0xFD;
	}

	//Consulto boton BACK
	aux = GPIO_ReadValue(APP_APPLICATION_GPIO_PORT);
	aux = (aux >> APP_APPLICATION_BACK_GPIO_PIN);
	aux = (aux & 0x01);
	if(!aux)
	{
		//Paso un 1
		btnStatus[3] = (btnStatus[3] << 1) | 0x01;
	}
	else
	{
		//Paso un 0
		btnStatus[3] = (btnStatus[3] << 1) & 0xFD;
	}

	//Cargo el estado del boton a NO_ACTION
	appApplicationButtonPressed = BUTTON_NONE;
	//Evaluo que boton se presiono de forma correcta y envio la combinacion de botones activos.
	if(btnStatus[0] == APP_APPLICATION_DEBOUNCE_MASK)
	{
		appApplicationButtonPressed = BUTTON_DOWN;
	}
	if(btnStatus[1] == APP_APPLICATION_DEBOUNCE_MASK)
	{
		appApplicationButtonPressed = BUTTON_OK;
	}
	if(btnStatus[2] == APP_APPLICATION_DEBOUNCE_MASK)
	{
		appApplicationButtonPressed = BUTTON_UP;
	}
	if(btnStatus[3] == APP_APPLICATION_DEBOUNCE_MASK)
	{
		appApplicationButtonPressed = BUTTON_BACK;
	}

	if(appApplicationButtonPressed != BUTTON_NONE)
	{
		xSemaphoreGiveFromISR( appApplicationButtonSemaphore, &xHigherPriorityTaskWoken );
	}
}


void appApplicationConfiguration()
{
	int32_t size;
	uint32_t counter;

	while(1)
	{
		//Obtenemos la informacion de red
		if(networkGetInterfaceStatus() == INTERFACE_DOWN)
		{
			//Imprimo que la interfaz esta caida
			printWaitingConfigurationScreen();

			while(networkGetInterfaceStatus() == INTERFACE_DOWN)
			{
				//Evaluo si me presionan el boton de atras
				if(appApplicationButtonPressed == BUTTON_BACK)
				{
					return;
				}
			}
		}
		else
		{
			//Obtengo los datos de la interfaz
			size =sprintf(appApplicationNetworkInterfaces[0], "%s", "IP:");
			size += networkGetAddress(0, &appApplicationNetworkInterfaces[0][size]);

			size =sprintf(appApplicationNetworkInterfaces[1], "%s", "NM:");
			size += networkGetAddress(1, &appApplicationNetworkInterfaces[1][size]);

			size =sprintf(appApplicationNetworkInterfaces[2], "%s", "GW:");
			size += networkGetAddress(2, &appApplicationNetworkInterfaces[2][size]);

			//Imprimo los datos de la interfaz
			printConfigurationScreen();

			while(networkGetInterfaceStatus() == INTERFACE_UP)
			{
				//Evaluo si me presionan el boton de atras
				if(appApplicationButtonPressed == BUTTON_BACK)
				{
					return;
				}
			}
		}
	}


}
/**
 * @fn		appApplicationSetOutput(appApplicationEState state)
 * @brief	Setea el modo de los pines del multiplexor analogico previo al amplificador de audio.
 * @param	state
 */
void appApplicationSetOutput(appApplicationEState state)
{
	switch(state)
	{
		case SELECT_MODE:
		case ETHERNET_MODE:
		case SD_MODE:
			//Seteo el multiplexor para que se switchee al modo de entrada interno
			GPIO_ClearValue(APP_APPLICATION_MUX_PORT, APP_APPLICATION_PINS);
			break;

		case HELP_MODE:
		case CONFIGURATION_MODE:
			//Seteo el multiplexor para que se switchee al modo de entrada externo
			GPIO_SetValue(APP_APPLICATION_MUX_PORT, APP_APPLICATION_PINS);
			break;
	}
}


void printWelcomeScreen()
{
	//Limpio Pantalla
	Erase_Pant(WHITE);
	//Escribo
	Draw_String( 5, 7, "--------------------", BLACK);
	Draw_String( 5, 15, "Alba Chicar", BLACK);
	Draw_String( 5, 23, "Maroli", BLACK);
	Draw_String( 5, 31, "Vega", BLACK);
	Draw_String( 5, 39, "UTN FRBA TDII", BLACK);
	Draw_String( 5, 47, "--------------------", BLACK);
}

void printSelectScreen(int option)
{
	switch(option)
		{
			case 0:
				//Limpio Pantalla
				Erase_Pant(WHITE);
				//Escribo
				Draw_String( 5, 7, "--------------------", BLACK);
				Draw_String( 5, 15, "*SD", BLACK);
				Draw_String( 5, 23, " SERVIDOR", BLACK);
				Draw_String( 5, 31, " INTERFAZ RED", BLACK);
				Draw_String( 5, 39, " AYUDA", BLACK);
				Draw_String( 5, 47, "--------------------", BLACK);
				break;
			case 1:
				//Limpio Pantalla
				Erase_Pant(WHITE);
				//Escribo
				Draw_String( 5, 7, "--------------------", BLACK);
				Draw_String( 5, 15, " SD", BLACK);
				Draw_String( 5, 23, "*SERVIDOR", BLACK);
				Draw_String( 5, 31, " INTERFAZ RED", BLACK);
				Draw_String( 5, 39, " AYUDA", BLACK);
				Draw_String( 5, 47, "--------------------", BLACK);
				break;
			case 2:
				//Limpio Pantalla
				Erase_Pant(WHITE);
				//Escribo
				Draw_String( 5, 7, "--------------------", BLACK);
				Draw_String( 5, 15, " SD", BLACK);
				Draw_String( 5, 23, " SERVIDOR", BLACK);
				Draw_String( 5, 31, "*INTERFAZ RED", BLACK);
				Draw_String( 5, 39, " AYUDA", BLACK);
				Draw_String( 5, 47, "--------------------", BLACK);
				break;
			case 3:
				//Limpio Pantalla
				Erase_Pant(WHITE);
				//Escribo
				Draw_String( 5, 7, "--------------------", BLACK);
				Draw_String( 5, 15, " SD", BLACK);
				Draw_String( 5, 23, " SERVIDOR", BLACK);
				Draw_String( 5, 31, " INTERFAZ RED", BLACK);
				Draw_String( 5, 39, "*AYUDA", BLACK);
				Draw_String( 5, 47, "--------------------", BLACK);
				break;
		}

}

void printSDScreen()
{
	//Limpio Pantalla
	Erase_Pant(WHITE);
	//Escribo
	Draw_String( 5, 7, "SD", BLACK);
	Draw_String( 0, 18, "-------------------", BLACK);
}

void printEthernetScreen()
{
	//Limpio Pantalla
	Erase_Pant(WHITE);
	//Escribo
	Draw_String( 5, 7, "SERVIDOR", BLACK);
	Draw_String( 0, 18, "--------------------", BLACK);
}

void printHelpScreen()
{
	char serial[20];
	sprintf(serial, "%s%07d", "Serial:", APP_SERIAL_ID);

	//Limpio Pantalla
	Erase_Pant(WHITE);
	//Escribo
	Draw_String( 5, 7, "CREDITOS", BLACK);
	Draw_String( 0, 15, "---------------------", BLACK);
	Draw_String( 5, 23, "Agustin", BLACK);
	Draw_String( 5, 31, "Gabriel", BLACK);
	Draw_String( 5, 39, "Maximiliano", BLACK);
	Draw_String( 0, 47, "---------------------", BLACK);
	Draw_String( 5, 55, serial, BLACK);
}

void printConfigurationScreen()
{
	//Limpio Pantalla
	Erase_Pant(WHITE);
	//Escribo
	Draw_String( 5, 7, "INTERFAZ RED", BLACK);
	Draw_String( 0, 15, "---------------------", BLACK);
	Draw_String( 0, 23, appApplicationNetworkInterfaces[0], BLACK);
	Draw_String( 0, 31, appApplicationNetworkInterfaces[1], BLACK);
	Draw_String( 0, 39, appApplicationNetworkInterfaces[2], BLACK);
	Draw_String( 0, 47, "---------------------", BLACK);
}
void printWaitingConfigurationScreen()
{
	//Limpio Pantalla
	Erase_Pant(WHITE);
	//Escribo
	Draw_String( 5, 7, "CONFIGURACION", BLACK);
	Draw_String( 5, 15, "--------------------", BLACK);
	Draw_String( 5, 31, "Interfaz caida", BLACK);
	Draw_String( 5, 47, "--------------------", BLACK);
}

void printError(playerEType playerType)
{
	Erase_Pant(WHITE);
	Draw_String(0, 7 , "---------------------", BLACK);
	if(playerType == SD_PLAYER)
	{
		//Escribo
		Draw_String( 5, 23, "ERROR EN SD", BLACK);
	}
	else
	{
		//Escribo
		Draw_String( 5, 23, "ERROR EN SERVIDOR", BLACK);
	}
	Draw_String(0, 58, "---------------------", BLACK);
}

void printSDEmpty()
{
	//Escribo
	Erase_Pant(WHITE);
	Draw_String(0, 7 , "---------------------", BLACK);
	Draw_String( 5, 28, "SD VACIA", BLACK);
	Draw_String(0, 58, "---------------------", BLACK);
}


void printSongList(int currentOption, playerTSong *songList, playerEType playerType, int32_t lastSongTitle)
{
	int32_t i;

	if(playerType == SD_PLAYER)
		{
			//Limpio Pantalla
			Erase_Pant(WHITE);
			//Escribo
			Draw_String( 0, 7, "---------SD---------", BLACK);
			//Escribo
			Draw_String( 0, 15, actual_dir.path, BLACK);
		}
		else
		{
			//Limpio Pantalla
			Erase_Pant(WHITE);
			//Escribo
			Draw_String( 0, 7, "------SERVIDOR------", BLACK);

		}

	for(i = 0; i <= lastSongTitle; i++)
		{

			if(currentOption == i)
			{
				Draw_String( 5, 31+(i*9), songList[i].name, BLACK);
			}
			else
			{
				Draw_String( 0, 31+(i*9), songList[i].name, BLACK);
			}

		}
		Draw_String(0, 58, "---------------------", BLACK);
}

void printPlaySong(playerTSong *song)
{
	Erase_Pant(WHITE);
	Draw_String(0, 7 , "---------------------", BLACK);
	Draw_String( 0, 28, song->name, BLACK);
	Draw_String(0, 58, "---------------------", BLACK);
}

