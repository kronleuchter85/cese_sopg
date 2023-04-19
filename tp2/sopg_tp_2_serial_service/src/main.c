#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "stdbool.h"
#include <commons-threads.h>

#include "interface_service_manager.h"
#include "SerialManager.h"
#include "communication_channel.h"

pthread_t serial_connection_thread;

bool SIGNAL_ARRIVED = false;

void signal_sigint_handler(int args) {

	SIGNAL_ARRIVED = true;

	serial_connection_manager_thread_finish();
	interface_service_connection_manager_thread_finish();
//	pthread_cancel(serial_connection_thread);
}

int main(void) {

	printf("Inicio Serial Service\r\n");

	struct sigaction sa;
	sa.sa_handler = signal_sigint_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1 || sigaction(SIGTERM, &sa, NULL) == -1) {
		perror("Error enmascarando las signals");
		exit(1);
	}

	//
	// inicializacion de jobs
	//
	int thread_creation_error = pthread_create(&serial_connection_thread, NULL, serial_connection_manager_thread_start, "test");

	if (thread_creation_error) {
		puts("Error creando el hilo de conexion con el Emulador");
		exit(1);
	}

	interface_service_connection_manager_thread_start("test");

	while (!SIGNAL_ARRIVED) {
		sleep(1);
	}

	pthread_cancel(serial_connection_thread);
	pthread_join(serial_connection_thread, NULL);

	puts("finalizando todo");
	serial_close();
	interface_service_connection_manager_close_client();
	interface_service_connection_manager_close_server();

//	exit(EXIT_SUCCESS);
	return EXIT_SUCCESS;
}
