#include <stdio.h>
#include <assert.h>

#include "../wsc/options.h"
#include "../core/errorcodes.h"

int call_help_handler(const char *arg[]) {
    return help_handler(NULL);
}

int call_version_handler(const char *arg[]) {
    return version_handler(NULL);
}
int call_port_handler(const char *args[]) {
    return port_handler(args);
}
// int call_ip_handler(const char *args[]) {
//     return ip_handler(args);
// }
// int call_verbose_handler(const char *args[]) {
//     return verbose_handler(args);
// }

int main() {
    printf("=== CLI Options Tests ===\n\n");

    // Test help handler
    const char *help_args[] = {NULL};
    int help_result = call_help_handler(help_args);
    assert(help_result == 0);
    printf("Help handler test PASSED\n");

    // Test version handler
    const char *version_args[] = {NULL};
    int version_result = call_version_handler(version_args);
    assert(version_result == 0);
    printf("Version handler test PASSED\n");

    // Test port handler with valid argument
    const char *port_args[] = {"8080"};
    int port_result = call_port_handler(port_args);
    assert(port_result == 0);
    printf("Correct port test PASSED\n");

    const char *port_args1[] = {"0"};
    int port_result1 = call_port_handler(port_args1);
    assert(port_result1 == ERR_INVALID_PORT);
    printf("Too small port test PASSED\n");

    const char *port_args2[] = {"65536"};
    int port_result2 = call_port_handler(port_args2);
    assert(port_result2 == ERR_INVALID_PORT);
    printf("Too large port test PASSED\n");

    const char *port_args3[] = {"-1"};
    int port_result3 = call_port_handler(port_args3);
    assert(port_result3 == ERR_INVALID_PORT);
    printf("Negative port test PASSED\n");

    const char *port_args4[] = {NULL};
    int port_result4 = call_port_handler(port_args4);
    assert(port_result4 == ERR_PORT_MISSING_VALUE);
    printf("No port test PASSED\n");

    const char *port_args5[] = {"eighty"};
    int port_result5 = call_port_handler(port_args5);
    assert(port_result5 == ERR_INVALID_PORT);
    printf("Invalid port test PASSED\n");

    // Add more tests for other handlers as needed

    printf("\nAll CLI options tests passed.\n");
    return 0;
}