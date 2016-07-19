#include "APP_AuxFunctions.h"
/*********************************************************/
/*              auxiliary functions                      */
/*********************************************************/


/*int hexToInt(char *h){
	int i, result=0;
	int tmp;
	
	for(i=0;h[i] && i<8;i++)
    {
		tmp = hexCharToInt(h[i]);
		if(tmp == -1) return result;
		result = result*0x10+tmp;
	}
	return result;
}

int hexCharToInt(char digit)
{
	if('0'<=digit && digit<='9')
		return digit-'0';
	if('A'<=digit && digit<='F')
		return digit-'A'+10;
	if('a'<=digit && digit<='f')
		return digit-'a'+10;
	return -1;
}
*/
