/*
 * mensajeria.h
 *
 *  Created on: 17 may. 2020
 *      Author: utnso
 */

#ifndef MENSAJERIA_H_
#define MENSAJERIA_H_

#include <pthread.h>

#include "buzon.h"

t_buzon* buzon;

void mensajeria_inicializar();
void mensajeria_despachar_mensajes();
void mensajeria_gestionar_clientes();

#endif /* MENSAJERIA_H_ */
