/*
 *
 void* serial_connection_manager_thread.h
 *
 *  Created on: 16 Apr 2023
 *      Author: gonzalo
 */

#ifndef SERIAL_CONNECTION_MANAGER_THREAD_H_
#define SERIAL_CONNECTION_MANAGER_THREAD_H_

void* serial_connection_manager_thread_start(void *args);
void serial_connection_manager_thread_finish();

void* interface_service_connection_manager_thread_start(void*);
void interface_service_connection_manager_thread_finish();

#endif /* SERIAL_CONNECTION_MANAGER_THREAD_H_ */
