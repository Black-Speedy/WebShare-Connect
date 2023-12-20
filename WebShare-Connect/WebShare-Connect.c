#include "WebShare-Connect.h"
#include "server.h"
#include "client.h"

int main(int argc, char const* argv[])
{
    if (argc < 2) {
        printf("Usage: %s [server|client]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "server") == 0) {
        return server_main(argc - 1, argv + 1);
    }
    else if (strcmp(argv[1], "client") == 0) {
        return client_main(argc - 1, argv + 1);
    }
    else {
        printf("Usage: %s [server|client]\n", argv[0]);
        return 1;
    }
}
