#ifndef _KB_H
#define _KB_H

unsigned char kdbus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    15,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   14,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   14,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

char shift_conversion(char char_code)
{
int i;
char special[19][2]={
{'0',')'},
{'1','!'},
{'2','@'},
{'3','#'},
{'4','$'},
{'5','%'},
{'6','^'},
{'7','&'},
{'8','*'},
{'9','('},
{'`','~'},
{'[','{'},
{']','}'},
{';',':'},
{'"','\''},
{',','<'},
{'.','>'},
{'/','?'},
{'\\','|'}
};

for(i=0;i<19;i++)
{
  if(special[i][0]==char_code)
{
  char_code=special[i][1];
 } 
}

if (char_code>=97 && char_code<=122)
{
  char_code-=32;
}

return char_code;
}		



#endif
