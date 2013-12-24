#include <defs.h>
#include <syscall.h>

typedef __builtin_va_list va_list;
#define va_start(v,l) __builtin_va_start(v,l)
#define va_arg(v,l)   __builtin_va_arg(v,l)
#define va_end(v)     __builtin_va_end(v)
#define va_copy(d,s)  __builtin_va_copy(d,s)
/*
char str_tmp[100];

char * convert_to_string(uint64_t value, char * str, int base )
{
	//int length=0,start, end;
        char *tmp=str_tmp;
        *tmp='\0';
	int j, length = 0;
	//char c;
	char temp[30];
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }

    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *str++ = '-';
    }
    // Remember where the numbers start.
    //low = str;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        temp[length++] = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    temp[length] = '\0';
    for(j= length-1; j >=0 ; j--)
    {
    	*str++ = temp[j];
    }
    *str = '\0';
    return str;
}
*/
/*
int printf(const char *fmt,... ){
	int d0,number,tmp=0;
	uint64_t number2;
	char *str,c;
	void *ptr;
	unsigned long ptr_num;
	va_list args;
	va_start(args,fmt);

	char string_buffer[80];
                                        char *tmp_c=str_tmp;
	//char int_to_hex[]= "0123456789abcdef";
	char ascii_my[]={'0','1','2','3','4','5','6','7','8','9'};

	while(*fmt){
		if(*fmt !='%' && *fmt!='\t' && *fmt!='\n' ){
			string_buffer[tmp]=*fmt;
			tmp++;
			fmt=fmt+1;
		}
		else if(*fmt=='\t') {
			string_buffer[tmp]='\t';
			tmp++;
			fmt =fmt+1;
		}
		else if(*fmt == '\n') {
			string_buffer[tmp]='\n';
			tmp++;
			fmt =fmt+1;
		}
		else{
			switch(*(fmt+1)){
				case 'd':
					number = va_arg(args, int);
                                        convert_to_string(number,str_tmp,10);
                                        tmp_c=str_tmp;
					while(*tmp_c!='\0'){
						string_buffer[tmp]=*tmp_c;
                                                tmp_c++;
						tmp++;
					}
                                        break;

				case 's':
					str = va_arg(args, char *);
					while(*str){
						string_buffer[tmp]=*str;
						tmp++;
						str++;
					}
					break;

				case 'x':
					number2 = va_arg(args, uint64_t);
                                        convert_to_string(number2,str_tmp,16);
                                        tmp_c=str_tmp;
					while(*tmp_c!='\0'){
						string_buffer[tmp]=*tmp_c;
                                                tmp_c++;
						tmp++;
					}
					break;

				case 'c':
					c =va_arg(args, int);
					string_buffer[tmp]=c;
					tmp++;
					break;

				case 'p':
					ptr = va_arg(args, void *);
					ptr_num=(unsigned long)ptr;
					while(ptr_num>0){
						d0=ptr_num%10;//remainder
						string_buffer[tmp]=ascii_my[d0];
						ptr_num = ptr_num/10;
						tmp++;
					}
					break;
			}
			fmt=fmt+2;
		}

	}
	va_end(args);
	string_buffer[tmp]='\0';
 	return __syscall_prototype(10, (uint64_t)string_buffer,0,0,0,0);
}


*/
