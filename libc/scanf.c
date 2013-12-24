#include<syscall.h>
#include<stdio.h>
#include <defs.h>
void scanf(char* a, void *b)
{
	uint64_t base0=(uint64_t)a;
	uint64_t base=(uint64_t)b;
	//scanf("%s",c);
	__syscall2(13,base0,base);

}