#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define HEX 16
#define DECIMAL 10

void my_printf(const char *, ...);
void my_vprintf(const char *, va_list);
int format_specifier(const char *, va_list, int, int *);
void print_string(char *);
void unsignedNumberToString(uint64_t, int, char *);
void numberToString(int64_t, int, char *);


int main(void)
{
	uint64_t n = (uint64_t) - 1;
	char *ptr = "hello";
	void (*ptr_func)(char *) = print_string;

	my_printf("Characters: %c %c done.\n", 'a', 65);
	my_printf("Decimals: %d %ld done\n", 1977, 650000L);
	my_printf("Some different radicals: %d %x %X done.\n", 100, 100, 100);
	my_printf("%s \n", "Alx is Amaizing");
	my_printf("testing with pointer: %p, %p, %ld, %lx\n",ptr_func, ptr, n, n);
	my_printf("testing other possible integer: %d %d\n", INT_MAX, INT_MIN);

	return (0);
}

void my_printf(const char *format, ...)
{
	va_list args;

	va_start(args, format);
	my_vprintf(format, args);
	va_end(args);
}

void my_vprintf(const char *format, va_list args)
{
	int state = 0, reset_flag, reset = 1, is_long = 0;
	int *ptr = &is_long;
	
	while (*format)
	{
		if (state == 0)
		{
			if (*format == '%')
				state = 1;
			else
				putchar(*format);	
		}
		else
		{
			reset_flag = format_specifier(format, args, reset, ptr);

			if (reset_flag == 1)
			{
				state = 0;
				is_long = 0;
			}
			else
				reset_flag = 1;
		}
		
		format++;
	}
}

int format_specifier(const char *format, va_list args, int reset, int *ptr_is_long)
{
	int n;
	char ch, *s, buffer[65];

	switch (*format)
	{
		case 'd':
			{
				if (*ptr_is_long)
					n = va_arg(args, long);
				else
					n = va_arg(args, int);
				numberToString(n, DECIMAL, buffer);
				print_string(buffer);
				break;
			}
		case 'c':
			{
				ch = va_arg(args, int);
				putchar(ch);
				break;
			}
		case 's':
			{
				s = va_arg(args, char *);
				print_string(s);
				break;
			}
		case 'p':
			{
				putchar('0');
				putchar('x');
				n = va_arg(args, int);
				unsignedNumberToString(n, HEX, buffer);
				print_string(buffer);
				break;
			}
		case 'x':
			{
				if (*ptr_is_long)
					n = va_arg(args, long);
				else
					n = va_arg(args, int);
				unsignedNumberToString(n, HEX, buffer);
				print_string(buffer);
				break;
			}
		case 'l':
			{
				*ptr_is_long = 1;
				reset = 0;
				break;
			}
		case '%':
			{
				putchar('%');
				break;
			}
	}
	return (reset);
}

void print_string(char *s)
{
	while (*s)
	{
		putchar(*s);
		s++;
	}
}

void unsignedNumberToString(uint64_t number, int base, char *buf)
{
	char tmp[65];
	int rem, i = 0, j = 0;

	if (number == 0)
	{
		*buf++ = '0';
		*buf = '\0';
		return;
	}

	while (number)
	{
		rem = number % base;
		if (rem >= 10)
			tmp[i++] = 'a' + (rem - 10);
		else
			tmp[i++] = '0' + rem;
		
		number /= base;
	}
	for (j = i - 1; j >= 0; j--)
		*buf++ = tmp[j];
	*buf = '\0';
}

void numberToString(int64_t number, int base, char *buf)
{
	if (number < 0)
	{
		buf[0] = '-';
		number *= -1;
	}
	unsignedNumberToString(number, base, buf);
}
