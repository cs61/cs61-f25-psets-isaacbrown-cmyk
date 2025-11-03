#include <cstdlib>
#include <cstdio>
#include <cctype>
int main() {
    unsigned long words = 0;
    unsigned long lines = 0;
    unsigned long bytes = 0;
    char output = "";
    while (output != EOF) {
        output = fgetc(stdin)
        if(isspace(output)){
            words += 1
        } else if(output == "\n") {
            lines += 1
        }
        i = i + 1;
    }
    fprintf(stdout, "%d\n", i);
    exit(0);
}