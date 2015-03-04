/**
 * @author
 * @file
 * @brief
 * @date
 * @version
 * @todo
 * @bug
 */

/**
 * @note Includes
 */
#include "Player.h"
#include "PlayerPrivate.h"

/**
 * @note Variables globales
 */

/*
 * @var		FATFS SD_sector
 * @brief	Variable del File System
 */
extern FATFS SD_sector;

/*
 * @var		XCHAR lf_name[_MAX_LFN+1]
 * @brief	Buffer para contener nombres al leer de directorio
 * @debug 	Imperativo de usar sino se rompe el manejo de nombres del FATSF con LFN
 */
static XCHAR lf_name[_MAX_LFN+1] = {0};

/*
 * @var		FIL song_file
 * @brief 	Estructura FIL de la cancion en actual reproduccion
 */
FIL 	playing_song_file;

/*
 * @var		dir_type	actual_dir
 * @brief	Directorio actual
 */
dir_type	actual_dir;

/*
 * @var		log_handler 	log_pointer
 * @brief	Variable con datos para manejo del .log
 */
log_type 	log_handler;


/*
 * @var 	back_op		back_option
 * @brief	Flag para identificar opcion de retroceso si s epudo crear o no el log por espacio libre
 */
back_op	back_option = NOT_ENABLED;


/**
 * @note Funciones publicas
 */

/**
 * @note Funciones Privadas
 */

int32_t playerSDInit()
{
	DWORD free_cluster, free_bytes;
	FATFS *dummy;
	init_stat res;

	//Inicializo el fie System
	res = FAT_Init();

	//En caso de erro devuelvo error
	if(res!= fatInit_ok) return(DISK_INIT_FAIL) ;
	//if(res!= fatInit_ok) return(res) ;

	//Cargo directorio actual
	strcpy(actual_dir.path , "/");

	//Abro nuevo directorio
	f_opendir(&(actual_dir.data) , actual_dir.path);

	//Obtengo espacio libre
	dummy = &SD_sector;
	f_getfree(0, &free_cluster, &(dummy));
	free_bytes = ((free_cluster) * (SD_sector.csize)) * _MAX_SS;

	//Decido opcion de retroceso en funcion de espacio libre
	if(free_bytes >= (LOG_SECTOR_SIZE * CANT_LOG_ITEMS)){
		//Creo y borro previo LogFile.log
		f_open( &(log_handler.acces), "LogFile.log", FA_WRITE | FA_CREATE_ALWAYS | FA_READ);
		//Cargo por default handler del LogFile.log
		log_handler.actual_pos = 0;
		log_handler.end_pos = 0;
		log_handler.org_pos = 0;
		log_handler.new_dir = 0;
		//Habilito posibilidad de retroceso
		back_option = ENABLED;
	}
	else {
		back_option = NOT_ENABLED;
	}

	//Inicializo con '0' toda la variable de cancion en reproduccion
	memset(&(playing_song_file), 0, sizeof(playing_song_file));

	return 0;
}

