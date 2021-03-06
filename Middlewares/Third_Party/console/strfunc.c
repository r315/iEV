
#include "strfunc.h"

#define FLOAT_MAX_PRECISION 6

size_t strlen(const char *str) {
	size_t count = 0;
	if (str == NULL) return 0;
	while (*str++) {
		count++;
	}
	return count;
}
/**
 * Searches for the first occurrence of the character c
 * in the string pointed to by the argument str.
 *
 * \param str   - input string
 * \param c     - character to be found
 * \return      - pointer to found char if found, pointer to string terminator if not
 * */
char *chrinstr(const char *str, char c) {
	if (str == NULL) return 0;
	while (*str) {
		if (*str == c)
			break;
		str += 1;
	}
	return (char*)str;
}

/**
 * Return pointer to next separated word by ' ' in the given string
 *
 * \param line  - pointer to string
 * \return      - pointer to first character of next word, pointer to string terminator
 * */
char *nextWord(char *line) {
	line = chrinstr(line, ' ');
	if (*line != '\0')
		line++;
	return line;
}

/**
 * Try to parse an hex string to integer from parameter line
 * and move to line pointer to next parameter
 *
 * \param  line pointer to string pointer
 * \param  value pointer to output value
 * \return 1 if success and increments line pointer, 0 on failure no parameters are affected
 * */
uint8_t nextHex(char **line, uint32_t *value) {
	if (hatoi(*line, value)) {
		*line = nextWord(*line);
		return 1;
	}
	return 0;
}

/**
 * Try to get an integer number from a given string
  *
 * \param  line - pointer to string pointer
 * \param  value - pointer to output value
 * \return 1 - if success and move to next substring, 0 - on failure no parameters are affected
 * */
uint8_t nextInt(char **line, int32_t *value) {
	if (yatoi(*line, value)) {
		*line = nextWord(*line);
		return 1;
	}
	return 0;
}

/**
 * Try to parse a double number from a given string
  *
 * \param  line - pointer to string pointer
 * \param  value - pointer to output value
 * \return 1:if success and move to next substring, 0:on failure no parameters are affected
 * */
uint8_t nextDouble(char **line, double *value) {
	if (fatoi(*line, value)) {
		*line = nextWord(*line);
		return 1;
	}
	return 0;
}


/**
 * Get first char of sprint pointed by line, and advance that pointer to next char
 * by skiping space character
 * \param line		pointer string pointer
 * \return		first char of current word
 *
 * */
char nextChar(char **line) {
	char c;
	c = *line[0];
	*line = nextWord(*line);
	return c;
}

/**
 * Compare if first word in a string is equal to the given word.
 *
 * \param str   - pointer to word pointer to be compared
 * \param word  - pointer to comparing word
 * \return      - 1 if match and move word pointer to next word,
 *                0 not equal and no parameter changed
 * */
uint8_t isNextWord(char **str, const char *word) {
	char *str1 = *str;

	while (*word != '\0') {
		if (*str1 != *word)
			return 0;
		str1++;
		word++;
	}	
	*str = nextWord(*str);	
	return 1;
}


/**
 * get first occuring substring from a token split of a given string
 *
 * @param str:		reference to input string, on return the reference will pointer
 * 					for then remaining string.
 * @param token:	token to split the string
 * @param len:		length of the given string
 * @param saveptr:	pointer for saving the remaining string
 *
 * returns: 		pointer for found sub string or null if not splitted
 **/
char *strsub(char *str, const char token, uint8_t len, char **saveptr) {
	char *ptr, i = 0;

	if (str == NULL) {
		return NULL;
	}

	// save pointer for return
	ptr = str;

	// search token, if finds it 
	// replace it with string terminator character
	while (*ptr && i < len) {
		if (*ptr == token) {
			*ptr = '\0';
			break;
		}
		ptr += 1;
		i++;
	}

	// got to the end of the string and token was not found return null
	// or string is empty
	if (i == len || ptr == str) {
		return NULL;
	}

	// if specified, return pointer to remaining string
	if (saveptr != NULL) {
		*saveptr = ptr + 1;
	}

	// return pointer to substring
	return str;
}

char xstrcmp(char const *str1, char const *str2) {
	while (*str1 == *str2) {
		if (*str1 == '\0')
			return 0;
		str1++;
		str2++;
	}
	return (*str1 - *str2);
}

void xstrcpy(char *dst, char *src, uint8_t maxLen) {
	while (*src != '\0' && maxLen--) {
		*dst++ = *src++;
	}
}

/**
 * Try to parse a string representing a integer
 *
 * \param  str - pointer to input string
 * \param  value - pointer to output value
 * \return number of converted digits
 * */
uint8_t yatoi(char *str, int32_t *value) {
	int val = 0;
	char c = *str;
	uint8_t s = 0;

	if (c == '-') {
		s = (1 << 7); // Set signal flag
		str++;
		c = *str;
	}

	do{
		if (c > '/' && c < ':') {
			c -= '0';
			val = val * 10 + c;
			s++;
		}
		else {
			return 0;
		}
		c = *(++str);
	}while (c != ' ' && c != '\n' && c != '\r' && c != '\0');
		
	// check signal flag
	*value = (s & (1 << 7)) ? -val : val;

	return s & 0x7F;
}

/**
 * Try to parse a string representing a hex number to integer
 * \param  str	pointer to input string
 * \param  value  pointer to output value
 * \return 1 if success, 0 if failed
 * */
