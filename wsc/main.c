#include <stdio.h>
#include "core.h"
#include "handlearguments.h"

int main(int argc, char *argv[], char *envp[])
{
    printf("Testig command line options:\n");
    int result = handle_cli_arguments(argc-1, argv + 1);

    // core_test();
    return 0;
}
