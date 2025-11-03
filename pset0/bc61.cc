#include <cstdlib>
#include <cstdio>
int main() {
    int i = 0;
    while (fgetc(stdin) != -1) {
        i = i + 1;
    }
    fprintf(stdout, "%d\n", i);
    exit(0);
}