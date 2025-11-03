#include <cstdlib>
#include <cstdio>
int main() {
    int i = 0;
    fgetc(stdin);
    i = i + 1;
    fprintf(stdout, "Number of bytes: %d\n", i);
    exit(0);
}