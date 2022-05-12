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


struct message {
    long mtype ;
    char val[4000];
};

const int categories_number = 2;
void write_haiku(int category);


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

int access_queue ( void ) {
    key_t k ;
    int id ;
    k = ftok ("/etc/passwd", 'F') ;
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

int write_value_msg_queue (int id , char* val) {
    struct message m ; int r ;
    m.mtype = 25;
    strcpy(m.val, val);
    r = msgsnd (id , &m, sizeof m - sizeof m.mtype , 0) ;
    if (r!=-1)
    {
        return 0;
    }
    return -1;
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
    printf("-----------------Writer start-----------------\n");
    srand(time(NULL));
    int type = rand() % 2 + 1;

    write_haiku(type);
}


void* reader(void* arg) {
    printf("-----------------Reader start-----------------\n");
    
    int queue_id = access_queue();
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


void write_haiku(int category) {
     int queue_id = access_queue();

    char file_con[3000];
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
    fclose(fptr);

     write_value_msg_queue(queue_id, file_con);
}



int main(int argc, char** argv) {
    int id_q = create_queue();
    printf("Created Queue Id: %d\n", id_q);


    pthread_t reader_tid, writer_tid; int e;
    
    // Starting reader thread
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
    
    remove_queue(id_q);

    return 0;
}

