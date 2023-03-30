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

int main(void) {
	puts("Producer");
	named_fifo_producer_launch(FIFO_NAME);
	return EXIT_SUCCESS;
}
