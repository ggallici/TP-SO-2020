/*
 * logger.c
 *
 *  Created on: 22 jul. 2020
 *      Author: utnso
 */

#include "logger.h"

void logger_conexion_iniciada(int cliente) {
    log_info(logger, "< CONEXION INICIADA [%i] >", cliente);
}

void logger_suscripcion_recibida(t_suscripcion* suscripcion) {
    log_info(logger, "\tSUSCRIPCION RECIBIDA { cola: %s | suscriptor: %i }",
            mensaje_get_tipo_as_string(suscripcion->tipo_mensaje),
            suscripcion->id_suscriptor);
}

void logger_ack_recibido(t_ack* ack) {
    log_info(logger, "\tACK RECIBIDO { mensaje: %i | suscriptor: %i }", ack->id_mensaje, ack->id_suscriptor);
}

void logger_mensaje_recibido(t_paquete* paquete) {
    log_info(logger, "\t%s RECIBIDO { id: AUN_NO_ASIGNADO | correlation_id: %i }",
            mensaje_get_tipo_as_string(paquete->header->tipo_mensaje),
            paquete->header->correlation_id_mensaje);
}

//void logger_mensaje_almacenado(t_mensaje_despachable* mensaje_despachable) {
//    if(mensaje_despachable)
//        log_info(logger, "\tMENSAJE ALMACENADO { id: %i | size: %i } ==> PARTICION ASIGNADA { base: %i | size: %i }",
//                mensaje_despachable->id,
//                mensaje_despachable->size,
//                mensaje_despachable->particion_asociada->base,
//                mensaje_despachable->particion_asociada->tamanio);
//    else
//        log_warning(logger_debug, "El MENSAJE fue ignorado por ser REDUNDANTE");
//}

void logger_anda_a_saber_que_llego() {
    log_error(logger_debug, "Anda a saber que le llego al pobre BROKER :(");
}

void logger_dump_ejecutado(char* dump_file_path) {
    log_info(logger, "< DUMP DE CACHE EJECUTADO > { path_archivo: %s }", dump_file_path);
}






























/*

NO TOCAR: ANDA Y PAJA HACERLE TEST

int list_sum_by(t_list* lista, int (*key_extractor)(void*)) {
    int _sum_with_extractor(int acumulador, void* elemento) {
        return acumulador + key_extractor(elemento);
    }

    return (int) list_fold(lista, (void*) 0, (void*) _sum_with_extractor);
}

int _tamanio_particion_si_esta_libre_sino_0(t_particion* particion) {
    return particion->esta_libre ? particion->tamanio : 0;
}
int _tamanio_particion_si_esta_ocupada_sino_0(t_particion* particion) {
    return !particion->esta_libre ? particion->tamanio : 0;
}

log_debug(logger_debug, "Memoria libre: %i", list_sum_by(buzon->memoria->particiones, _tamanio_particion_si_esta_libre_sino_0));
log_debug(logger_debug, "Memoria ocupada: %i", list_sum_by(buzon->memoria->particiones, _tamanio_particion_si_esta_ocupada_sino_0));

*/

/*

NO TOCAR: ANDA Y PAJA HACERLE TEST

char* get_particiones_as_string(t_list* particiones) {
    char* _concat_with_particion(char* acumulador, t_particion* particion) {
        char* detalle_particion = string_from_format("[%s=%i] ", particion->esta_libre ? "L" : "X", particion->tamanio);
        string_append(&acumulador, detalle_particion);
        free(detalle_particion);

        return acumulador;
    }

    return list_fold(particiones, (void*) string_new(), (void*) _concat_with_particion);
}

log_debug(logger_debug, "Detalle particiones: %s", get_detalle_particiones(buzon->memoria->particiones));

*/

/*
    BOLUDECES
    * Inicialización del Broker
    * Comienzo de hilos despachantes
    * Comienzo de hilo que escucha
*/