int32_t playerSDGetNextSong(playerTSong *actualSong, playerTSong *nextSong)
{
	static dir_stat hay_datos = dir_full;

	//Debug
	//back_option = NOT_ENABLED;

	//Si esta habilitada la opcion de retroceso
	if(back_option == ENABLED){


		//Si se pide la siguente cancion y actual_pos no apunta a la anterior de la que se pide
		if(actualSong != NULL){
		if(log_handler.new_dir) //FIXME: NO SE QUE ONDA
			log_handler.actual_pos = actualSong->pos;
		}

		//Si el pedido es sobre items ya almacenados el LogFile.log
		if(log_handler.actual_pos != log_handler.end_pos) {
			//Muevo posicion del actual
			if(actualSong != NULL) log_handler.actual_pos += (LOG_SECTOR_SIZE);
			//Leo item del LogFile.log
			log_gets( &(log_handler.actual_pos), nextSong);
		}
		//Si el pedido se realiza sobre items aun no cargados en LogFile.log
		else{

			//Tomo un nuevo item y lo dejo en nextSong hasta que se tome un item valido o se terminen los items
			do{
			hay_datos = dir_peek(&(actual_dir.data), nextSong);
			}while((hay_datos != dir_empty) && (hay_datos != valid_item));

			//Si todos los items fueron cargados en el LogFile.log (dato en nextSong no valido)
			if(hay_datos == dir_empty) {
				//Dentro del LogFile.log pego la vuelta
				log_handler.actual_pos = log_handler.org_pos;
				//Tomo path del item desde LogFile.log
				log_gets( &(log_handler.actual_pos), nextSong);
			}
			//Si hay elementos que extraer en el directorio
			else{


				//Actualizo posicion actual en LogFile.log si no me encuentro en el primer ingreso
				if(!log_handler.new_dir) log_handler.new_dir = 1;
				else log_handler.actual_pos += LOG_SECTOR_SIZE;

				//Cargo elemento en LogFile.log (dependiente de log_handler!!!)
				log_puts(nextSong);

				//Actualizo ultima posicion en el LogFile.log
				log_handler.end_pos = log_handler.actual_pos;

			}
		}
	}



	//Si no hay memoria para crear el LogFile.log y no se da la funcionalidad de retroceder
	else{

	static uint32_t already_read = 0;
	// Si se esta pidiendo leer item nuevo o se pide el siguiente d ela lista
	if((actualSong->id+1) < already_read){

		//Tomo un nuevo item y lo dejo en nextSong hasta que se tome un item valido o se terminen los items
		do{
			hay_datos = dir_peek(&(actual_dir.data), nextSong);
		}while((hay_datos != dir_empty) && (hay_datos != valid_item));

		//Si hay items por sacar aun el dato esta en nextSong
		if(hay_datos == dir_empty){

			//Desmonto el drive actual
			f_mount(0 , NULL);
			//Re inicializo el FATFS
			FAT_Init();
			//Abro el raiz
			f_opendir(&(actual_dir.data) , actual_dir.path);

			//Tomo un nuevo item y lo dejo en nextSong hasta que se tome un item valido o se terminen los items
			do{
				hay_datos = dir_peek(&(actual_dir.data), nextSong);
			}while((hay_datos != dir_empty) && (hay_datos != valid_item));

			//Actualizo cantidad de items leidos
			already_read = 1;
			//Asigno ID
			nextSong->id = 0;
		}

		else{
			//Asigno ID
			nextSong->id = already_read;
			//Incremento cantdiad de elementos leidos
			already_read++;
		}
	}
  //Si se pide un tema siguiente listado pero ya leido del directorio
  else{

	  //Reabro el directorio
	  {
	  //Desmonto el drive actual
	  f_mount(0 , NULL);
	  //Re inicializo el FATFS
	  FAT_Init();
	  //Abro el raiz
	  f_opendir(&(actual_dir.data) , actual_dir.path);
	  }
	  //Busco desde el principio hasta el siguiente
	  uint32_t f;
	  for(f = 0; f <= (actualSong->id+1); f++){
		  //Tomo un nuevo item y lo dejo en nextSong hasta que se tome un item valido o se terminen los items
		  do{
			  hay_datos = dir_peek(&(actual_dir.data), nextSong);
		  }while((hay_datos != dir_empty) && (hay_datos != valid_item));
	  }
  	 }
 }
	return 0;
}


int32_t playerSDGetNextPlaying(playerTSong *actualSong, playerTSong *nextSong){

	playerTSong next_aux, actual_aux;

	next_aux = *nextSong;
	actual_aux = *actualSong;

	do{
		playerSDGetNextSong(&actual_aux, &next_aux);
		//Para permutar y obtener el siguiente
		if(next_aux.type == directorio) actual_aux = next_aux;
	}while(next_aux.type == directorio);

	*nextSong = next_aux;
	*actualSong = actual_aux;

	return(0);
}

int32_t playerSDGetPreviousPlaying(playerTSong *actualSong, playerTSong *nextSong){

	playerTSong next_aux, actual_aux;

	next_aux = *nextSong;
	actual_aux = *actualSong;

	do{
		playerSDGetPreviousSong(&actual_aux, &next_aux);
		//Para permutar y obtener el siguiente
		if(next_aux.type == directorio) actual_aux = next_aux;
	}while(next_aux.type == directorio);

	*nextSong = next_aux;
	*actualSong = actual_aux;


	return(0);
}

