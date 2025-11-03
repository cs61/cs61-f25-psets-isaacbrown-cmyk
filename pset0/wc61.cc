#include <cstdlib>
#include <cstdio>
#include <cctype>
int main() {
    unsigned long words = 0;
    unsigned long lines = 0;
    unsigned long bytes = 0;
    int output = 0;
    int newline = '\n';
    while (!feof(stdin)) {
        output = fgetc(stdin);
        if(output == -1){
            break;
        }
        if(isspace(output)){
            words = words + 1;
        } else if(output == newline) {
            lines = lines + 1;
        }
        bytes = bytes + 1;
    }
    fprintf(stdout, "%lu %lu %lu\n", words, lines, bytes);
    exit(0);
}