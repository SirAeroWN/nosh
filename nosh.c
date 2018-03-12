/*
 *  norv shell
 *  by Will Norvelle
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <curses.h>
#include <term.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "internal_commands.h"
#include "tokenizer.h"

/**** globals ****/
char* prompt;
char* default_config_file;

/**** declerations ****/
int load_config(char* config_file);
int init();
int main_loop();
char** parse_line(char* line);
int run(char** args); // args[0] should be program/internal command

/**** definitions ****/
/**** primary functions ****/
int load_config(char* config_file) {
    return 1;
}

int init() {
    prompt = (char*)malloc(sizeof(char)*3);
    strncpy(prompt, "> ", 3);
    default_config_file = (char*)malloc(sizeof(char)*11);
    strncpy(default_config_file, "~/.nsh.rc", 10);
    if (load_config(default_config_file) < 0) {
        return -1;
    }
    return 1;
}

int main_loop() {
    int retVal = 0;

    while (retVal >= 0) {
        char* line = readline(prompt);
        char** args = parse_line(line);
        retVal = run(args);
        add_history(line);

        free(line);
        free(args);
    }
    return 1;
}

/**** helper functions ****/
char** parse_line(char* line) {
    /*
        This doesn't handle escapes or quotes
    */
    // allocate a bunch of space
    int buf_size = 1024;
    int loc = 0;
    char** args = (char**)malloc(sizeof(char*)*buf_size);
    char* indv_arg;
    char* save_pnt;

    //indv_arg = strtok(line, " \t\n"); // regex is space, tab, carriage return, newline
    indv_arg = get_next_token(line, save_pnt);
    while (indv_arg != NULL) {
        // for each individual argument, put it in the array of arguments and increment loc
        args[loc] = indv_arg;
        loc++;
        // get another argument, pass NULL so it keeps doing line
        //indv_arg = strtok(NULL, " \t\n");
        //save_pnt = line;
        indv_arg = get_next_token(NULL, save_pnt);
    }
    return args;
}

int run(char** args) {
    // check first that the program/command isn't a built in
    int i = 0;
    //fprintf(stdout, "%s\n", args[0]);
    while(strcmp(internal_cmd_strs[i], "EOL") != 0) {
        if (strcmp(args[0], internal_cmd_strs[i]) == 0) {
            // it's an internal command, so run that and return whatever it returns
            //fprintf(stdout, "running internal command\n");
            int retVal = (*internal_cmds[i])(args);
            return retVal;
        }
        //fprintf(stdout, "%s!=%s\n", args[0], internal_cmd_strs[i]);
        //fflush(stdout);
        i++;
    }
    //fprintf(stdout, "running as system program\n");
    //fflush(stdout);
    // if we've got here in execution then it's a program to run
    if (fork()==0) {
        // use execvp because we can pass array of args instead of listing them
        int retVal = execvp(args[0], args);
        if (retVal == -1) {
            fprintf(stderr, "\"%s\" is not a valid program name or internal command\n", args[0]);
            fflush(stderr);
            exit(-1);
        }
    }
    // child won't reach here
    // wait for child to exit and store it's exit value in prev_result
    int retVal = wait(&prev_result);
    if (retVal < 0) {
        //quit(args);
        //exit(0);
    }
    // now we're done
    return 1;
}

int main(int argc, const char** argv) {
    // load config files
    if (init() < 0) {
        fprintf(stdout, "Configuration failed");
        fflush(stdout);
        return -1;
    }

    // run main loop
    main_loop();
    return 0;
}