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

int main(void) {
	puts("Launching Consumer"); /* prints !!!Hello World!!! */

	example_named_fifo_reader_launch();
	return EXIT_SUCCESS;
}
