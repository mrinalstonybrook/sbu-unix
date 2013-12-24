#include<syscall.h>
#include<stdio.h>
#include <defs.h>
#include <stdlib.h>
void user_fork(void){
__syscall0(25);
}