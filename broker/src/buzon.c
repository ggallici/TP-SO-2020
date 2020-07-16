/*
 * buzon.c
 *
 *  Created on: 18 jun. 2020
 *      Author: utnso
 */

#include "buzon.h"

t_buzon* buzon_crear(
    int tamanio_memoria,
    int tamanio_minimo_particion,
    t_algoritmo_memoria algoritmo_memoria,
    t_algoritmo_reemplazo algoritmo_reemplazo,
    t_algoritmo_particion_libre algoritmo_particion_libre,
    int frecuencia_compactacion
) {
    t_buzon* buzon = malloc(sizeof(t_buzon));
    buzon->administrador_colas = administrador_colas_crear();
    buzon->memoria = memoria_crear(
        tamanio_memoria,
        tamanio_minimo_particion,
        algoritmo_memoria,
        algoritmo_particion_libre,
        algoritmo_reemplazo,
        frecuencia_compactacion
    );
    pthread_mutex_init(&buzon->mutex_memoria, NULL);

    return buzon;
}

t_mensaje_despachable* buzon_almacenar_mensaje(t_buzon* buzon, t_paquete* paquete) {
    t_mensaje_despachable* mensaje_despachable = NULL;
    t_cola* cola = administrador_colas_get_cola_from(buzon->administrador_colas, paquete->header->tipo_mensaje);

    if(!cola_es_mensaje_redundante(cola, paquete)) {
        pthread_mutex_lock(&buzon->mutex_memoria);
        buzon_vaciar_hasta_tener(buzon, paquete->header->payload_size);

        mensaje_despachable = mensaje_despachable_from_paquete(paquete, buzon->memoria);

        administrador_colas_asignar_id_mensaje_a(buzon->administrador_colas, mensaje_despachable);

        memoria_dividir_particion_si_es_mas_grande_que(buzon->memoria, mensaje_despachable->particion_asociada, mensaje_despachable->size);

        memoria_asignar_paquete_a_la_particion(buzon->memoria, paquete, mensaje_despachable->particion_asociada);
        pthread_mutex_unlock(&buzon->mutex_memoria);

        log_info(
            logger,
            "MENSAJE ALMACENADO - Id_mensaje: %i | Size_mensaje: %i | Base_particion: %i",
            mensaje_despachable->id,
            mensaje_despachable->size,
            mensaje_despachable->particion_asociada->base
        );

        cola_push_mensaje_sin_despachar(cola, mensaje_despachable);
    }

    return mensaje_despachable;
}

void buzon_despachar_mensaje_de(t_buzon* buzon, t_cola* cola) {
    t_mensaje_despachable* mensaje_despachable = cola_pop_mensaje_sin_despachar(cola);

    void _despachar_mensaje_a(t_suscriptor* suscriptor) {
        pthread_mutex_lock(&buzon->mutex_memoria);
        t_paquete* paquete = mensaje_despachable_to_paquete(mensaje_despachable, buzon->memoria);
        pthread_mutex_unlock(&buzon->mutex_memoria);

        paquete_set_tipo_mensaje(paquete, cola->tipo_mensaje);
        bool enviado = ipc_enviar_a(suscriptor->socket_asociado, paquete);

        if(enviado) {
            mensaje_despachable_add_suscriptor_enviado(mensaje_despachable, suscriptor);
            logger_enviado_a_un_suscriptor(logger, paquete, suscriptor->id);
        }

        paquete_liberar(paquete);
    }

    cola_iterate_suscriptores(cola, _despachar_mensaje_a);

    cola_push_mensaje_despachado(cola, mensaje_despachable);
}

