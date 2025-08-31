#include "login.h"
#include "utils.h"


static LoginNode* push_login(LoginNode **head, const char *u, const char *p){
LoginNode *n = (LoginNode*)malloc(sizeof(LoginNode));
if(!n){ perror("malloc"); exit(EXIT_FAILURE);}
strncpy(n->username, u, sizeof(n->username)-1); n->username[sizeof(n->username)-1]='\0';
strncpy(n->password, p, PASSWD_LEN); n->password[PASSWD_LEN]='\0';
n->next = *head; *head = n; return n;
}


LoginNode* load_logins(const char *path){
FILE *f = fopen(path, "r");
if(!f){ perror("open login file"); return NULL; }
LoginNode *head=NULL; char user[64], pass[64];
while(fscanf(f, "%63s %63s", user, pass)==2){
if(strlen(pass)!=PASSWD_LEN){ fprintf(stderr, "Skipping login with non-6-char password: %s\n", user); continue; }
push_login(&head, user, pass);
}
fclose(f);
return head;
}


void free_logins(LoginNode *head){
while(head){ LoginNode *n=head->next; free(head); head=n; }
}


bool authenticate(LoginNode *head){
if(!head){ puts("No login records loaded."); return false; }
char user[64]; char pass[PASSWD_LEN+1];
int attempts=3;
while(attempts-- > 0){
prompt_string("Username: ", user, sizeof(user));
get_password_masked(pass, sizeof(pass));
for(LoginNode *n=head; n; n=n->next){
if(strcmp(n->username, user)==0 && strncmp(n->password, pass, PASSWD_LEN)==0){
puts("Login successful.\n");
return true;
}
}
printf("Invalid credentials. %d attempt(s) remaining.\n\n", attempts);
}
return false;
}