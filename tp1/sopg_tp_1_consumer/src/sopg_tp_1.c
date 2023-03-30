/*
 ============================================================================
 Name        : sopg_tp_1.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define FIFO_NAME "/home/gonzalo/temp/myfifo"
#define SIGN_FILE "/home/gonzalo/temp/signals.txt"
#define DATA_FILE "/home/gonzalo/temp/log.txt"

int main(void) {
	puts("Consumer"); /* prints !!!Hello World!!! */

	named_fifo_consumer_launch(FIFO_NAME, SIGN_FILE, DATA_FILE);

	return EXIT_SUCCESS;
}
