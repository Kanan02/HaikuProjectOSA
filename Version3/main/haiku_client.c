#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <time.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct pid_message {
    long mtype ;
    pid_t val;
};
struct message {
    long mtype ;
    char val[4000];
};
const int categories_number = 2;
void write_haiku(int category);
void reader();

int access_queue ( char q ) {
    key_t k ;
    int id ;
    k = ftok ("/etc/passwd", q) ;
    if (k == -1){
        printf (" ftok ") ;}
    id = msgget (k, 0) ;
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


pid_t read_pid_msg_queue (int id, struct pid_message* m) {
    int r;
    r = msgrcv (id , m, sizeof *m - sizeof m->mtype, 25, 0) ;
    if (r == -1){
        printf (" msgrcv ") ;
    }
    return m->val;
}

int server_pid=0;
void signal_handler(int sig){
    kill(server_pid,sig);
    if (sig==20)
    {
        kill(getpid(),9);
    }

}
int main(int argc, char const *argv[])
{
    int id_q = access_queue('F');
    struct pid_message m;
    server_pid =read_pid_msg_queue(id_q,&m);
    printf("Server process id has been obtained!\n");
    struct sigaction s;
    s.sa_handler = signal_handler;
    s.sa_flags=0;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &s, NULL); 
    sigaction(SIGQUIT, &s, NULL); 
    sigaction(SIGTSTP, &s, NULL);
    printf("Haiku client, press Ctrl+Z to kill both server and client.\n");
    while (1){
       
    }
    remove_queue(id_q);

    return 0;
}
