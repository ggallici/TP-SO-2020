/*
 * mensaje_despachable.h
 *
 *  Created on: 1 jun. 2020
 *      Author: utnso
 */

#ifndef MENSAJE_DESPACHABLE_H_
#define MENSAJE_DESPACHABLE_H_

#include <stdlib.h>
#include <pthread.h>

#include <delibird-commons/utils/list.h>

#include "suscriptor.h"
#include "memoria.h"

typedef struct {
    t_tipo_mensaje tipo;
    uint32_t id;
    uint32_t correlation_id;
    uint32_t size;

    t_particion* particion_asociada;

    t_list* ids_suscriptores_a_los_que_fue_enviado;
    t_list* ids_suscriptores_que_lo_recibieron;

    pthread_mutex_t mutex_ack;

} t_mensaje_despachable;

t_mensaje_despachable* mensaje_despachable_from_paquete(t_paquete* paquete, t_memoria* memoria);
t_paquete* mensaje_despachable_to_paquete(t_mensaje_despachable* mensaje_despachable, t_memoria* memoria);
void mensaje_despachable_liberar(t_mensaje_despachable* mensaje_despachable);

void mensaje_despachable_add_suscriptor_enviado(t_mensaje_despachable* mensaje_despachable, t_suscriptor* suscriptor);
void mensaje_despachable_add_suscriptor_recibido(t_mensaje_despachable* mensaje_despachable, t_ack* ack);
bool mensaje_despachable_tiene_todos_los_acks(t_mensaje_despachable* mensaje_despachable);

bool mensaje_despachable_fue_recibido_por(t_mensaje_despachable* mensaje_despachable, t_suscriptor* suscriptor);

bool mensaje_despachable_es_misma_respuesta_que(t_mensaje_despachable* mensaje_despachable, t_paquete* paquete);

void mensaje_despachable_remove_by_id_from(t_list* lista, uint32_t id_mensaje);

#endif /* MENSAJE_DESPACHABLE_H_ */
