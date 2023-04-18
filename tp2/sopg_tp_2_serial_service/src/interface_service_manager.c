/*
 * InterfaceServiceManager.c
 *
 * Encapsula la conexion con el Interface Service
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

socklen_t addr_len;
struct sockaddr_in clientaddr;
struct sockaddr_in serveraddr;
int socket_fd;
int connection_socket;
char ipClient[32];

//
// inicializacion de conexion con interface service
//
int interface_service_connection_manager_initialize() {

	connection_socket = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char*) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(10000);
	//serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (inet_pton(AF_INET, "127.0.0.1", &(serveraddr.sin_addr)) <= 0) {
		fprintf(stderr, "ERROR invalid server IP\r\n");
		return 1;
	}

	if (bind(connection_socket, (struct sockaddr*) &serveraddr, sizeof(serveraddr)) == -1) {
		close(connection_socket);
		perror("listener: bind");
		return 1;
	}

	if (listen(connection_socket, 10) == -1) {
		perror("error en listen");
		return 1;
	}

	return 0;

}

int interface_service_connection_manager_accept_new_client() {

	addr_len = sizeof(struct sockaddr_in);
	if ((socket_fd = accept(connection_socket, (struct sockaddr*) &clientaddr, &addr_len)) == -1) {
		perror("error en accept");
		return 1;
	}

	inet_ntop(AF_INET, &(clientaddr.sin_addr), ipClient, sizeof(ipClient));
	printf("server:  conexion desde:  %s connection_socket\n", ipClient);

	return 0;
}

//
// leer del interface service y guardar en el buffer la cantidad de bytes indicada
//
int interface_service_connection_manager_read(char *buffer, int size) {
	int bytes_read;
	if ((bytes_read = read(socket_fd, buffer, size)) == -1) {
		perror("Error leyendo mensaje en socket");
	}
	buffer[bytes_read] = 0x00;
	return bytes_read;
}

//
// enviar al interface service el mensaje
//
int interface_service_connection_send(char *msg) {
	int bytes_writen = 0;
	if ((bytes_writen = write(socket_fd, msg, strlen(msg))) == -1) {
		perror("Error escribiendo mensaje en socket");
	}
	return bytes_writen;
}

//
// cerrar conexion con interface service
//
void interface_service_connection_manager_close_client() {
	close(socket_fd);
}

void interface_service_connection_manager_close_server() {
	close(connection_socket);
}
