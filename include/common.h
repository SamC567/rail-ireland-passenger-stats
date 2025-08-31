#ifndef COMMON_H
#define COMMON_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


#define MAX_NAME 30
#define MAX_EMAIL 64
#define MAX_AREA 16
#define MAX_CLASS 16
#define MAX_TRIPS 32


#define DATA_DIR "data"
#define LOGIN_FILE DATA_DIR "/login.txt"
#define PASSENGER_FILE DATA_DIR "/passenger.txt"
#define REPORT_FILE "report.txt"


// Fixed 6-char passwords per brief
#define PASSWD_LEN 6


// Safe input helpers
static inline void trim_newline(char *s){ if(!s) return; size_t n=strlen(s); if(n>0 && (s[n-1]=='\n' || s[n-1]=='\r')) s[n-1]='\0'; }


static inline void flush_stdin(void){ int c; while((c=getchar())!='\n' && c!=EOF){} }


#endif // COMMON_H