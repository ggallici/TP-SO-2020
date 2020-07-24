/*
 * logger.h
 *
 *  Created on: 22 jul. 2020
 *      Author: utnso
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <commons/log.h>

t_log* logger_debug;
t_log* logger;

void logger_iniciando_broker(int tamanio_memoria);
void logger_iniciando_despacho_de_mensajes_de(t_tipo_mensaje tipo_mensaje);
void logger_iniciando_escucha_de_clientes();

void logger_conexion_iniciada(int cliente);
void logger_suscripcion_recibida(t_suscripcion* suscripcion);
void logger_ack_recibido(t_ack* ack);
void logger_mensaje_recibido(t_paquete* paquete);
void logger_anda_a_saber_que_llego();

void logger_mensaje_tiene_todos_los_acks(t_mensaje_despachable* mensaje_despachable);

void logger_dump_ejecutado(char* dump_file_path);

void logger_mensaje_enviado(t_paquete* paquete, t_suscriptor* suscriptor);
void logger_mensaje_almacenado(t_mensaje_despachable* mensaje_despachable);
void logger_mensaje_eliminado(uint32_t id_mensaje, t_particion* particion_asociada);

void logger_mensaje_sin_despachar_eliminado(t_cola* cola, t_mensaje_despachable* mensaje_sin_despachar);
void logger_mensaje_eliminado_antes_de_recibir_ack(t_ack* ack);
void logger_particiones_consolidadas(t_memoria* memoria, t_particion* una_particion, t_particion* otra_particion, int una_posicion, int otra_posicion);
void logger_compactacion_ejecutada();

void logger_espacio_libre(t_memoria* memoria);
void logger_espacio_ocupado(t_memoria* memoria);
void logger_detalle_memoria(t_memoria* memoria);

void logger_archivo_no_encontrado(char* file_path);
void logger_archivo_invalido(char* file_path);

#endif /* LOGGER_H_ */
