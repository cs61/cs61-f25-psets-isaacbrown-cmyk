#include <cstdlib>
#include <cstdio>
#include <cctype>
int main() {
    unsigned long words = 0;
    unsigned long lines = 0;
    unsigned long bytes = 0;
    int output;
    bool in_a_space = true;
    bool new_space;
    while (!feof(stdin)) {
        output = fgetc(stdin);
        fprintf(stdout, "Current output: %d\n", output);
        if(output == EOF){
            break;
        }
        new_space = isspace((unsigned char)output);
        if(in_a_space && !new_space){
            words = words + 1;
        }
        in_a_space = new_space;
        if(output == '\n') {
            lines = lines + 1;
        }
        bytes = bytes + 1;
    }
    fprintf(stdout, "%lu %lu %lu\n", lines, words, bytes);
    exit(0);
}