#include "ProtocolTypes.h"


//! @note	Variables globales

/**
 * @var protocolConfig
 * @brief Es la estructura de configuraciones del protocolo
 */
TProtocolConfig protocolConfig;
/**
 * @var protocolReceptionSemph
 * @brief Semaforo para controlar la recepcion de datos.
 */
xSemaphoreHandle protocolReceptionSemph;

