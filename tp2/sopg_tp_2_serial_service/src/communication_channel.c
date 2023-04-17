/*
 * communication_channel.c
 *
 *  Created on: 17 Apr 2023
 *      Author: gonzalo
 */
#include <pthread.h>

pthread_mutex_t comm_channel = PTHREAD_MUTEX_INITIALIZER;

void communication_channel_lock() {
	pthread_mutex_lock(&comm_channel);
}

void communication_channel_unlock() {
	pthread_mutex_unlock(&comm_channel);
}
