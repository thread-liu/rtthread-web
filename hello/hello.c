#include <unistd.h>
#include <stdio.h>

int main(int argc, char ** argv) {
    for (size_t i = 0; i < 1000; i++)
    {
        printf("Hello, world!\n");
        // sleep(1);
    }
}
