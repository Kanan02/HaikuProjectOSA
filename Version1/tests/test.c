#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "CUnit/Basic.h"

static FILE* temp_file = NULL;

int sigrand();
char* get_haiku_type(int sig);
void get_matrix_content(char* file_con);


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



void testSIGRAND(void){
   int num=sigrand();
   CU_ASSERT(num==2||num==3);
}
void testGET_MATRIX_CONTENT(){
   char*file_con=malloc(1000);
   get_matrix_content(file_con);
   CU_ASSERT_PTR_NOT_NULL(file_con);
}
void testGET_HAIKU_TYPE(){
   char* type = get_haiku_type(2);
   CU_ASSERT_STRING_EQUAL(type,"japanese");
}
void test2GET_HAIKU_TYPE(){
   char* type = get_haiku_type(3);
   CU_ASSERT_STRING_EQUAL(type,"western");
}
void test3GET_HAIKU_TYPE(){
   char* type = get_haiku_type(1);
   CU_ASSERT_STRING_EQUAL(type,"Error");
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
   if (NULL == CU_add_test(clientSuite, "test of sigrand()", testSIGRAND)
      || NULL == CU_add_test(serverSuite, "test of get_matrix_content()", testGET_MATRIX_CONTENT)
      ||NULL == CU_add_test(serverSuite, "test of get_haiku_type()", testGET_HAIKU_TYPE)
      ||NULL == CU_add_test(serverSuite, "second test of get_haiku_type()", test2GET_HAIKU_TYPE)
      ||NULL == CU_add_test(serverSuite, "third test of get_haiku_type()", test3GET_HAIKU_TYPE)
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



int sigrand()
{
   int n=rand()%2+2;
   return n;    
}
char* get_haiku_type(int sig){
    char file_content[1000];
    get_matrix_content(file_content);
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
    return "Error";
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
