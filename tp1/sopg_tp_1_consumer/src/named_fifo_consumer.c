/*
 * example_named_fifo.c
 *
 *  Created on: 29 Mar 2023
 *      Author: gonzalo
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

#define BUFFER_SIZE 300

char *FIFO_NAME;
char *SIGN_FILE;
char *DATA_FILE;

uint8_t inputBuffer[BUFFER_SIZE];

FILE *sign_file;
FILE *data_file;

typedef enum {

	MESSAGE_TYPE_SIGN, MESSAGE_TYPE_DATA

} message_type;

bool utils_startsWith(const char *a, const char *b)
{
	if (strncmp(a, b, strlen(b)) == 0)
		return true;
	return false;
}

void named_fifo_consumer_initialize() {
	sign_file = fopen(SIGN_FILE, "w+");
	data_file = fopen(DATA_FILE, "w+");
}

void named_fifo_consumer_finalize() {
	fclose(sign_file);
	fclose(data_file);
	fclose(data_file);
//	fclose(named_fifo_fd);

}

void named_fifo_consumer_log_message(message_type t, char *content) {

	if (t != MESSAGE_TYPE_DATA && t != MESSAGE_TYPE_SIGN) {
		perror("Wrong message type!!");
		return;
	}

	FILE *file;
	if (t == MESSAGE_TYPE_SIGN) {
		file = sign_file;
	} else if (t == MESSAGE_TYPE_DATA) {
		file = data_file;
	}

	fputs(content, file);
	fputs("\n", file);
	fflush(file);
}

int named_fifo_consumer_process_messages(int32_t bytesRead) {

	if (utils_startsWith(inputBuffer, "DATA:")) {
		printf("Data: %s\n", inputBuffer);
		named_fifo_consumer_log_message(MESSAGE_TYPE_DATA, inputBuffer);
	} else if (utils_startsWith(inputBuffer, "SIGN:")) {
		printf("Sign: %s\n", inputBuffer);
		named_fifo_consumer_log_message(MESSAGE_TYPE_SIGN, inputBuffer);
	} else if (utils_startsWith(inputBuffer, "quit")) {

		puts("Finalizando aplicacion");

		return 2;
	} else {
		printf("Recibidos %d bytes: \"%s\"\n", bytesRead, inputBuffer);
	}

	return 0;
}

int named_fifo_consumer_launch(char *fifo_file, char *signal_file, char *data_file) {

	int32_t named_fifo_fd;

	FIFO_NAME = fifo_file;
	SIGN_FILE = signal_file;
	DATA_FILE = data_file;

	int32_t bytesRead, returnCode;

	if ((returnCode = mknod(FIFO_NAME, S_IFIFO | 0666, 0)) < -1) {
		printf("Error creating named fifo: %d\n", returnCode);
		exit(1);
	}

	printf("Esperando por conexion productores...\n");
	if ((named_fifo_fd = open(FIFO_NAME, O_RDONLY)) < 0) {
		printf("Error opening named fifo file: %d\n", named_fifo_fd);
		exit(1);
	}

	printf("Productor conectado\n");

	named_fifo_consumer_initialize();

	do
	{
		if ((bytesRead = read(named_fifo_fd, inputBuffer, BUFFER_SIZE)) == -1) {
			perror("read");
		}
		else {
			inputBuffer[bytesRead] = '\0';
			int result = named_fifo_consumer_process_messages(bytesRead);

			if (result == 2) {
				break;
			}
		}
	}
	while (bytesRead > 0);

	named_fifo_consumer_finalize();

	return 0;
}
