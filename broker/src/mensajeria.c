/*
 * mensajeria.c
 *
 *  Created on: 17 may. 2020
 *      Author: utnso
 */

#include "mensajeria.h"

void _procesar_paquete_de(t_paquete* paquete, int cliente) {
    switch(paquete->header->tipo_paquete) {
    case SUSCRIPCION: {
        t_suscripcion* suscripcion = paquete_to_suscripcion(paquete);

        log_info(logger, "\tSUSCRIPCION RECIBIDA { cola: %s | suscriptor: %i }",
                mensaje_get_tipo_as_string(suscripcion->tipo_mensaje),
                suscripcion->id_suscriptor);

        t_suscriptor* suscriptor = suscriptor_crear(suscripcion->id_suscriptor, suscripcion->tipo_mensaje, cliente);

        buzon_registrar_suscriptor(buzon, suscriptor);

        suscripcion_liberar(suscripcion);

        break;
    }
    case ACK: {
        t_ack* ack = paquete_to_ack(paquete);

        log_info(logger, "\tACK RECIBIDO { mensaje: %i | suscriptor: %i }", ack->id_mensaje, ack->id_suscriptor);

        buzon_recibir_ack(buzon, ack);

        ack_liberar(ack);

        break;
    }
    case MENSAJE: {
        log_info(logger, "\t%s RECIBIDO { id: AUN_NO_ASIGNADO | correlation_id: %i }",
                mensaje_get_tipo_as_string(paquete->header->tipo_mensaje),
                paquete->header->correlation_id_mensaje);


//        "[{L=0},{X=30}]";
//        "[[L=0],[X=30]]";
//        "{[L=0],[X=30]}";


        //log_debug(logger_debug, get_particiones_serializadas(buzon->memoria->particiones));
//
//        int _tamanio_si_esta_libre_sino_0(t_particion* particion) {
//            return particion->esta_libre ? particion->tamanio : 0;
//        }
//        int _tamanio_si_esta_ocupada_sino_0(t_particion* particion) {
//            return !particion->esta_libre ? particion->tamanio : 0;
//        }
//        log_debug(logger_debug, list_sum_by(buzon->memoria->particiones, (void*) _tamanio_si_esta_libre_sino_0));
//        log_debug(logger_debug, list_sum_by(buzon->memoria->particiones, (void*) _tamanio_si_esta_ocupada_sino_0));


        t_mensaje_despachable* mensaje_despachable = buzon_almacenar_mensaje(buzon, paquete);

        if(mensaje_despachable)
            log_info(logger, "\t%s ALMACENADO { id: %i | size: %i } ==> PARTICION ASIGNADA { base: %i | size: %i }",
                    mensaje_get_tipo_as_string(paquete->header->tipo_mensaje),
                    mensaje_despachable->id,
                    mensaje_despachable->size,
                    mensaje_despachable->particion_asociada->base,
                    mensaje_despachable->particion_asociada->tamanio);
        else
            log_warning(logger_debug, "El MENSAJE fue ignorado por ser REDUNDANTE");

        mensaje_despachable_informar_id_a(mensaje_despachable, cliente);

        break;
    }
    default:
        printf("Anda a saber que le llego al pobre broker");
        break;
    }

    paquete_liberar(paquete);
}

void _gestionar_a(int cliente) {
    while(ipc_hay_datos_para_recibir_de(cliente)) {
        t_paquete* paquete = ipc_recibir_de(cliente);
        _procesar_paquete_de(paquete, cliente);
    }
}

void _gestionar_clientes() {
    int broker = ipc_escuchar_en(configuracion->ip_broker, configuracion->puerto_broker);

    while(1) {
        pthread_t gestor_de_un_cliente;
        int cliente = ipc_esperar_cliente(broker);
        log_info(logger, "< CONEXION INICIADA [%i] >", cliente);
        pthread_create(&gestor_de_un_cliente, NULL, (void*) _gestionar_a, (void*) cliente);
        pthread_detach(gestor_de_un_cliente);
    }
}

void _despachar_mensajes_de(t_cola* cola) {
    while(1) {
        buzon_despachar_mensaje_de(buzon, cola);
    }
}

void mensajeria_inicializar() {
    logger_debug = log_create("broker_debug.log", "BROKER", true, LOG_LEVEL_DEBUG);
    configuracion = configuracion_crear();
    logger = log_create(configuracion->log_file, "BROKER", true, LOG_LEVEL_INFO);
    buzon = buzon_crear(
        configuracion->tamanio_memoria,
        configuracion->tamanio_minimo_particion,
        configuracion->algoritmo_memoria,
        configuracion->algoritmo_particion_libre,
        configuracion->algoritmo_reemplazo,
        configuracion->frecuencia_compactacion
    );
}

void mensajeria_gestionar_signal(int signal) {
    if(signal == SIGUSR1) {
        buzon_imprimir_estado_en(buzon, configuracion->dump_file);

        log_info(logger, "DUMP DE CACHE EJECUTADO { path_archivo: %s }", configuracion->dump_file);
    }
}

void mensajeria_despachar_mensajes() {
    void _despachar_mensajes(char* key, t_cola* cola) {
        pthread_t gestor_de_una_cola;

        pthread_create(&gestor_de_una_cola, NULL, (void*) _despachar_mensajes_de, (void*) cola);
        pthread_detach(gestor_de_una_cola);
    }

    dictionary_iterator(buzon->administrador_colas->colas, (void*) _despachar_mensajes);
}

void mensajeria_gestionar_clientes() {
    pthread_t gestor_de_clientes;

    pthread_create(&gestor_de_clientes, NULL, (void*) _gestionar_clientes, NULL);
    pthread_join(gestor_de_clientes, NULL);
}
