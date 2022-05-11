#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

int sigrand()
{
    int n=rand()%2+2;
    return n;    
}
int main(int argc, char const *argv[])
{
    srand(time(0));
    for(int i=0;i<100;i++){
        kill(getppid(),sigrand());
        usleep(10000);
    }

    return 0;
}