uint8_t hatoi(char *str, uint32_t *value) {
	uint32_t val = 0;
	char c = *str;
	do {
		val <<= 4;
		if (c > '`' && c < 'g') {
			c -= 'W';
		}
		else if ((c > '@' && c < 'G')) {
			c -= '7';
		}
		else if (c > '/' && c < ':') {
			c -= '0';
		}
		else {
			return 0;
		}

		val |= c;
		c = *(++str);

	} while (c != '\0' && c != ' ' && c != '\n' && c != '\r');

	*value = val;
	return 1;
}

/**
 * Try to parse a string representing a double
 *
 * \param  str - pointer to input string
 * \param  value - pointer to output value
 * \return 1:success, 0:failed
 * */
uint8_t fatoi(char *str, double *value) {
	uint8_t s = 0;
	double val = 0;
	char c = *str;
	int decimal = 1;

	if (c == '-') {
		s = (1 << 7); // Set signal flag
		str++;
		c = *str;
	}

	do {
		if (c > '/' && c < ':') {
			c -= '0';
			val = val * 10 + c;
			if (s & (1 << 6))
				decimal *= 10;
			s++;
		}else if(c == '.'){
			s |= (1 << 6);	// Set decimal point flag
		}else {
			return 0;
		}
		c = *(++str);
	} while (c != ' ' && c != '\n' && c != '\r' && c != '\0');

	val = val / decimal;

	// check signal flag
	*value = (s & (1 << 7)) ? -val : val;

	return s & 0x3F;
}
/**
* Original code by ELM_ChaN. Modified by Martin Thomas
*
* Convert string with diferent radix to integer
*  0x-----
*  0b-----
*  01-----
* */
int xatoi(char **str, long *res)
{
	int32_t val;
	uint8_t c, radix, s = 0;


	while ((c = **str) == ' ') (*str)++;
	if (c == '-') {
		s = 1;
		c = *(++(*str));
	}
	if (c == '0') {
		c = *(++(*str));
		if (c <= ' ') {
			*res = 0; return 1;
		}
		if (c == 'x') {
			radix = 16;
			c = *(++(*str));
		}
		else {
			if (c == 'b') {
				radix = 2;
				c = *(++(*str));
			}
			else {
				if ((c >= '0') && (c <= '9'))
					radix = 8;
				else
					return 0;
			}
		}
	}
	else {
		if ((c < '1') || (c > '9'))
			return 0;
		radix = 10;
	}
	val = 0;
	while (c > ' ') {
		if (c >= 'a') c -= 0x20;
		c -= '0';
		if (c >= 17) {
			c -= 7;
			if (c <= 9) return 0;
		}
		if (c >= radix) return 0;
		val = val * radix + c;
		c = *(++(*str));
	}
	if (s) val = -val;
	*res = val;
	return 1;
}

/**
 * Convert integer to string
 *
 * \param val		value to be converted
 * \param radix		base of convertion [-10,10,16]
 * \param len 		number of digits, len > 0 pad with ' ', len < 0 pad with '0'
 * \return 			pointer to string
 * */
#define XTOA_BUF_SIZE 20
char* pitoa(long val, int radix, int len)
{
	static uint8_t s[XTOA_BUF_SIZE];
	uint8_t i, c, r, sgn = 0, pad = ' ';
	uint32_t v;

	if (radix < 0) {
		radix = -radix;
		if (val < 0) {
			val = -val;
			sgn = '-';
		}
	}

	v = val;
	r = radix;

	if (len < 0) {
		len = -len;
		pad = '0';
	}

	if (len > XTOA_BUF_SIZE) {
		len = XTOA_BUF_SIZE;
	}

	len = XTOA_BUF_SIZE - 1 - len;
	i = XTOA_BUF_SIZE;
	s[--i] = '\0';

	do {
		c = (uint8_t)(v % r);
		if (c >= 10) c += 7;
		c += '0';
		s[--i] = c;
		v /= r;
	} while (v);

	if (sgn) s[--i] = sgn;

	while (i > len) {
		s[--i] = pad;
	}

	return (char*)(s + i);
}

/**
 * Convert double to string
 *
 * https://en.wikipedia.org/wiki/Single-precision_floating-point_format
 * https://wirejungle.wordpress.com/2011/08/06/displaying-floating-point-numbers
 *
 * \param f			value to be converted
 * \param places	number of decimal places
 * \return 			pointer to string
 * */
char *pftoa(double f, char places) {
	long int_part, frac_part;
	char prec;
	static char s[XTOA_BUF_SIZE], *pstr, *pdst;

	int_part = (long)(f);

	if (places > FLOAT_MAX_PRECISION)
		places = FLOAT_MAX_PRECISION;

	frac_part = 0;
	prec = 0;

	while ((prec++) < places) {
		f *= 10;
		frac_part = (frac_part * 10) + (long)f - ((long)f / 10) * 10;  //((long)f%10);			
	}

	pdst = s;
	pstr = pitoa(int_part, -10, 0);

	while (*pstr) {
		*(pdst++) = (*pstr++);
	}

	*pdst++ = '.';

	pstr = pitoa(abs(frac_part), 10, -places);

	do {
		*(pdst++) = (*pstr);
	} while (*pstr++);

	return s;
}

//-----------------------------------------------------------
//
//-----------------------------------------------------------
void * memcpy(void * destination, const void * source, size_t num) {
	for (size_t i = 0; i < num; i++) {
		*((uint8_t*)destination + i) = *((uint8_t*)source);
		source = (uint8_t*)source + 1;
	}
	return destination;
}

void * memset(void * ptr, int value, size_t num) {
	for (size_t i = 0; i < num; i++) {
		*((uint8_t*)ptr + i) = (uint8_t)value;
	}
	return ptr;
}
