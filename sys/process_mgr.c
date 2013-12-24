#include <task.h>
#include <stdio.h>
#include <defs.h>
#include <math.h>
#include <sys/pic.h>
#include <pagetab.h>
#include <sys/gdt.h>
#include <pmap.h>
#include <mm.h>
#include <sys/tarfs.h>


void timer_interrupt_1(void);
static task_struct* prev = NULL;
task_struct* curr = NULL;
//static task_struct* next = NULL;
static task_struct* ready_task = NULL;
static task_struct* sleep_task = NULL;
//static task_struct* zombie_task = NULL;

static int timer_ticks = 0;
static int number_seconds = 0;
static int scheduler_start = 0;

static int global_pid = 0;
 int process_instruction = 0;
int schedule = 0;

int getcurpid(){
  return curr->pid;
} 

void set_user_stack(task_struct* task_struct_ptr){
       task_struct_ptr->user_stack[USER_STACK_SIZE-1] = (void *)0x23;
	task_struct_ptr->user_stack[USER_STACK_SIZE -2] = &task_struct_ptr->user_stack[USER_STACK_SIZE -1];
	task_struct_ptr->user_stack[USER_STACK_SIZE -3] = (void *)0x20202;
	task_struct_ptr->user_stack[USER_STACK_SIZE -4] = (void *)0x1b;
	task_struct_ptr->user_stack[USER_STACK_SIZE -5] = task_struct_ptr->rip;

}

void create_user_stack_exec(task_struct* t1){
 
     t1->user_stack =curr->user_stack;
       set_user_stack(t1);
       t1->rsp = &curr->user_stack[USER_STACK_SIZE -21] ;     
}

int sys_exec(char* a,int b) {
   	
   task_struct* new_task = load_elf(a);
   struct PML4 * curr_pml4e = (struct PML4 *)curr->pml4e;
   new_task->sleep_count = b;
   new_task->pml4e = (uint64_t)curr_pml4e;
   new_task->pid = curr->pid;   
   new_task->parent = curr->parent;
   create_user_stack_exec(new_task);

   new_task->next = ready_task;
   ready_task = new_task;
       
   __asm__ __volatile__("sti");
   schedule=1;
   timer_interrupt_1();
   return 0;

}


int sys_fork()
{
	__asm__ __volatile__("cli");
	struct task_struct *new_task = kmalloc(sizeof(struct task_struct));    
       crt_new_user_prs_stack(new_task, curr->rip);
       new_task->pml4e = NULL;	
	new_task->parent = curr;
       new_task->mm = curr->mm;


	//copy_page_tables((uint64_t)curr_pml4e, (uint64_t)new_task->pml4e);
       schedule =1;
       new_task->next = ready_task;
       ready_task = new_task;
       
	__asm__ __volatile__("sti");

	timer_interrupt_1();
	return 0;
}


void decr_sleep_count(){
	task_struct *task_ptr_head = sleep_task;

	if (task_ptr_head == NULL)
		return;
	else{
		if (task_ptr_head->sleep_count == 0){
		
		sleep_task = task_ptr_head->next;
		task_ptr_head->next= NULL;
		  task_ptr_head->next = ready_task;
                  ready_task = task_ptr_head;
		task_ptr_head = sleep_task;
              schedule = 1;
		}
		task_struct *temp1;
		task_struct *temp = task_ptr_head;
			if(temp==NULL)
			return;
  			do
			{
			if(temp->sleep_count == 0){
				task_ptr_head->next = temp->next;
				temp1 = temp->next;
				temp->next = NULL;
                             //schedule =1;
                             add_task_to_ready(temp->next);        			
				temp = temp1;
				}

			else{
				temp->sleep_count--;
				task_ptr_head = temp;
				temp= temp->next;
			   }
			//task_ptr_head = temp;
			
    		}while(temp != NULL);
            
		return;
	}	

}


void add_task_to_sleep(task_struct* task_struct_ptr)
{


	task_struct* task_ptr = sleep_task;
	task_struct_ptr->next = NULL;
	if(task_ptr == NULL){

		sleep_task = task_struct_ptr;
	       }
         else {
		  while(task_ptr->next != NULL) {
			task_ptr = task_ptr->next;
		   }
		task_ptr->next = task_struct_ptr;
	       }

}


void sleep(int time){
//curr->sleep_count = time;
process_instruction = 2;
add_task_to_sleep(curr);
schedule=1;
timer_interrupt_1();



}





////////////////
void add_task_to_ready(task_struct* task_struct_ptr)
{
	task_struct* task_ptr = ready_task;
	task_struct_ptr->next = NULL;
	if(ready_task == NULL){

		ready_task = task_struct_ptr;
	       }
         else {
		  while(task_ptr->next != NULL) {
			task_ptr = task_ptr->next;
		   }
		task_ptr->next = task_struct_ptr;
	       }
}


