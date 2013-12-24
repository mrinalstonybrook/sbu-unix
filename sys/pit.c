#include <stdio.h>
#include <task.h>

//void timer_phase(int hz)
//{
  //  int divisor = 1193180 / hz;       /* Calculate our divisor */
//outportb(0x43, 0x36);             /* Set our command byte 0x36 */
 //   outportb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
  //  outportb(0x40, divisor >> 8);     /* Set high byte of divisor */
//}
	
//void _x86_64_asm_switch_to(struct task_struct *,struct task_struct *);
//extern struct task_struct tsa[2];
extern void process1(void);

/* This will keep track of how many ticks that the system
*  has been running for */
static int timer_ticks = 0;
static int number_seconds = 0;
/* Handles the timer. In this case, it's very simple: We
*  increment the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 18.222 times
*  per second. Why 18.222Hz? Some engineer at IBM must've
*  been smoking something funky */
void timer_handler(){
    /* Increment our 'tick count' */
    	//print_timer("anurag");
	
	timer_ticks++;
	//print_timer("timer:%d ",timer_ticks);
	if(number_seconds ==0)
	print_timer("Number of  seconds has passed=%d",number_seconds);		
	
    /* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */
    if (timer_ticks % 18 == 0)
    {
       // printk("printer came here");
         //if(number_seconds ==2)
          // _x86_64_asm_switch_to(&tsa[0],&tsa[1]);
        number_seconds++;			
	 print_timer("Number of  seconds has passed=%d",number_seconds);
    	//process1();	
	}
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
//void timer_install()
//{
    /* Installs 'timer_handler' to IRQ0 */
//    irq_install_handler(0, timer_handler);
//}
		

