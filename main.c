#include <stdio.h>
#include "core/core.h"

int main() {
    printf("Starting core test...\n");

    if (core_test() != 0) {
        fprintf(stderr, "Core test failed.\n");
        return 1;
    }

    printf("Core test completed successfully.\n");
    return 0;
}