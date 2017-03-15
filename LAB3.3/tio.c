typedef unsigned int u32;

int BASE = 10;
u32 y;
char *table = "0123456789ABCDEF";

prints(char *s)
{
	while(*s != '\0')
		putc(*s++);
}

int rpu(u32 x)
{
	char c;
	if(x)
	{
		c = table[x % BASE];
		rpu(x / BASE);
		putc(c);
	}
}
/*
int rpl(u32 x, u32 z)
{
	char c;
}//????*/

int printu(u32 x)
{
	BASE = 10;
	if(x == 0)
		putc('0');
	else
		rpu(x);
	putc(' ');//???
}
int printd(int x)
{
	BASE = 10;
	if(x < 0)
	{
		putc('-');
		x = x * -1;
	}
	y = x;
	printu(y);
}
int printo(u32 x)
{
	BASE = 8;
	putc('0');
	printu(x);
}
int printx(u32 x)
{
	BASE = 16;
	putc('0');
	putc('x');
	printu(x);
}

printf(char *fmt, ...)
{
	int *ip = &fmt + 1;
	char *cp = fmt;

	while(*cp != '\0')
	{
		if(*cp == '%')
		{
			cp = cp + 1;
			switch(*cp)
			{
				case 'c':
					putc(*ip);
					break;
				case 's':
					prints(*ip);
					break;
				case 'u':
					printu(*ip);
					break;
				/*case 'l':
					printl(*ip, *ip + 1);
					break;*///???
				case 'd':
					printd(*ip);
					break;
				case 'o':
					printo(*ip);
					break;
				case 'x':
					printx(*ip);
					break;
				default:
					break;
			}
			ip++;
		}
		else
			putc(*cp);
		if(*cp == '\n')
			putc('\r');
		cp++;
	}
}

