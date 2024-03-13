#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc,char *argv[])
{   
    //子进程写入管道
    int pchild[2];
    //父进程写入管道
    int pparent[2];
    pipe(pchild);
    pipe(pparent);
    int pid=fork();
    int status=0;
    char buf='P';
    if(pid<0){
        fprintf(2,"fork() error!\n");
        close(pchild[0]);
        close(pchild[1]);
        close(pparent[0]);
        close(pparent[1]);
        exit(1);
    }
    else if(pid==0){
        close(pparent[1]);
        close(pchild[0]);

        if(read(pparent[0],&buf,sizeof(char))!=sizeof(char)){
            fprintf(2,"child read() error!\n");
            status=1;
        }else{
            fprintf(1,"%d: received ping\n",getpid());
        }     

        if(write(pchild[1],&buf,sizeof(char))!=sizeof(char)){
            fprintf(2,"child write() error!\n");
            status=1;
        }

        close(pparent[0]);
        close(pchild[1]);
        exit(status);
    }else{
        close(pparent[0]);
        close(pchild[1]);
        if(write(pparent[1],&buf,sizeof(char))!=sizeof(char)){
            fprintf(2,"parent write() error!\n");
            status=1;
        }
        
        if(read(pchild[0],&buf,sizeof(char))!=sizeof(char)){
            fprintf(2,"parent read() error!\n");
            status=1;
        }else{
            fprintf(1,"%d: received pong\n",getpid());
        }
        close(pparent[1]);
        close(pchild[0]);
        exit(status);
    }
    exit(status);
}