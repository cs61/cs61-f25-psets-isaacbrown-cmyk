#include <cstdlib>
#include <cstdio>
#include <cctype>
int main() {
    unsigned long words = 0;
    unsigned long lines = 0;
    unsigned long bytes = 0;
    int output;
    while (!feof(stdin)) {
        output = fgetc(stdin);
        fprintf(stdout, "Current output: %d\n", output);
        if(output == EOF){
            break;
        }
        if(isspace(output)){
            words = words + 1;
        }
        if(output == '\n') {
            lines = lines + 1;
        }
        bytes = bytes + 1;
    }
    fprintf(stdout, "%lu %lu %lu\n", lines, words, bytes);
    exit(0);
}