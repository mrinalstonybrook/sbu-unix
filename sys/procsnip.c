#include <defs.h>
#include <stdio.h>
#include <procsnip.h>
#include <task.h>
#include <syscall.h>



void process1(void){
       int k = 8;
       
       printk("here it comes---");
//	__syscall0(0);
  	//printk("\n syscall :: [%x]",__syscall_prototype(0x1,0x2,0x3,0x4,0x5,0x6));	
       printk("Anurag");     
	printk("\n I AM IN PROCESS 1 %d",k);
       printk("\n I AM IN PROCESS 1 %d",k);
       printk("\n I AM IN PROCESS 1 %d",k);
       printk("\n I AM IN PROCESS 1 %d",k);
       printk("\n I AM IN PROCESS 1 %d",k);


 printk("end of  process1"); 

while(1){
   printk("I am in Process 1");
}      

}

void process2(void){
	 int  i,k,g;
       i=0;
       k=0;
	printk("\n I AM IN PROCESS 2");
       while(1){
        printk("\n In process2 while loop");
       }
	printk("babaji ki booti");
       for(k=0;k<20000;k++){
        for(i=0;i<20000;i++){
           g++;
          }
        }
        printk("end of loop in process2");
}

void process3(void){
	 int  i,k,g;
       i=0;
       k=0;
	printk("\n I AM IN PROCESS 3");
       while(1){
	printk("\nbabaji ki booti3");
 
        //printk("\n In process3 while loop");
       }
       for(k=0;k<20000;k++){
        for(i=0;i<20000;i++){
           g++;
          }
        }
    
	printk("end of loop in process3");
}

task_struct p1,p2,p3;
void set_up_task_struct() {

crt_new_user_prs_stack( &p1, &process1);
crt_new_user_prs_stack( &p2, &process2);
crt_new_user_prs_stack( &p3, &process3);

}