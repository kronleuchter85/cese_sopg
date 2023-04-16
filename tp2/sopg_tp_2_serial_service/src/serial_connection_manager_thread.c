/*
 * serial_connection_manager_thread.c
 *
 *  Created on: 16 Apr 2023
 *      Author: gonzalo
 */

#include <commons-threads.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "SerialManager.h"
#include "stdbool.h"

//
// buffer de comunicacion con el dispositivo serie (Emulador)
//
char serial_communication_buffer[12];

//
// thread de serial connection
//
void* serial_connection_manager_thread_start(void *args) {

	if (!serial_open(1, 115200)) {

		while (true) {

			//
			// recibimos el mensaje del emulador
			//
			int bytes = serial_receive(serial_communication_buffer, 10);

			if (bytes > 0) {
				puts(serial_communication_buffer);

				//
				// enviamos al interface service
				//
				interface_service_connection_send(serial_communication_buffer);

			}

			sleep(5);
		}

	} else {
		puts("Se produjo un error conectando al Emulador");
	}

	return NULL;
}

