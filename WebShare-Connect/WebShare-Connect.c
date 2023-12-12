// WebShare-Connect.cpp : Defines the entry point for the application.
//

#include "WebShare-Connect.h"

int main(){
	printf("Starting server...\n");
	zsock_t* responder = zsock_new(ZMQ_REP);
	int r = zsock_bind(responder, "tcp://*:5555");
	if (r != 5555) {
		printf("did not work, idiot\n");
	}
	while (true) {
		char* msg = zstr_recv(responder);
		if (strcmp(msg, "Din Far") == 0) {
			printf("Received: %s\n", msg);
			zstr_send(responder, "Hello");
		}
	}
}