void buzon_vaciar_hasta_tener(t_buzon* buzon, int espacio) {
    while(!memoria_existe_particion_libre_con(buzon->memoria, espacio)) {
        if(memoria_corresponde_compactar(buzon->memoria)) {
            memoria_compactar(buzon->memoria);

            memoria_resetear_contador_particiones_desocupadas(buzon->memoria);

            if(memoria_existe_particion_libre_con(buzon->memoria, espacio))
                break;
        }

        t_particion* particion_victima = memoria_get_particion_a_desocupar(buzon->memoria);

        administrador_colas_remove_and_destroy_mensaje_despachable_by_id(buzon->administrador_colas, particion_victima->id_mensaje_asociado);
        memoria_desocupar_particion(buzon->memoria, particion_victima);

        memoria_aumentar_contador_particiones_desocupadas(buzon->memoria);
    }
}

void buzon_registrar_suscriptor(t_buzon* buzon, t_suscriptor* suscriptor) {
    t_cola* cola = administrador_colas_get_cola_from(buzon->administrador_colas, suscriptor->tipo_mensaje);

    cola_add_or_update_suscriptor(cola, suscriptor);

    void _enviar_al_suscriptor_si_no_lo_recibio(t_mensaje_despachable* mensaje_despachable) {
        bool fue_recibido = mensaje_despachable_fue_recibido_por(mensaje_despachable, suscriptor);

        if(!fue_recibido) {
            pthread_mutex_lock(&buzon->mutex_memoria);
            t_paquete* paquete = mensaje_despachable_to_paquete(mensaje_despachable, buzon->memoria);
            pthread_mutex_unlock(&buzon->mutex_memoria);

            paquete_set_tipo_mensaje(paquete, cola->tipo_mensaje);
            bool enviado = ipc_enviar_a(suscriptor->socket_asociado, paquete);

            if(enviado) {
                bool fue_enviado_anteriormente = mensaje_despachable_fue_enviado_a(mensaje_despachable, suscriptor);
                if(!fue_enviado_anteriormente)
                    mensaje_despachable_add_suscriptor_enviado(mensaje_despachable, suscriptor);

                logger_enviado_a_un_suscriptor(logger, paquete, suscriptor->id);
            }

            paquete_liberar(paquete);
        }
    }

    cola_iterate_mensajes_despachados(cola, _enviar_al_suscriptor_si_no_lo_recibio);
}

void buzon_recibir_ack(t_buzon* buzon, t_ack* ack) {
    t_mensaje_despachable* mensaje_despachable = administrador_colas_find_mensaje_despachable_by_id(buzon->administrador_colas, ack->id_mensaje);

    mensaje_despachable_add_suscriptor_recibido(mensaje_despachable, ack);
}

void buzon_imprimir_estado_en(t_buzon* buzon, char* path_archivo) {
    FILE* dump_file = fopen(path_archivo, "w");

    time_t epoch;
    struct tm* info_datetime;
    char* string_datetime = malloc(80);

    time(&epoch);
    info_datetime = localtime(&epoch);
    strftime(string_datetime, 80, "%d/%m/%Y %X", info_datetime);

    fprintf(dump_file, "Dump: %s\n", string_datetime);

    free(string_datetime);

    int i = 0;
    void _write(t_particion* particion) {
        //TODO: quieren la base o la direccion fisica?
        int desde = particion->base;
        int hasta = particion->base + particion->tamanio;
        char* estado = particion->esta_libre ? "L" : "X";
        int tamanio = particion->tamanio;
        int LRU = particion->tiempo_ultima_referencia;

        t_cola* cola = administrador_colas_find_cola_by_id_mensaje(buzon->administrador_colas, particion->id_mensaje_asociado);
        char* tipo_mensaje_string = mensaje_get_tipo_as_string(cola ? cola->tipo_mensaje : NO_APLICA);

        int id = particion->id_mensaje_asociado;

        fprintf(dump_file, "Partición %i: %06x - %06x.  [%s]    Size: %ib   LRU:%i  Cola:%s   ID:%i\n", i, desde, hasta, estado, tamanio, LRU, tipo_mensaje_string, id);
        i++;
    }

    pthread_mutex_lock(&buzon->mutex_memoria);
    list_iterate(buzon->memoria->particiones, (void*) _write);
    pthread_mutex_unlock(&buzon->mutex_memoria);

    fclose(dump_file);

    log_info(logger, "DUMP DE CACHE EJECUTADO");
}
