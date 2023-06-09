/*
 * interface_service_connection_manager_thread.c
 *
 *  Created on: 16 Apr 2023
 *      Author: gonzalo
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <stdbool.h>

#include <commons-threads.h>
#include "interface_service_manager.h"
#include "SerialManager.h"
#include "communication_channel.h"

volatile bool KEEP_RUNNING_SERVICE_CONNECTION_MANAGER_THREAD = true;

void interface_service_connection_manager_thread_finish() {

	KEEP_RUNNING_SERVICE_CONNECTION_MANAGER_THREAD = false;

}

//
// buffer de comunicacion con el Interface Service
//
char interface_service_communication_buffer[10];

//
// thread de interface service
//
void* interface_service_connection_manager_thread_start(void *args) {

	//
	// seteamos el puerto de conexion, bindeamos y nos ponemos a escuchar
	//
	while (KEEP_RUNNING_SERVICE_CONNECTION_MANAGER_THREAD && interface_service_connection_manager_initialize()) {
		interface_service_connection_manager_close_server();
		sleep(4);
	}

	//
	// aceptamos nuevas conexiones clientes
	//
	while (KEEP_RUNNING_SERVICE_CONNECTION_MANAGER_THREAD && interface_service_connection_manager_accept_new_client()) {
		interface_service_connection_manager_close_client();
		sleep(4);
	}

	while (KEEP_RUNNING_SERVICE_CONNECTION_MANAGER_THREAD) {

		//
		// Leemos mensajes desde Interface Service
		//
		int bytes_read = interface_service_connection_manager_read(interface_service_communication_buffer, 10);

		if (KEEP_RUNNING_SERVICE_CONNECTION_MANAGER_THREAD) {

			if (bytes_read > 0) {

				//
				// bloqueamos el canal de actualizaciones hacia hacia el emulador si
				// tambien estamos enviando actualizaciones el interface service
				//
				communication_channel_lock();

				printf("Mensaje del Interface Service - Bytes: %d, Mensaje: %s\n", bytes_read, interface_service_communication_buffer);

				//
				// enviamos al emulador
				//
				serial_send(interface_service_communication_buffer, strlen(interface_service_communication_buffer));

				communication_channel_unlock();
			}

			//
			// validamos que el socket no se haya interrumpido porque estemos finalizando la aplicacion
			//
			else {

				//
				// reiniciamos la conexion ya que el cliente se desconecto
				//
				interface_service_connection_manager_close_client();

				//
				// aceptamos nuevas conexiones clientes
				//
				if (interface_service_connection_manager_accept_new_client()) {
					puts("Recreando la conexion con Interface Service");
					interface_service_connection_manager_close_client();
					sleep(4);
				}
			}
		}

		//
		// en caso de estar finalizando la aplicacion...
		//
		else {
			break;
		}

	}

	puts("Interface Service connection thread finalizado");

	return NULL;
}
