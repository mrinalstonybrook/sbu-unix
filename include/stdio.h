#ifndef _STDIO_H
#define _STDIO_H

#include <defs.h>
typedef __builtin_va_list va_list;
#define va_start(v,l) __builtin_va_start(v,l)
#define va_arg(v,l)   __builtin_va_arg(v,l)
#define va_end(v)     __builtin_va_end(v)
#define va_copy(d,s)  __builtin_va_copy(d,s)

void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, char val, size_t count);

void remap_printf();


extern void print_timer(const char *fmt,... );
extern void change_print_buffer_location();
extern void printk(const char *fmt,... );
extern void printy(const char *fmt,... );
int printf(const char *format, ...);
//int scanf(const char *format, ...);
extern void  clear_screen(void);

void clear_screen();
void scanf(char*,void*);
void scank(char*,char*);




#endif
