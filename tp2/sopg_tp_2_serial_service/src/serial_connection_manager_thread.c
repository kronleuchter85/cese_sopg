/*
 * serial_connection_manager_thread.c
 *
 *  Created on: 16 Apr 2023
 *      Author: gonzalo
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "stdbool.h"
#include <commons-threads.h>
#include "SerialManager.h"
#include "communication_channel.h"
#include "interface_service_manager.h"
//
// buffer de comunicacion con el dispositivo serie (Emulador)
//
char serial_communication_buffer[9];

volatile bool KEEP_RUNNING_SERIAL_CONNECTION_MANAGER_THREAD = true;

void serial_connection_manager_thread_finish() {

	KEEP_RUNNING_SERIAL_CONNECTION_MANAGER_THREAD = false;

	puts("Finalizando Serial manager thread");
	serial_close();
}

//
// thread de serial connection
//
void* serial_connection_manager_thread_start(void *args) {

	if (serial_open(1, 115200)) {
		puts("Se produjo un error conectando al Emulador. Intentando nuevamente.");
		return NULL;
	}

	while (KEEP_RUNNING_SERIAL_CONNECTION_MANAGER_THREAD) {

		//
		// recibimos el mensaje del emulador
		//
		int bytes = serial_receive(serial_communication_buffer, 9);

		//
		// tenemos updates desde el emulador
		//
		if (bytes > 0) {

			//
			// bloqueamos el canal de actualizaciones hacia el interface service si
			// tambien estamos enviando actualizaciones hacia el emulador
			//
			communication_channel_lock();

			printf("Mensaje del Emulador - Bytes: %d, Mensaje: %s\n", bytes, serial_communication_buffer);

			//
			// enviamos al interface service
			//
			interface_service_connection_send(serial_communication_buffer);

			communication_channel_unlock();

		}

		sleep(5);
	}

	puts("Serial manager thread finalizado");

	return NULL;
}

