#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1

const uint INT_LEN=sizeof(int);

//左管道第一个数据
int left_first_data(int lpipe[2],int* dst){
    if(read(lpipe[RD],dst,sizeof(int))==sizeof(int)){
        printf("prime %d\n",*dst);
        return 0;
    }
    return 1;
}

//左管道剩余数据
void left_data(int lpipe[2],int rpipe[2],int first){
    int data;
    while(read(lpipe[RD],&data,sizeof(int))==sizeof(int)){
        if(data%first!=0){
            //无法整除数据传入右管道
            write(rpipe[WR],&data,sizeof(int));
        }
    }
    close(lpipe[RD]);
    close(rpipe[WR]);
}

void primes(int lpipe[2]){
    close(lpipe[WR]);
    int first;
    if(left_first_data(lpipe,&first)==0){
        int p[2];
        pipe(p);
        left_data(lpipe,p,first);
        //递归
        if(fork()==0){
            primes(p);
        }else{
            close(p[RD]);
            wait(0);
        }
    }
    exit(0);
}

int
main(int argc,char *argv[])
{
    int p[2];
    pipe(p);
    for(int i=2;i<=35;i++){
        write(p[WR],&i,INT_LEN);
    }
    if(fork()==0){
        primes(p);
    }else{
        close(p[WR]);
        close(p[RD]);
        wait(0);
    }
    exit(0);
}