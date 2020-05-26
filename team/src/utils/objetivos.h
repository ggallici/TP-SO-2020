/*
 * objetivos.h
 *
 *  Created on: 10 may. 2020
 *      Author: utnso
 */

#ifndef UTILS_OBJETIVOS_H_
#define UTILS_OBJETIVOS_H_

#include <commons/collections/list.h>
#include "parser.h"

t_dictionary* calcular_objetivos_globales(t_list* entrenadores);
void calcular_objetivos_entrenador(void*);
void actualizar_objetivos_globales(void*);
bool is_pokemon_requerido(char* nombre);

#endif /* UTILS_OBJETIVOS_H_ */