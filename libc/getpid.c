#include<syscall.h>
#include<stdio.h>
#include <defs.h>
#include<stdlib.h>

int getpid() {
uint64_t pid = 0x0;
__syscall1(60,(uint64_t)&pid );
return (int)pid ;
}