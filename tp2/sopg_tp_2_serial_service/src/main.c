#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <commons-threads.h>

int main(void) {

	printf("Inicio Serial Service\r\n");

	//
	//
	//
	pthread_t serial_connection_thread;
	pthread_t interface_service_connection_thread;

	//
	//
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
