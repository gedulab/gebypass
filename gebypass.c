/*
* gebypass - a tutorial sample to bypass password using heap buffer overflow.
* Raymond Zhang 2023-12-2 
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define GE_MAX_PASSWORD 20
#define GE_TOKEN_SIZE   26
#define GE_TOKEN_PREFIX "#TOKEN#"
#define GE_MINI_PASSWORD 6

int ge_set_token(char* user_token, int size)
{
    if(size < GE_TOKEN_SIZE)
        return -1;

    snprintf(user_token, size, "%s:%08d%08d#", GE_TOKEN_PREFIX, getpid(), getppid());
    return 0;
}

int ge_check_token(char* user_token, int size)
{
    int i;
    if(strncmp(user_token, GE_TOKEN_PREFIX, strlen(GE_TOKEN_PREFIX))!=0){
        return -1;
    }
    char* ptr = user_token+ strlen(GE_TOKEN_PREFIX) + 1;
    for( i = 0; i< 16; i++) {
        if(isdigit(*ptr) == 0){
            return -1;
        }
        ptr++;
    }
    if(*ptr != '#')
        return -1;

    return 0;
}

void ge_auth(const char* input, char* user_token, int size)
{
    // check the password via user database etc.
    if(strcmp(input, "$$secret$$") == 0){
        printf("Your password is correct\n");
        ge_set_token(user_token, size);
    }else{
        printf("You have entered the incorrect password\n");
    }
}

int main()
{
    int ret = -1;
    char * buffer = (char*)malloc(GE_MAX_PASSWORD);
    char * token = (char*)malloc(GE_TOKEN_SIZE);

    puts("Please enter password:");
    if(gets(buffer) == NULL || strlen(buffer) < GE_MINI_PASSWORD)  {
        printf("password is too short %ld\n", strlen(buffer));
        goto tag_exit;
    }
    ge_auth(buffer, token, GE_TOKEN_SIZE);

    if(ge_check_token(token, GE_TOKEN_SIZE) != 0) {
        printf("Login failed. You are denied.\n");
        goto tag_exit;
    }
    printf("Login succeeded. You are welcome.\n");
    puts("You can do privileged operations now...\n");
    getchar();
    ret = 0;

tag_exit:
    free(buffer);
    free(token);

    return ret;
}