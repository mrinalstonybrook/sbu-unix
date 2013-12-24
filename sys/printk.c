#include <defs.h>
#include <stdio.h>
#include <pagetab.h>


char *cursor_pos=(char *) 0xB8000;
char *start_pos=(char *) 0xB8000;

void change_print_buffer_location(){

	cursor_pos=PHY_TO_VIRT(cursor_pos, char *);
	start_pos =PHY_TO_VIRT(start_pos, char *);;
}

static int get_row_pos(char *cursor_pos){
	int row_no;
	row_no= (int)((cursor_pos-start_pos)/160);
	return row_no;
}

static int get_col_pos(char *cursor_pos){
	int col_no;
	col_no=(int)(((int)(cursor_pos - start_pos)%160)/2);
	return col_no;
}

static char* get_cursor_pos(int row_no, int col_no){
	cursor_pos=(char *)(((row_no*80+col_no)*2)+start_pos);
	return cursor_pos;
}

char* checkcursor(char* cursor){
	int l;
		
		l =(int) (cursor - start_pos);
		if (l >=3359){
			char * swap_buffer= start_pos;
			int j;
			for( j=0; j<=1679;j++){

				* swap_buffer= *(swap_buffer + 160 );
				swap_buffer=swap_buffer+2;
			
		}
		cursor=start_pos+ 3200 ;
		return (char*)cursor;
		}
		else
		return (char*)cursor;

}


void  clear_screen(){
	char *start_tmp=start_pos;

	while(start_tmp <= ((char *)start_pos + 80*25*2)){
		*start_tmp=' ';
		start_tmp=start_tmp+2;
	}
	cursor_pos=start_pos;
}

void printk(const char *fmt,... ){
	int d0,number,tmp=0;
	uint64_t number2;
	char *str,c;
	void *ptr;
	unsigned long ptr_num;
	va_list args;
	va_start(args,fmt);

	char string_buffer[200];
	char int_to_hex[]= "0123456789abcdef";
	char ascii_my[]={'0','1','2','3','4','5','6','7','8','9'};

	while(*fmt){
		if(*fmt !='%' && *fmt!='\t' && *fmt!='\n' ){
			cursor_pos = checkcursor(cursor_pos);
			*cursor_pos=*fmt;
			cursor_pos=cursor_pos+2;
			fmt=fmt+1;
		}
		else if(*fmt=='\t'){
			//write cursor and all
			int col_num= get_col_pos(cursor_pos);
			int row_num= get_row_pos(cursor_pos);
			int temp = col_num;
			temp= temp%8;
			col_num= col_num +(temp*2);
			cursor_pos=get_cursor_pos(row_num,col_num);
			fmt =fmt+1;
		}
		else if(*fmt == '\n'){
			int col= get_col_pos(cursor_pos);
			int rol= get_row_pos(cursor_pos);
			if (rol<1){
				int diff= 160-(2*col);
				cursor_pos = checkcursor(cursor_pos);
				cursor_pos= cursor_pos + diff;
				fmt = fmt+1;
			}
			else{
				int diff= 160-(2*col);
				cursor_pos = checkcursor(cursor_pos);
				cursor_pos= cursor_pos + diff;
				fmt = fmt+1;

			}
		}
		else{
			switch(*(fmt+1)){
				case 'd':
					tmp=0;
					number = va_arg(args, int);
					if(number == 0){
						string_buffer[tmp]=ascii_my[0];
						tmp++;
					}
					else{
						while(number>0){
							d0=number%10;//remainder
							string_buffer[tmp]=ascii_my[d0];
							number = number/10;
							tmp++;
						}
						tmp--;
					}
					while(tmp>=0){
						cursor_pos = checkcursor(cursor_pos);
						*cursor_pos=string_buffer[tmp];
						cursor_pos=cursor_pos+2;
						tmp--;
					}
					break;

				case 's':
					str = va_arg(args, char *);
					while(*str){
						cursor_pos = checkcursor(cursor_pos);
						*cursor_pos=*str;
						cursor_pos=cursor_pos+2;
						str++;
					}
					break;

				case 'x':
					number2 = va_arg(args, uint64_t);
					tmp=0;
					do
						string_buffer[tmp++]=int_to_hex[number2%16];
					while((uint64_t)(number2/=16)>0);
					while(tmp--){
						cursor_pos = checkcursor(cursor_pos);
						*cursor_pos=string_buffer[tmp];
						cursor_pos=cursor_pos+2;
					}
					break;

				case 'c':
					c =va_arg(args, int);
					cursor_pos = checkcursor(cursor_pos);
					*cursor_pos=c;
					cursor_pos=cursor_pos+2;
					break;

				case 'p':
					ptr = va_arg(args, void *);
					ptr_num=(unsigned long)ptr;
					tmp=0;
					while(ptr_num>0){
						d0=ptr_num%10;//remainder
						string_buffer[tmp]=ascii_my[d0];
						ptr_num = ptr_num/10;
						tmp++;
					}
					while(tmp--){
						cursor_pos = checkcursor(cursor_pos);
						*cursor_pos=string_buffer[tmp];
						cursor_pos=cursor_pos+2;
					}
					break;
			}
			fmt=fmt+2;
		}
		/*
		int l;
		l =(int) (cursor_pos - start_pos);
		if (l >=3359){
			int i;
			char * temp_buffer=start_pos;
			for( i =0 ; i<80;i++){
				string_buffer[i]= *temp_buffer;
				temp_buffer=temp_buffer+2;
			}
			char * swap_buffer= start_pos;
			int j;
			for( j=0; j<=1679;j++){
			//for( j=0; j<=3359;j++){

				* swap_buffer= *(swap_buffer + 160 );
				swap_buffer=swap_buffer+2;
			}
			cursor_pos=start_pos+ 3200 ;
		}

	} */
}	
va_end(args);
}

