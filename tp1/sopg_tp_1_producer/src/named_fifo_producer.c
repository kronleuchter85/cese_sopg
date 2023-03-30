/*
 * example_named_fifo_writer.c
 *
 *  Created on: 29 Mar 2023
 *      Author: gonzalo
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 300

#define SIGN_USER_1 "SIGUSR1"
#define SIGN_USER_2 "SIGUSR2"

#define QUIT_CMD "quit"

#define DATA_HEADER  "DATA:"
#define SIGN_MSG_1  "SIGN:1"
#define SIGN_MSG_2  "SIGN:2"

//char DATA_HEADER[] = "DATA:";
//char SIGN_MSG_1[] = "SIGN:1";
//char SIGN_MSG_2[] = "SIGN:2";

bool utils_startsWith(const char *a, const char *b) {
	if (strncmp(a, b, strlen(b)) == 0)
		return true;
	return false;
}

int32_t named_fifo_producer_message_send(int32_t fd, char *msg) {
	uint32_t bytesWrote;
	if ((bytesWrote = write(fd, msg, strlen(msg) - 1)) == -1) {
		perror("write");
	} else {
		printf("Enviando %d bytes msg:%s \n", bytesWrote, msg);
	}
	return bytesWrote;
}

uint32_t named_fifo_producer_process_message(int32_t fd, char *entry) {

	char message_buffer[300];
	memset(message_buffer, 0, sizeof(message_buffer));

	if (utils_startsWith(entry, SIGN_USER_1)) {

		//
		// SIGN:1
		//
		strcpy(message_buffer, SIGN_MSG_1);
		strcat(message_buffer, "\n");

	} else if (utils_startsWith(entry, SIGN_USER_2)) {

		//
		// SIGN:2
		//
		strcpy(message_buffer, SIGN_MSG_2);
		strcat(message_buffer, "\n");

	} else if (utils_startsWith(entry, QUIT_CMD)) {

		//
		// Quit
		//
		strcpy(message_buffer, QUIT_CMD);
		strcat(message_buffer, "\n");

	} else {

		//
		// DATA:<entry>
		//
		strcpy(message_buffer, DATA_HEADER);
		strcat(message_buffer, entry);

	}

	int32_t bytesSent = named_fifo_producer_message_send(fd, message_buffer);

	if (utils_startsWith(entry, QUIT_CMD)) {
		return 2;
	} else {
		return bytesSent;
	}

}

void named_fifo_producer_finalize() {
}

int named_fifo_producer_launch(char *named_fifo_path) {

	char outputBuffer[BUFFER_SIZE];
	int32_t returnCode;

	int32_t named_fifo_fd;

	if ((returnCode = mknod(named_fifo_path, S_IFIFO | 0666, 0)) < -1) {
		printf("Error creando el FIFO: %d\n", returnCode);
		exit(1);
	}

	printf("Esperando por conexion de Consumidores...\n");
	if ((named_fifo_fd = open(named_fifo_path, O_WRONLY)) < 0) {
		printf("Error abriendo named fifo: %d\n", named_fifo_fd);
		exit(1);
	}

	printf("---------------------------------------------------------------\n");
	printf("Se ha conectado un consumidor\n");
	printf("---------------------------------------------------------------\n");

	while (1) {

		fgets(outputBuffer, BUFFER_SIZE, stdin);

		uint32_t result = named_fifo_producer_process_message(named_fifo_fd, outputBuffer);
		if (result == 2) {
			break;
		}
	}

	named_fifo_producer_finalize();
	return 0;
}
