/*
 * logger.c
 *
 *  Created on: 22 jul. 2020
 *      Author: utnso
 */

#include "logger.h"






































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
