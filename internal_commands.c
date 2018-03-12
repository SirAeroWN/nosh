
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int prev_result;

/**** internal commands ****/
int cd(char** args) {
    // use chdir
    if (args[1] == NULL) {
        fprintf(stdout, "Need argument to %s\n", args[0]);
        fflush(stdout);
        return -1;
    }
    return chdir(args[1]);
}

int quit(char** args) {
    // clean up
    exit(0);
}

int exit_status(char** args) {
    fprintf(stdout, "%d\n", prev_result);
    fflush(stdout);
    return 1;
}