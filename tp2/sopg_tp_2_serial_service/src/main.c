#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include <commons-threads.h>

void signal_sigint_handler(int args) {
	puts("Finalizando aplicacion");
	serial_connection_manager_thread_finish();
	interface_service_connection_manager_thread_finish();
}

int main(void) {

	printf("Inicio Serial Service\r\n");

	signal(SIGINT, signal_sigint_handler);
	signal(SIGTERM, signal_sigint_handler);

	pthread_t serial_connection_thread;
	pthread_t interface_service_connection_thread;

	//
	// inicializacion de jobs
	//
	pthread_create(&serial_connection_thread, NULL, serial_connection_manager_thread_start, "test");
	pthread_create(&interface_service_connection_thread, NULL, interface_service_connection_manager_thread_start, "test");

	puts("Hilos inicializados");

	pthread_join(serial_connection_thread, NULL);
	pthread_join(interface_service_connection_thread, NULL);

	puts("finalizando todo");

	exit(EXIT_SUCCESS);
	return EXIT_SUCCESS;
}