int32_t playerSDGetPreviousSong(playerTSong *actualSong, playerTSong *nextSong)
{

	if(back_option == ENABLED){

		//Si se pide la siguente cancion y actual_pos no apunta a la anterior de la que se pide
		if(actualSong != NULL) log_handler.actual_pos = actualSong->pos;

		//Si estoy en la primera y me piden la anterior
		if(log_handler.actual_pos == log_handler.org_pos) return(NO_SONG);


		//Si no se encuentra en el principio
		if(log_handler.actual_pos != log_handler.org_pos){
			//Muevo posicion en el LogFile.log
			log_handler.actual_pos -= (LOG_SECTOR_SIZE);
		}
		//Leo item del LogFile.log
		log_gets( &(log_handler.actual_pos), nextSong);
	}
	else{

		//Reabro el directorio
		{
			//Desmonto el drive actual
			f_mount(0 , NULL);
			//Re inicializo el FATFS
			FAT_Init();
			//Abro el raiz
			f_opendir(&(actual_dir.data) , actual_dir.path);
		}
		//Busco desde el principio hasta el siguiente
		uint32_t f;
		dir_stat hay_datos; //FIXME: Puee tener que ser static
		//FIXME: PUEDE HITERAR UNA SOLA VES
		for(f = 0; f < actualSong->id; f++){
			//Tomo un nuevo item y lo dejo en nextSong hasta que se tome un item valido o se terminen los items
			do{
				hay_datos = dir_peek(&(actual_dir.data), nextSong);
			}while((hay_datos != dir_empty) && (hay_datos != valid_item));
		}
	}

	return(0);
}
int32_t playerSDPlaySong(playerTSong *song)
{
	//Realizo accion segun el tipo de item que se selecciono
	switch(song->type){
	case mp3:
			//FIXME: No se si esta bien asi en Ethernet
			//Reanudo reproduccion de tema actual o nuevo
			if(playerState == END_OF_FILE || playerState == DIR_CHANGED) playerState = OK;

			if(f_read(song->file , playerSongBuffer, PLAYER_SONGBUFFER_SIZE, &(playerReadBytes)) != FR_OK)
				return(FILE_READ_FAIL );
			else{

				//Muevo indice de song
				song->index += playerReadBytes;
				//Cargo el Buffer para tarea de streaming
				playerBuffer.buffer = playerSongBuffer;
				playerBuffer.size = playerReadBytes;

				//Debug
				/*if(song->index == song->size){
					int x;
					x++;
				}*/

				//Doy semaforo para habilitar el streaming de la cancion
				xSemaphoreGive( playerStreamingSemaphore );

				//Si se termino el tema
				if(song->index >= song->size){
					//Cambio estado a fin de archivo
					playerState = END_OF_FILE;
				}

				//Debug
				//playerState = END_OF_FILE;
			}
			break;

	case directorio:


			//Si hay opcion de retroceso
			if(back_option == ENABLED){

			//Guardo posicion de origen del sirectorio anterior
			actual_dir.predir_org = actual_dir.org_pos;

			//Armo path absoluto y actualizo directorio actual
			//Si es el raiz (strcmp entrega 0 con coincidencia)
			if(strcmp("/", actual_dir.path)) strcat(actual_dir.path, "/");
			strcat(actual_dir.path, song->name);

			//Abro el directorio
			f_opendir(&(actual_dir.data), actual_dir.path);
			//Cambio directorio
			f_chdir(actual_dir.path);

			//Muevo origen FIXME: Hay que volverlo a cero  o al anterior en el momento en que se retroceda
			log_handler.org_pos = (log_handler.end_pos + LOG_SECTOR_SIZE);

			//Cambio posicion actual al final para poder ingresar temas nuevos al LogFile.log
			log_handler.actual_pos = log_handler.org_pos;

			//Cambio fin del archivo
			log_handler.end_pos = log_handler.org_pos;

			//Limpio primera incersion
			log_handler.new_dir = 0;

			//Cargo posicion en el actual_dir
			actual_dir.org_pos = log_handler.org_pos;

			}
			//Devuelvo fin de archivo
			playerState = DIR_CHANGED;

			break;

	default:
			break;
	}

	return (playerState);
}
int32_t playerSDPauseSong()
{
	return 0;
}
int32_t playerSDPrepaireSong(playerTSong *song)
{
	//Pido puntero a archivo FIL
	song->file = &(playing_song_file);

	//Abro archivo o directorio
	if(song->type != directorio)
		f_open(song->file, song->name, FA_READ);
	//Cargo en actual_dir.data el DIR del directorio selecioado ya que de inmediato se cambia el directorio
	else
		f_opendir(&(actual_dir.data), song->name);

	return 0;
}
int32_t playerSDDisposeSong(playerTSong *song)
{
	//Si es archivo lo cierro
	if(song->type != directorio) f_close(song->file);

	return(0);
}
int32_t playerSDDeinit()
{
	//Cierro archivos pendientes

	//Cierro ultimo archivo abierto sin cerrar
	f_close(&(playing_song_file));

	//Cierro LogFile.log
	if(back_option == ENABLED) f_close(&(log_handler.acces));

	//CVuelvo al raiz (log_handler se inicializa con el SDPlayer)
	f_chdir("/");

	//Desmonto el drive actual
	f_mount(0 , NULL);


	return 0;
}


