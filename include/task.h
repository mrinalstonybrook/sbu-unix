#ifndef _TASK_H
#define _TASK_H
#include<defs.h>
/*
struct task_struct{
	void *rsp;
	 uint64_t *stack;
        int pid;
        void (*fptr)(void);
};

*/

#define KERNEL_STACK_SIZE 4096
#define USER_STACK_SIZE 1000

struct task_struct{
	void* rsp;
       void* kernel_rsp;
       void* rip;
       uint64_t pml4e;
       void* kernel_stack[KERNEL_STACK_SIZE];
       void** user_stack;
	int sleep_count ; 
 	int pid; 
	int num_count;
       uint64_t rflags;
	struct task_struct* next; 
	struct task_struct* parent; 
	struct task_struct* last_run;
       struct mm_struct *mm;
	
};

typedef struct task_struct task_struct;
void crt_new_prs_stack(task_struct* task_struct_ptr, void* fun_ptr);
void crt_new_user_prs_stack(task_struct* task_struct_ptr, void* fun_ptr);
void add_task_to_ready(task_struct* task_struct_ptr);
int sys_exec(char* a,int b);
int sys_fork(void);
int getcurpid();
 void getps();
#endif
