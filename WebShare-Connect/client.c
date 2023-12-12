#include "WebShare-Connect.h"


int main(int argc, char const* argv[])
{
	printf("Connecting to hello world server...\n");
	zsock_t* requester = zsock_new(ZMQ_REQ);
	zsock_connect(requester, "tcp://localhost:5555");
	zstr_send(requester, "Din Far");
	Sleep(1);
	char* reply = zstr_recv(requester);
	printf("Reply: %s\n", reply);
	zsock_destroy(&requester);
	
	return 0;
}