/*
 * @note Funciones Auxiliares
 */
/**
 * @fn 		sd_ext	item_filter(FILINFO *info , playerTSong	*item)
 * @brief 	Funcion para filtrasr items de un directorio
 * @param 	FILINFO info: Informcaicon del item a filtrar
 * 			playerTSong *item: Item sd_item con formato util
 * @return 	sd_ext: Extension reconocida o no
 * 			Deja en *item el item filtrado
 * @bug 	Carga el item del tipo playerTSong
 **/
sd_ext item_filter(FILINFO info, playerTSong *item){

	static uint32_t _id = 0;
	char extention[MAX_EXT+1] = {0};
	char *new_name;

	//Rescato nombre del item
	new_name = (info.lfname[0])?   info.lfname : info.fname;

	//FIXME: PRUEBA POR ERROR QUE SURGIO POR LOS PRIMEROS DEL PATH CON ..
	if(new_name[0] == '.') return(no_recon);

	//DIRECTORIO
	if(info.fattrib == AM_DIR){

		//Si no esta habilitada la opcion de retroceso no se habilita el manejo de directorios
		if(back_option == NOT_ENABLED) return(no_recon);

		item->type = directorio;

		//Limpio index
		item->index = 0;

		//Cargo ID
		if(back_option == ENABLED) item->id =_id;

		//Cargo fuente
		item->source = SD_PLAYER;
		//Cargo tam del item
		item->size = info.fsize; //FIXME: si e sdirectorio no veo la utilidad

		//Pido un Buffer de nombre
		item->name = playerGetSongNameBuffer();
		//Cargo nombre
		strcpy(item->name, new_name);
		//FIXME: ESPERO NO ROMPA EL ETHERNET

		_id++;

		return(ext_recon);
	}

	//ES ARCHIVO
	if(info.fattrib == AM_ARC){

	uint32_t i, k;
	for(i = strlen(new_name); new_name[i]!='.' && i>0 ; i--); //Hubico el .ext

	for(k = 0, i++ ; new_name[i]!='\0' && k <= MAX_EXT ; k++, i++ ) extention[k] = new_name[i];

	if(k > MAX_EXT && extention[k]!='\0') {
		//extension equivocada
		return( no_recon );
	}

	//MP3
	if(!strcmp(extention,"mp3")||!strcmp(extention,"MP3")) {
		item->type = mp3;


		//Pido un Buffer de nombre
		item->name = playerGetSongNameBuffer();
		//Cargo nombre
		strcpy(item->name, new_name);
		//FIXME: ESPERO NO ROMPA EL ETHERNET

		//Limpio index
		item->index = 0;

		//Cargo ID
		if(back_option == ENABLED) item->id = _id;

		//Cargo fuente
		item->source = SD_PLAYER;
		//Cargo tam del item
		item->size = info.fsize; //FIXME: si e sdirectorio no veo la utilidad

		_id++;

		return(ext_recon);
			}
	}

	else{
		//Si es otra clase de item no es contemplado
		return(no_recon);
	}

	return(no_recon);
}


