
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// similar to strtok_r but handling some escapes and quotes
char* get_next_token(char* str, char* save_pnt) {
    const char whitespace[2] = " \t";
    char* start;
    char* stop;
    // if str==NULL then need to continue (probs)
    if (str == NULL) {
        str = save_pnt;
    }
    // if str points to nullchar then at end of string, return NULL
    if (str[0] == '\0') {
        return NULL;
    }
    // scan past initial whitespace
    start = str + strspn(str, whitespace);
    if (start[0] == '\0') {
        // it was all whitespace, nothing to return
        save_pnt = start;
        return NULL;
    }
    // now step through rest of string looking for whitespace or quotes
    stop = start;
    while(stop[0] != '\0' && strcspn(stop, " \t\"") > 0) {
        if (stop[0] == '\\') {
            stop = stop + 2;
        }
        stop++;
    }
    if (stop[0] != '\0') {
        save_pnt = stop + 1;
    } else {
        save_pnt = stop;
    }
    int len = stop - start+1;
    char* tok = (char*)malloc(sizeof(char)*len);
    strncpy(tok, start, len);
    tok[len-1] = '\0';
    fprintf(stderr, "%s\n", tok);
    return tok;
}