/*
 * logger.c
 *
 *  Created on: 22 jul. 2020
 *      Author: utnso
 */

#include "logger.h"

int list_sum_by(t_list * lista, int (*transformador)(void*)) {
    int _sumar_aplicando_transformador(int acumulador, void* elemento) {
        return acumulador + transformador(elemento);
    }

    return (int) list_fold(lista, (void*) 0, (void*) _sumar_aplicando_transformador);
}


char* get_particiones_serializadas(t_list* particiones) {
//    char* _sasa_aplicando_transformador(char* acumulador, t_particion* particion) {
//        char* particion_string = string_from_format("[%s=%i] ", particion->esta_libre ? "L" : "X", particion->tamanio);
//        char* resultado = string_append(acumulador, particion_string);
//        free(particion_string);
//
//        return resultado;
//    }
//
//    return list_fold(particiones, (void*) string_new(), (void*) _sasa_aplicando_transformador);
    return NULL;
}







/*
//BOLUDECES
Inicialización del Broker
Comienzo de hilos despachantes
Comienzo de hilo que escucha

//COMPLICADOS O QUE DAN PAJA
Memoria libre y ocupada ⇒ después de eliminar y después de almacenar (debugg) y tmb para consolidacion y compactacion (NO SE SI ANTES O DESPUÉS)
Memoria libre = 0 ; Memoria ocupada = 64
[LI=50] [OC=40] [LI=12] [LI=10]
 */







