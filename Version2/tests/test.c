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
#include "CUnit/Basic.h"

static FILE* temp_file = NULL;
struct message {
    long mtype ;
    char val[4000];
};
int create_queue ( void ) ;
int access_queue ( void );
void remove_queue ( int id );
int write_value_msg_queue (int id , char* val);
char* read_value_msg_queue (int id, struct message* m);
int init_suite1(void)
{
   if (NULL == (temp_file = fopen("temp.txt", "w+"))) {
      return -1;
   }
   else {
      return 0;
   }
}

int clean_suite1(void)
{
   if (0 != fclose(temp_file)) {
      return -1;
   }
   else {
      temp_file = NULL;
      return 0;
   }
}



void testWRITE_VALUE_MESSAGE_QUEUE(void){
    int id=create_queue();

   int r=write_value_msg_queue(id,"Hello");
   CU_ASSERT_NOT_EQUAL(r,-1);
}
void testREAD_VALUE_MESSAGE_QUEUE(){
   char*file_con=malloc(1000);
   //get_matrix_content(file_con);
   CU_ASSERT_PTR_NOT_NULL(file_con);
}

int main()
{
   CU_pSuite clientSuite = NULL,serverSuite=NULL;
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   clientSuite = CU_add_suite("Client_Suite", init_suite1, clean_suite1);
   serverSuite = CU_add_suite("Server_Suite", init_suite1, clean_suite1);
   if (NULL == clientSuite||NULL==serverSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if (NULL == CU_add_test(serverSuite, "test of write_value_message_queue()", testWRITE_VALUE_MESSAGE_QUEUE)
  )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
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
    return r;
}

char* read_value_msg_queue (int id, struct message* m) {
    int r;
    r = msgrcv (id , m, sizeof *m - sizeof m->mtype, 25, 0) ;
    if (r == -1){
        printf (" msgrcv ") ;
    }
    return m->val;
}




void write_haiku(int category) {
     int queue_id = access_queue();

    char file_con[3000];
    char * categories[] = {"../haiku_reference/japanese.txt","../haiku_reference/western.txt"};
    
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
