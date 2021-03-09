#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf("An integer is needed.\n");
        return 0;
    }
    int a = atoi(argv[1]);
    int result = 0;
    for (int i = 0; i < 1000000; i++) {
        result += a * 32;
        result += a * 0xFF;
    }
    printf("%d\n", result);
    return 0;
}
