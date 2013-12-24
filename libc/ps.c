#include<syscall.h>
#include<stdio.h>
#include <defs.h>
#include<stdlib.h>
void ps() {
__syscall0(65);
return;
}