void printy(const char *fmt,... ){
	char c;
	va_list args;
	va_start(args,fmt);
	char *print_buffer;

	print_buffer = (char*)start_pos+3840;

	while(*fmt){
		if(*fmt !='%' && *fmt!='\t'){
			*print_buffer=*fmt;
			print_buffer=print_buffer+2;
			fmt=fmt+1;
		}
		else{
			switch(*(fmt+1)){
				case 'c':
					c =va_arg(args, int);
					*print_buffer=c;
					print_buffer=print_buffer+2;
					break;
			}
			fmt=fmt+2;
		}
	}
	va_end(args);
}

void print_timer(const char *fmt,... ){
	int d0,number,tmp=0;

	char c;
	va_list args;
	va_start(args,fmt);
	volatile register char *print_buffer;
	char ascii_my[]={'0','1','2','3','4','5','6','7','8','9'};

	char string_buffer[1024];
	print_buffer = (char*)start_pos+3900;

	while(*fmt){
		if(*fmt !='%' && *fmt!='\t'){
			*print_buffer=*fmt;
			print_buffer=print_buffer+2;
			fmt=fmt+1;
		}
		else{
			switch(*(fmt+1)){
				case 'c':
					c =va_arg(args, int);
					*print_buffer=c;
					print_buffer=print_buffer+2;
					break;
				case 'd':
					number = va_arg(args, int);
					while(number>0){
						d0=number%10;//remainder
						string_buffer[tmp]=ascii_my[d0];
                                                number = number/10;
                                                tmp++;
                                        }
                                        while(tmp--){
                                                *print_buffer=string_buffer[tmp];
                                                print_buffer=print_buffer+2;
                                        }
                                        break;

                        }
                        fmt=fmt+2;
                }
        }
        va_end(args);
}
void *memcpy(void *dest, const void *src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void *memset(void *dest, char val, size_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}
/*
size_t strlen(const char *str)
{
    size_t len;
    for(len = 0; *str != '\0'; str++) len++;
    return len;
}
*/

