#include<syscall.h>
#include<stdio.h>
#include <defs.h>
#include <stdlib.h>



void clrscr() {

__syscall0(11);
}