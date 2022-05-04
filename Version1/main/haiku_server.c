#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
void signal_handler(int sig);
char* get_haiku_type(int sig);

int main()
{

    if (fork()==0)
    {
        execlp("./haiku_client","haiku_client",NULL);
    }
    else{
        struct sigaction s;
        s.sa_handler = signal_handler;
        s.sa_flags=0;
        sigemptyset(&s.sa_mask);
        s.sa_flags = SA_SIGINFO;
        sigaction(SIGINT, &s, NULL); // change here to your signal
        sigaction(SIGQUIT, &s, NULL); // add how many you want
        sigaction(SIGTSTP, &s, NULL); // add how many you want
        printf("Haiku server, press Ctrl+Z to kill this process.\n");
        while (1){

        }
        
        pause();
    }
}

void signal_handler(int sig)
{
    char* haiku_type;
    if (sig==20)
    {
        kill(getpid(),9);
        
    }
    haiku_type=get_haiku_type(sig);
    printf("Haiku type is: %s\n",haiku_type);
}

void get_matrix_content(char* file_con){
    FILE* ptr;
    int i=0;
    char ch;
    ptr = fopen("../../haiku_reference/signal_type_haiku_type_matrix.txt", "r");
    if (NULL == ptr) {
        printf("file can't be opened \n");
    }
    do {
        ch = fgetc(ptr);
        file_con[i]=ch;
        i++;
    } while (ch != EOF);

    fclose(ptr);
}


char* get_haiku_type(int sig){
    char file_content[1000];
    get_matrix_content(file_content);
    
    /*split my content 2 times: by lines and by :*/
    char *token;
    token = strtok(file_content, "\n");
    while( token != NULL ) {
        char*tmp_token=malloc(strlen(token)+1);
        strcpy(tmp_token,token);
        token = strtok(NULL, "\n");
        char *token2;
        token2 = strtok(tmp_token, ":");
        if (atoi(token2)==sig)
        {
            return strtok(NULL, ":");
        }
    }
}

