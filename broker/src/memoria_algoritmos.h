/*
 * memoria_algoritmos.h
 *
 *  Created on: 24 jul. 2020
 *      Author: utnso
 */

#ifndef MEMORIA_ALGORITMOS_H_
#define MEMORIA_ALGORITMOS_H_

typedef enum {
    PARTICIONES_DINAMICAS, BUDDY_SYSTEM
} t_algoritmo_memoria;

typedef enum {
    FIRST_FIT, BEST_FIT
} t_algoritmo_particion_libre;

typedef enum {
    FIFO, LRU
} t_algoritmo_reemplazo;

#endif /* MEMORIA_ALGORITMOS_H_ */
