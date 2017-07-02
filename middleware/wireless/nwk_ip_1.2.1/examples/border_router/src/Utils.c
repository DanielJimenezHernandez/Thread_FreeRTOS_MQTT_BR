/*
 * Utils.c
 *
 *  Created on: Apr 21, 2017
 *      Author: daniel
 */
#include "Utils.h"

/*Return size of string including the null terminator*/
int sizeof_srt(char * str1){
        char i;
        for (i = 0; *str1 != 0; ++i, str1++){
        }
        return (int)i+1;
}

// reverse the given null-terminated string in place
void reverse(char * str)
{
  if (str)
  {
    char * end = str + sizeof_srt(str) - 2;

    // swap the values in the two given variables
    // XXX: fails when a and b refer to same memory location
#   define XOR_SWAP(a,b) do\
    {\
      a ^= b;\
      b ^= a;\
      a ^= b;\
    } while (0)

    // walk inwards from both ends of the string,
    // swapping until we get to the middle
    while (str < end)
    {
      XOR_SWAP(*str, *end);
      str++;
      end--;
    }
#   undef XOR_SWAP
  }
}

// Implementation of itoa()
char* int2string(int num, char* str, int base)
{
    int i = 0;
    int isNegative = 0;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (char)((rem-10) + 'a') : (char)(rem + '0');
        num = num/base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str);

    return str;
}


/* Concatenates 2 strings and returns the number of bytes written */
int str_concat(char *str1, char *str2, char *buffer){
        int i,j;
        for(i = 0; *str1 != 0; i++,str1++,buffer++){
                *buffer = *str1;
        }

        for(j = 0; *str2 != 0; ++j,++str2,buffer++){
                *buffer = *str2;
        }
        *buffer = '\0';
        return i+j;

}

int str_compare(char *str1, char *str2){

        while(*str1 == *str2){
                if(*str1 == '\0' || *str2 == '\0')
                        break;
                str1++;
                str2++;
        }

        if(*str1 == '\0' && *str2 == '\0'){
                return 0;
        }
        else{
                return 1;
        }
}

