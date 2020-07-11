/*
 * memoria.h
 *
 *  Created on: 10 jun. 2020
 *      Author: utnso
 */

#ifndef MEMORIA_H_
#define MEMORIA_H_

#include <time.h>
#include <math.h>

#include <delibird-commons/utils/paquete.h>
#include <delibird-commons/utils/list.h>

typedef enum {
    FIRST_FIT, BEST_FIT
} t_tipo_algoritmo_ocupado;

typedef enum {
    FIFO, LRU
} t_tipo_algoritmo_desocupado;

typedef struct {
    bool esta_libre;

    int base;
    int tamanio;

    int tiempo_carga;
    int tiempo_ultima_referencia;

    uint32_t id_mensaje_asociado;
} t_particion;

typedef struct {
    void* data;

    int tamanio_minimo_particion;
    t_tipo_algoritmo_ocupado tipo_algoritmo_ocupado;
    t_tipo_algoritmo_desocupado tipo_algoritmo_desocupado;
    int frecuencia_compactacion;
    int contador_busquedas_fallidas;

    t_list* particiones;
} t_memoria;

t_particion* memoria_crear_particion(int base, int tamanio);
void memoria_liberar_particion(t_particion* particion);

t_memoria* memoria_crear(
    int tamanio_minimo_particion,
    int tamanio_maximo_memoria,
    t_tipo_algoritmo_ocupado tipo_algoritmo_ocupado,
    t_tipo_algoritmo_desocupado tipo_algoritmo_desocupado,
    int frecuencia_compactacion
);

void* memoria_get_direccion_fisica_de(t_memoria* memoria, t_particion* particion);

bool memoria_existe_particion_libre_con(t_memoria* memoria, int tamanio_minimo);
t_particion* memoria_buscar_particion_libre_con(t_memoria* memoria, int tamanio_minimo);
void memoria_dividir_particion_si_es_mas_grande_que(t_memoria* memoria, t_particion* particion, int tamanio);
void memoria_asignar_paquete_a_la_particion(t_memoria* memoria, t_paquete* paquete, t_particion* particion);
t_particion* memoria_get_particion_a_desocupar(t_memoria* memoria);
void memoria_desocupar_particion(t_memoria* memoria, t_particion* particion);

void memoria_aumentar_contador_busquedas_fallidas(t_memoria* memoria);
void memoria_resetear_contador_busquedas_fallidas(t_memoria* memoria);

bool memoria_corresponde_compactar(t_memoria* memoria);
void memoria_compactar(t_memoria* memoria);
void memoria_consolidar(t_memoria* memoria);

#endif /* MEMORIA_H_ */
