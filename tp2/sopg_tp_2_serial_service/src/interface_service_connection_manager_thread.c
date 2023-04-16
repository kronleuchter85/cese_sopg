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
#include <commons-threads.h>
#include <netdb.h>

#include <stdbool.h>

#include "SerialManager.h"

//
// buffer de comunicacion con el Interface Service
//
char interface_service_communication_buffer[12];

//
// thread de interface service
//
void* interface_service_connection_manager_thread_start(void *args) {

	if (interface_service_connection_manager_initialize()) {
		interface_service_connnection_manager_close();
		return NULL;
	}

	while (true) {

		//
		// Leemos mensajes desde Interface Service
		//
		interface_service_connection_manager_read(interface_service_communication_buffer, 12);

		//
		// enviamos al emulador
		//
		serial_send(interface_service_communication_buffer, strlen(interface_service_communication_buffer));

	}

	return NULL;
}
