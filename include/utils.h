#ifndef UTILS_H
#define UTILS_H


#include "common.h"


// Cross-platform masked password input (exactly PASSWD_LEN chars)
void get_password_masked(char *out, size_t outsz);


// Input helpers
int prompt_int(const char *msg, int minVal, int maxVal);
void prompt_string(const char *msg, char *buf, size_t bufsz);


#endif // UTILS_H