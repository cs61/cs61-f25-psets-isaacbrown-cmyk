#include <cstdlib>
#include <cstdio>
#include <cctype>
int main() {
    unsigned long words = 0;
    unsigned long lines = 0;
    unsigned long bytes = 0;
    int output = 0;
    while (!feof(stdin)) {
        output = fgetc(stdin);
        fprintf(stdout, "Current output: %d\n", output);
        if(feof(stdin)){
            if(words != 0){
                lines = lines + 1;
            }
            break;
        }
        if(isspace(output)){
            words = words + 1;
        } else if(output == '\n') {
            lines = lines + 1;
        }
        bytes = bytes + 1;
    }
    fprintf(stdout, "%lu %lu %lu\n", lines, words, bytes);
    exit(0);
}