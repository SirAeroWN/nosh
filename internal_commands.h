
extern int prev_result;

// internal commands
int cd(char** args);
int quit(char** args);
int exit_status(char** args);

char* internal_cmd_strs[] = {
    "cd",
    "exit",
    "quit",
    "!?",
    "EOL"
};

// function pointer list to avoid using switch/ifs
// its a really neat trick, explainer here: https://www.geeksforgeeks.org/function-pointer-in-c/
int (*internal_cmds[]) (char**) = {
    &cd,
    &quit,
    &quit,
    &exit_status
};