void crt_new_prs_stack(task_struct* task_struct_ptr, void* fun_ptr)
{	
	task_struct_ptr->rsp = &task_struct_ptr->kernel_stack[KERNEL_STACK_SIZE -21];
	task_struct_ptr->rip = fun_ptr;
	task_struct_ptr->rflags = 0x20202;
	task_struct_ptr->next = NULL;
	task_struct_ptr->num_count = 0;
	task_struct_ptr->last_run = NULL;
	task_struct_ptr->kernel_stack[KERNEL_STACK_SIZE -1] = (void *)0x10;
	task_struct_ptr->kernel_stack[KERNEL_STACK_SIZE -2] = &task_struct_ptr->kernel_stack[KERNEL_STACK_SIZE -1];
	task_struct_ptr->kernel_stack[KERNEL_STACK_SIZE -3] = (void *)0x20202;
	task_struct_ptr->kernel_stack[KERNEL_STACK_SIZE -4] = (void *)0x08;
	task_struct_ptr->kernel_stack[KERNEL_STACK_SIZE -5] = fun_ptr;
	task_struct_ptr->pml4e = NULL;
	task_struct_ptr->user_stack = NULL;	
	
	add_task_to_ready(task_struct_ptr);
}

void crt_new_user_prs_stack(task_struct* task_struct_ptr, void* fun_ptr)
{	

	task_struct_ptr->kernel_rsp = &task_struct_ptr->kernel_stack[KERNEL_STACK_SIZE -1];
	task_struct_ptr->rip = fun_ptr;
       task_struct_ptr->pid = ++global_pid;
	task_struct_ptr->rflags = 0x20202;
	task_struct_ptr->next = NULL;
	task_struct_ptr->num_count = 0;
	task_struct_ptr->last_run = NULL;

	//task_struct_ptr->kernel_stack[KERNEL_STACK_SIZE -1] = (void *)0x23;
	//task_struct_ptr->kernel_stack[KERNEL_STACK_SIZE -2] = &task_struct_ptr->kernel_stack[KERNEL_STACK_SIZE -1];
	//task_struct_ptr->kernel_stack[KERNEL_STACK_SIZE -3] = (void *)0x20202;
	//task_struct_ptr->kernel_stack[KERNEL_STACK_SIZE -4] = (void *)0x1b;
	//task_struct_ptr->kernel_stack[KERNEL_STACK_SIZE -5] = fun_ptr;

	task_struct_ptr->pml4e = NULL;
	task_struct_ptr->user_stack = NULL;
       task_struct_ptr->parent = NULL;
	
	//add_task_to_ready(task_struct_ptr);

}


void print_user_stack(void* rsp,void* stack){
    uint64_t* start = (uint64_t*) rsp;
    uint64_t* end  = (uint64_t*)(stack + (USER_STACK_SIZE-1)*8);
    while(start<=end){
   // printk("%x,",*start);
     start++;   
   }

}

void set_user_stack_child(task_struct* task_struct_ptr){
    uint64_t* child_stack = (uint64_t*)task_struct_ptr->user_stack;
   uint64_t* parent_stack = (uint64_t*)task_struct_ptr->parent->user_stack;
   uint64_t* parent_rsp = (uint64_t*)task_struct_ptr->parent->rsp;
  //print_user_stack(parent_rsp,parent_stack);
   uint64_t* temp1 = (uint64_t*)(parent_stack + USER_STACK_SIZE-1) ;
   uint64_t* temp2 = (uint64_t*)(child_stack + USER_STACK_SIZE-1) ;
   while(temp1 >= parent_rsp) {
     *temp2 = *temp1;    
     temp1--; temp2--;
    }
 //printk("the value copied are [%x] --- [%x]",*(temp2+1),*(temp1+1));
  task_struct_ptr->rsp = (void*)(++temp2);
   print_user_stack(task_struct_ptr->rsp,child_stack);
 
}

void create_user_stack(task_struct* curr){
 
     
      uint64_t user_stack =  (uint64_t)kmalloc_mem();
      uint64_t virt_user_stack = PHY_TO_VIRT_DIFF(user_stack, uint64_t);
      mmap(virt_user_stack,user_stack);
      memset((void *)virt_user_stack, 0, PAGE_SIZE);

      user_stack =  (uint64_t)kmalloc_mem();
      mmap(virt_user_stack + PAGE_SIZE,user_stack);
      memset((void *)(virt_user_stack + PAGE_SIZE), 0, PAGE_SIZE);

     curr->user_stack =(void *)virt_user_stack;

     if(curr->parent == NULL) {
       set_user_stack(curr);
       curr->rsp = &curr->user_stack[USER_STACK_SIZE -21] ;
      }
     else {
       set_user_stack_child(curr);
       curr->rip = curr->parent->rip;
      }
     
}



 


