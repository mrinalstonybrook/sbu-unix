nclude <stdio.h>
#include <stdlib.h>
#include <random.h>




char * strcpy_new(char *dst, const char *src) {
	char *ret;

	ret = dst;
	while ((*dst++ = *src++) != '\0')
		/* do nothing */;
        *dst='\0';
	return ret;
}


int main(int argc, char* argv[], char* envp[]) {
	printf("Hello World 1!\n");
	printf("Hello World 2!\n");
	printf("Hello World 3!\n");
	printf("Hello World 4!\n");
	printf("Hello World 6!\n");
	printf("Hello World 7!\n");
	printf("Hello World 8!\n");
	
	char shell_buffer[100];
	char *tmp1,*tmp2,*tmp3;
	tmp1=shell_buffer;	
	char *mountpt="/hiphop/";
	struct file fd;
	struct file *fd_p;
	int i=0;
	char pwd[40]="";
      	pwd[0]='\0';
	int k=30;
	int loop=50;
	clrscr();
	while(loop>0){
		loop--;
		fd_p=NULL;
		for(i=0;i<k;i++)
			shell_buffer[i]='\0';
		printf("\n Shell ~%s>",pwd);
		scanf("%s",shell_buffer);
		tmp1=shell_buffer;

if(strncmp(tmp1,"echo",4)==){
	tmp2=shell_buffer;
	tmp2=tmp2+5;
	printf("\n%s",tmp2);
}
else if(strncmp(tmp1,"ls",2)){
	tmp2=shell_buffer;
	tmp2=tmp2+3;
	tmp3=mountpt;
	while(*tmp1==*tmp3){
		tmp1++;
		tmp3++;
	}
	flag=0;
	if((*tmp1!='\0')&&(*tmp3=='\0') && *(tmp1-1)=='/')
		flag=1;

	strcpy_new(fd.filename,tmp2);
	fd.location=flag;

	for(i=0;i<k;i++)
		shell_buffer[i]='\0';

	readdir_ls(&fd,shell_buffer,80);

	tmp2=shell_buffer;
	while(*tmp2!='\0'){
		if(*tmp2==',')
			*tmp2='\t';
		tmp2++;
	}
	printf("\n%s",shell_buffer);

}
else if(strncmp(tmp1,"cd",2)){
	tmp2=shell_buffer;
	tmp2=tmp2+3;
	tmp3=mountpt;
	while(*tmp1==*tmp3){
		tmp1++;
		tmp3++;
	}
	flag=0;
	if((*tmp1!='\0')&&(*tmp3=='\0') && *(tmp1-1)=='/')
		flag=1;
	strcpy_new(fd.filename,tmp2);
	fd.location=flag;
	fd_p=opendir(fd.filename);
	if(fd_p==NULL){
		printf("NO directory exists");
		continue;
	}
	strcpy_new(pwd,tmp2);

}
else if(strncmp(tmp1,"sleep",5)){
	tmp2=shell_buffer;
	tmp2=tmp2+5;
	user_fork();
	if(getpid() != 1)
		exec("bin/child",tmp2);
}
else if(strncmp(tmp1,"pwd",3)){
printf("\n%s",pwd);
}
else
printf("\ncommand not found");

return 0;
}
}
