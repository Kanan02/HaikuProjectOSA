#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
struct message {
    long mtype ;
    pid_t val;
};
void signal_handler(int sig);
char* get_haiku_type(int sig);
int create_queue ( void );
void remove_queue ( int id );
void write_value_msg_queue (int id , pid_t val) ;


int main()
{
    int id_q=create_queue();
    write_value_msg_queue(id_q,getpid());
    printf("Server is running!\nQueue has been created!\n");
    struct sigaction s;
    s.sa_handler = signal_handler;
    s.sa_flags=0;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &s, NULL); 
    sigaction(SIGQUIT, &s, NULL); // add how many you want
    sigaction(SIGTSTP, &s, NULL); // add how many you want
    while (1){

    }
    remove_queue(id_q);
}





int create_queue ( void ) {
    key_t k ;
    int id ;
    k = ftok ("/etc/passwd", 'F') ;
    if (k == -1){
        printf (" ftok ") ;}
    id = msgget (k, IPC_CREAT | 0666) ;
    if (id == -1){
        printf (" msgget ") ;}
    return id ;
}


void remove_queue ( int id ) {
    int r ;
    r = msgctl (id , IPC_RMID , NULL ) ;
    if (r == -1){
        printf (" msgctl ") ;}
}

void write_value_msg_queue (int id , pid_t val) {
    struct message m ; int r ;
    m.mtype = 25;
    m.val=val;
    r = msgsnd (id , &m, sizeof m - sizeof m.mtype , 0) ;
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
    ptr = fopen("../haiku_reference/signal_type_haiku_type_matrix.txt", "r");
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

