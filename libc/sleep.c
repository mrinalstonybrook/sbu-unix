#include<syscall.h>
#include<stdio.h>
#include <defs.h>
extern int sleep_time;
void sleep(int time) {
__syscall1(1,time);
}
