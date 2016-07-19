/*********************************************************/
/*              auxiliary functions                      */
/*********************************************************/
//int hexCharToInt(BYTE digit);

int hexToInt(char *h);
int hexCharToInt(char digit);

#define set_bit(ADDRESS,BIT) (ADDRESS |= (1L<<BIT))
#define clear_bit(ADDRESS,BIT) (ADDRESS &= ~(1L<<BIT))
#define toggle_bit(ADDRESS,BIT) (ADDRESS ^= (1L<<BIT))
#define test_bit(ADDRESS,BIT) (ADDRESS & (1L<<BIT))