/**
 * @fn 		dir_stat dir_peek (const char* dir_path, sd_item *buffer)
 * @brief	Funcion que toma un item valido del directorio que se le pasa
 * @param 	playerTSong *item: Buffer del item recolectado
 * 			DIR *carpeta: Archivo de directorio ya abierto
 *  @return dir_stat: Devuelve si hay o no items para tomar del directorio
 *  @debug	En caso de no tener mas items para sacar buffer queda con valor no valido
 *  		Asume que el directorio ya se encuentra abierto
 **/
dir_stat dir_peek(DIR *carpeta, playerTSong *item){

	FILINFO info; //Puede ser necesario que ponerla como static

	//Asigno buffer para utilizar _LFN
	info.lfname = lf_name;
	info.lfsize = sizeof lf_name;

	//Tomo un archivo del directorio
	f_readdir(carpeta , &info);

	//Si hay items por sacar
	if(info.fname[0]) {
		//Si aun hay datos para leer
		if(item_filter(info , item) == ext_recon) return(valid_item);
		//Si el item tomado dle directorio no es valido vuelvo a tomar dato hasta encontrar uno valido
		else return(invalid_item);
	}
	//Si no hay mas items por sacar
	else return(dir_empty);

return(invalid_item);

}


/*
 * @fn 		uint8_t	log_puts(playerTSong *item)
 * @brief	Funcion que coloca path de un item en LogFile.log en el cursor interno del archivo no actual_pos
 * @param	playerTSong *item: item a ingresar
 * @return	uint8_t: Devuelve 0 en caso de exito en la escritura o 0xff en falla
 */
uint8_t	 log_puts(playerTSong *item){


	DWORD fp_org;
	int res;

	//Rescato estado actual del cursor
	fp_org = log_handler.actual_pos;//(log_handler.acces).fptr;

	//Expando en un LOG_SECTOR_SIZE el Log_Size.log
	f_lseek(&(log_handler.acces), /*fp_org*/log_handler.actual_pos+LOG_SECTOR_SIZE/*+1*/); //TODO: puede que el formato de LOG_SECTOR_SIZE deba cambiarse

	//Retomo cursor dentro del archivo a su posicion original
	f_lseek(&(log_handler.acces), fp_org);

	//Escribo Path en el archivo

	if(item->type != directorio) res = f_printf(&(log_handler.acces), "%s#%ld#*%ld*\r\n", item->name, item->size, item->id);
	else res = f_printf(&(log_handler.acces), "%s.dir#%ld#*%ld*\r\n", item->name, item->size, item->id); //Le pongo una extension dir para reconocerlo como directorio
	//Doy fin al string

	if( res > 0){

		//TODO: Estetico
		//Muevo el cursor y pego espacio vacio
		uint32_t j = 0;
		for(j = (log_handler.acces).fptr; j < fp_org+LOG_SECTOR_SIZE-2; j++ ) f_putc(' ', &(log_handler.acces));
		f_putc('\r', &(log_handler.acces));
		f_putc('\n', &(log_handler.acces));

		//Cargo posicion en LogFile al elemento
		item->pos = fp_org;

		//Debug
		/*static uint8_t x = 0;
		if((x++) > 10){
			f_close(&(log_handler.acces));
		}*/

		return(0);
	}
	else return(255);

return(255);
}

/*
 * @fn 		uint8_t	log_gets(log_pos, playerTSong *item)
 * @brief	Obtengo un string del archivo LogFile.log
 * @param	log_pos *pos: Posicion inicial del string a leer en el archivo
 * 			playerTSong *item: Buffer donde quedara el elemento obtenido
 * @return	uint8_t: 0 si OK
 */
