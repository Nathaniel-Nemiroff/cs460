
/*************************************************************************
  usp  1   2   3   4   5   6   7   8   9  10   11   12    13  14  15  16
----------------------------------------------------------------------------
 |uds|ues|udi|usi|ubp|udx|ucx|ubx|uax|upc|ucs|uflag|retPC| a | b | c | d |
----------------------------------------------------------------------------
***************************************************************************/

/*
u16 get_word(u16 segment, u16 offset)
{
	u16 retval = 0;
	retval = get_byte(segment, offset) << 8;
	retval += get_byte(segment, offset+1);
	return retval;
}
int put_word(u16 WORD, u16 segment, u16 offset)
{
	u8 HIGH = 0;
	u8 LOW  = 0;
	HIGH = WORD >> 8;
	LOW  = WORD & 0xFF;
	put_byte(HIGH, segment, offset);
	put_byte(LOW , segment, offset+1);
}
*/
/****************** syscall handler in C ***************************/
int kcinth()
{
   int a,b,c,d, r;

//==> WRITE CODE TO GET get syscall parameters a,b,c,d from ustack 
   a = get_word(running->uss, running->usp+13*2);
   b = get_word(running->uss, running->usp+14*2);
   c = get_word(running->uss, running->usp+15*2);
   d = get_word(running->uss, running->usp+16*2);
   switch(a){
       case 0 : r = kgetpid();        break;
       case 1 : r = kps();            break;
       case 2 : r = kchname(b);       break;
       case 3 : r = kkfork();         break;
       case 4 : r = ktswitch();       break;
       case 5 : r = kkwait(b);        break;
       case 6 : r = kkexit(b);        break;
       case 7 : r = kufork();	      break;
       case 8 : r = kkexec(b);	      break;//???
       case 9 : r = khop(b);	      break;

       case 99: kkexit(b);            break;
       default: printf("invalid syscall # : %d\n", a); 
   }

//==> WRITE CODE to let r be the return value to Umode
   put_word(r, running->uss, running->usp+8*2);

}

//============= WRITE C CODE FOR syscall functions ======================

int kgetpid()
{
    //WRITE YOUR C code
	return running->pid;
}

int kps()
{
    //WRITE C code to print PROC information
	char *stat = "0";
	switch(running->status){
		case 0: stat = "FREE";    break;
		case 1: stat = "READY";   break;
		case 2: stat = "RUNNING"; break;
		case 3: stat = "STOPPED"; break;
		case 4: stat = "SLEEP";   break;
		case 5: stat = "ZOMBIE";  break;
		default: break;
	}
	printf("PROC %d STATUS : %s\n", running->pid, stat);

	return running->status;
}

int kchname(char *name)
{
    //WRITE C CODE to change running's name string;
	*running->name = name;
}

int kkfork()
{
  //use you kfork() in kernel;
  //return child pid or -1 to Umode!!!
	int retval = kfork("/bin/u1");
	return retval;	
}
int kufork()//not kfork
{
	return ufork(0);
}
int kkexec(char *y)
{
	int i;
	i = kexec(y);
	goUmode();
	return i;
}
int khop(int p)
{
	return hop(p);
}

int ktswitch()
{
    return tswitch();
}

int kkwait(int *status)
{

  //use YOUR kwait() in LAB3;
  //return values to Umode!!!
	return kwait(status);
}

int kkexit(int value)
{
    //use your kexit() in LAB3
    //do NOT let P1 die
	if(running->pid > 1)
		kexit(value);
	else
		printf("CAN'T EXIT PROC %d\n", running->pid);
}

