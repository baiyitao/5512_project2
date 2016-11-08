#include <signal.h> 
#include <setjmp.h> 
#include <stdarg.h> 
#include <stdlib.h> 
#include <stdio.h>
#include<unistd.h>


jmp_buf env;

//int tmp = 1;
//int* s = 123;

//信号处理函数 

void recvSignal(int sig) {
    printf("received signal %d !!!\n", sig);
    siglongjmp(env, 1);
}

int readAddr(void *p, int *v) {
    //保存上下文
    printf("before siggetjmp\n");
    int r = sigsetjmp(env, 1);
    if (r == 0) {
        //初次来到这里， r=0
        //给信号注册一个处理函数  
        signal(SIGSEGV, recvSignal);
        printf("下面要运行可能报段错的代码，如果不报错，就不进入handler直接返回0\n");

        v = (int *) (p);
        printf("output the v in readAddr: %d\n\n", v);
        return 0;

        //        if (tmp) {
        //            tmp = 0;
        //            printf("\n\n不 ok \n\n");
        //            int* s = 0;
        //            (*s) = 1;
        //
        //            //char *phys_addr = virt_to_phys(p);
        //            //printf("word is %c", phys_addr);               
        //
        //        } else {
        //            printf("\n\nok\n\n\n");
        //            tmp = 1;
        //        }
        //
        //        printf("address of s is %p, :%d\n\n", s, s);
        //        (s)++;
        //        printf("address of s is %p,!!!!!!!!!!!!!!!!!!!!!!\n\n", s);
        //
        //        printf("address of s is %p, :%d\n\n", s, s);
        //char *phys_addr = virt_to_phys(p);
        //        v = p;
        //
        //        printf("v is %c\n\n\n", v);
        //        printf("address of p is %p, value is :%d\n\n", &p, p);
        //        return 0;
    } else {
        //这里是longjmp跳转回来的
        printf("Return from handler.\n");
        printf("jump this code!!\n");
        printf("发生错误，由longjmp跳转，并且sigset返回非0\n");
        return -1;
    }



}

int main(int argc, char** argv) {


    unsigned long TASK_SIZE = 0, PAGE_SIZE = 0;

    PAGE_SIZE = sysconf(_SC_PAGESIZE);

    printf("page_size is %d\n\n", PAGE_SIZE);

    TASK_SIZE = 0xc0000000UL; //32bit



    printf("before for\n");


    int *p = 0;
    for (; p < TASK_SIZE; p += PAGE_SIZE * 1024) {
        int a = 0;
        int r = readAddr(p, &a);
        if (r == -1) {
            printf("\n\n读到无效地址\n");
            printf("%p: %d\n", p, a);
        } else {
            printf("%p: %d\n", p, a);
            printf("读到有效地址\n");
        }

    }


    while (1) {
        sleep(1);
        printf("sleep 1 \n");
    }
    return 0;
} 