// src/utils.c
#include "utils.h"
// If your utils.h doesn't include <stdbool.h>, include it here too.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
  #include <conio.h>
#else
  #include <termios.h>
  #include <unistd.h>
#endif

// ------------------------------------------------------------
// Case-insensitive equality helper (used by passenger.c)
bool ci_equal(const char *a, const char *b) {
    if (!a || !b) return false;
    while (*a && *b) {
        unsigned char ca = (unsigned char)*a++;
        unsigned char cb = (unsigned char)*b++;
        if (tolower(ca) != tolower(cb)) return false;
    }
    return *a == '\0' && *b == '\0';
}
// ------------------------------------------------------------

void get_password_masked(char *out, size_t outsz){
    if(!out || outsz == 0) return;
    size_t idx = 0;
    printf("Enter 6-char password: ");

#ifdef _WIN32
    while(idx < PASSWD_LEN){
        int ch = _getch();
        if(ch == '\r' || ch == '\n') continue;            // ignore Enter until 6 chars
        if(ch == '\b'){
            if(idx > 0){ idx--; printf("\b \b"); }
            continue;
        }
        if(idx + 1 < outsz){
            out[idx++] = (char)ch;
            printf("*");
        }
    }
    out[idx] = '\0';
    printf("\n");
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    // disable echo (cast to quiet -Wconversion on tcflag_t)
    newt.c_lflag = (tcflag_t)(newt.c_lflag & (tcflag_t)~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while(idx < PASSWD_LEN){
        int ch = getchar();
        if(ch == '\n' || ch == '\r') continue;
        if(ch == 127 || ch == '\b'){                      // backspace
            if(idx > 0){ idx--; printf("\b \b"); fflush(stdout); }
            continue;
        }
        if(idx + 1 < outsz){
            out[idx++] = (char)ch;
            printf("*"); fflush(stdout);
        }
    }
    out[idx] = '\0';

    // restore terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");
#endif
}

int prompt_int(const char *msg, int minVal, int maxVal){
    int x; char line[128];
    for(;;){
        printf("%s", msg);
        if(!fgets(line, sizeof(line), stdin)) continue;
        char *end = NULL;
        long v = strtol(line, &end, 10);
        if(end == line){ puts("Invalid number."); continue; }
        if(v < minVal || v > maxVal){
            printf("Enter a value between %d and %d.\n", minVal, maxVal);
            continue;
        }
        x = (int)v; break;
    }
    return x;
}

void prompt_string(const char *msg, char *buf, size_t bufsz){
    for(;;){
        printf("%s", msg);
        if(!fgets(buf, bufsz, stdin)) continue; // use size_t as fgets expects
        trim_newline(buf);
        if(strlen(buf) == 0){ puts("Input cannot be empty."); continue; }
        break;
    }
}

