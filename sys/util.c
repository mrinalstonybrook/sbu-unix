#include<defs.h>
#include<math.h>

int strncmp(const char *s1, const char *s2, uint32_t n)
{
  for ( ; n > 0; s1++, s2++, --n)
    if (*s1 != *s2)
      return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
    else if (*s1 == '\0')
      return 0;
  return 0;
}

uint64_t octalToDecimal(uint64_t octal){
	uint64_t  decimal = 0, i=0;
	while(octal!=0){
		decimal = decimal + (octal % 10) * pow(8,i++);
		octal = octal/10;
	}
	return decimal;
}
/*
int matchString( char *s , char *t){

  int ret = 0;

  while (!(ret = *(unsigned char *) s - *(unsigned char *) t) && *t){
    ++s;
    ++t;
  }

  if (ret < 0){
    ret = -1;
  }
  else if (ret > 0){
    ret = 1 ;
  }

  return ret;

}*/