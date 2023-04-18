/*
 * interface_service_manager.h
 *
 *  Created on: 16 Apr 2023
 *      Author: gonzalo
 */

#ifndef INTERFACE_SERVICE_MANAGER_H_
#define INTERFACE_SERVICE_MANAGER_H_

int interface_service_connection_manager_read(char *buffer, int size);
int interface_service_connection_send(char *msg);

int interface_service_connection_manager_initialize();
int interface_service_connection_manager_accept_new_client();

void interface_service_connection_manager_close_server();
void interface_service_connection_manager_close_client();

#endif /* INTERFACE_SERVICE_MANAGER_H_ */
