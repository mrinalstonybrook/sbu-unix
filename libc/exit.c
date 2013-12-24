#include<syscall.h>
#include<stdio.h>
#include <defs.h>
void exit(int status) {
__syscall1(0,1);
}
