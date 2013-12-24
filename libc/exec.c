#include<syscall.h>
#include<stdio.h>
#include <defs.h>
#include<stdlib.h>
void exec(char* a,int b) {
__syscall2(55,(uint64_t)a,(uint64_t)b);
}