uint8_t log_gets( log_pos *pos, playerTSong *item){

	char *buffer;

	//Muevo cursor hasta la posicion anterior (hasta actual_pos)
	f_lseek(&(log_handler.acces), *pos);  //FIXME: Puede romper si no matchean la correlatividad de los pos y el cursor del archivo

	//Pido Buffer de nombre
	buffer = playerGetSongNameBuffer();
	//FIXME: ESPERO NO ROMPA EL ETHERNET

	//Leo un string desde ahi y lo cargo en el buffer global de nombe
	f_gets(buffer, PLAYER_SONG_NAME_BUFFER, &(log_handler.acces));

	//Cargo nombre
	item->name = buffer;

	//Cargo posicion del item (primer caracter)
	item->pos = *pos;

	//Filtro el item y lo clasifico
	log_filter(item);

	return(0);
}

/*
 * @fn		void log_filter (playerTSong)
 * @brief	Funcion para filtrar y llenar campos de un item extraido del LogFile.log
 * @param	playerTSong *item: punetro al item aa clasifica
 * @return	void
 */
void log_filter( playerTSong *item){

	char extention[MAX_EXT+1] = {0};
	char aux_buff[_MAX_LOG_EXTRA+1] = {0};
	uint32_t i, k;

	for(i = strlen(item->name); item->name[i]!='.' && i > 0; i--); //Hubico el .ext
	for(k = 0, i++ ; item->name[i]!='#' && k <= MAX_EXT ; k++, i++ ) extention[k] = item->name[i];

	//DIRECTORIO
	if(!strcmp(extention,"dir")||!strcmp(extention,"DIR"))
		item->type = directorio;

	//ES ARCHIVO
	//MP3
	if(!strcmp(extention,"mp3")||!strcmp(extention,"MP3"))
		item->type = mp3;

	//Obtengo tamaño del archivo
	uint32_t f;
	for(f = 0, ++i; item->name[i] != '#'; i++, f++) aux_buff[f] = item->name[i];
	aux_buff[f] = '\0';
	item->size = atoi(aux_buff);

	//Rescato ID del item
	for(; item->name[i] != '*'; i++);
	for(f = 0, ++i; item->name[i] != '*'; i++, f++) aux_buff[f] = item->name[i];
	aux_buff[f] = '\0';
	item->id = atoi(aux_buff);

	//Limpio index
	item->index = 0;
	//Cargo fuente
	item->source = SD_PLAYER;

	//Estetico
	for(; item->name[i] != '#'; i--) item->name[i] = 0;
	for(--i; item->name[i] != '#'; i--) item->name[i] = 0;


	if(item->type == directorio) {
	//Si es directorio devo quitarle al path la extension .dir
	for(; item->name[i] != '.'; i--) item->name[i] = 0;
	}
	item->name[i] = '\0';

	return;
}

/*
 * @fn		void ret_dir(void)
 * @bref	Retorno al directorio previo
 * @param	void
 * @return	void
 */
void	ret_void(void){


	//Si hay opcion de retroceso
	if(back_option == ENABLED){
	//Armo path absoluto y actualizo directorio actual
	uint32_t i = 0;
	for(i = 0; actual_dir.path[i] != '/'; i++);
	actual_dir.path[i] = '\0';

	//Abro el directorio
	f_opendir(&(actual_dir.data), actual_dir.path);
	//Cambio directorio
	f_chdir(actual_dir.path);

	//Actualizo fin del directorio al inmediato anterior al que estaba abierto
	log_handler.end_pos = log_handler.org_pos - LOG_SECTOR_SIZE ;

	//Actualizo origen del directorio
	log_handler.org_pos = actual_dir.predir_org;

	//Cambio posicion actual al final para poder ingresar temas nuevos al LogFile.log
	log_handler.actual_pos = log_handler.org_pos;

	//Limpio primera incersion
	log_handler.new_dir = 0;

	//Cargo posicion en el actual_dir
	actual_dir.org_pos = log_handler.org_pos;

	return;

	}

return;
}
