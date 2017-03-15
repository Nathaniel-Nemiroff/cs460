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
				case 'l':
					printl(*ip, *ip + 1);
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
