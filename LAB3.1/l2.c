typedef unsigned int u32;

int BASE = 10;
u32 y;
char *table = "0123456789ABCDEF";

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
int rpl(u32 x, u32 z)
{
	char c;
}

int printu(u32 x)
{
	BASE = 10;
	if(x == 0)
		putc('0');
	else
		rpu(x);
	putc(' ');
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