void random_function(void) {

printk("dummy function");
}

void timer_interrupt_1(void)
{ 
  
      			__asm__ __volatile__(
					"popq %rbx\n\t"
					"popq %rbp\n\t");

if (schedule == 0){

	__asm__ __volatile__("cli");
	__asm__ __volatile__(
			"pushq %rax\n\t"
			"pushq %rbx\n\t"
			"pushq %rcx\n\t"
			"pushq %rdx\n\t"
			"pushq %rsi\n\t"
			"pushq %rdi\n\t"
			"pushq %rbp\n\t"
			"pushq %r8\n\t"
			"pushq %r9\n\t"
			"pushq %r10\n\t"
			"pushq %r11\n\t"
			"pushq %r12\n\t"
			"pushq %r13\n\t"
			"pushq %r14\n\t"
			"pushq %r15\n\t"
			);
	
	timer_ticks++;
      if(timer_ticks%18 == 0) {
       ///// in sleep list//////
			decr_sleep_count();
		////			
 
	number_seconds++;	
        }
      if(timer_ticks%18000 == 0){
        //  schedule = 1 -schedule;
       }
	 print_timer("Number of  seconds has passed=%d",number_seconds);
        PIC_sendEOI(0x20); 
      
      			__asm__ __volatile__(
					"popq %r15\n\t"
					"popq %r14\n\t"
					"popq %r13\n\t"
					"popq %r12\n\t"
					"popq %r11\n\t"
					"popq %r10\n\t"
					"popq %r9\n\t"
					"popq %r8\n\t"
					"popq %rbp\n\t"
					"popq %rdi\n\t"
					"popq %rsi\n\t"
					"popq %rdx\n\t"
					"popq %rcx\n\t"
					"popq %rbx\n\t"
					"popq %rax\n\t"

					"popq %rbp\n\t"
					);
        __asm__ __volatile__(
                           "sti\n\t"
                             "iretq\n\t"   );

}




else {	

	__asm__ __volatile__("cli");
	__asm__ __volatile__(
			"pushq %rax\n\t"
			"pushq %rbx\n\t"
			"pushq %rcx\n\t"
			"pushq %rdx\n\t"
			"pushq %rsi\n\t"
			"pushq %rdi\n\t"
			"pushq %rbp\n\t"
			"pushq %r8\n\t"
			"pushq %r9\n\t"
			"pushq %r10\n\t"
			"pushq %r11\n\t"
			"pushq %r12\n\t"
			"pushq %r13\n\t"
			"pushq %r14\n\t"
			"pushq %r15\n\t"
			);


     void* curr_kernel_rsp;
     void* curr_user_rsp;
     void* curr_kernel_end_rsp;	
     
  if (curr != NULL && curr->num_count > 0) {

  if(process_instruction == 3 ||process_instruction == 2 ) {

    curr_kernel_rsp = curr->kernel_rsp;

      curr_user_rsp = *(void **)(curr_kernel_rsp + 152);
      curr_user_rsp-=8; 
      curr_kernel_end_rsp = curr_kernel_rsp + 160; 

	int i;
       for(i = 0 ;i<21 ;i++) {
           *(uint64_t *)curr_user_rsp = *(uint64_t *)curr_kernel_end_rsp;
           curr_user_rsp-=8;
           curr_kernel_end_rsp-=8;
        }

	curr_user_rsp+=8;
        print_user_stack(curr_user_rsp,curr->user_stack);	
        curr_kernel_rsp = curr_kernel_rsp + 168;
        curr->kernel_rsp = curr_kernel_rsp;
        curr->rsp = curr_user_rsp; 

   }

   else if(process_instruction == 1 ||  process_instruction == 4) {
     }

     
  else{
	__asm__ __volatile__(
                   "movq %%rsp, %[curr_kernel_rsp]"
                   : [curr_kernel_rsp] "=r"(curr_kernel_rsp)
                );

	
      curr_user_rsp = *(void **)(curr_kernel_rsp + 152);
      curr_user_rsp-=8; 
      curr_kernel_end_rsp = curr_kernel_rsp + 160; 


	int i;
       for(i = 0 ;i<21 ;i++) {
           *(uint64_t *)curr_user_rsp = *(uint64_t *)curr_kernel_end_rsp;
           curr_user_rsp-=8;
           curr_kernel_end_rsp-=8;
        }



	curr_user_rsp+=8;
        print_user_stack(curr_user_rsp,curr->user_stack);	
        curr_kernel_rsp = curr_kernel_rsp + 168;
        curr->kernel_rsp = curr_kernel_rsp;
        curr->rsp = curr_user_rsp; 


    } 

}


// timer code starts
          
	//timer_ticks++;	
	 print_timer("Number of  seconds has passed=%d",number_seconds);
       schedule = 0;
// timer code ends

      
               if (!scheduler_start){
			scheduler_start = 1;

			curr = ready_task;
			   ready_task = ready_task->next;
                     
                     curr->num_count = curr->num_count+1;
			if(curr->pml4e == NULL){
			curr->pml4e = setup_user_paging();
			}
                    
						
		      _cpu_write_cr3((uint64_t)curr->pml4e);
                    populate_instruction(curr);

                    if(curr->user_stack == NULL)
		        create_user_stack(curr);

                    //install_tss_user((uint64_t)curr->kernel_rsp); 
                    // saving the tss values
			tss.rsp0 = (uint64_t)curr->kernel_rsp;
			uint16_t sel = 43;
		
			__asm__ __volatile__(
                   				"ltr %[sel]"
                   				: 
                   				:[sel] "r"(sel)
                				);
                     // saving tss values complete

			__asm__ __volatile__(
					     "movq %[curr_rsp], %%rsp\n\t"
					     :
					     :[curr_rsp] "m" (curr->rsp));

			__asm__ __volatile__(
					"popq %r15\n\t"
					"popq %r14\n\t"
					"popq %r13\n\t"
					"popq %r12\n\t"
					"popq %r11\n\t"
					"popq %r10\n\t"
					"popq %r9\n\t"
					"popq %r8\n\t"
					"popq %rbp\n\t"
					"popq %rdi\n\t"
					"popq %rsi\n\t"
					"popq %rdx\n\t"
					"popq %rcx\n\t"
					"popq %rbx\n\t"
					"popq %rax\n\t"
					"add  $0x08,%rsp\n\t"
					);
					
                     
			__asm__ __volatile__("mov $0x20, %al\n\t"
					     "out %al, $0x20\n\t"
					     "out %al, $0xA0\n\t"
                                        "sti\n\t"					     
                                      "iretq\n\t");
		}
          else {
			prev = curr;

			curr = ready_task;
                     if (curr == NULL) 
                       curr = prev;

              if(prev != curr){

               if(process_instruction!=1 && process_instruction !=2 && process_instruction !=4)
			{
				add_task_to_ready(prev);
			}

			
			   ready_task = ready_task->next;
                     
                     curr->num_count = curr->num_count + 1;
                     if(process_instruction == 4)
                      {   //do nothing
                       } 
			else if(curr->pml4e == NULL){			
			    curr->pml4e = setup_user_paging();
			}
                     else {
                         curr->pml4e = setup_user_paging_p(curr->pml4e);
                      }

			
                    _cpu_write_cr3((uint64_t)curr->pml4e);
			populate_instruction(curr);

			if(curr->user_stack == NULL)
			   create_user_stack(curr);
                    
                  }

                    // install_tss_user((uint64_t)curr->kernel_rsp);
                   // saving the tss values
			tss.rsp0 = (uint64_t)curr->kernel_rsp;
			/*
                     uint16_t sel = 43;
		
			__asm__ __volatile__(
                   				"ltr %[sel]"
                   				: 
                   				:[sel] "r"(sel)
                				);
                      */
                     // saving tss values complete




			__asm__ __volatile__(
					     "movq %[curr_rsp], %%rsp\n\t"
					     :
					     :[curr_rsp] "m" (curr->rsp));


		       __asm__ __volatile__(
               		    "movq %%rsp, %[curr_kernel_rsp]"
                		   : [curr_kernel_rsp] "=r"(curr_kernel_rsp)
                			); 
                    
			*(uint64_t *)(curr_kernel_rsp+24+128) = (uint64_t)(curr_kernel_rsp+40+128);  // actiallly a user rsp
                      process_instruction =0;

			__asm__ __volatile__(
					"popq %r15\n\t"
					"popq %r14\n\t"
					"popq %r13\n\t"
					"popq %r12\n\t"
					"popq %r11\n\t"
					"popq %r10\n\t"
					"popq %r9\n\t"
					"popq %r8\n\t"
					"popq %rbp\n\t"
					"popq %rdi\n\t"
					"popq %rsi\n\t"
					"popq %rdx\n\t"
					"popq %rcx\n\t"
					"popq %rbx\n\t"
					"popq %rax\n\t"
					"add  $0x08,%rsp\n\t"
					);	

			

			__asm__ __volatile__("mov $0x20, %al\n\t"
					     "out %al, $0x20\n\t"
					     "out %al, $0xA0\n\t"
                                        "sti\n\t"
					     "iretq\n\t");
			
		
		}


       }

}



void getps(){

	struct task_struct *rdy_tmp;
	rdy_tmp=ready_task;
	printk("\n [%d]",curr->pid);
	if(ready_task == NULL)
		return;
	while(rdy_tmp!=NULL){
		rdy_tmp=ready_task;
		printk("\n [%d]",rdy_tmp->pid);
		rdy_tmp=rdy_tmp->next;
	}
}


