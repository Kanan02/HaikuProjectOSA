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
#include <pthread.h>
struct pid_message {
    long mtype ;
    pid_t val;
};

struct message {
    long mtype ;
    char val[4000];
};
void signal_handler(int sig);
char* get_haiku_type(int sig);
int create_queue ( char q );
void remove_queue ( int id );
void write_pid_msg_queue (int id , pid_t val) ;

void *writer (void * arg);
void* reader(void* arg);
int main()
{
    int pid_mq_id=create_queue('F');
   
    write_pid_msg_queue(pid_mq_id,getpid());
    printf("Server is running!\nQueue has been created!\n");

    pthread_t reader_tid, writer_tid; int e;
    if ((e = pthread_create(&reader_tid, NULL, reader, 0)) != 0) {
        perror("Error creating reader thread");
    }

    if ((e = pthread_create(&writer_tid, NULL, writer, 0)) != 0) {
        perror("Error creating writer thread");
    }

    void* r;
    if ((e = pthread_join(writer_tid, &r)) != 0) {
        printf("Error waiting thread");
    }
    if ((e = pthread_join(reader_tid, &r)) != 0) {
        printf("Error waiting thread");
    }
    remove_queue(pid_mq_id);
        
}




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

int create_queue ( char q  ) {
    key_t k ;
    int id ;
    k = ftok ("/etc/passwd", q) ;
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

void write_pid_msg_queue (int id , pid_t val) {
    struct pid_message m ; int r ;
    m.mtype = 25;
    m.val=val;
    r = msgsnd (id , &m, sizeof m - sizeof m.mtype , 0) ;
}


void write_value_msg_queue (int id , char* val) {
    struct message m ; int r ;
    m.mtype = 25;
    strncpy(m.val, val,strlen(val));
    
    r = msgsnd (id , &m, sizeof m - sizeof m.mtype , 0) ;
}



void write_haiku(int category) {
   
    int queue_id = access_queue('A');
    char file_con[10000];
    char * categories[] = {"../../haiku_reference/japanese.txt","../../haiku_reference/western.txt"};
    
    FILE* fptr; char ch; int i = 0;
    if (category == 1) {
        puts("\tWriting japanese haikus");
    } 
    else if (category == 2) {
        puts("\tWriting western haikus");
    } 

    fptr = fopen(categories[category-1], "r");
    if (fptr == NULL) {printf("Error reading file\n"); exit(1);}
    while (ch != EOF) {
        ch = fgetc(fptr);  
        file_con[i]=ch;
        i++;
    }
    write_value_msg_queue(queue_id, file_con);
    fclose(fptr);
    ch=' ';
}


char* read_value_msg_queue (int id, struct message* m) {
    int r;
    r = msgrcv (id , m, sizeof *m - sizeof m->mtype, 25, 0) ;
    if (r == -1){
        printf (" msgrcv ") ;
    }
    return m->val;
}

void *writer (void * arg) {
    int haiku_mq_id=create_queue('A');
    printf("-----------------Writer start-----------------\n");

    struct sigaction s;
    s.sa_handler = signal_handler;
    s.sa_flags=0;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &s, NULL); 
    sigaction(SIGQUIT, &s, NULL);
    sigaction(SIGTSTP, &s, NULL); 
    while (1){
        
    }
    remove_queue(haiku_mq_id);

}
int queue_id=0;
void signal_handler2(int sig);
void* reader(void* arg) {
    while (1)
    {
        queue_id = access_queue('A');
        printf("-----------------Reader start-----------------\n");
        struct message m;
        char* val = read_value_msg_queue(queue_id, &m);
        printf("\n");

        int count = 0, i = 0;
        while (count < 3) {
            printf("%c", m.val[i]);
            i++;
            if (m.val[i] == '\n') count++;
        }
        printf("\n");

    }
}

void signal_handler(int sig)
{
    if (sig==20)
    {
        kill(getpid(),9);
        
    }
    else if (sig==SIGINT)
    { 
        write_haiku(1);
       
    }
    else if(sig==SIGQUIT){
        write_haiku(2);
    }
}
