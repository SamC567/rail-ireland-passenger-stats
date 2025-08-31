#ifndef LOGIN_H
#define LOGIN_H


#include "common.h"


typedef struct LoginNode {
char username[32];
char password[PASSWD_LEN+1]; // fixed length 6 + NUL
struct LoginNode *next;
} LoginNode;


LoginNode* load_logins(const char *path);
void free_logins(LoginNode *head);
bool authenticate(LoginNode *head);


#endif // LOGIN_H